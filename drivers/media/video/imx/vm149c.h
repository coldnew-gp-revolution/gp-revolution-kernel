#ifndef __VM149C_H__
#define __VM149C_H__

#include <linux/atomisp_platform.h>
#include <linux/types.h>
#include <linux/time.h>

#define VM149C_VCM_ADDR	0x0c

/* vm149c device structure */
struct vm149c_device {
	const struct camera_af_platform_data *platform_data;
	struct timespec timestamp_t_focus_abs;
	struct timespec focus_time;	/* Time when focus was last time set */
	s32 focus;			/* Current focus value */
	s16 number_of_steps;
	bool initialized;		/* true if vm149c is detected */
};

#define VM149C_INVALID_CONFIG	0xffffffff
#define VM149C_MAX_FOCUS_POS	1023
#define DELAY_PER_STEP_NS	1000000
#define DELAY_MAX_PER_STEP_NS	(1000000 * 1023)

#define VM149C_CONTROL				2
#define VM149C_VCM_CURRENT		3
#define VM149C_STATUS				5
#define VM149C_MODE				6
#define VM149C_VCM_FREQ			7

#define DEFAULT_CONTROL_VAL		2
#define VM149C_RESET				1
#define WAKEUP_DELAY_US			100
#define VCM_CODE_MASK	0x03ff

int imx_vcm_soft_power_down(struct v4l2_subdev *sd);

int vm149c_t_focus_vcm(struct v4l2_subdev *sd, u16 val);

#endif


