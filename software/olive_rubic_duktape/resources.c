/*
 * resources.c
 *
 *  Created on: Oct 30, 2016
 *      Author: kimu_shu
 */

#include "peridot_i2c_master.h"
#include "peridot_spi_master.h"
#include "peridot_pfc_interface.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"

void *peridot_query_start_led(int *width, int *offset)
{
	alt_u32 addr = (alt_u32)IOADDR_ALTERA_AVALON_PIO_DATA(LED_BASE);
	*width = LED_DATA_WIDTH;
	*offset = 0;
	return (void *)(addr | (1u << 31));
}

const peridot_pfc_map_io i2c_scl_pfc_map =
{
	.out_funcs =
	{
		12,12,12,12,12,12,12,12, /* Bank 0 */
		12,12,12,12,12,12,12,12, /* Bank 1 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
	.in_bank = 1,
	.in_func = 4,
	.in_pins =
	{
		-1,-1, 2, 3, 4, 5, 6, 7, /* Bank 0 */
		 8, 9,10,11,12,13,14,15, /* Bank 1 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
};

const peridot_pfc_map_io i2c_sda_pfc_map =
{
	.out_funcs =
	{
		13,13,13,13,13,13,13,13, /* Bank 0 */
		13,13,13,13,13,13,13,13, /* Bank 1 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
	.in_bank = 1,
	.in_func = 5,
	.in_pins =
	{
		-1,-1, 2, 3, 4, 5, 6, 7, /* Bank 0 */
		 8, 9,10,11,12,13,14,15, /* Bank 1 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
};

extern peridot_i2c_master_state i2c;
const peridot_i2c_master_state *peridot_i2c_drivers[] =
{
	&i2c,
	NULL
};

static const peridot_pfc_map_out spi_ss_n_pfc_map =
{
	.out_funcs =
	{
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 0 */
		11,11,11,11,11,11,11,11, /* Bank 1 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
};

static const peridot_pfc_map_out spi_sclk_pfc_map =
{
	.out_funcs =
	{
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 0 */
		10,10,10,10,10,10,10,10, /* Bank 1 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
};

static const peridot_pfc_map_out spi_mosi_pfc_map =
{
	.out_funcs =
	{
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 0 */
		 9, 9, 9, 9, 9, 9, 9, 9, /* Bank 1 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
};

static const peridot_pfc_map_in spi_miso_pfc_map =
{
	.in_bank = 1,
	.in_func = 0,
	.in_pins =
	{
		-1,-1, 2, 4, 5, 6, 7, 8, /* Bank 0 */
		 8, 9,10,11,12,13,14,15, /* Bank 1 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
};

extern peridot_spi_master_state spi;
static const peridot_spi_master_pfc_map spi_map = {
	&spi,
	&spi_ss_n_pfc_map,
	&spi_sclk_pfc_map,
	&spi_mosi_pfc_map,
	&spi_miso_pfc_map,
};
const peridot_spi_master_pfc_map *peridot_spi_drivers[] =
{
	&spi_map,
	NULL
};

const peridot_pfc_map_out_ch servo_pwm_pfc_map =
{
	.out_funcs =
	{
		 4, 4, 4, 4, 4, 4, 4, 4, /* Bank 0 */
		 4, 4, 4, 4, 4, 4, 4, 4, /* Bank 1 */
		 4, 4, 4, 4, 4, 4,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
	.out_channels =
	{
		 0, 1, 2, 3, 4, 5, 6, 7, /* Bank 0 */
		 8, 9,10,11,12,13,14,15, /* Bank 1 */
		16,17,18,19,20,21,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
};

const peridot_pfc_map_out_ch servo_dsm_pfc_map =
{
	.out_funcs =
	{
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 0 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 1 */
		 5, 5, 5, 5, 5, 5,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
	.out_channels =
	{
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 0 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 1 */
		16,17,18,19,20,21,-1,-1, /* Bank 2 */
		-1,-1,-1,-1,-1,-1,-1,-1, /* Bank 3 */
	},
};
