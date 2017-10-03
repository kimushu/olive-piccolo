/**
 * @file runtime_duktape.c
 * @brief Runtime invoker for Duktape engine
 */

#include <unistd.h>
#include "duktape.h"
#include "dukext.h"
#include "bson.h"

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
 * @param filename File name to compile
 * @return Result of duk_pcompile*()
 */
static int compile_file(duk_context *ctx, const char *filename)
{
	int fd;
	char *src, *next;
	duk_size_t file_len, total_len, remainder;
	duk_int_t result;

	fd = open(filename, O_RDONLY);
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
	total_len = file_len + CJS_PROLOGUE_LEN + CJS_EPILOGUE_LEN;
	src = malloc(total_len);
	if (!src) {
		duk_push_error_object(ctx, DUK_ERR_ERROR, "Not enough memory");
		return -1;
	}
	next = src;
	memcpy(next, CJS_PROLOGUE, CJS_PROLOGUE_LEN);
	next += CJS_PROLOGUE_LEN;
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
	memcpy(next, CJS_EPILOGUE, CJS_EPILOGUE_LEN);

	duk_push_string(ctx, filename);
	result = duk_pcompile_lstring_filename(ctx, 0, src, total_len);
	free(src);
	return result;
}

duk_size_t debug_read_cb(void *udata, char *buffer, duk_size_t length)
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

duk_size_t debug_write_cb(void *udata, const char *buffer, duk_size_t length)
{
    alt_fd *fd = (alt_fd *)udata;
    int result = (*fd->dev->write)(fd, buffer, length);
    return (result >= 0) ? result : 0;
}

duk_size_t debug_peek_cb(void *udata)
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
 * @file runtime_duktape_run
 * @brief Duktape runner
 * @param request Request object (BSON)
 * @param file File name to run
 */
int runtime_duktape_run(const void *request, const char *file)
{
    duk_context *ctx;
    int off_debug = -1;
    int enable_debug = 0;

    bson_get_props(request, "debug", &off_debug, NULL);
    enable_debug = bson_get_boolean(request, off_debug, 0);

    // Initialize Duktape context
    ctx = duk_create_heap_default();

    // Initialize duktape-extension
    dux_initialize(ctx);

    // Attach debugger
    if (enable_debug) {
        duk_debugger_attach(ctx,
            debug_read_cb,
            debug_write_cb,
            debug_peek_cb,
            NULL,   // read_flush
            NULL,   // write_flush
            NULL,   // request
            NULL,   // detached

        );
    }

    // Compile and run
    if (compile_file(ctx, file) == 0 && duk_pcall(ctx, 0) == 0) {
        // Start tick
        while (dux_tick(ctx)) {
            const void *intr = rubic_agent_has_interrupt();
            if (intr) {
            }
        }
    } else {
        // Error
        fprintf(stderr, "Error: Compile error\n%s\n", duk_safe_to_string(ctx, -1));
    }
    duk_destroy_heap(ctx);

}