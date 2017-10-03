/*
 * main.c
 *
 *  Created on: Apr 6, 2017
 *      Author: kimu_shu
 */

#include <unistd.h>
#include <fcntl.h>

#include "named_fifo.h"
#include "rubic_agent.h"
#include "bson.h"
#include "epcs_fatfs.h"

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
