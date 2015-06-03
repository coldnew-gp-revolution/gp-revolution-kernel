#ifndef LINUX_INTEL_MSIC_VIBRATOR_H
#define LINUX_INTEL_MSIC_VIBRATOR_H


struct msic_pwm_platform_data {
	void (*init)(void);
	int (*set_power)(int);
	int (*set_duty)(int);
};

#endif
