/*
 * Copyright © 2013 Borqs Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and eto permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */
#include "displays/gwm_qhd_vid.h"
#include "mdfld_dsi_dpi.h"
#include "mdfld_dsi_pkg_sender.h"
#include "mdfld_dsi_esd.h"
#include <asm/intel_scu_pmic.h>
#include <asm/intel-mid.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>

/*GPIO Pins */
#define GPIO_MIPI_RESET	57     
#define INIT_IC_READ_CHECK

static struct regulator *vemmc2;

static u8 gwm_set_brightness[2] = {0x51, 0xFF};
static u8 gwm_select_CABC_mode[] = {0x55, 0x03};
static u8 gwm_enable_CABC_bl_off[] = {0x53, 0x28};
static u8 gwm_enable_CABC_bl_on[] = {0x53, 0x2c};
static u8 gwm_exit_sleep[] = {0x11, 0x00};
static u8 gwm_enter_sleep[] = {0x10, 0x00};
static u8 gwm_display_on[] = {0x29, 0x0};
static u8 gwm_display_off[] = {0x28, 0x00};
static u8 gwm_soft_reset[] = {0x01, 0x00};
#ifdef INIT_IC_READ_CHECK
static u8 gwm_invoff[] = {0x20, 0x00};
#endif
static u8 gwm_customer_shift1[]         = {0x00, 0x00};
static u8 gwm_customer_cmd1[]        = {0xff, 0x96, 0x05, 0x01};
static u8 gwm_customer_shift2[]        = {0x00, 0x80};
static u8 gwm_customer_cmd2[]       = {0xff, 0x96, 0x05};
static u8 gwm_customer_shift3[]        = {0x00, 0x89};
static u8 gwm_customer_cmd3[]       = {0xc0,0x01};
static u8 gwm_customer_shift4[]        = {0x00, 0xa2};
static u8 gwm_customer_cmd4[]    = {0xc0, 0x0c, 0x05, 0x02};
static u8 gwm_customer_shift5[]        = {0x00, 0xb4};
static u8 gwm_customer_cmd5[]       = {0xc0, 0x50};
static u8 gwm_customer_shift6[]        = {0x00, 0x80};
static u8 gwm_customer_cmd6[]       = {0xc1, 0x36, 0x66};
static u8 gwm_customer_shift7[]        = {0x00, 0xa0};
static u8 gwm_customer_cmd7[]       = {0xc1, 0x00};
static u8 gwm_customer_shift8[]        = {0x00, 0xa6};
static u8 gwm_customer_cmd8[]       = {0xc1, 0x01};
static u8 gwm_customer_shift9[]       = {0x00, 0x80};
static u8 gwm_customer_cmd9[]      = {0xc5, 0x08, 0x00, 0xa0, 0x11};
static u8 gwm_customer_shift10[]        = {0x00, 0x90};
static u8 gwm_customer_cmd10[]       = {0xc5, 0x96, 0x57, 0x01};
static u8 gwm_customer_shift11[]        = {0x00, 0xb0};
static u8 gwm_customer_cmd11[]       = {0xc5, 0x05, 0x28};
static u8 gwm_customer_shift12[]        = {0x00, 0x80};
static u8 gwm_customer_cmd12[]       = {0xcb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static u8 gwm_customer_shift13[]        = {0x00, 0x90};
static u8 gwm_customer_cmd13[]       = {0xcb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 gwm_customer_shift14[]        = {0x00, 0xa0};
static u8 gwm_customer_cmd14[]       = {0xCB,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static u8 gwm_customer_shift15[]        = {0x00, 0xb0};
static u8 gwm_customer_cmd15[]       = {0xcb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static u8 gwm_customer_shift16[]        = {0x00, 0xc0};
static u8 gwm_customer_cmd16[]       = {0xcb, 0x04, 0x04, 0x04, 0x04, 0x08, 0x04, 0x08, 0x04, 0x08, 0x04, 0x08, 0x04, 0x04, 0x04, 0x08};
static u8 gwm_customer_shift17[]        = {0x00, 0xd0};
static u8 gwm_customer_cmd17[]       = {0xCB,0x08,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x08,0x04,0x08,0x04,0x08,0x04};
static u8 gwm_customer_shift18[]        = {0x00, 0xe0};
static u8 gwm_customer_cmd18[]       = {0xCB,0x08,0x04,0x04,0x04,0x08,0x08,0x00,0x00,0x00,0x00};
static u8 gwm_customer_shift19[]        = {0x00, 0xf0};
static u8 gwm_customer_cmd19[]       = {0xCB,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static u8 gwm_customer_shift20[]        = {0x00,0x80};
static u8 gwm_customer_cmd20[]       = {0xCC,0x26,0x25,0x21,0x22,0x00,0x0F,0x00,0x0D,0x00,0x0B};
static u8 gwm_customer_shift21[]        = {0x00,0x90};
static u8 gwm_customer_cmd21[]       = {0xCC,0x00,0x09,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x25,0x21,0x22,0x00};
static u8 gwm_customer_shift22[]        = {0x00,0xA0};
static u8 gwm_customer_cmd22[]       = {0xCC,0x10,0x00,0x0E,0x00,0x0C,0x00,0x0A,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00};
static u8 gwm_customer_shift23[]        = {0x00,0x80};
static u8 gwm_customer_cmd23[]       = {0xCE,0x8B,0x03,0x06,0x8A,0x03,0x06,0x89,0x03,0x06,0x88,0x03,0x06};
static u8 gwm_customer_shift24[]        = {0x00,0x90};
static u8 gwm_customer_cmd24[]       = {0xCE,0xF0,0x00,0x00,0xF0,0x00,0x00,0xF0,0x00,0x00,0xF0,0x00,0x00,0x00,0x00};
static u8 gwm_customer_shift25[]        = {0x00,0xA0};
static u8 gwm_customer_cmd25[]       = {0xCE,0x38,0x03,0x03,0xC0,0x00,0x06,0x00,0x38,0x02,0x03,0xC1,0x00,0x06,0x00};
static u8 gwm_customer_shift26[]        = {0x00,0xB0};
static u8 gwm_customer_cmd26[]       = {0xCE,0x38,0x01,0x03,0xC2,0x00,0x06,0x00,0x38,0x00,0x03,0xC3,0x00,0x06,0x00};
static u8 gwm_customer_shift27[]        = {0x00,0xC0};
static u8 gwm_customer_cmd27[]       = {0xCE,0x38,0x07,0x03,0xBC,0x00,0x06,0x00,0x38,0x06,0x03,0xBD,0x00,0x06,0x00};
static u8 gwm_customer_shift28[]        = {0x00,0xD0};
static u8 gwm_customer_cmd28[]       = {0xCE,0x38,0x05,0x03,0xBE,0x00,0x06,0x00,0x38,0x04,0x03,0xBF,0x00,0x06,0x00};
static u8 gwm_customer_shift29[]        = {0x00,0x80};
static u8 gwm_customer_cmd29[]       = {0xCF,0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00};
static u8 gwm_customer_shift30[]        = {0x00,0x90};
static u8 gwm_customer_cmd30[]       = {0xCF,0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00};
static u8 gwm_customer_shift31[]        = {0x00,0xA0};
static u8 gwm_customer_cmd31[]       = {0xCF,0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00};
static u8 gwm_customer_shift32[]        = {0x00,0xB0};
static u8 gwm_customer_cmd32[]       = {0xCF,0xF0,0x00,0x00,0x10,0x00,0x00,0x00,0xF0,0x00,0x00,0x10,0x00,0x00,0x00};
static u8 gwm_customer_shift33[]        = {0x00,0xC0};
static u8 gwm_customer_cmd33[]       = {0xCF,0x02,0x02,0x20,0x20,0x00,0x00,0x01,0x00,0x00,0x02};
static u8 gwm_customer_shift34[]        = {0x00,0x00};
static u8 gwm_customer_cmd34[]       = {0xD8,0x77,0x77};
static u8 gwm_customer_shift35[]        = {0x00,0x00};
static u8 gwm_customer_cmd35[]       = {0xD9,0x4D};
static u8 gwm_customer_shift36[]        = {0x00,0x00};
static u8 gwm_customer_cmd36[]       = {0xE1,0x01,0x07,0x0C,0x0D,0x06,0x0E,0x0B,0x0A,0x04,0x07,0x0D,0x07,0x0E,0x17,0x12,0x0D};
static u8 gwm_customer_shift37[]        = {0x00,0x00};
static u8 gwm_customer_cmd37[]       = {0xE2,0x01,0x07,0x0C,0x0D,0x06,0x0E,0x0B,0x0A,0x04,0x07,0x0C,0x08,0x0E,0x17,0x13,0x0D};
static u8 gwm_customer_shift38[]        = {0x00,0x00};
static u8 gwm_customer_cmd38[]       = {0xFF,0xFF,0xFF,0xFF};

#define gwm_send_cmd(shift,cmd) \
{ \
	mdfld_dsi_send_gen_long_lp(sender, shift, sizeof(shift), 0); \
	mdfld_dsi_send_gen_long_lp(sender, cmd, sizeof(cmd), 0); \
}

#define gwm_read_cmd(shift,cmd) \
{ \
	mdfld_dsi_send_gen_long_lp(sender, shift, sizeof(shift),0); \
	ret = mdfld_panel_read_data(sender,shift,cmd,sizeof(cmd) - 1); \
	if (ret) \
		return -1;				\
}
#define gwm_send_softreset_cmd  \
{ \
	err = mdfld_dsi_send_mcs_long_hs(sender, gwm_soft_reset, 2, 0); \
	if (err) {                                              \
		DRM_ERROR("Fail to soft reset\n");           \
	}                                                  \
	mdelay(5);						\
}


static
int gwm_gpio_init(void)
{
	PSB_DEBUG_ENTRY("\n");

	gpio_request(GPIO_MIPI_RESET, "mipi-reset");

	return 0;
}

static int gwm_lcd_remove(struct i2c_client *client)
{
	PSB_DEBUG_ENTRY("\n");
	gpio_free(GPIO_MIPI_RESET);
	if (vemmc2) {
		regulator_disable(vemmc2);
		regulator_put(vemmc2);
	}

	return 0;
}

static int mdfld_panel_read_data(struct mdfld_dsi_pkg_sender *sender, u8 *shift, u8 *dsc, u32 len)
{
	u8 *data = NULL;
	int i;
	data = (u8*)kmalloc(len*sizeof(u8), GFP_KERNEL);
	memset(data, 0, len);

	mdfld_dsi_read_mcs_hs(sender, dsc[0], data, len);
	if (sender->status == MDFLD_DSI_CONTROL_ABNORMAL) {
		printk("read init regs failed\n");
		kfree(data);
		return -EIO;
	}

	i = 0;
	while(i < len) {
		if (dsc[i+1] !=  data[i] ) {
			printk("%x%x's byte%d is :%x\n ", dsc[0], shift[1], i, data[i]);
			kfree(data);
			return -1;
		}
		i++;
	}
	kfree(data);
	mdelay(1);
        return 0;
}


static int read_customer_regs_data(struct mdfld_dsi_config *dsi_config)
{
	struct mdfld_dsi_pkg_sender *sender =
		mdfld_dsi_get_pkg_sender(dsi_config);
	int ret = 0;
	if (!sender) {
		DRM_ERROR("Cannot get sender\n");
		return -EINVAL;
	}
	sender->status = MDFLD_DSI_PKG_SENDER_FREE;

	//gwm_read_cmd(gwm_customer_shift3, gwm_customer_cmd3);
	gwm_read_cmd(gwm_customer_shift4, gwm_customer_cmd4);
	gwm_read_cmd(gwm_customer_shift5, gwm_customer_cmd5);
	gwm_read_cmd(gwm_customer_shift6, gwm_customer_cmd6);
	gwm_read_cmd(gwm_customer_shift7, gwm_customer_cmd7);
	gwm_read_cmd(gwm_customer_shift8, gwm_customer_cmd8);
	gwm_read_cmd(gwm_customer_shift9, gwm_customer_cmd9);
	gwm_read_cmd(gwm_customer_shift10, gwm_customer_cmd10);
	gwm_read_cmd(gwm_customer_shift11, gwm_customer_cmd11);
//	gwm_read_cmd(gwm_customer_shift12, gwm_customer_cmd12);
//	gwm_read_cmd(gwm_customer_shift13, gwm_customer_cmd13);
//	gwm_read_cmd(gwm_customer_shift14, gwm_customer_cmd14);
//	gwm_read_cmd(gwm_customer_shift15, gwm_customer_cmd15);
	gwm_read_cmd(gwm_customer_shift16, gwm_customer_cmd16);
	gwm_read_cmd(gwm_customer_shift17, gwm_customer_cmd17);
	gwm_read_cmd(gwm_customer_shift18, gwm_customer_cmd18);
	//gwm_read_cmd(gwm_customer_shift19, gwm_customer_cmd19);
	gwm_read_cmd(gwm_customer_shift20, gwm_customer_cmd20);
	gwm_read_cmd(gwm_customer_shift21, gwm_customer_cmd21);
	gwm_read_cmd(gwm_customer_shift22, gwm_customer_cmd22);
	gwm_read_cmd(gwm_customer_shift23, gwm_customer_cmd23);
	gwm_read_cmd(gwm_customer_shift24, gwm_customer_cmd24);
	gwm_read_cmd(gwm_customer_shift25, gwm_customer_cmd25);
	gwm_read_cmd(gwm_customer_shift26, gwm_customer_cmd26);
	gwm_read_cmd(gwm_customer_shift27, gwm_customer_cmd27);
	gwm_read_cmd(gwm_customer_shift28, gwm_customer_cmd28);
//	gwm_read_cmd(gwm_customer_shift29, gwm_customer_cmd29);
//	gwm_read_cmd(gwm_customer_shift30, gwm_customer_cmd30);
//	gwm_read_cmd(gwm_customer_shift31, gwm_customer_cmd31);
//	gwm_read_cmd(gwm_customer_shift32, gwm_customer_cmd32);
	gwm_read_cmd(gwm_customer_shift33, gwm_customer_cmd33);
	gwm_read_cmd(gwm_customer_shift34, gwm_customer_cmd34);
	gwm_read_cmd(gwm_customer_shift35, gwm_customer_cmd35);
	gwm_read_cmd(gwm_customer_shift36, gwm_customer_cmd36);
	gwm_read_cmd(gwm_customer_shift37, gwm_customer_cmd37);

	return 0;
}

static int gwm_vid_drv_ic_init(struct mdfld_dsi_config *dsi_config)
{
	struct mdfld_dsi_pkg_sender *sender =
		mdfld_dsi_get_pkg_sender(dsi_config);
	int err = 0;
	if (!sender) {
		DRM_ERROR("Cannot get sender\n");
		return -EINVAL;
	}
	sender->status = MDFLD_DSI_PKG_SENDER_FREE;
	#ifdef INIT_IC_READ_CHECK
//	gwm_send_softreset_cmd;
	#endif
	gwm_send_cmd(gwm_customer_shift1, gwm_customer_cmd1);
	gwm_send_cmd(gwm_customer_shift2, gwm_customer_cmd2);
	//gwm_send_cmd(gwm_customer_shift3, gwm_customer_cmd3);
	gwm_send_cmd(gwm_customer_shift4, gwm_customer_cmd4);
	gwm_send_cmd(gwm_customer_shift5, gwm_customer_cmd5);
	gwm_send_cmd(gwm_customer_shift6, gwm_customer_cmd6);
	gwm_send_cmd(gwm_customer_shift7, gwm_customer_cmd7);
	gwm_send_cmd(gwm_customer_shift8, gwm_customer_cmd8);
	gwm_send_cmd(gwm_customer_shift9, gwm_customer_cmd9);
	gwm_send_cmd(gwm_customer_shift10, gwm_customer_cmd10);
	gwm_send_cmd(gwm_customer_shift11, gwm_customer_cmd11);
//	gwm_send_cmd(gwm_customer_shift12, gwm_customer_cmd12);
//	gwm_send_cmd(gwm_customer_shift13, gwm_customer_cmd13);
//	gwm_send_cmd(gwm_customer_shift14, gwm_customer_cmd14);
//	gwm_send_cmd(gwm_customer_shift15, gwm_customer_cmd15);
	gwm_send_cmd(gwm_customer_shift16, gwm_customer_cmd16);
	gwm_send_cmd(gwm_customer_shift17, gwm_customer_cmd17);
	gwm_send_cmd(gwm_customer_shift18, gwm_customer_cmd18);
	//gwm_send_cmd(gwm_customer_shift19, gwm_customer_cmd19);
	gwm_send_cmd(gwm_customer_shift20, gwm_customer_cmd20);
	gwm_send_cmd(gwm_customer_shift21, gwm_customer_cmd21);
	gwm_send_cmd(gwm_customer_shift22, gwm_customer_cmd22);
	gwm_send_cmd(gwm_customer_shift23, gwm_customer_cmd23);
	gwm_send_cmd(gwm_customer_shift24, gwm_customer_cmd24);
	gwm_send_cmd(gwm_customer_shift25, gwm_customer_cmd25);
	gwm_send_cmd(gwm_customer_shift26, gwm_customer_cmd26);
	gwm_send_cmd(gwm_customer_shift27, gwm_customer_cmd27);
	gwm_send_cmd(gwm_customer_shift28, gwm_customer_cmd28);
//	gwm_send_cmd(gwm_customer_shift29, gwm_customer_cmd29);
//	gwm_send_cmd(gwm_customer_shift30, gwm_customer_cmd30);
//	gwm_send_cmd(gwm_customer_shift31, gwm_customer_cmd31);
//	gwm_send_cmd(gwm_customer_shift32, gwm_customer_cmd32);
	gwm_send_cmd(gwm_customer_shift33, gwm_customer_cmd33);
	gwm_send_cmd(gwm_customer_shift34, gwm_customer_cmd34);
	gwm_send_cmd(gwm_customer_shift35, gwm_customer_cmd35);
	gwm_send_cmd(gwm_customer_shift36, gwm_customer_cmd36);
	gwm_send_cmd(gwm_customer_shift37, gwm_customer_cmd37);
	#ifdef INIT_IC_READ_CHECK
	err = read_customer_regs_data(dsi_config);
	if (err) {
		DRM_ERROR("panel init data write failed\n");
		return -1;
	}
	#endif
	gwm_send_cmd(gwm_customer_shift38, gwm_customer_cmd38);

	return 0;
}
static
int gwm_vid_reset(struct mdfld_dsi_config *dsi_config)
{
	if (vemmc2)
		regulator_enable(vemmc2);
	mdelay(100);
	gpio_direction_output(GPIO_MIPI_RESET, 1);
	mdelay(10);
	gpio_direction_output(GPIO_MIPI_RESET, 0);
	mdelay(20);
	gpio_direction_output(GPIO_MIPI_RESET, 1);
	mdelay(10);

	return 0;
}


static
int gwm_vid_power_on(struct mdfld_dsi_config *dsi_config)
{
	struct mdfld_dsi_pkg_sender *sender =
		mdfld_dsi_get_pkg_sender(dsi_config);

	int err = 0;

	if (!sender) {
		DRM_ERROR("Failed to get DSI packet sender\n");
		return -EINVAL;
	}

	/*change power state*/
	err = mdfld_dsi_send_mcs_long_hs(sender, gwm_exit_sleep, 2, 0);
	if (err) {
		DRM_ERROR("%s: %d: Exit Sleep Mode\n", __func__, __LINE__);
		goto power_on_err;
	}

	msleep(120);

	/*enable CABC with backlight off*/
	mdfld_dsi_send_mcs_long_hs(sender, gwm_select_CABC_mode, 2, 0);
#ifdef	INIT_IC_READ_CHECK
	mdfld_dsi_send_mcs_long_hs(sender, gwm_invoff, 2, 0);
#endif
	//mdfld_dsi_send_mcs_long_hs(sender, gwm_enable_CABC_bl_off, 2, 0);
	/* Set Display on */
	err = mdfld_dsi_send_mcs_long_hs(sender, gwm_display_on, 2, 0);
	if (err) {
		DRM_ERROR("%s: %d: Set Display On\n", __func__, __LINE__);
		goto power_on_err;
	}
	msleep(8);
	if (drm_psb_enable_cabc) {
		mdfld_dsi_send_mcs_long_hs(sender, gwm_enable_CABC_bl_on, 2, 0);
	}
	/*send TURN_ON packet*/
	err = mdfld_dsi_send_dpi_spk_pkg_hs(sender,
			MDFLD_DSI_DPI_SPK_TURN_ON);
	if (err) {
		DRM_ERROR("Faild to send turn on packet\n");
		goto power_on_err;
	}

	return 0;
power_on_err:
	err = -EIO;
	return err;
}

static
int gwm_vid_power_off(struct mdfld_dsi_config *dsi_config)
{
	struct mdfld_dsi_pkg_sender *sender =
		mdfld_dsi_get_pkg_sender(dsi_config);
	int err;

	PSB_DEBUG_ENTRY("\n");

	if (!sender) {
		DRM_ERROR("Failed to get DSI packet sender\n");
		return -EINVAL;
	}

	/*send SHUT_DOWN packet */
	err = mdfld_dsi_send_dpi_spk_pkg_hs(sender,
			MDFLD_DSI_DPI_SPK_SHUT_DOWN);
	if (err) {
		DRM_ERROR("Failed to send turn off packet\n");
		goto power_off_err;
	}
	/* According HW DSI spec, need to wait for 100ms. */
	msleep(100);

	/* Set Display off */
	err = mdfld_dsi_send_mcs_long_hs(sender, gwm_display_off, 2, 0);
	if (err) {
		DRM_ERROR("%s: %d: Set Display On\n", __func__, __LINE__);
		goto power_off_err;
	}
	err = mdfld_dsi_send_mcs_long_hs(sender, gwm_enable_CABC_bl_off, 2, 0);
	if (err) {
		DRM_ERROR("%s: %d: Set bl off\n", __func__, __LINE__);
		goto power_off_err;
	}
	/* Wait for 1 frame after set_display_on. */
	msleep(20);

	/* Sleep In */
	err = mdfld_dsi_send_mcs_long_hs(sender, gwm_enter_sleep, 2, 0);
	if (err) {
		DRM_ERROR("%s: %d: Exit Sleep Mode\n", __func__, __LINE__);
		goto power_off_err;
	}

	/* Wait for 3 frames after enter_sleep_mode. */
	msleep(120);
	gpio_direction_output(GPIO_MIPI_RESET, 0);
	msleep(10);
	if (vemmc2)
		regulator_disable(vemmc2);

	return 0;

power_off_err:
	err = -EIO;
	return err;

}

static
int gwm_vid_detect(struct mdfld_dsi_config *dsi_config)
{
	int status;
	struct drm_device *dev = dsi_config->dev;
	struct mdfld_dsi_hw_registers *regs = &dsi_config->regs;
	int pipe = dsi_config->pipe;
	u32 dpll_val, device_ready_val;

	PSB_DEBUG_ENTRY("\n");

	if (pipe == 0) {
		/*
		 * FIXME: WA to detect the panel connection status, and need to
		 * implement detection feature with get_power_mode DSI command.
		 */
		if (!ospm_power_using_hw_begin(OSPM_DISPLAY_ISLAND,
					OSPM_UHB_FORCE_POWER_ON)) {
			DRM_ERROR("hw begin failed\n");
			return -EAGAIN;
		}

		dpll_val = REG_READ(regs->dpll_reg);
		device_ready_val = REG_READ(regs->device_ready_reg);
		if ((device_ready_val & DSI_DEVICE_READY) &&
		    (dpll_val & DPLL_VCO_ENABLE)) {
			dsi_config->dsi_hw_context.panel_on = true;
			psb_enable_vblank(dev, pipe);
		} else {
			dsi_config->dsi_hw_context.panel_on = false;
			DRM_INFO("%s: panel is not detected!\n", __func__);
		}

		status = MDFLD_DSI_PANEL_CONNECTED;

		ospm_power_using_hw_end(OSPM_DISPLAY_ISLAND);
	} else {
		DRM_INFO("%s: do NOT support dual panel\n", __func__);
		status = MDFLD_DSI_PANEL_DISCONNECTED;
	}

	return status;
}


static
struct drm_display_mode *gwm_vid_get_config_mode(void)
{
	struct drm_display_mode *mode;

	PSB_DEBUG_ENTRY("\n");

	mode = kzalloc(sizeof(*mode), GFP_KERNEL);
	if (!mode)
		return NULL;
	mode->hdisplay = 540;
	mode->vdisplay = 960;
	mode->hsync_start = 572;
	mode->hsync_end = 576;
	mode->htotal = 608;
	mode->vsync_start = 975;
	mode->vsync_end = 976;
	mode->vtotal = 992;
	
	mode->vrefresh = 60;
	mode->clock = mode->vrefresh * mode->vtotal *
		mode->htotal / 1000;
	mode->type |= DRM_MODE_TYPE_PREFERRED;

	drm_mode_set_name(mode);
	drm_mode_set_crtcinfo(mode, 0);

	PSB_DEBUG_ENTRY("hdisplay is %d\n", mode->hdisplay);
	PSB_DEBUG_ENTRY("vdisplay is %d\n", mode->vdisplay);
	PSB_DEBUG_ENTRY("HSS is %d\n", mode->hsync_start);
	PSB_DEBUG_ENTRY("HSE is %d\n", mode->hsync_end);
	PSB_DEBUG_ENTRY("htotal is %d\n", mode->htotal);
	PSB_DEBUG_ENTRY("VSS is %d\n", mode->vsync_start);
	PSB_DEBUG_ENTRY("VSE is %d\n", mode->vsync_end);
	PSB_DEBUG_ENTRY("vtotal is %d\n", mode->vtotal);
	PSB_DEBUG_ENTRY("clock is %d\n", mode->clock);

	return mode;
}

static
void gwm_vid_get_panel_info(int pipe, struct panel_info *pi)
{
	PSB_DEBUG_ENTRY("\n");

	if (pipe == 0) {
		pi->width_mm = GWM_PANEL_WIDTH;
		pi->height_mm = GWM_PANEL_HEIGHT;
	}
}

static
int gwm_vid_set_brightness(struct mdfld_dsi_config *dsi_config, int level)
{
	struct mdfld_dsi_pkg_sender *sender =
		mdfld_dsi_get_pkg_sender(dsi_config);
	int duty_val = 0;

	PSB_DEBUG_ENTRY("level = %d\n", level);

	if (!sender) {
		DRM_ERROR("Failed to get DSI packet sender\n");
		return -EINVAL;
	}
	if (level == 7)
		level = 1;
	else
		level = level * 8/10;

	duty_val = (255 * level) / 100;
	gwm_set_brightness[1] = duty_val;
        mdfld_dsi_send_gen_long_hs(sender, gwm_set_brightness, 2, 0);
	return 0;
}

static
void gwm_vid_dsi_controller_init(struct mdfld_dsi_config *dsi_config)
{
	struct mdfld_dsi_hw_context *hw_ctx = &dsi_config->dsi_hw_context;

	PSB_DEBUG_ENTRY("\n");

	dsi_config->lane_count = 2;
	dsi_config->lane_config = MDFLD_DSI_DATA_LANE_2_2;
#ifdef	CONFIG_MFLD_MIPI_BYPASS_MODE
	hw_ctx->cck_div = 0;
	/* Set it to 0 for over 400mhz */
	hw_ctx->pll_bypass_mode = 1;
#else
	hw_ctx->cck_div = 1;
	hw_ctx->pll_bypass_mode = 0;
#endif

	hw_ctx->mipi_control = 0x18;
	hw_ctx->intr_en = 0xffffffff;
	hw_ctx->hs_tx_timeout = 0xffffff;
	hw_ctx->lp_rx_timeout = 0xffffff;
	hw_ctx->turn_around_timeout = 0x14;
	hw_ctx->device_reset_timer = 0xff;
	hw_ctx->high_low_switch_count = 0x25;
	hw_ctx->init_count = 0xf0;
	hw_ctx->eot_disable = 0x0;
	hw_ctx->lp_byteclk = 0x4;
	hw_ctx->clk_lane_switch_time_cnt = 0xa0014;
	hw_ctx->dphy_param = 0x150a600f;
#ifdef	CONFIG_MFLD_MIPI_BYPASS_MODE
	/*800Mbps bypass mode DSI data rate*/
	if (hw_ctx->pll_bypass_mode && !hw_ctx->cck_div)
		hw_ctx->dphy_param = 0x2A18681F;
#endif
	/*setup video mode format*/
	hw_ctx->video_mode_format = 0xf;

	/*set up func_prg*/
	hw_ctx->dsi_func_prg = (0x200 | dsi_config->lane_count);
	/*setup mipi port configuration*/
	hw_ctx->mipi = PASS_FROM_SPHY_TO_AFE | dsi_config->lane_config;

}

void gwm_qhd_vid_init(struct drm_device *dev, struct panel_funcs *p_funcs)
{
	PSB_DEBUG_ENTRY("\n");

	p_funcs->get_config_mode = gwm_vid_get_config_mode;
	p_funcs->get_panel_info = gwm_vid_get_panel_info;
	p_funcs->dsi_controller_init = gwm_vid_dsi_controller_init;
	p_funcs->detect = gwm_vid_detect;
	p_funcs->power_on = gwm_vid_power_on;
	p_funcs->power_off = gwm_vid_power_off;
	p_funcs->set_brightness = gwm_vid_set_brightness;
	p_funcs->drv_ic_init = gwm_vid_drv_ic_init;
	p_funcs->reset = gwm_vid_reset;
}

static int gwm_lcd_probe(struct platform_device *pdev)
{
	DRM_INFO("%s:GWM QHD LCD DETECT\n",__func__);
	gwm_gpio_init();
	intel_mid_panel_register(gwm_qhd_vid_init);
	vemmc2 = regulator_get(&pdev->dev, "vemmc2");
	if (IS_ERR(vemmc2)) {
		printk("unable to get vemmc2\n");
		vemmc2 = NULL;
	}
	if (vemmc2)
		regulator_enable(vemmc2);
	update_lcdsize_cmdline(540, 960);

	return 0;
}
static struct platform_driver gwm_lcd_driver = {
	.probe = gwm_lcd_probe,
	.remove = __devexit_p(gwm_lcd_remove),
	.driver = {
		.name = "GWM BB Prx",
		.owner = THIS_MODULE,
	},
};

static int __init gwm_lcd_init(void)
{
	DRM_INFO("%s:GWM QHD module\n",__func__);
	return platform_driver_register(&gwm_lcd_driver);
}
module_init(gwm_lcd_init);
