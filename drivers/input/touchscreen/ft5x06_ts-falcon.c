/*
 * drivers/input/touchscreen/ft5x06_ts.c
 *
 * FocalTech ft5x06 TouchScreen driver.
 *
 * Copyright (c) 2010  Focal tech Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 *	note: only support mulititouch	Wenfs 2010-10-01
 */

#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <linux/fcntl.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/miscdevice.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/input-polldev.h>
#include <linux/i2c.h>
#include <linux/workqueue.h>
#ifdef CONFIG_ANDROID_POWER
#include <linux/android_power.h>
#endif
#include <asm/setup.h>
#include <linux/i2c/ft5x06_ts-falcon.h>
#include <linux/gpio.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include <asm/intel_scu_pmic.h>
#include <linux/regulator/consumer.h>
#include <linux/async.h>

/*don't enable auto upgrade*/
#define FTS_AUTO_UPGRADEG
#define FTS_CTL_IIC
#define FTS_APK_DEBUG
#define SYSFS_DEBUG

#ifdef FTS_CTL_IIC
#include <linux/i2c/focaltech_ctl.h>
#endif

#ifdef SYSFS_DEBUG
#include <linux/i2c/ft5x06_ex_fun.h>
#endif

struct ts_event {
	u16 au16_x[5];	/*x coordinate */
	u16 au16_y[5];	/*y coordinate */
	u8 au8_touch_event[5];	/*touch event:0 -- down; 1-- contact; 2 -- contact */
	u8 au8_finger_id[5];	/*touch ID */
	u16 pressure;
	u8 touch_point;

};

struct ft5x06_ts_data {
	struct i2c_client *client;
	struct input_dev *input_dev;
	int irq;
	struct ts_event event;
	struct regulator        *regulator;
#ifdef CONFIG_HAS_EARLYSUSPEND
	struct early_suspend early_suspend_low;
	struct early_suspend early_suspend_high;
#endif
};

#ifdef CONFIG_HAS_EARLYSUSPEND
static void ft5x06_ts_early_suspend_low(struct early_suspend *h);
static void ft5x06_ts_late_resume_low(struct early_suspend *h);
static void ft5x06_ts_early_suspend_high(struct early_suspend *h);
static void ft5x06_ts_late_resume_high(struct early_suspend *h);
#endif

#define MAX_POINT 5
#define VKEY_Y   1010
#define THGROUP_VAL 25
#define PERIOD_ACTIVE_VAL 10

#define FT_READ_BUF 33
#define MAX_FINGER 5
#define MAX_CHANNEL 0xF
static u16 screen_max_x, screen_max_y;
struct i2c_client *hopping_client;
static int touchscreen_hopping_ready=0;
/*
*ft5x0x_i2c_Read-read data and write data by i2c
*@client: handle of i2c
*@writebuf: Data that will be written to the slave
*@writelen: How many bytes to write
*@readbuf: Where to store data read from slave
*@readlen: How many bytes to read
*
*Returns negative errno, else the number of messages executed
*
*
*/
int ft5x06_i2c_read(struct i2c_client *client, char *writebuf,
		    int writelen, char *readbuf, int readlen)
{
	int ret;

	if (writelen > 0) {
		struct i2c_msg msgs[] = {
			{
			 .addr = client->addr,
			 .flags = 0,
			 .len = writelen,
			 .buf = writebuf,
			 },
			{
			 .addr = client->addr,
			 .flags = I2C_M_RD,
			 .len = readlen,
			 .buf = readbuf,
			 },
		};
		ret = i2c_transfer(client->adapter, msgs, 2);
		if (ret < 0)
			dev_err(&client->dev, "f%s: i2c read error.\n",
				__func__);
	} else {
		struct i2c_msg msgs[] = {
			{
			 .addr = client->addr,
			 .flags = I2C_M_RD,
			 .len = readlen,
			 .buf = readbuf,
			 },
		};
		ret = i2c_transfer(client->adapter, msgs, 1);
		if (ret < 0)
			dev_err(&client->dev, "%s:i2c read error.\n", __func__);
	}
	return ret;
}

/*write data by i2c*/
int ft5x06_i2c_write(struct i2c_client *client, char *writebuf, int writelen)
{
	int ret;

	struct i2c_msg msg[] = {
		{
		 .addr = client->addr,
		 .flags = 0,
		 .len = writelen,
		 .buf = writebuf,
		 },
	};

	ret = i2c_transfer(client->adapter, msg, 1);
	if (ret < 0)
		dev_err(&client->dev, "%s i2c write error.\n", __func__);

	return ret;
}

int ft5x06_ts_filter(bool enabled)
{
	unsigned char reg_value;
	unsigned char reg_addr;

	if(!touchscreen_hopping_ready) {
		//printk(KERN_INFO"Touchscreen not ready!\n");
		return 0;
	}

	if(enabled) {
		ft5x0x_write_reg(hopping_client, FT5X0X_REG_FILTER,0x01);
		//printk(KERN_INFO "%s: [FTS] TP Hopping enabled\n",__func__);
	} else {
		ft5x0x_write_reg(hopping_client, FT5X0X_REG_FILTER,0x00);
		//printk(KERN_INFO "%s: [FTS] TP Hopping disabled\n",__func__);
	}
	reg_addr = FT5X0X_REG_FILTER;
	ft5x06_i2c_read(hopping_client, &reg_addr, 1, &reg_value, 1);
	//printk(KERN_INFO "%s: [FTS] TP Hopping is 0x%x\n",__func__,reg_value);

	return 0;
}

/*release the point*/
static void ft5x06_ts_release(struct ft5x06_ts_data *data)
{
	int i;
	for(i=0;i<MAX_CHANNEL;i++){
		input_mt_slot(data->input_dev, i);
		input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, -1);
	}
	input_sync(data->input_dev);
}

/*Read touch point information when the interrupt  is asserted.*/
static int ft5x06_read_data(struct ft5x06_ts_data *data)
{
	struct ts_event *event = &data->event;
	u8 buf[FT_READ_BUF] = {0};
	int ret = -1;
	int i = 0;
	u8 pointid = 0x0F;

	ret = ft5x06_i2c_read(data->client, buf, 1, buf, FT_READ_BUF);
	if (ret < 0) {
		printk(KERN_WARNING "%s read_data i2c_rxdata failed: %d\n",
					__func__, ret);
		return ret;
	}
	memset(event, 0, sizeof(struct ts_event));
	event->touch_point = 0;
	for (i = 0; i < MAX_FINGER; i++) {
		pointid = (buf[5+6*i]) >> 4;
		if (pointid >= 0x0F)
			break;
		else
			event->touch_point++;

		if (event->touch_point == 0) {
			ft5x06_ts_release(data);
			return 1;
		}

		event->au16_x[i] = (s16)(buf[3+6*i]&0x0F)<<8 | (s16)buf[4+6*i];
		event->au16_y[i] = (s16)(buf[5+6*i]&0x0F)<<8 | (s16)buf[6+6*i];
		event->au8_touch_event[i] = buf[3+6*i] >> 6;
		event->au8_finger_id[i] = buf[5+6*i] >> 4;
		#if 0
		pr_debug("id=%d event=%d x=%d y=%d\n", event->au8_finger_id[i],
			event->au8_touch_event[i], event->au16_x[i], event->au16_y[i]);
		#endif
	}

	event->pressure = 0x08;

	return 0;
}

static int ft5x06_report_value(struct ft5x06_ts_data *data)
{
	struct ts_event *event = &data->event;
	int i;
	int uppoint = 0;

	/*protocol B*/
	for (i = 0; i < event->touch_point; i++) {
		input_mt_slot(data->input_dev, event->au8_finger_id[i]);
		if (event->au8_touch_event[i]== 0 || event->au8_touch_event[i] == 2) {
			input_mt_report_slot_state(data->input_dev, MT_TOOL_FINGER, true);
			//input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, event->au8_touch_event[i]);
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->au16_x[i]);
			if(event->au16_y[i] > screen_max_y) {
				input_report_abs(data->input_dev,ABS_MT_POSITION_Y, VKEY_Y);
			} else {
				input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->au16_y[i]);
			}
		} else {
			uppoint++;
			input_mt_report_slot_state(data->input_dev, MT_TOOL_FINGER, false);
			//input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, -1);
		}
	}
	if(event->touch_point == uppoint)
		input_report_key(data->input_dev, BTN_TOUCH, 0);
	else
		input_report_key(data->input_dev, BTN_TOUCH, event->touch_point > 0);
	input_sync(data->input_dev);
	return 0;
}

static irqreturn_t ft5x06_ts_interrupt(int irq, void *dev_id)
{
	int ret;
	struct ft5x06_ts_data *ft5x06_ts = dev_id;
	disable_irq_nosync(ft5x06_ts->irq);
	ret = ft5x06_read_data(ft5x06_ts);
	if (ret == 0)
		ft5x06_report_value(ft5x06_ts);
	else
		pr_debug("Touch Point 0\n");
	enable_irq(ft5x06_ts->irq);

	return IRQ_HANDLED;
}

static int ft5x06_suspend(struct device *dev)
{
	struct ft5x06_ts_data *ts = dev_get_drvdata(dev);
	struct i2c_client *client = to_i2c_client(dev);
	struct ft_ts_platform_data *pdata = client->dev.platform_data;

	printk(KERN_INFO "ft5x06 touch screen : Suspend\n");

	disable_irq(ts->irq);
	/*release tracking id */
	ft5x06_ts_release(ts);
	ft5x0x_write_reg(client, 0xa5, PMODE_HIBERNATE);

	gpio_set_value(pdata->reset, 0);
	msleep(5);

	/*disable vemmc2 power*/
	if (ts->regulator)
		regulator_disable(ts->regulator);

	return 0;
}

static int ft5x06_resume(struct device *dev)
{
	int needwait;
	struct ft5x06_ts_data *ts = dev_get_drvdata(dev);
	struct i2c_client *client = to_i2c_client(dev);
	struct ft_ts_platform_data *pdata = client->dev.platform_data;
	struct ts_event *event;

	printk(KERN_INFO "ft5x06 touch screen : Resume\n");

	event = &ts->event;

	/*enable vmmc2 power*/
	if (ts->regulator) {
		/*need wait to stable if regulator first output*/
		needwait = !regulator_is_enabled(ts->regulator);
		regulator_enable(ts->regulator);
		if (needwait)
			msleep(50);
	}

	/*reset touchscreen*/
	gpio_set_value(pdata->reset, 0);
	msleep(5);
	gpio_set_value(pdata->reset, 1);

	return 0;
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void ft5x06_ts_early_suspend_low(struct early_suspend *h)
{
	struct ft5x06_ts_data *ts;
	struct device *dev;
	ts = container_of(h, struct ft5x06_ts_data, early_suspend_low);
	dev = &ts->client->dev;
	if (ft5x06_suspend(dev) != 0)
		dev_err(dev, "suspend failed\n");
}

static void ft5x06_ts_late_resume_low(struct early_suspend *h)
{
	struct ft5x06_ts_data *ts;
	struct device *dev;
	ts = container_of(h, struct ft5x06_ts_data, early_suspend_low);
	dev = &ts->client->dev;
	if (ft5x06_resume(dev) != 0)
		dev_err(dev, "resume failed\n");
}

static void ft5x06_ts_early_suspend_high(struct early_suspend *h)
{
	/*do nothing*/
}

static void ft5x06_ts_late_resume_high(struct early_suspend *h)
{
	struct ft5x06_ts_data *ts;
	ts = container_of(h, struct ft5x06_ts_data, early_suspend_high);
	ft5x06_ts_release(ts);
	enable_irq(ts->irq);
}
#endif

static struct kobj_attribute falcon_touch_vkeys_attr;
static struct attribute *falcon_attributes[] = {
        &falcon_touch_vkeys_attr.attr,
        NULL,
};

static ssize_t falcon_touch_vkeys_kobj_output(struct kobject *kobj,
                struct kobj_attribute *attr, char *buf)
{
	sprintf(buf,"0x01:139:90:1010:180:100:0x01:172:270:1010:180:100:0x01:158:450:1010:180:100\n");
	return strlen(buf);
}

static struct kobj_attribute falcon_touch_vkeys_attr = {
        .attr = {
                .name = NULL,
                .mode = S_IRUGO,
        },
        .show = &falcon_touch_vkeys_kobj_output,
};

static struct attribute_group falcon_attribute_group = {
        .attrs = falcon_attributes,
};

static int set_vkey_map(void)
{
        const char *str1 = "virtualkeys.ft5x06_ts";
        char *filename = NULL;
	int err=-ENOMEM;
        struct kobject *board_props_kobj;

        filename = kzalloc(strlen(str1) + (sizeof(char) * (I2C_NAME_SIZE + 1)),
                                GFP_KERNEL);
        if (filename == NULL) {
                printk(KERN_ERR "%s: No memory for filename.\n", __func__);
                err = -ENOMEM;
                return err;
        }

        strncat(filename, str1, strlen(str1));
        //strncat(filename, name, I2C_NAME_SIZE);
        falcon_touch_vkeys_attr.attr.name = filename;

	board_props_kobj = kobject_create_and_add("board_properties",NULL);
	if (!board_props_kobj)
	{
		kfree(filename);
		return -EFAULT;
	}
	err = sysfs_create_group(board_props_kobj, &falcon_attribute_group);
	if ( err )
	{
		kfree(filename);
		kobject_put(board_props_kobj);
		return err;
	}
	return 0;
}

static int  ft5x06_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	struct ft5x06_ts_data *ft5x06_ts;
	struct input_dev *input_dev;
	struct ft_ts_platform_data *pdata = pdata = client->dev.platform_data;
	unsigned char reg_addr, reg_val;
	int err = 0;
	int ret = 0;

	printk(KERN_INFO "==falcon ft5x06_ts_probe=\n");

	if (!pdata) {
		dev_err(&client->dev, "platform data is required!\n");
		return -EINVAL;
	}

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
		return -EIO;

/* Direct interface for control PMIC /VEMMC2   2.8v voltage
	printk(KERN_INFO "Ready for enable touch 2.8v\n");
	intel_scu_ipc_iowrite8(0xda,0x36);
	msleep(10);
*/

	ft5x06_ts = kzalloc(sizeof(*ft5x06_ts), GFP_KERNEL);
	if (!ft5x06_ts)	{
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}

	if (pdata->regulator_en && pdata->regulator_name) {
		ft5x06_ts->regulator = regulator_get(&client->dev,pdata->regulator_name);
		if (IS_ERR(ft5x06_ts->regulator)) {
			printk(KERN_INFO "get regulator %s failed\n",pdata->regulator_name);
			goto err_regulator;
		}

		ret = regulator_enable(ft5x06_ts->regulator);
		if (ret < 0) {
			printk(KERN_INFO "enable regulator %s failed with ret %d\n",pdata->regulator_name, ret);
			regulator_put(ft5x06_ts->regulator);
			goto err_regulator;
		}
	}

	/* reset the chip */
	gpio_set_value(pdata->reset, 0);
	msleep(20);
	gpio_set_value(pdata->reset, 1);
	msleep(100);

	screen_max_x = pdata->screen_max_x;
	screen_max_y = pdata->screen_max_y;
	printk(KERN_INFO "screen_max_x:%d  screen_max_y:%d \n",screen_max_x,screen_max_y);

	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		printk(KERN_WARNING "failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}
	__set_bit(KEY_PROG1, input_dev->evbit);
	ret = set_vkey_map();
	printk("ft5x06: set_vkey_map %s\n",ret?"failed":"success");
	ft5x06_ts->client = client;
	hopping_client = client;
	touchscreen_hopping_ready= 1;
	ft5x06_ts->irq = gpio_to_irq(pdata->irq_gpio);
	client->irq = ft5x06_ts->irq;
	ft5x06_ts->input_dev = input_dev;

	__set_bit(EV_ABS, input_dev->evbit);
	__set_bit(EV_KEY, input_dev->evbit);
	__set_bit(EV_SYN, input_dev->evbit);
	__set_bit(BTN_TOUCH, input_dev->keybit);
	__set_bit(INPUT_PROP_DIRECT, input_dev->propbit);

	set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	input_mt_init_slots(input_dev, 5);
	input_set_abs_params(input_dev,ABS_MT_POSITION_X, 0, screen_max_x, 0, 0);
	input_set_abs_params(input_dev,ABS_MT_POSITION_Y, 0, screen_max_y, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_WIDTH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TRACKING_ID, 0, 5, 0, 0);
	input_dev->name	= FT5X06_NAME;
	i2c_set_clientdata(client, ft5x06_ts);
	input_set_drvdata(input_dev, ft5x06_ts);

	err = input_register_device(input_dev);
	if (err) {
		printk(KERN_WARNING "ft5x06_ts_probe: failed to register input device:\n");
		goto exit_input_register_device_failed;
	}

	printk(KERN_INFO "client->dev.driver->name %s  ,%d\n",
			client->dev.driver->name, ft5x06_ts->irq);

	ret = request_threaded_irq(ft5x06_ts->irq, NULL, ft5x06_ts_interrupt, IRQF_TRIGGER_FALLING,
			client->dev.driver->name, ft5x06_ts);

	if (ret < 0) {
		dev_err(&client->dev, "irq %d busy?\n", ft5x06_ts->irq);
		goto fail3;
	}

	disable_irq_nosync(ft5x06_ts->irq);

	reg_addr = 0xa6;
	err = ft5x06_i2c_read(client, &reg_addr, 1, &reg_val, 1);
	printk("Firmware id 0x%x ..\n", reg_val);
	reg_addr = 0x88;
	err |= ft5x0x_write_reg(client, reg_addr, PERIOD_ACTIVE_VAL);
	msleep(10);
	reg_addr = 0x80;
	err |= ft5x0x_write_reg(client, reg_addr, THGROUP_VAL);
	msleep(10);
	reg_addr = 0x88;
	err |= ft5x06_i2c_read(client, &reg_addr, 1, &reg_val, 1);
	printk("Point Rate:%x val %d ..\n", reg_addr, reg_val*10);
	reg_addr = 0x80;
	err |= ft5x06_i2c_read(client, &reg_addr, 1, &reg_val, 1);
	printk("Thgroup:%x val %d ..\n", reg_addr, reg_val*4);
	if (err<0) {
		dev_err(&client->dev, "ft5x06_ts_probe: failed to find device\n");
		goto fail3;
	}


#ifdef CONFIG_HAS_EARLYSUSPEND
	ft5x06_ts->early_suspend_low.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	ft5x06_ts->early_suspend_low.suspend = ft5x06_ts_early_suspend_low;
	ft5x06_ts->early_suspend_low.resume = ft5x06_ts_late_resume_low;
	register_early_suspend(&ft5x06_ts->early_suspend_low);

	ft5x06_ts->early_suspend_high.level = EARLY_SUSPEND_LEVEL_DISABLE_FB+ 1;
	ft5x06_ts->early_suspend_high.suspend = ft5x06_ts_early_suspend_high;
	ft5x06_ts->early_suspend_high.resume = ft5x06_ts_late_resume_high;
	register_early_suspend(&ft5x06_ts->early_suspend_high);
#endif

#ifdef FTS_AUTO_UPGRADEG
	fts_ctpm_auto_upgrade(client);
#endif

#ifdef SYSFS_DEBUG
	ft5x0x_create_sysfs(client);
#endif

#ifdef FTS_APK_DEBUG
	ft5x0x_create_apk_debug_channel(client);
#endif

#ifdef FTS_CTL_IIC
	if(ft_rw_iic_drv_init(client) < 0)
		printk(KERN_INFO "%s: [FTS] create fts control iic driver failed\n",__func__);
#endif

	/*drv probe fine, panel find,can enable interrupt now*/
	enable_irq(ft5x06_ts->irq);
	return 0;
fail3:
	free_irq(ft5x06_ts->irq, ft5x06_ts);
exit_input_register_device_failed:
	input_free_device(input_dev);
	if (ft5x06_ts->regulator) {
		regulator_disable(ft5x06_ts->regulator);
		regulator_put(ft5x06_ts->regulator);
	}
exit_input_dev_alloc_failed:
err_regulator:
	printk(KERN_WARNING "==singlethread error =\n");
	kfree(ft5x06_ts);
exit_alloc_data_failed:
	return err;
}

static int __devexit ft5x06_remove(struct i2c_client *client)
{
	struct ft5x06_ts_data *ft5x06_ts = i2c_get_clientdata(client);
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&ft5x06_ts->early_suspend_low);
	unregister_early_suspend(&ft5x06_ts->early_suspend_high);
#endif
	free_irq(ft5x06_ts->irq, ft5x06_ts);
	input_unregister_device(ft5x06_ts->input_dev);
	kfree(ft5x06_ts);
	i2c_set_clientdata(client, NULL);
	return 0;
}


static struct i2c_device_id ft5x06_idtable[] = {
	{ FT5X06_NAME, 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, ft5x06_idtable);

static const struct dev_pm_ops ft5x06_pm_ops = {
	.suspend	= ft5x06_suspend,
	.resume		= ft5x06_resume,
};
static struct i2c_driver ft5x06_driver  = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= FT5X06_NAME,
#ifndef CONFIG_HAS_EARLYSUSPEND
		.pm	= &ft5x06_pm_ops,
#endif
	},
	.id_table	= ft5x06_idtable,
	.probe = ft5x06_probe,
	.remove = __devexit_p(ft5x06_remove),
};


static int __init ft5x06_ts_init(void)
{
	return i2c_add_driver(&ft5x06_driver);
}

static void __exit ft5x06_ts_exit(void)
{
	i2c_del_driver(&ft5x06_driver);
}

late_initcall_async(ft5x06_ts_init);
module_exit(ft5x06_ts_exit);

MODULE_AUTHOR("<wenfs@Focaltech-systems.com>");
MODULE_DESCRIPTION("FocalTech ft5x06 TouchScreen driver");
MODULE_LICENSE("GPL");

