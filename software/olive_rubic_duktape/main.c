/*
 * main.c
 *
 *  Created on: Apr 6, 2017
 *      Author: kimu_shu
 */

#include "duktape.h"
#include "dukext.h"
#include <unistd.h>
#include <fcntl.h>

#include "named_fifo.h"
#include "rubic_agent.h"
#include "bson.h"
#include "epcs_fatfs.h"

static const char CJS_PROLOGUE[] = "(function(require,module,exports){";
static const int CJS_PROLOGUE_LEN = sizeof(CJS_PROLOGUE) - 1;
static const char CJS_EPILOGUE[] = "\n})(require,m={exports:{}},m.exports)";
static const int CJS_EPILOGUE_LEN = sizeof(CJS_EPILOGUE) - 1;

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

int main(void)
{
	for (;;) {
		const char *request;
		const void *params = rubic_agent_wait_request(&request, NULL);
		int off_target, off_debug, enable_debug;
		const char *target;
		
		if (bson_get_props(params, "target", &off_target, "debug", &off_debug, NULL) < 0) {
			goto invalid_params;
		}
		target = bson_get_string(params, off_target, NULL);
		if (!target) {
			goto invalid_params;
		}
		enable_debug = bson_get_boolean(params, off_debug, 0);

		if (strcmp(request, "start") == 0) {
			duk_context *ctx = duk_create_heap_default();
			dux_initialize(ctx);
			if (compile_file(ctx, target) == 0 && duk_pcall(ctx, 0) == 0) {
				// Start tick
				while (!rubic_agent_is_aborting() && dux_tick(ctx));
			} else {
				// Error
				fprintf(stderr, "Error: Compile error\n%s\n", duk_safe_to_string(ctx, -1));
			}
			duk_destroy_heap(ctx);
		} else if (strcmp(request, "format") == 0) {
			epcs_fatfs_format();
		} else {
			// unknown request
		}

invalid_params:
		rubic_agent_finish_request();
	}
	return 0;
}
