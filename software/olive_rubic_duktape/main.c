/*
 * main.c
 *
 *  Created on: Apr 6, 2017
 *      Author: kimu_shu
 */

#include "duktape.h"
#include "dux.h"
#include <unistd.h>
#include <fcntl.h>

#include "rubic_agent.h"
#include "epcs_fatfs.h"

volatile int abort_request = 0;

static void set_abort_request(int reason)
{
	abort_request = 1;
}

static int compile_file(duk_context *ctx, const char *filename)
{
	int fd;
	char *src, *next;
	duk_size_t len, remainder;
	duk_int_t result;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		duk_push_error_object(ctx, DUK_ERR_ERROR, "Cannot open file");
		return -1;
	}
	len = lseek(fd, 0, SEEK_END);
	if (len == ((duk_size_t)-1)) {
		duk_push_error_object(ctx, DUK_ERR_ERROR, "Cannot seek file");
		return -1;
	}
	lseek(fd, 0, SEEK_SET);
	src = malloc(len);
	if (!src) {
		duk_push_error_object(ctx, DUK_ERR_ERROR, "Not enough memory");
		return -1;
	}
	next = src;
	for (remainder = len; remainder > 0;) {
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

	duk_push_string(ctx, filename);
	result = duk_pcompile_lstring_filename(ctx, 0, src, len);
	free(src);
	return result;
}

int main(void)
{
	rubic_agent_set_interrupt_handler(set_abort_request);

	for (;;) {
		rubic_agent_message msg;

		abort_request = 0;
		rubic_agent_wait_request(&msg);

		if (msg.request == RUBIC_AGENT_REQUEST_START) {
			duk_context *ctx = duk_create_heap_default();
			dux_initialize(ctx);
			if (compile_file(ctx, msg.body.start.program) == 0 && duk_pcall(ctx, 0) == 0) {
				// Start tick
				while (!abort_request && dux_tick(ctx));
			} else {
				// Error
			}
			duk_destroy_heap(ctx);
		} else if (msg.request == RUBIC_AGENT_REQUEST_FORMAT) {
			epcs_fatfs_format();
		}

		rubic_agent_send_response(&msg);
	}
	return 0;
}
