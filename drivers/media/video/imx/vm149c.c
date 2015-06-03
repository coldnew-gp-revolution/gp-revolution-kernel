#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/kmod.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-device.h>

#include "vm149c.h"

static struct vm149c_device vm149c_dev;
#if 0
static int vm149c_i2c_rd8(struct i2c_client *client, u8 reg, u8 *val)
{
	struct i2c_msg msg[2];
	u8 buf[2];
	buf[0] = reg;

	msg[0].addr = VM149C_VCM_ADDR;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = &buf[0];

	msg[1].addr = VM149C_VCM_ADDR;
	msg[1].flags = I2C_M_RD;
	msg[1].len = 1;
	msg[1].buf = &buf[1];
	*val = 0;
	if (i2c_transfer(client->adapter, msg, 2) != 2)
		return -EIO;
	*val = buf[1];
	return 0;
}

static int vm149c_i2c_wr8(struct i2c_client *client, u8 reg, u8 val)
{
	struct i2c_msg msg;
	u8 buf[2];
	buf[0] = reg;
	buf[1] = val;
	msg.addr = VM149C_VCM_ADDR;
	msg.flags = 0;
	msg.len = 2;
	msg.buf = &buf[0];
	if (i2c_transfer(client->adapter, &msg, 1) != 1)
		return -EIO;
	return 0;
}

static int vm149c_i2c_wr16(struct i2c_client *client, u8 reg, u16 val)
{
	struct i2c_msg msg;
	u8 buf[3];
	buf[0] = reg;
	buf[1] = (u8)(val >> 8);
	buf[2] = (u8)(val & 0xff);
	msg.addr = VM149C_VCM_ADDR;
	msg.flags = 0;
	msg.len = 3;
	msg.buf = &buf[0];
	if (i2c_transfer(client->adapter, &msg, 1) != 1)
		return -EIO;
	return 0;
}
#endif
static int vm149c_i2c_write16(struct i2c_client *client,  u16 val)
{
	struct i2c_msg msg;
	u8 buf[2];
	buf[0] = (u8)(val >> 8);
	buf[1] = (u8)(val & 0xff);
	msg.addr = VM149C_VCM_ADDR;
	msg.flags = 0;
	msg.len = 2;
	msg.buf = &buf[0];
	if (i2c_transfer(client->adapter, &msg, 1) != 1)
		return -EIO;
	return 0;
}
int vm149c_vcm_power_up(struct v4l2_subdev *sd)
{
	vm149c_dev.focus = VM149C_MAX_FOCUS_POS;
	vm149c_dev.initialized = true;
	return 0;
}

int imx_vcm_soft_power_down(struct v4l2_subdev *sd)
{
    int ret = 0;
	struct i2c_client *client = v4l2_get_subdevdata(sd);

    // b444: Try to smooth the VCM movement, but it didn't work.
#if 0
    int duration = 100;

    ret = vm149c_t_focus_vcm(sd, 500);
    msleep(duration);
    ret = vm149c_t_focus_vcm(sd, 400);
    msleep(duration);
    ret = vm149c_t_focus_vcm(sd, 300);
    msleep(duration);
    ret = vm149c_t_focus_vcm(sd, 200);
    msleep(duration);
    ret = vm149c_t_focus_vcm(sd, 100);
    msleep(duration);
    ret = vm149c_t_focus_vcm(sd, 0);
    msleep(duration);
#endif

    //printk("%s(), in\n", __func__);
    ret = vm149c_i2c_write16(client, 0x8000);

	return ret;
}

int vm149c_vcm_power_down(struct v4l2_subdev *sd)
{
	return 0;
}

int vm149c_t_focus_vcm(struct v4l2_subdev *sd, u16 val)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	u16 data = val & VCM_CODE_MASK;

	data *= 16;

	if (!vm149c_dev.initialized)
		return -ENODEV;
	return vm149c_i2c_write16(client, data);

}

int vm149c_t_focus_abs(struct v4l2_subdev *sd, s32 value)
{
	int ret;
	value = min(value, VM149C_MAX_FOCUS_POS);
	ret = vm149c_t_focus_vcm(sd, VM149C_MAX_FOCUS_POS - value);
	if (ret == 0) {
		vm149c_dev.number_of_steps = value - vm149c_dev.focus;
		vm149c_dev.focus = value;
		getnstimeofday(&(vm149c_dev.timestamp_t_focus_abs));
	}

	return ret;
}

int vm149c_t_focus_rel(struct v4l2_subdev *sd, s32 value)
{
	return vm149c_t_focus_abs(sd, vm149c_dev.focus + value);
}

int vm149c_q_focus_status(struct v4l2_subdev *sd, s32 *value)
{
	u32 status = 0;
	struct timespec temptime;
	const struct timespec timedelay = {
		0,
		min_t(u32, abs(vm149c_dev.number_of_steps)*DELAY_PER_STEP_NS,
			DELAY_MAX_PER_STEP_NS),
	};

	ktime_get_ts(&temptime);

	temptime = timespec_sub(temptime, (vm149c_dev.timestamp_t_focus_abs));

	if (timespec_compare(&temptime, &timedelay) <= 0) {
		status |= ATOMISP_FOCUS_STATUS_MOVING;
		status |= ATOMISP_FOCUS_HP_IN_PROGRESS;
	} else {
		status |= ATOMISP_FOCUS_STATUS_ACCEPTS_NEW_MOVE;
		status |= ATOMISP_FOCUS_HP_COMPLETE;
	}
	*value = status;

	return 0;
}

int vm149c_q_focus_abs(struct v4l2_subdev *sd, s32 *value)
{
	s32 val;
	vm149c_q_focus_status(sd, &val);
	if (val & ATOMISP_FOCUS_STATUS_MOVING)
		*value  = vm149c_dev.focus - vm149c_dev.number_of_steps;
	else
		*value  = vm149c_dev.focus ;
	return 0;
}

int vm149c_t_vcm_slew(struct v4l2_subdev *sd, s32 value)
{
	return 0;
}

int vm149c_t_vcm_timing(struct v4l2_subdev *sd, s32 value)
{
	return 0;
}

int vm149c_vcm_init(struct v4l2_subdev *sd)
{
	return 0;
}



