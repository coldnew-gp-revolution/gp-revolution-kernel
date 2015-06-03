/*
 * platform_ov9724.c: ov9724 platform data initilization file
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
#include <linux/delay.h>
#include <linux/atomisp_platform.h>
#include <asm/intel_scu_ipcutil.h>
#include <asm/intel-mid.h>
#include <media/v4l2-subdev.h>
#include "platform_camera.h"
#include "platform_ov9724.h"


static int camera_reset;
static int camera_power_down;
static int subcamera_power_en;

/*
 * MRFLD VV secondary camera sensor - OV9724 platform data
 */

static int ov9724_gpio_ctrl(struct v4l2_subdev *sd, int flag)
{
	/*
	int ret;

	if (camera_reset < 0) {
		ret = camera_sensor_gpio(-1, GP_CAMERA_1_RESET,
					GPIOF_DIR_OUT, 1);
		if (ret < 0)
			return ret;
		camera_reset = ret;
	}

	if (flag) {
		//gpio_set_value(camera_power_down, 1);
		//usleep_range(1500, 1500);
	} else {
		//gpio_set_value(camera_reset, 0);
	}
	*/
	return 0;
}

static int ov9724_flisclk_ctrl(struct v4l2_subdev *sd, int flag)
{
	static const unsigned int clock_khz = 19200;
	return intel_scu_ipc_osc_clk(OSC_CLK_CAM1, flag ? clock_khz : 0);
}

static int ov9724_power_ctrl(struct v4l2_subdev *sd, int flag)
{
	int ret = 0;
	/*
	if (camera_power_down < 0) {
		ret = camera_sensor_gpio(-1, GP_CAMERA_1_POWER_DOWN,
					GPIOF_DIR_OUT, 0);
		if (ret < 0)
			return ret;
		camera_power_down = ret;
		}
	if (camera_reset < 0) {
		ret = camera_sensor_gpio(-1, GP_CAMERA_1_RESET,
					GPIOF_DIR_OUT, 0);
		if (ret < 0)
			return ret;
		camera_reset = ret;
		}
	*/
	if (subcamera_power_en < 0) {
		ret = camera_sensor_gpio(-1, GP_SUBCAMERA_EN,
					 GPIOF_DIR_OUT, 0);
		if (ret < 0)
			return ret;
		subcamera_power_en = ret;
		}
	if (flag) {
	//	gpio_set_value(camera_power_down,1);
	//	gpio_set_value(camera_reset, 1);
	//	mdelay(1);
		gpio_set_value(subcamera_power_en, 1);
		mdelay(2);
			} else {
		gpio_set_value(subcamera_power_en, 0);
	//	gpio_set_value(camera_power_down, 0);
		}
	return 0;
}


static int ov9724_csi_configure(struct v4l2_subdev *sd, int flag)
{
	static const int LANES = 1;
	return camera_sensor_csi(sd, ATOMISP_CAMERA_PORT_SECONDARY, LANES,
		ATOMISP_INPUT_FORMAT_RAW_10, atomisp_bayer_order_bggr, flag);
}

static struct camera_sensor_platform_data ov9724_sensor_platform_data = {
	.gpio_ctrl      = ov9724_gpio_ctrl,
	.flisclk_ctrl   = ov9724_flisclk_ctrl,
	.power_ctrl     = ov9724_power_ctrl,
	.csi_cfg        = ov9724_csi_configure,
};

void *ov9724_platform_data(void *info)
{
	camera_reset = -1;
	camera_power_down = -1;
	subcamera_power_en  = -1;
	return &ov9724_sensor_platform_data;
}

