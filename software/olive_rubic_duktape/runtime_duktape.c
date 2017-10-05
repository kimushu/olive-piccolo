/**
 * @file runtime_duktape.c
 * @brief Runtime invoker for Duktape engine
 */

#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "sys/alt_dev.h"
#include "priv/alt_file.h"
#include "duktape.h"
#include "dukext.h"
#include "bson.h"
#include "rubic_agent.h"

#define AUTO_GC_CYCLES  50

static const char CJS_PROLOGUE[] = "(function(require,module,exports){";
static const int CJS_PROLOGUE_LEN = sizeof(CJS_PROLOGUE) - 1;
static const char CJS_EPILOGUE[] = "\n})(require,m={exports:{}},m.exports)";
static const int CJS_EPILOGUE_LEN = sizeof(CJS_EPILOGUE) - 1;

extern int debug_stream_fd;

char debug_has_push_back;
char debug_push_back_value;

/**
 * @func compile_file
 * @brief Compile JS file to Duktape function object (and push it)
 * @param ctx Duktape context
 * @param data File name to compile or source data
 * @param direct Direct mode (data contains source)
 * @return Result of duk_pcompile*()
 */
static int compile_file(duk_context *ctx, const char *data, int direct)
{
	int fd;
	char *src, *next;
	duk_size_t file_len, total_len, remainder;
	duk_int_t result;

    if (direct) {
        file_len = strlen(data);
    } else {
        fd = open(data, O_RDONLY);
        if (fd < 0) {
            duk_push_error_object(ctx, DUK_ERR_ERROR, "Cannot open file");
            return -1;
        }
        file_len = lseek(fd, 0, SEEK_END);
        if (file_len == ((duk_size_t)-1)) {
            duk_push_error_object(ctx, DUK_ERR_ERROR, "Cannot seek file");
            return -1;
        }
        lseek(fd, 0, SEEK_SET);
    }
	total_len = file_len + CJS_PROLOGUE_LEN + CJS_EPILOGUE_LEN;
	src = malloc(total_len);
	if (!src) {
        if (!direct) {
            close(fd);
        }
		duk_push_error_object(ctx, DUK_ERR_ERROR, "Not enough memory");
		return -1;
	}
	next = src;
	memcpy(next, CJS_PROLOGUE, CJS_PROLOGUE_LEN);
    next += CJS_PROLOGUE_LEN;
    if (direct) {
        memcpy(next, data, file_len);
        next += file_len;
    } else {
        for (remainder = file_len; remainder > 0;) {
            int done = read(fd, next, remainder);
            if (done < 0) {
                close(fd);
                free(src);
                duk_push_error_object(ctx, DUK_ERR_ERROR, "Cannot read file");
                return -1;
            }
            next += done;
            remainder -= done;
        }
        close(fd);
    }
    memcpy(next, CJS_EPILOGUE, CJS_EPILOGUE_LEN);

    duk_push_lstring(ctx, src, total_len);
    /* [ source ] */
	free(src);
    if (direct) {
        duk_push_string(ctx, "input");
    } else {
        duk_push_string(ctx, data);
    }
    /* [ source filename ] */
    result = duk_pcompile(ctx, 0);
    /* [ function/err ] */
	return result;
}

/**
 * @func debug_read_cb
 * @brief Read callback for Duktape debug stream
 */
static duk_size_t debug_read_cb(void *udata, char *buffer, duk_size_t length)
{
    alt_fd *fd = (alt_fd *)udata;
    int result;

    if (length == 0) {
        return 0;
    }
    if (debug_has_push_back) {
        *buffer = debug_push_back_value;
        debug_has_push_back = 0;
        return 1;
    }
    result = (*fd->dev->read)(fd, buffer, length);
    return (result >= 0) ? result : 0;
}

/**
 * @func debug_write_cb
 * @brief Write callback for Duktape debug stream
 */
static duk_size_t debug_write_cb(void *udata, const char *buffer, duk_size_t length)
{
    alt_fd *fd = (alt_fd *)udata;
    int result = (*fd->dev->write)(fd, buffer, length);
    return (result >= 0) ? result : 0;
}

/**
 * @func debug_peek_cb
 * @brief Peek callback for Duktape debug stream
 */
static duk_size_t debug_peek_cb(void *udata)
{
    alt_fd *fd = (alt_fd *)udata;
    int result;
    if (!debug_has_push_back) {
        fd->fd_flags |= O_NONBLOCK;
        result = (*fd->dev->read)(fd, &debug_push_back_value, 1);
        if (result == 1) {
            debug_has_push_back = 1;
        }
        fd->fd_flags &= ~O_NONBLOCK;
    }
    return debug_has_push_back ? 1 : 0;
}

/**
 * @file runner
 * @brief Duktape runner for Rubic agent
 */
static int runner(const char *data, int flags, void *agent_context)
{
    duk_context *ctx;
    int result;
#ifdef AUTO_GC_CYCLES
    int autoGc = 0;
#endif

    // Initialize Duktape context
    ctx = duk_create_heap_default();
    if (!ctx) {
        return -ENOMEM;
    }

    // Initialize duktape-extension
    result = dux_initialize(ctx);
    if (result != 0) {
        return result;
    }

    // Attach debugger
    if ((flags & RUBIC_AGENT_RUNNER_FLAG_DEBUG) && (debug_stream_fd >= 0)) {
    	alt_fd *fd = &alt_fd_list[debug_stream_fd];
        duk_debugger_attach(ctx,
            debug_read_cb,
            debug_write_cb,
            debug_peek_cb,
            NULL,   // read_flush
            NULL,   // write_flush
            NULL,   // request
            NULL,   // detached
            fd
        );
    }

    // Compile
    result = compile_file(ctx, data, (flags & RUBIC_AGENT_RUNNER_FLAG_SOURCE));

    // Notify initialization complete
    if (rubic_agent_runner_notify_init(agent_context) != 0) {
        duk_destroy_heap(ctx);
        return 0;
    }

    // Run
    if ((result == 0) && (duk_pcall(ctx, 0) == 0)) {
        // Start tick
        while (dux_tick(ctx)) {
            rubic_agent_runner_cooperate(agent_context);
            if (rubic_agent_runner_query_abort(agent_context)) {
                break;
            }
#ifdef AUTO_GC_CYCLES
            if (--autoGc < 0) {
                autoGc = AUTO_GC_CYCLES;
                duk_gc(ctx, 0);
            }
#endif
        }
    } else {
        // Error
        fprintf(stderr, "Error: Compile error\n%s\n", duk_safe_to_string(ctx, -1));
    }
    duk_destroy_heap(ctx);
    return 0;
}

/**
 * @func runtime_duktape_init
 * @brief Initialize runtime for Duktape
 */
int runtime_duktape_init(void)
{
    return rubic_agent_register_runtime("duktape", DUK_GIT_DESCRIBE + 1, runner);
}
