#ifndef __LINUX_FT5X06_TS_FALCON_H__
#define __LINUX_FT5X06_TS_FALCON_H__

#define MAX_PRESSURE       255

#define FT5X06_NAME	"ft5x06_ts"

#ifndef SCALING_TOUCH_MAX_X_RESOLUTION
#define SCALING_TOUCH_MAX_X_RESOLUTION 540
#endif

#ifndef SCALING_TOUCH_MAX_Y_RESOLUTION
#define SCALING_TOUCH_MAX_Y_RESOLUTION 960
#endif

//#define SCREEN_MAX_X 540
//#define SCREEN_MAX_Y 960

struct ft5x06_ts_platform_data {
	u16	intr;		/* irq number	*/
};

enum ft5x06_ts_regs {
	FT5X06_REG_THGROUP				= 0x80,
	FT5X06_REG_THPEAK				= 0x81,
	FT5X06_REG_TIMEENTERMONITOR			= 0x87,
	FT5X06_REG_PERIODACTIVE				= 0x88,
	FT5X06_REG_PERIODMONITOR			= 0x89,
	FT5X06_REG_AUTO_CLB_MODE			= 0xa0,
	FT5X06_REG_PMODE				= 0xa5,	/* Power
								Consume Mode */
	FT5X06_REG_FIRMID				= 0xa6,
	FT5X06_REG_ERR					= 0xa9,
	FT5X06_REG_CLB					= 0xaa,
};

#define PMODE_ACTIVE        0x00
#define PMODE_MONITOR       0x01
#define PMODE_STANDBY       0x02
#define PMODE_HIBERNATE     0x03

/*register address*/
#define FT5X0X_REG_PMODE 0xa5
#define FT5X0X_REG_FW_VER 0xa6
#define FT5X0X_REG_POINT_RATE 0x88
#define FT5X0X_REG_THGROUP 0x80
#define FT5X0X_REG_FILTER 0x8b

#define PMODE_HIBERNATE 0x03

struct tp_key {
	int key;
	s16 x1;
	s16 x2;
	s16 y1;
	s16 y2;
};

struct ft_ts_platform_data {
	int irq_gpio;
	void (*power)(int on);
	int screen_max_x;
	int screen_max_y;
	u8 swap_xy:1;
	u8 xpol:1;
	u8 ypol:1;
	struct tp_key *tp_key;
	u8 tp_key_num;
	void (*key_led_ctrl)(int on);
	unsigned int reset;
	bool regulator_en;
	char *regulator_name;
};

struct i2c_client;

int ft5x06_i2c_read(struct i2c_client *client, char *writebuf,int writelen, char *readbuf, int readlen);
int ft5x06_i2c_write(struct i2c_client *client, char *writebuf, int writelen);

#endif
