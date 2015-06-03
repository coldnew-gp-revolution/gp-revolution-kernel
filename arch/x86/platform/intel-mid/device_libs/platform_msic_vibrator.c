/*
 * platform_switch_mid.c: switch_mid platform data initilization file
 *
 * (C) Copyright 2008 Intel Corporation
 * Author:
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/input.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <asm/intel-mid.h>
#include <asm/intel_mid_pwm.h>
#include <linux/intel_msic_vibrator.h>

/*TODO: Need work out a solution to get different PWMx from FW or from SPID */
static int msic_pwm_vibrator_setpower(int on)
{
	if ( on )
	{
		intel_mid_pwm(PWM_0, 100);
	}else
	{
		intel_mid_pwm(PWM_0, 0);
	}
	return 0;
}

static int msic_pwm_vibrator_setduty(int duty)
{
	intel_mid_pwm(PWM_0, duty);
	return 0;
}

static struct msic_pwm_platform_data msic_pwm_vibrator_pdata = {
	.set_power = msic_pwm_vibrator_setpower,
	.set_duty = msic_pwm_vibrator_setduty
};

static struct platform_device platform_msic_pwm_vibrator_device = {
	.name		= "msic-pwm-vibrator",
	.id		= -1,
	.dev		={
		.platform_data = &msic_pwm_vibrator_pdata
	}
};

static int __init msic_pwm_vibrator_init(void)
{
	int err;
	err = platform_device_register(&platform_msic_pwm_vibrator_device);
	if (err < 0)
		pr_err("Fail to register msic_pwm_vibrator platform device.\n");
	return 0;
}

device_initcall(msic_pwm_vibrator_init);
