/*
 * /drivers/misc/vibrator/vibrator.c
 *
 * Copyright (C) 2011-2012 Borqs Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/intel_msic_vibrator.h>
#include "../../drivers/staging/android/timed_output.h"
#include <linux/module.h>

/*************************** FUNCTION PROTOTYPE  ****************************/
static int __init init_vibrator(void);
static void __exit exit_vibrator(void);
static int __devinit vibrator_probe(struct platform_device *pdev);
static int __devexit vibrator_remove(struct platform_device *pdev);

static void vibrator_enable(struct timed_output_dev *sdev, int timeout);
static int vib_get_time(struct timed_output_dev *sdev);
static struct msic_pwm_platform_data *pinfo;

static struct platform_driver vibrator_platform_driver = {
	.probe   = vibrator_probe,
	.remove  = __devexit_p(vibrator_remove),
	.driver    = {
		.name  = "msic-pwm-vibrator",
		.owner = THIS_MODULE,
	},
};

/*********************** GLOBAL VARIABLE  ************************************/
static struct timed_output_dev vib_device; /* Vibrator device object */

/*********************** FUNCTION IMPLEMENT   ********************************/
/*
* vib_get_time: Get the current number of milliseconds remaining on the timer
* @sdev		: Pointer to structure timed_output_dev
* return	: the current number of milliseconds remaining on the timer
* Note		: This function use for TPU and PCM2PWM driver
*/
static int vib_get_time(struct timed_output_dev *sdev)
{
	return sdev->state;
}

/*
* vibrator_enable: Handle request Turn off vibration,
*			or turn on vibration for a specified duration
* @sdev		: Pointer to structure timed_output_dev
* @timeout	: The duration of vibration
* return	: the current number of milliseconds remaining on the timer
*/
static void vibrator_enable(struct timed_output_dev *sdev, int timeout)
{
	if(timeout)
	{
		if(pinfo && pinfo->set_power)
			pinfo->set_power(1);
		sdev->state = 1;
	}else
	{
		if(pinfo && pinfo->set_power)
			pinfo->set_power(0);
		sdev->state = 0;
	}
}

/*
* init_vibrator: init vibrator driver
* return:
	0: if no error
	other value if error
*/
static int __devinit vibrator_probe(struct platform_device *pdev)
{
	int ret = 0;

	pinfo = pdev->dev.platform_data ;
	if (!pinfo || !pinfo->set_power || !pinfo->set_duty)
	{
		dev_err(&pdev->dev,"no platform_data\n");
		return -EFAULT;
	}

	/* Initializing vibrator device object */
	vib_device.name = "vibrator";
	vib_device.enable = vibrator_enable;
	vib_device.get_time = vib_get_time;

	/* Register vibrator to timed_output device */
	ret = timed_output_dev_register(&vib_device);
	if (ret) {
		pr_err("register timed_output_dev failded\n");
		return ret;
	}

	return ret;
}

static int __devexit vibrator_remove(struct platform_device *pdev)
{
	return 0;
}

static int __init init_vibrator(void)
{
	int ret = 0;

	/* Register platform driver */

	ret = platform_driver_register(&vibrator_platform_driver);
	if (ret)
		printk(KERN_ERR "[PWM ERR - init_vibrator] can't register TPU driver\n");

	return ret;
}

/*
* tpu_exit : Unregister TPU driver and destroy work queue.
* return   : None
*/
static void __exit exit_vibrator(void)
{
	platform_driver_unregister(&vibrator_platform_driver);
}

/* device_initcall_sync(init_vibrator); */
late_initcall(init_vibrator);
module_exit(exit_vibrator);

MODULE_AUTHOR("Borqs");
MODULE_DESCRIPTION("timed output vibrator device");
MODULE_LICENSE("GPL v2");
