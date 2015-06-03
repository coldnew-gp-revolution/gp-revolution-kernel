/*
 * platform_gt828.c: gt828 platform data initilization file
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
#include <linux/gt9xx.h>
#include "platform_gt9xx.h"

void *goodix9xx_platform_data(void *info)
{
	static struct gt9xx_platform_data gt9xx_pdata;

	gt9xx_pdata.numtouch       = 10;
	gt9xx_pdata.max_x          = 540;
	gt9xx_pdata.max_y          = 960;
	gt9xx_pdata.reset          = get_gpio_by_name("ts_rst");
	gt9xx_pdata.irq            = get_gpio_by_name("ts_int");

	return &gt9xx_pdata;
}
