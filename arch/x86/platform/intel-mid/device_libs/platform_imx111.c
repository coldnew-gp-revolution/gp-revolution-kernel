/*
 * platform_imx111.c: imx111 platform data initilization file
 *
 * (C) Copyright 2012 Intel Corporation
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
#include <linux/regulator/consumer.h>
#include "platform_camera.h"
#include "platform_imx111.h"

static struct regulator *vemmc1_reg;
#define VEMMC1_VAL 2850000
static struct regulator *vprog1_reg;  
#define VPROG1_VAL 2800000  
static int camera_reset;
static int camera_vprog1_on;
static int camera_vemmc1_on;
static int camera_flash;
static int camera_torch;
//extern int camera_af_power_gpio;  This is being set in platform_camera.c *//*

/*
 * Manhattan primary camera sensor - IMX111 platform data
 */
static int imx111_flash_ctrl(struct v4l2_subdev *sd, int flag)
{
	int ret;
	if (camera_flash < 0) {
		ret = camera_sensor_gpio(-1, GP_CAMERA_0_FLASH,
					GPIOF_DIR_OUT, 1);
		if (ret < 0)
			return ret;
		camera_flash = ret;
	}
	if (flag) {
		gpio_set_value(camera_flash, 1);
	} else {
		gpio_set_value(camera_flash, 0);
	}
	return 0;
}

static int imx111_torch_ctrl(struct v4l2_subdev *sd, int flag)
{
	int ret;
	if (camera_torch < 0) {
		ret = camera_sensor_gpio(-1, GP_CAMERA_0_TORCH,
					GPIOF_DIR_OUT, 1);
		if (ret < 0)
			return ret;
		camera_torch = ret;
	}
	if (flag) {
		gpio_set_value(camera_torch, 1);

	} else {
		gpio_set_value(camera_torch, 0);
	}
	return 0;
}

static int imx111_gpio_ctrl(struct v4l2_subdev *sd, int flag)
{
	int ret;

	if (camera_reset < 0) {
		ret = camera_sensor_gpio(-1, GP_CAMERA_0_RESET,
					GPIOF_DIR_OUT, 1);
		if (ret < 0)
			return ret;
		camera_reset = ret;
	}

	if (flag) {
		gpio_set_value(camera_reset, 1);
		/* min 250us -Initializing time of silicon */
		usleep_range(250, 300);

	} else {
		gpio_set_value(camera_reset, 0);
	}

	return 0;
}

static int imx111_flisclk_ctrl(struct v4l2_subdev *sd, int flag)
{
	static const unsigned int clock_khz = 19200;
	return intel_scu_ipc_osc_clk(OSC_CLK_CAM0, flag ? clock_khz : 0);
}

static int imx111_power_ctrl(struct v4l2_subdev *sd, int flag)
{
	int ret;
	struct i2c_client *client = v4l2_get_subdevdata(sd);

    //printk(KERN_ALERT "%s() in, flag = %d\n", __func__, flag);

	if (flag) {
#if 0
        // Hardware power up for VCM.
		if (!camera_vemmc1_on) {
            if (vemmc1_reg == NULL) {
	            vemmc1_reg = regulator_get(&client->dev, "vemmc1");
	            if (IS_ERR(vemmc1_reg)) {
		            dev_err(&client->dev, "regulator_get failed\n");
		            return PTR_ERR(vemmc1_reg);
	            }

	            //ret = regulator_set_voltage(vemmc1_reg, VEMMC1_VAL, VEMMC1_VAL);
	            //if (ret) {
		        //    dev_err(&client->dev, "regulator voltage set failed\n");
		        //    regulator_put(vemmc1_reg);
	            //}
            }

			camera_vemmc1_on = 1;
			ret = regulator_enable(vemmc1_reg);
			if (ret) {
				printk(KERN_ALERT "Failed to enable regulator vemmc1\n");
				return ret;
			}
		}
#endif

		if (!camera_vprog1_on) {
			ret = intel_scu_ipc_msic_vprog1(1);
			if (!ret) {
				/* imx1x5 VDIG rise to XCLR release */
				usleep_range(1000, 1200);
				camera_vprog1_on = 1;
			}
			return ret;
		}
	} else {
#if 0
        // Hardware power down for VCM.
		if (camera_vemmc1_on) {
			camera_vemmc1_on = 0;

            if (vemmc1_reg != NULL) {
			    ret = regulator_disable(vemmc1_reg);
			    if (ret) {
				    printk(KERN_ALERT "Failed to disable regulator vemmc1\n");
				    return ret;
			    }
            }
		}
#endif

		if (camera_vprog1_on) {
			ret = intel_scu_ipc_msic_vprog1(0);
			if (!ret)
				camera_vprog1_on = 0;
			return ret;
		}
	}
	return 0;

}

static int imx111_csi_configure(struct v4l2_subdev *sd, int flag)
{
	static const int LANES = 2;
	return camera_sensor_csi(sd, ATOMISP_CAMERA_PORT_PRIMARY, LANES,
		ATOMISP_INPUT_FORMAT_RAW_10, atomisp_bayer_order_rggb, flag);
}

static int imx111_platform_init(struct i2c_client *client)
{
	int ret;

    //printk("%s() in\n", __func__);

#if 0
	vemmc1_reg = regulator_get(&client->dev, "vemmc1");
	if (IS_ERR(vemmc1_reg)) {
		dev_err(&client->dev, "regulator_get failed\n");
		return PTR_ERR(vemmc1_reg);
	}

	ret = regulator_set_voltage(vemmc1_reg, VEMMC1_VAL, VEMMC1_VAL);
	if (ret) {
		dev_err(&client->dev, "regulator voltage set failed\n");
		regulator_put(vemmc1_reg);
	}
#endif
      vprog1_reg = regulator_get(&client->dev, "vprog1");
	if (IS_ERR(vprog1_reg)) {
		dev_err(&client->dev, "regulator_get failed\n");
		return PTR_ERR(vprog1_reg);
	}
	ret = regulator_set_voltage(vprog1_reg, VPROG1_VAL, VPROG1_VAL);
	if (ret) {
		dev_err(&client->dev, "regulator voltage set failed\n");
		regulator_put(vprog1_reg);
	}
	return ret;
}

static int imx111_platform_deinit(void)
{
    //printk("%s() in\n", __func__);

	regulator_put(vemmc1_reg);

	return 0;
}

static struct camera_sensor_platform_data imx111_sensor_platform_data = {
	.flash_ctrl      = imx111_flash_ctrl,
	.torch_ctrl      = imx111_torch_ctrl,
	.gpio_ctrl      = imx111_gpio_ctrl,
	.flisclk_ctrl   = imx111_flisclk_ctrl,
	.power_ctrl     = imx111_power_ctrl,
	.csi_cfg        = imx111_csi_configure,
	.platform_init = imx111_platform_init,
	.platform_deinit = imx111_platform_deinit,
};

void *imx111_platform_data(void *info)
{
	camera_reset = -1;
	camera_flash  = -1;
	camera_torch  = -1;

	return &imx111_sensor_platform_data;
}

