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

extern int debug_stream_fd;

static duk_ret_t file_reader(duk_context *ctx, const char *path);

static const dux_file_accessor file_accessor = {
    .reader = file_reader,
};

char debug_has_push_back;
char debug_push_back_value;

/**
 * @func file_reader
 * @brief Read file
 */
static duk_ret_t file_reader(duk_context *ctx, const char *path)
{
    /* [ ... ] */
    int fd;
    duk_size_t file_len, remainder;
    char *next;

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        if (errno == ENOENT) {
            duk_push_error_object(ctx, DUK_ERR_ERROR, "File not found: %s", path);
        } else {
            duk_push_error_object(ctx, DUK_ERR_ERROR, "Cannot open file: %s", path);
        }
        return DUK_EXEC_ERROR;
    }
    file_len = lseek(fd, 0, SEEK_END);
    if (file_len == ((duk_size_t)-1)) {
        duk_push_error_object(ctx, DUK_ERR_ERROR, "Cannot seek file");
        close(fd);
        return DUK_EXEC_ERROR;
    }
    lseek(fd, 0, SEEK_SET);
    next = (char *)duk_push_fixed_buffer(ctx, file_len);
    /* [ ... buffer ] */
    for (remainder = file_len; remainder > 0;) {
        int done = read(fd, next, remainder);
        if (done < 0) {
            close(fd);
            duk_pop(ctx);
            duk_push_error_object(ctx, DUK_ERR_ERROR, "Cannot read file: %s", path);
            return DUK_EXEC_ERROR;
        }
        next += done;
        remainder -= done;
    }
    close(fd);

    duk_buffer_to_string(ctx, -1);
    /* [ ... string ] */
    return DUK_EXEC_SUCCESS;
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
    result = dux_initialize(ctx, &file_accessor);
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

    // Notify initialization complete
    if (rubic_agent_runner_notify_init(agent_context) != 0) {
        duk_destroy_heap(ctx);
        return 0;
    }

    if (flags & RUBIC_AGENT_RUNNER_FLAG_SOURCE) {
        result = dux_peval_module_string(ctx, data);
    } else {
        result = dux_peval_module_file(ctx, data);
    }

    // Run
    if (result == DUK_EXEC_SUCCESS) {
        // Drop result
        duk_pop(ctx);

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
        // Report error
        duk_get_prop_string(ctx, -1, "stack");
        if (!duk_is_string(ctx, -1)) {
            duk_pop(ctx);
        }
        fprintf(stderr, "%s\n", duk_safe_to_string(ctx, -1));
    }
    duk_destroy_heap(ctx);
    return result;
}

/**
 * @func runtime_duktape_init
 * @brief Initialize runtime for Duktape
 */
int runtime_duktape_init(void)
{
    return rubic_agent_register_runtime("duktape", DUK_GIT_DESCRIBE + 1, runner);
}
