/*
 * platform_apds990x.c: apds990x platform data initilization file
 *
 * (C) Copyright 2008 Intel Corporation
 * Author:
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/gpio.h>
#include <linux/lnw_gpio.h>
#include <linux/i2c/lsm303d.h>
#include <asm/intel-mid.h>
#include "platform_lsm303d.h"

static struct lsm303d_mag_platform_data lsm303d_mag_platdata = {
	.poll_interval = 100,
	.min_interval = LSM303D_MAG_MIN_POLL_PERIOD_MS,
	.fs_range = LSM303D_MAG_FS_2G,
	.axis_map_x = 0,
	.axis_map_y = 1,
	.axis_map_z = 2,
	.negate_x = 0,
	.negate_y = 0,
	.negate_z = 0,
};

static struct lsm303d_acc_platform_data lsm303d_acc_platdata = {
	.fs_range = LSM303D_ACC_FS_2G,
	.axis_map_x = 0,
	.axis_map_y = 1,
	.axis_map_z = 2,
	.negate_x = 0,
	.negate_y = 0,
	.negate_z = 0,
	.poll_interval = 100,
	.min_interval = LSM303D_ACC_MIN_POLL_PERIOD_MS,
	.aa_filter_bandwidth = ANTI_ALIASING_773,
	.gpio_int1 = DEFAULT_INT1_GPIO,
	.gpio_int2 = DEFAULT_INT2_GPIO,
};

void *lsm303d_platform_data(void *info)
{
	static struct lsm303d_main_platform_data lsm303d_main_platdata = {
		.pdata_acc = &lsm303d_acc_platdata,
		.pdata_mag = &lsm303d_mag_platdata,
	};
	return &lsm303d_main_platdata;
}
