/*
 * platform_ft5x06.c: ft5336 platform data initilization file
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
#include <asm/intel-mid.h>
#include <linux/i2c/ft5x06_ts-falcon.h>
#include "platform_ft5x06.h"

void *ft5x06_platform_data(void *info)
{
	static struct ft_ts_platform_data ft5x06_pdata;

	ft5x06_pdata.irq_gpio = get_gpio_by_name("ts_int");
	//ft5x06_pdata.power = ft_power,
	ft5x06_pdata.screen_max_x = SCALING_TOUCH_MAX_X_RESOLUTION,//540
	ft5x06_pdata.screen_max_y = SCALING_TOUCH_MAX_Y_RESOLUTION,//960
	ft5x06_pdata.reset = get_gpio_by_name("ts_rst");
	ft5x06_pdata.regulator_en = true;
	ft5x06_pdata.regulator_name = "vemmc2";

	return &ft5x06_pdata;
}
