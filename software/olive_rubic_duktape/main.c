/*
 * main.c
 *
 *  Created on: Apr 6, 2017
 *      Author: kimu_shu
 */

#include "system.h"
#include "rubic_agent.h"
//#include "epcs_fatfs.h" // for epcs_fatfs_format()

extern int runtime_duktape_init(void);

int debug_stream_fd = -1;

int main(void)
{
	runtime_duktape_init();
	rubic_agent_register_storage("internal", EPCS_FATFS_MOUNT_POINT);
	rubic_agent_service();
	return 0;
}
