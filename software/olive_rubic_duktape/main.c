/*
 * main.c
 *
 *  Created on: Apr 6, 2017
 *      Author: kimu_shu
 */

#include "system.h"
#include "rubic_agent.h"
#include "peridot_sw_hostbridge_gen2.h"
#include <unistd.h>
#include <sys/fcntl.h>

extern int runtime_duktape_init(void);

extern void prog_init();
extern int prog_blocksize(const char *area, void *user_data);
extern int prog_reader(const char *area, void *user_data, int offset, void *ptr, int len);
extern int prog_writer(const char *area, void *user_data, int offset, const void *ptr, int len);

int debug_stream_fd;

int main(void)
{
	// Make pipes for stdio
	peridot_sw_hostbridge_gen2_mkpipe(2, STDOUT_FILENO, STDIN_FILENO, 1024);
	peridot_sw_hostbridge_gen2_mkpipe(3, STDERR_FILENO, -1, 0);

	// Make pipe for debug stream
	debug_stream_fd = open("/dev/null", 0);
	peridot_sw_hostbridge_gen2_mkpipe(4, debug_stream_fd, debug_stream_fd, 1024);

	// Register runtimes
	runtime_duktape_init();

	// Register storages
	rubic_agent_register_storage("internal", EPCS_FATFS_MOUNT_POINT);

	// Register firmware updater
	prog_init();
	rubic_agent_register_programmer(prog_blocksize, prog_reader, prog_writer, NULL);

	// Start rubic agent
	rubic_agent_service(0);
	return 0;
}
