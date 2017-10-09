/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_fast' in SOPC Builder design 'olive_std_core'
 * SOPC Builder design path: ../../olive_std_core.sopcinfo
 *
 * Generated: Mon Oct 09 17:49:40 JST 2017
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

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x0fff0020
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 100000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x1d
#define ALT_CPU_DCACHE_BYPASS_MASK 0x80000000
#define ALT_CPU_DCACHE_LINE_SIZE 32
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_DCACHE_SIZE 8192
#define ALT_CPU_EXCEPTION_ADDR 0x00000020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 100000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 1
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_DIVISION_ERROR_EXCEPTION
#define ALT_CPU_HAS_EXTRA_EXCEPTION_INFO
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 16384
#define ALT_CPU_INITDA_SUPPORTED
#define ALT_CPU_INST_ADDR_WIDTH 0x1c
#define ALT_CPU_NAME "nios2_fast"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 7
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x0f800000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x0fff0020
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 100000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x1d
#define NIOS2_DCACHE_BYPASS_MASK 0x80000000
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 8192
#define NIOS2_EXCEPTION_ADDR 0x00000020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 1
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_DIVISION_ERROR_EXCEPTION
#define NIOS2_HAS_EXTRA_EXCEPTION_INFO
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 16384
#define NIOS2_INITDA_SUPPORTED
#define NIOS2_INST_ADDR_WIDTH 0x1c
#define NIOS2_NUM_OF_SHADOW_REG_SETS 7
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x0f800000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTCHIP_ID_AVM_WRAPPER
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SYSID_QSYS
#define __ALTERA_AVALON_TIMER
#define __ALTERA_AVALON_UART
#define __ALTERA_DUAL_BOOT
#define __ALTERA_NIOS2_GEN2
#define __ALTERA_ONCHIP_FLASH
#define __BUFFERED_UART
#define __PERIDOT_I2C_MASTER
#define __PERIDOT_PFC_INTERFACE
#define __PERIDOT_SERVO
#define __PERIDOT_SPI_MASTER


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "MAX 10"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/null"
#define ALT_STDERR_BASE 0x0
#define ALT_STDERR_DEV null
#define ALT_STDERR_TYPE ""
#define ALT_STDIN "/dev/null"
#define ALT_STDIN_BASE 0x0
#define ALT_STDIN_DEV null
#define ALT_STDIN_TYPE ""
#define ALT_STDOUT "/dev/null"
#define ALT_STDOUT_BASE 0x0
#define ALT_STDOUT_DEV null
#define ALT_STDOUT_TYPE ""
#define ALT_SYSTEM_NAME "olive_std_core"


/*
 * chipid configuration
 *
 */

#define ALT_MODULE_CLASS_chipid altchip_id_avm_wrapper
#define CHIPID_BASE 0x10000008
#define CHIPID_IRQ -1
#define CHIPID_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CHIPID_NAME "/dev/chipid"
#define CHIPID_SPAN 8
#define CHIPID_STATUS_BIT 0
#define CHIPID_TYPE "altchip_id_avm_wrapper"
#define CHIPID_WAIT_MASTER 1
#define CHIPID_ZERO_INVALID 0


/*
 * dual_boot_0 configuration
 *
 */

#define ALT_MODULE_CLASS_dual_boot_0 altera_dual_boot
#define DUAL_BOOT_0_BASE 0x10000060
#define DUAL_BOOT_0_IRQ -1
#define DUAL_BOOT_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define DUAL_BOOT_0_NAME "/dev/dual_boot_0"
#define DUAL_BOOT_0_SPAN 32
#define DUAL_BOOT_0_TYPE "altera_dual_boot"


/*
 * epcs configuration
 *
 */

#define ALT_MODULE_CLASS_epcs peridot_spi_master
#define EPCS_BASE 0xff00000
#define EPCS_FREQ 100000000
#define EPCS_IRQ 2
#define EPCS_IRQ_INTERRUPT_CONTROLLER_ID 0
#define EPCS_NAME "/dev/epcs"
#define EPCS_SPAN 8
#define EPCS_TYPE "peridot_spi_master"


/*
 * epcs_fatfs configuration
 *
 */

#define EPCS_FATFS_FLASH_CMD_ERASE 0x20
#define EPCS_FATFS_FLASH_END 0x0
#define EPCS_FATFS_FLASH_SECTOR 4096
#define EPCS_FATFS_FLASH_START 0x80000
#define EPCS_FATFS_IF_INST_NAME 0
#define EPCS_FATFS_IF_PERIDOT_SPI_FLASH
#define EPCS_FATFS_IF_SPI_SLAVE 0
#define EPCS_FATFS_MOUNT_POINT "/mnt/internal"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 32
#define ALT_SYS_CLK SYSTIMER
#define ALT_TIMESTAMP_CLK none


/*
 * hostbridge configuration
 *
 */

#define ALT_MODULE_CLASS_hostbridge buffered_uart
#define HOSTBRIDGE_BASE 0x10000010
#define HOSTBRIDGE_DATA_BITS 8
#define HOSTBRIDGE_DIVIDER_BITS 6
#define HOSTBRIDGE_FIXED_BAUD 1
#define HOSTBRIDGE_IRQ 1
#define HOSTBRIDGE_IRQ_INTERRUPT_CONTROLLER_ID 0
#define HOSTBRIDGE_NAME "/dev/hostbridge"
#define HOSTBRIDGE_RX_DEPTH_BITS 10
#define HOSTBRIDGE_SPAN 16
#define HOSTBRIDGE_TX_DEPTH_BITS 10
#define HOSTBRIDGE_TYPE "buffered_uart"


/*
 * i2c configuration
 *
 */

#define ALT_MODULE_CLASS_i2c peridot_i2c_master
#define I2C_BASE 0x10000140
#define I2C_FREQ 25000000
#define I2C_IRQ 5
#define I2C_IRQ_INTERRUPT_CONTROLLER_ID 0
#define I2C_NAME "/dev/i2c"
#define I2C_SPAN 8
#define I2C_TYPE "peridot_i2c_master"


/*
 * led configuration
 *
 */

#define ALT_MODULE_CLASS_led altera_avalon_pio
#define LED_BASE 0x10000040
#define LED_BIT_CLEARING_EDGE_REGISTER 0
#define LED_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LED_CAPTURE 0
#define LED_DATA_WIDTH 1
#define LED_DO_TEST_BENCH_WIRING 0
#define LED_DRIVEN_SIM_VALUE 0
#define LED_EDGE_TYPE "NONE"
#define LED_FREQ 25000000
#define LED_HAS_IN 0
#define LED_HAS_OUT 1
#define LED_HAS_TRI 0
#define LED_IRQ -1
#define LED_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LED_IRQ_TYPE "NONE"
#define LED_NAME "/dev/led"
#define LED_RESET_VALUE 0
#define LED_SPAN 16
#define LED_TYPE "altera_avalon_pio"


/*
 * named_fifo configuration
 *
 */

#define NAMED_FIFO_STDERR_ENABLE 1
#define NAMED_FIFO_STDERR_NAME "/dev/stderr"
#define NAMED_FIFO_STDERR_SIZE 1024
#define NAMED_FIFO_STDIN_ENABLE 1
#define NAMED_FIFO_STDIN_NAME "/dev/stdin"
#define NAMED_FIFO_STDIN_SIZE 1024
#define NAMED_FIFO_STDIO_INIT_OPENED 0
#define NAMED_FIFO_STDOUT_ENABLE 1
#define NAMED_FIFO_STDOUT_NAME "/dev/stdout"
#define NAMED_FIFO_STDOUT_SIZE 1024


/*
 * peridot_client_fs configuration
 *
 */

#define PERIDOT_CLIENT_FS_MAX_FDS 16
#define PERIDOT_CLIENT_FS_RO_PATH ""
#define PERIDOT_CLIENT_FS_RW_PATH "/mnt/internal/"
#define PERIDOT_CLIENT_FS_WO_PATH ""


/*
 * peridot_i2c_master_driver configuration
 *
 */

#define I2C_DRIVER_INSTANCE ({ extern peridot_i2c_master_state i2c; &i2c; })


/*
 * peridot_rpc_server configuration
 *
 */

#define PERIDOT_RPCSRV_CHANNEL 1
#define PERIDOT_RPCSRV_MAX_REQUEST_LENGTH 65536
#define PERIDOT_RPCSRV_WORKER_THREADS 1


/*
 * peridot_spi_master_driver configuration
 *
 */

#define EPCS_DRIVER_INSTANCE ({ extern peridot_spi_master_state epcs; &epcs; })
#define PERIDOT_SPI_FLASH_BITRATE 40000000
#define PERIDOT_SPI_FLASH_BOOT_DECOMPRESS_LZ4
#define PERIDOT_SPI_FLASH_BOOT_NAME epcs
#define PERIDOT_SPI_FLASH_BOOT_OFFSET 0x0
#define PERIDOT_SPI_FLASH_ENABLE
#define PERIDOT_SPI_FLASH_NAME epcs
#define PERIDOT_SPI_FLASH_SLAVE_NUMBER 0
#define PERIDOT_SPI_MASTER_DRIVER_FLASH_CLKDIV 1
#define PERIDOT_SPI_MASTER_DRIVER_FLASH_INSTANCE epcs
#define SPI_DRIVER_INSTANCE ({ extern peridot_spi_master_state spi; &spi; })


/*
 * peridot_sw_hostbridge_gen2 configuration
 *
 */

#define PERIDOT_SW_HOSTBRIDGE_GEN2_USE_RECEIVER_THREAD


/*
 * pfc configuration
 *
 */

#define ALT_MODULE_CLASS_pfc peridot_pfc_interface
#define PFC_BASE 0x10000080
#define PFC_IRQ -1
#define PFC_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PFC_NAME "/dev/pfc"
#define PFC_SPAN 64
#define PFC_TYPE "peridot_pfc_interface"


/*
 * rubic_agent configuration
 *
 */

#define RUBIC_AGENT_MAX_RUNTIMES 1
#define RUBIC_AGENT_MAX_STORAGES 1
#define RUBIC_AGENT_RUBIC_VERSION ">=0.99.100"
#define RUBIC_AGENT_WORKER_THREADS 1


/*
 * sdram configuration
 *
 */

#define ALT_MODULE_CLASS_sdram altera_avalon_new_sdram_controller
#define SDRAM_BASE 0x0
#define SDRAM_CAS_LATENCY 2
#define SDRAM_CONTENTS_INFO
#define SDRAM_INIT_NOP_DELAY 0.0
#define SDRAM_INIT_REFRESH_COMMANDS 2
#define SDRAM_IRQ -1
#define SDRAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_IS_INITIALIZED 1
#define SDRAM_NAME "/dev/sdram"
#define SDRAM_POWERUP_DELAY 200.0
#define SDRAM_REFRESH_PERIOD 15.625
#define SDRAM_REGISTER_DATA_IN 1
#define SDRAM_SDRAM_ADDR_WIDTH 0x16
#define SDRAM_SDRAM_BANK_WIDTH 2
#define SDRAM_SDRAM_COL_WIDTH 8
#define SDRAM_SDRAM_DATA_WIDTH 16
#define SDRAM_SDRAM_NUM_BANKS 4
#define SDRAM_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_SDRAM_ROW_WIDTH 12
#define SDRAM_SHARED_DATA 0
#define SDRAM_SIM_MODEL_BASE 0
#define SDRAM_SPAN 8388608
#define SDRAM_STARVATION_INDICATOR 0
#define SDRAM_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_T_AC 6.0
#define SDRAM_T_MRD 3
#define SDRAM_T_RCD 21.0
#define SDRAM_T_RFC 63.0
#define SDRAM_T_RP 21.0
#define SDRAM_T_WR 14.0


/*
 * servo configuration
 *
 */

#define ALT_MODULE_CLASS_servo peridot_servo
#define SERVO_BASE 0x10000180
#define SERVO_CHANNEL 8
#define SERVO_FREQ 25000000
#define SERVO_IRQ -1
#define SERVO_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SERVO_NAME "/dev/servo"
#define SERVO_SPAN 128
#define SERVO_TYPE "peridot_servo"


/*
 * spi configuration
 *
 */

#define ALT_MODULE_CLASS_spi peridot_spi_master
#define SPI_BASE 0x10000160
#define SPI_FREQ 25000000
#define SPI_IRQ 4
#define SPI_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SPI_NAME "/dev/spi"
#define SPI_SPAN 8
#define SPI_TYPE "peridot_spi_master"


/*
 * sysid configuration
 *
 */

#define ALT_MODULE_CLASS_sysid altera_avalon_sysid_qsys
#define SYSID_BASE 0x10000000
#define SYSID_ID 1923109377
#define SYSID_IRQ -1
#define SYSID_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSID_NAME "/dev/sysid"
#define SYSID_SPAN 8
#define SYSID_TIMESTAMP 1507084121
#define SYSID_TYPE "altera_avalon_sysid_qsys"


/*
 * systimer configuration
 *
 */

#define ALT_MODULE_CLASS_systimer altera_avalon_timer
#define SYSTIMER_ALWAYS_RUN 0
#define SYSTIMER_BASE 0x10000020
#define SYSTIMER_COUNTER_SIZE 32
#define SYSTIMER_FIXED_PERIOD 0
#define SYSTIMER_FREQ 25000000
#define SYSTIMER_IRQ 0
#define SYSTIMER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SYSTIMER_LOAD_VALUE 24999
#define SYSTIMER_MULT 0.001
#define SYSTIMER_NAME "/dev/systimer"
#define SYSTIMER_PERIOD 1
#define SYSTIMER_PERIOD_UNITS "ms"
#define SYSTIMER_RESET_OUTPUT 0
#define SYSTIMER_SNAPSHOT 1
#define SYSTIMER_SPAN 32
#define SYSTIMER_TICKS_PER_SEC 1000
#define SYSTIMER_TIMEOUT_PULSE_OUTPUT 0
#define SYSTIMER_TYPE "altera_avalon_timer"


/*
 * tinyth configuration
 *
 */

#define PTHREAD_STACK_MIN_OVERRIDE 0x1000
#define SCHED_POLICY_DEFAULT_FF 0
#define SCHED_PRIORITY_DEFAULT 10
#define SCHED_PRIORITY_MAX 99
#define SCHED_PRIORITY_MIN 1
#define TTHREAD_ENABLE_COND 1
#define TTHREAD_ENABLE_MUTEX 1
#define TTHREAD_ENABLE_ONCE 1
#define TTHREAD_ENABLE_PROF 0
#define TTHREAD_ENABLE_RWLOCK 1
#define TTHREAD_ENABLE_SEM 1
#define TTHREAD_ENABLE_SLEEP 1
#define TTHREAD_ENABLE_SPIN 1
#define TTHREAD_PREEMPTION_ENABLE 1
#define TTHREAD_PREEMPTION_INTERVAL 10
#define TTHREAD_THREAD_SAFE_NEWLIB 1
#define TTHREAD_TICKS_PER_SEC SYSTIMER_TICKS_PER_SEC


/*
 * uart0 configuration
 *
 */

#define ALT_MODULE_CLASS_uart0 altera_avalon_uart
#define UART0_BASE 0x10000100
#define UART0_BAUD 115200
#define UART0_DATA_BITS 8
#define UART0_FIXED_BAUD 0
#define UART0_FREQ 25000000
#define UART0_IRQ 8
#define UART0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define UART0_NAME "/dev/uart0"
#define UART0_PARITY 'N'
#define UART0_SIM_CHAR_STREAM ""
#define UART0_SIM_TRUE_BAUD 0
#define UART0_SPAN 32
#define UART0_STOP_BITS 1
#define UART0_SYNC_REG_DEPTH 2
#define UART0_TYPE "altera_avalon_uart"
#define UART0_USE_CTS_RTS 1
#define UART0_USE_EOP_REGISTER 0


/*
 * uart1 configuration
 *
 */

#define ALT_MODULE_CLASS_uart1 altera_avalon_uart
#define UART1_BASE 0x10000120
#define UART1_BAUD 115200
#define UART1_DATA_BITS 8
#define UART1_FIXED_BAUD 0
#define UART1_FREQ 25000000
#define UART1_IRQ 9
#define UART1_IRQ_INTERRUPT_CONTROLLER_ID 0
#define UART1_NAME "/dev/uart1"
#define UART1_PARITY 'N'
#define UART1_SIM_CHAR_STREAM ""
#define UART1_SIM_TRUE_BAUD 0
#define UART1_SPAN 32
#define UART1_STOP_BITS 1
#define UART1_SYNC_REG_DEPTH 2
#define UART1_TYPE "altera_avalon_uart"
#define UART1_USE_CTS_RTS 1
#define UART1_USE_EOP_REGISTER 0


/*
 * ufm configuration
 *
 */

#define ALT_MODULE_CLASS_ufm altera_onchip_flash
#define UFM_BASE 0xf800000
#define UFM_BYTES_PER_PAGE 2048
#define UFM_IRQ -1
#define UFM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define UFM_NAME "/dev/ufm"
#define UFM_READ_ONLY_MODE 1
#define UFM_SECTOR1_ENABLED 1
#define UFM_SECTOR1_END_ADDR 0x3fff
#define UFM_SECTOR1_START_ADDR 0
#define UFM_SECTOR2_ENABLED 1
#define UFM_SECTOR2_END_ADDR 0x7fff
#define UFM_SECTOR2_START_ADDR 0x4000
#define UFM_SECTOR3_ENABLED 0
#define UFM_SECTOR3_END_ADDR 0xffffffff
#define UFM_SECTOR3_START_ADDR 0xffffffff
#define UFM_SECTOR4_ENABLED 0
#define UFM_SECTOR4_END_ADDR 0xffffffff
#define UFM_SECTOR4_START_ADDR 0xffffffff
#define UFM_SECTOR5_ENABLED 0
#define UFM_SECTOR5_END_ADDR 0xffffffff
#define UFM_SECTOR5_START_ADDR 0xffffffff
#define UFM_SPAN 32768
#define UFM_TYPE "altera_onchip_flash"

#endif /* __SYSTEM_H_ */
