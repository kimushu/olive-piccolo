/*
 * alt_sys_init.c - HAL initialization source
 *
 * Machine generated for CPU 'nios2_fast' in SOPC Builder design 'olive_std_core'
 * SOPC Builder design path: ../../olive_std_core.sopcinfo
 *
 * Generated: Mon Oct 23 16:41:24 JST 2017
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#include "system.h"
#include "sys/alt_irq.h"
#include "sys/alt_sys_init.h"

#include <stddef.h>

/*
 * Device headers
 */

#include "altera_nios2_gen2_irq.h"
#include "altera_avalon_sysid_qsys.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_uart.h"
#include "altera_onchip_flash.h"
#include "buffered_uart.h"
#include "digests.h"
#include "epcs_fatfs.h"
#include "named_fifo.h"
#include "peridot_client_fs.h"
#include "peridot_i2c_master.h"
#include "peridot_pfc_interface.h"
#include "peridot_rpc_server.h"
#include "peridot_servo.h"
#include "peridot_spi_master.h"
#include "peridot_sw_hostbridge_gen2.h"
#include "rubic_agent.h"

/*
 * Allocate the device storage
 */

ALTERA_NIOS2_GEN2_IRQ_INSTANCE ( NIOS2_FAST, nios2_fast);
ALTERA_AVALON_SYSID_QSYS_INSTANCE ( SYSID, sysid);
ALTERA_AVALON_TIMER_INSTANCE ( SYSTIMER, systimer);
ALTERA_AVALON_UART_INSTANCE ( UART0, uart0);
ALTERA_AVALON_UART_INSTANCE ( UART1, uart1);
ALTERA_ONCHIP_FLASH_DATA_CSR_INSTANCE ( UFM, UFM_DATA, UFM_CSR, ufm);
BUFFERED_UART_INSTANCE ( HOSTBRIDGE, hostbridge);
DIGESTS_INSTANCE ( DIGESTS, digests);
EPCS_FATFS_INSTANCE ( EPCS_FATFS, epcs_fatfs);
NAMED_FIFO_INSTANCE ( NAMED_FIFO, named_fifo);
PERIDOT_CLIENT_FS_INSTANCE ( PERIDOT_CLIENT_FS, peridot_client_fs);
PERIDOT_I2C_MASTER_INSTANCE ( I2C, i2c);
PERIDOT_PFC_INTERFACE_INSTANCE ( PFC, pfc);
PERIDOT_RPC_SERVER_INSTANCE ( PERIDOT_RPC_SERVER, peridot_rpc_server);
PERIDOT_SERVO_INSTANCE ( SERVO, servo);
PERIDOT_SPI_MASTER_INSTANCE ( EPCS, epcs);
PERIDOT_SPI_MASTER_INSTANCE ( SPI, spi);
PERIDOT_SW_HOSTBRIDGE_GEN2_INSTANCE ( PERIDOT_SW_HOSTBRIDGE_GEN2, peridot_sw_hostbridge_gen2);
RUBIC_AGENT_INSTANCE ( RUBIC_AGENT, rubic_agent);

/*
 * Initialize the interrupt controller devices
 * and then enable interrupts in the CPU.
 * Called before alt_sys_init().
 * The "base" parameter is ignored and only
 * present for backwards-compatibility.
 */

void alt_irq_init ( const void* base )
{
    ALTERA_NIOS2_GEN2_IRQ_INIT ( NIOS2_FAST, nios2_fast);
    alt_irq_cpu_enable_interrupts();
}

/*
 * Initialize the non-interrupt controller devices.
 * Called after alt_irq_init().
 */

void alt_sys_init( void )
{
    ALTERA_AVALON_TIMER_INIT ( SYSTIMER, systimer);
    DIGESTS_INIT ( DIGESTS, digests);
    ALTERA_AVALON_SYSID_QSYS_INIT ( SYSID, sysid);
    ALTERA_AVALON_UART_INIT ( UART0, uart0);
    ALTERA_AVALON_UART_INIT ( UART1, uart1);
    ALTERA_ONCHIP_FLASH_INIT ( UFM, ufm);
    BUFFERED_UART_INIT ( HOSTBRIDGE, hostbridge);
    EPCS_FATFS_INIT ( EPCS_FATFS, epcs_fatfs);
    NAMED_FIFO_INIT ( NAMED_FIFO, named_fifo);
    PERIDOT_I2C_MASTER_INIT ( I2C, i2c);
    PERIDOT_PFC_INTERFACE_INIT ( PFC, pfc);
    PERIDOT_SERVO_INIT ( SERVO, servo);
    PERIDOT_SPI_MASTER_INIT ( EPCS, epcs);
    PERIDOT_SPI_MASTER_INIT ( SPI, spi);
    PERIDOT_SW_HOSTBRIDGE_GEN2_INIT ( PERIDOT_SW_HOSTBRIDGE_GEN2, peridot_sw_hostbridge_gen2);
    RUBIC_AGENT_INIT ( RUBIC_AGENT, rubic_agent);
    PERIDOT_RPC_SERVER_INIT ( PERIDOT_RPC_SERVER, peridot_rpc_server);
    PERIDOT_CLIENT_FS_INIT ( PERIDOT_CLIENT_FS, peridot_client_fs);
}
