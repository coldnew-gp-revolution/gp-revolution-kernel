#ifndef __IMX111_H__
#define __IMX111_H__
#include "common.h"

/* settings for imx111 */

/*****************************************************
FULL_SIZE_H_START, FULL_SIZE_V_START, ISP_H_PADDING
ISP_V_PADDING are used to calculate 3A Full size window
for LSC and other function. As IQ tool captures full
size RAW for characterization. We need past information
of full size to 3A
******************************************************/
#define IMX111_FULL_SIZE_H_START 8
#define IMX111_FULL_SIZE_V_START 48

static struct imx_reg const imx111_init_settings[] = {
	/* sw reset */
	{IMX_8BIT, 0x0100, 0x00}, /* mode select, 0: software standby */
	{IMX_8BIT, 0x0103, 0x01}, /* software reset, 1: reset */
	{IMX_TOK_DELAY, 0, 5},
	{IMX_8BIT, 0x0103, 0x00}, /* software reset, 0: normal operation */
	/* basic settings */
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x3080, 0x50}, /* undocumented */
	{IMX_8BIT, 0x3087, 0x53}, /* undocumented */
	{IMX_8BIT, 0x309D, 0x94}, /* undocumented */
	{IMX_8BIT, 0x30B1, 0x00}, /* ?0 in still, 3 in preview settings? */
	{IMX_8BIT, 0x30C6, 0x00}, /* undocumented */
	{IMX_8BIT, 0x30C7, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3115, 0x0B}, /* undocumented */
	{IMX_8BIT, 0x3118, 0x30}, /* undocumented */
	{IMX_8BIT, 0x311D, 0x25}, /* undocumented */
	{IMX_8BIT, 0x3121, 0x0A}, /* undocumented */
	{IMX_8BIT, 0x3212, 0xF2}, /* undocumented */
	{IMX_8BIT, 0x3213, 0x0F}, /* undocumented */
	{IMX_8BIT, 0x3215, 0x0F}, /* undocumented */
	{IMX_8BIT, 0x3217, 0x0B}, /* undocumented */
	{IMX_8BIT, 0x3219, 0x0B}, /* undocumented */
	{IMX_8BIT, 0x321B, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x321D, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x32AA, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3032, 0x40}, /* black level clamping, 64 (default 60) */
	{IMX_8BIT, 0x30D0, 0x40}, /* denoise and dpc init */

	GROUPED_PARAMETER_HOLD_DISABLE,
	{IMX_TOK_TERM, 0, 0}
};

/* Still capture / Preview (see imx.h) */

static const struct imx_reg const imx111_PREVIEW_820x464_30fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340,  628}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3536}, /* line_length_pck */
	{IMX_16BIT, 0x0344,    8}, /* x_addr_start */
	{IMX_16BIT, 0x0346,  312}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 3287}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 2167}, /* y_addr_end */
	{IMX_16BIT, 0x034C,  820}, /* x_output_size */
	{IMX_16BIT, 0x034E,  464}, /* y_output_size */
	{IMX_16BIT, 0x0380,    5}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    3}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    5}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    3}, /* y_odd_inc */

	{IMX_8BIT, 0x3033, 0x00}, /* HDCONFIG: 0 */
	{IMX_8BIT, 0x303D, 0x10}, /* FRMSTAMODE: 1 */
	{IMX_8BIT, 0x303E, 0x41}, /* undocumented */
	{IMX_8BIT, 0x3040, 0x00}, /* OPB start address */
	{IMX_8BIT, 0x3041, 0x00}, /* OPB end address */
	{IMX_8BIT, 0x3048, 0x01}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x6F}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x304D, 0x03}, /* HCNTHALF = 0x036F */
	{IMX_8BIT, 0x3064, 0x12}, /* Gain related = default */
	{IMX_8BIT, 0x3073, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3074, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3075, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3076, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3077, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3079, 0x00}, /* undocumented */
	{IMX_8BIT, 0x307A, 0x00}, /* undocumented */
	{IMX_8BIT, 0x309B, 0x28}, /* RGDAFD SUMEN = 1, double vert anal gain */
	{IMX_8BIT, 0x309C, 0x13}, /* undocumented */
	{IMX_8BIT, 0x309E, 0x00}, /* undocumented */
	{IMX_8BIT, 0x30A0, 0x14}, /* vertical binning related = default */
	{IMX_8BIT, 0x30A1, 0x09}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x02}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x03}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x09}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x00}, /* HADCOEF0: 0 or 1 binning, 0x85 non-bin */
	{IMX_8BIT, 0x30D7, 0x00}, /* HADCOEF1: 0 or 1 binning, 0x2A non-bin */
	{IMX_8BIT, 0x30D8, 0x00}, /* HADCOEF2: 0 or 0x64 bin, 0x64 non-bin */
	{IMX_8BIT, 0x30D9, 0x00}, /* HADCOEF3: 0 or 0x89 bin, 0x89 non-bin */
	{IMX_8BIT, 0x30DA, 0x00}, /* HADCOEF4 */
	{IMX_8BIT, 0x30DB, 0x00}, /* HADCOEF5 */
	{IMX_8BIT, 0x30DC, 0x00}, /* HADCOEF6 */
	{IMX_8BIT, 0x30DD, 0x00}, /* HADCOEF7 */
	{IMX_8BIT, 0x30DE, 0x04}, /* HADCOEF8: the binning factor, 0 non-bin */
	{IMX_8BIT, 0x30DF, 0x20}, /* undocumented */
	{IMX_8BIT, 0x3102, 0x08}, /* undocumented */
	{IMX_8BIT, 0x3103, 0x22}, /* undocumented */
	{IMX_8BIT, 0x3104, 0x20}, /* undocumented */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x87}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x03}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x02}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x03}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x9C}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x9B}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x9D}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x9C}, /* undocumented */
	{IMX_8BIT, 0x3301, 0x00}, /* RGLANESEL = 2 lane output */
	{IMX_8BIT, 0x3304, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3305, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3306, 0x15}, /* undocumented */
	{IMX_8BIT, 0x3307, 0x02}, /* undocumented */
	{IMX_8BIT, 0x3308, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3309, 0x07}, /* undocumented */
	{IMX_8BIT, 0x330A, 0x09}, /* undocumented */
	{IMX_8BIT, 0x330B, 0x05}, /* undocumented */
	{IMX_8BIT, 0x330C, 0x08}, /* undocumented */
	{IMX_8BIT, 0x330D, 0x06}, /* undocumented */
	{IMX_8BIT, 0x330E, 0x03}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x79}, /* undocumented */
	{IMX_8BIT, 0x3322, 0x03}, /* undocumented */
	{IMX_8BIT, 0x3342, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3348, 0xE0}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

static const struct imx_reg const imx111_PREVIEW_820x616_30fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340,  630}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3536}, /* line_length_pck */
	{IMX_16BIT, 0x0344,    8}, /* x_addr_start */
	{IMX_16BIT, 0x0346,   48}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 3287}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 2511}, /* y_addr_end */
	{IMX_16BIT, 0x034C,  820}, /* x_output_size 3280 / 4 = 820 */
	{IMX_16BIT, 0x034E,  616}, /* y_output_size 2456 / 4 = 614 */
	{IMX_16BIT, 0x0380,    5}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    3}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    5}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    3}, /* y_odd_inc */

	{IMX_8BIT, 0x3033, 0x00}, /* HDCONFIG: 0 */
	{IMX_8BIT, 0x303D, 0x10}, /* FRMSTAMODE: 1 */
	{IMX_8BIT, 0x303E, 0x41}, /* undocumented */
	{IMX_8BIT, 0x3040, 0x00}, /* OPB start address */
	{IMX_8BIT, 0x3041, 0x00}, /* OPB end address */
	{IMX_8BIT, 0x3048, 0x01}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x6F}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x304D, 0x03}, /* HCNTHALF = 0x036F */
	{IMX_8BIT, 0x3064, 0x12}, /* Gain related = default */
	{IMX_8BIT, 0x3073, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3074, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3075, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3076, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3077, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3079, 0x00}, /* undocumented */
	{IMX_8BIT, 0x307A, 0x00}, /* undocumented */
	{IMX_8BIT, 0x309B, 0x28}, /* RGDAFD SUMEN = 1, double vert anal gain */
	{IMX_8BIT, 0x309C, 0x13}, /* undocumented */
	{IMX_8BIT, 0x309E, 0x00}, /* undocumented */
	{IMX_8BIT, 0x30A0, 0x14}, /* vertical binning related = default */
	{IMX_8BIT, 0x30A1, 0x09}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x02}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x03}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x09}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x00}, /* HADCOEF0: 0 or 1 binning, 0x85 non-bin */
	{IMX_8BIT, 0x30D7, 0x00}, /* HADCOEF1: 0 or 1 binning, 0x2A non-bin */
	{IMX_8BIT, 0x30D8, 0x00}, /* HADCOEF2: 0 or 0x64 bin, 0x64 non-bin */
	{IMX_8BIT, 0x30D9, 0x00}, /* HADCOEF3: 0 or 0x89 bin, 0x89 non-bin */
	{IMX_8BIT, 0x30DA, 0x00}, /* HADCOEF4 */
	{IMX_8BIT, 0x30DB, 0x00}, /* HADCOEF5 */
	{IMX_8BIT, 0x30DC, 0x00}, /* HADCOEF6 */
	{IMX_8BIT, 0x30DD, 0x00}, /* HADCOEF7 */
	{IMX_8BIT, 0x30DE, 0x04}, /* HADCOEF8: 2 or 4 binning, 0x00 non-bin */
	{IMX_8BIT, 0x30DF, 0x20}, /* undocumented */
	{IMX_8BIT, 0x3102, 0x10}, /* undocumented */
	{IMX_8BIT, 0x3103, 0x44}, /* undocumented */
	{IMX_8BIT, 0x3104, 0x40}, /* undocumented */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x01}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x09}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x08}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x0F}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x5C}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x5E}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x3301, 0x00}, /* RGLANESEL = 2 lane output */
	{IMX_8BIT, 0x3304, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3305, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3306, 0x15}, /* undocumented */
	{IMX_8BIT, 0x3307, 0x02}, /* undocumented */
	{IMX_8BIT, 0x3308, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3309, 0x07}, /* undocumented */
	{IMX_8BIT, 0x330A, 0x09}, /* undocumented */
	{IMX_8BIT, 0x330B, 0x05}, /* undocumented */
	{IMX_8BIT, 0x330C, 0x08}, /* undocumented */
	{IMX_8BIT, 0x330D, 0x06}, /* undocumented */
	{IMX_8BIT, 0x330E, 0x03}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x79}, /* undocumented */
	{IMX_8BIT, 0x3322, 0x03}, /* undocumented */
	{IMX_8BIT, 0x3342, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3348, 0xE0}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

static const struct imx_reg const imx111_PREVIEW_1640x928_15fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340, 1250}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3536}, /* line_length_pck */

	{IMX_16BIT, 0x0344,    8}, /* x_addr_start */
	{IMX_16BIT, 0x0346,  312}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 3287}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 2167}, /* y_addr_end */
	{IMX_16BIT, 0x034C, 1640}, /* x_output_size */
	{IMX_16BIT, 0x034E,  928}, /* y_output_size */

	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    3}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    3}, /* y_odd_inc */

	{IMX_8BIT, 0x3033, 0x00}, /* HDCONFIG: 0 */
	{IMX_8BIT, 0x303D, 0x10}, /* FRMSTAMODE: 1 */
	{IMX_8BIT, 0x303E, 0x41}, /* undocumented */
	{IMX_8BIT, 0x3040, 0x00}, /* OPB start address */
	{IMX_8BIT, 0x3041, 0x00}, /* OPB end address */
	{IMX_8BIT, 0x3048, 0x01}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x6F}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x304D, 0x03}, /* HCNTHALF = 0x036F */
	{IMX_8BIT, 0x3064, 0x12}, /* Gain related = default */
	{IMX_8BIT, 0x3073, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3074, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3075, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3076, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3077, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3079, 0x00}, /* undocumented */
	{IMX_8BIT, 0x307A, 0x00}, /* undocumented */
	{IMX_8BIT, 0x309B, 0x28}, /* RGDAFD SUMEN = 1, double vert anal gain */
	{IMX_8BIT, 0x309C, 0x13}, /* undocumented */
	{IMX_8BIT, 0x309E, 0x00}, /* undocumented */
	{IMX_8BIT, 0x30A0, 0x14}, /* vertical binning related = default */
	{IMX_8BIT, 0x30A1, 0x09}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x02}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x05}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x09}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x01}, /* HADCOEF0: 1 binning, 0x85 non-binning */
	{IMX_8BIT, 0x30D7, 0x01}, /* HADCOEF1: 1 binning, 0x2A non-binning */
	{IMX_8BIT, 0x30D8, 0x64}, /* HADCOEF2: 0 or 0x64 bin, 0x64 non-bin */
	{IMX_8BIT, 0x30D9, 0x89}, /* HADCOEF3: 0 or 0x89 bin, 0x89 non-bin */
	{IMX_8BIT, 0x30DA, 0x00}, /* HADCOEF4 */
	{IMX_8BIT, 0x30DB, 0x00}, /* HADCOEF5 */
	{IMX_8BIT, 0x30DC, 0x00}, /* HADCOEF6 */
	{IMX_8BIT, 0x30DD, 0x00}, /* HADCOEF7 */
	{IMX_8BIT, 0x30DE, 0x02}, /* HADCOEF8: the binning factor, 0 non-bin */
	{IMX_8BIT, 0x30DF, 0x20}, /* undocumented */
	{IMX_8BIT, 0x3102, 0x08}, /* undocumented, related to clock rate */
	{IMX_8BIT, 0x3103, 0x22}, /* undocumented, related to clock rate */
	{IMX_8BIT, 0x3104, 0x20}, /* undocumented, related to clock rate */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x87}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x03}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x02}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x03}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x9c}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x9b}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x9d}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x9c}, /* undocumented */
	{IMX_8BIT, 0x3301, 0x00}, /* RGLANESEL = 2 lane output */
	{IMX_8BIT, 0x3304, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3305, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3306, 0x15}, /* undocumented */
	{IMX_8BIT, 0x3307, 0x02}, /* undocumented */
	{IMX_8BIT, 0x3308, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3309, 0x07}, /* undocumented */
	{IMX_8BIT, 0x330A, 0x09}, /* undocumented */
	{IMX_8BIT, 0x330B, 0x05}, /* undocumented */
	{IMX_8BIT, 0x330C, 0x08}, /* undocumented */
	{IMX_8BIT, 0x330D, 0x06}, /* undocumented */
	{IMX_8BIT, 0x330E, 0x03}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x70}, /* undocumented */
	{IMX_8BIT, 0x3322, 0x03}, /* undocumented */
	{IMX_8BIT, 0x3342, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3348, 0xE0}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

static const struct imx_reg const imx111_PREVIEW_1640x1232_15fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340, 1250}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3536}, /* line_length_pck */

	{IMX_16BIT, 0x0344,    8}, /* x_addr_start */
	{IMX_16BIT, 0x0346,   48}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 3287}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 2511}, /* y_addr_end */
	{IMX_16BIT, 0x034C, 1640}, /* x_output_size */
	{IMX_16BIT, 0x034E, 1232}, /* y_output_size */

	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    3}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    3}, /* y_odd_inc */

	{IMX_8BIT, 0x3033, 0x00}, /* HDCONFIG: 0 */
	{IMX_8BIT, 0x303D, 0x10}, /* FRMSTAMODE: 1 */
	{IMX_8BIT, 0x303E, 0x40}, /* undocumented */
	{IMX_8BIT, 0x3040, 0x00}, /* OPB start address */
	{IMX_8BIT, 0x3041, 0x00}, /* OPB end address */
	{IMX_8BIT, 0x3048, 0x01}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x6F}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x304D, 0x03}, /* HCNTHALF = 0x036F */
	{IMX_8BIT, 0x3064, 0x12}, /* Gain related = default */
	{IMX_8BIT, 0x3073, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3074, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3075, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3076, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3077, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3079, 0x00}, /* undocumented */
	{IMX_8BIT, 0x307A, 0x00}, /* undocumented */
	{IMX_8BIT, 0x309B, 0x28}, /* RGDAFD SUMEN = 1, double vert anal gain */
	{IMX_8BIT, 0x309C, 0x13}, /* undocumented */
	{IMX_8BIT, 0x309E, 0x00}, /* undocumented */
	{IMX_8BIT, 0x30A0, 0x14}, /* vertical binning related = default */
	{IMX_8BIT, 0x30A1, 0x09}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x02}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x05}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x09}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x01}, /* HADCOEF0: 1 binning, 0x85 non-binning */
	{IMX_8BIT, 0x30D7, 0x01}, /* HADCOEF1: 1 binning, 0x2A non-binning */
	{IMX_8BIT, 0x30D8, 0x64}, /* HADCOEF2: 0 or 0x64 bin, 0x64 non-bin */
	{IMX_8BIT, 0x30D9, 0x89}, /* HADCOEF3: 0 or 0x89 bin, 0x89 non-bin */
	{IMX_8BIT, 0x30DA, 0x00}, /* HADCOEF4 */
	{IMX_8BIT, 0x30DB, 0x00}, /* HADCOEF5 */
	{IMX_8BIT, 0x30DC, 0x00}, /* HADCOEF6 */
	{IMX_8BIT, 0x30DD, 0x00}, /* HADCOEF7 */
	{IMX_8BIT, 0x30DE, 0x02}, /* HADCOEF8: the binning factor, 0 non-bin */
	{IMX_8BIT, 0x30DF, 0x20}, /* undocumented */
	{IMX_8BIT, 0x3102, 0x08}, /* undocumented, related to clock rate */
	{IMX_8BIT, 0x3103, 0x22}, /* undocumented, related to clock rate */
	{IMX_8BIT, 0x3104, 0x20}, /* undocumented, related to clock rate */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x87}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x03}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x02}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x03}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x9c}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x9b}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x9d}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x9c}, /* undocumented */
	{IMX_8BIT, 0x3301, 0x00}, /* RGLANESEL = 2 lane output */
	{IMX_8BIT, 0x3304, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3305, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3306, 0x15}, /* undocumented */
	{IMX_8BIT, 0x3307, 0x02}, /* undocumented */
	{IMX_8BIT, 0x3308, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3309, 0x07}, /* undocumented */
	{IMX_8BIT, 0x330A, 0x09}, /* undocumented */
	{IMX_8BIT, 0x330B, 0x05}, /* undocumented */
	{IMX_8BIT, 0x330C, 0x08}, /* undocumented */
	{IMX_8BIT, 0x330D, 0x06}, /* undocumented */
	{IMX_8BIT, 0x330E, 0x03}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x70}, /* undocumented */
	{IMX_8BIT, 0x3322, 0x03}, /* undocumented */
	{IMX_8BIT, 0x3342, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3348, 0xE0}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

static const struct imx_reg const imx111_PREVIEW_2576x1456_15fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340, 2490}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3536}, /* line_length_pck */
	{IMX_16BIT, 0x0344,  360}, /* x_addr_start */
	{IMX_16BIT, 0x0346,  512}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 2935}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 1967}, /* y_addr_end */
	{IMX_16BIT, 0x034C, 2576}, /* x_output_size */
	{IMX_16BIT, 0x034E, 1456}, /* y_output_size */
	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    1}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    1}, /* y_odd_inc */

	{IMX_8BIT, 0x303E, 0x41}, /* undocumented */
	{IMX_8BIT, 0x3048, 0x00}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x57}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x309B, 0x20}, /* RGDAFD SUMEN = 0, double vert anal gain */
	{IMX_8BIT, 0x30A1, 0x08}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x03}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x07}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x00}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x85}, /* HADCOEF0: 1 binning, 0x85 non-binning */
	{IMX_8BIT, 0x30D7, 0x2A}, /* HADCOEF1: 1 binning, 0x2A non-binning */
	{IMX_8BIT, 0x30D8, 0x64}, /* HADCOEF2 */
	{IMX_8BIT, 0x30D9, 0x89}, /* HADCOEF3 */
	{IMX_8BIT, 0x30DE, 0x00}, /* HADCOEF8: the binning factor, 0 non-bin */
	{IMX_8BIT, 0x3102, 0x10}, /* undocumented */
	{IMX_8BIT, 0x3103, 0x44}, /* undocumented */
	{IMX_8BIT, 0x3104, 0x40}, /* undocumented */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x01}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x09}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x08}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x0F}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x5C}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x5E}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x5D}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x62}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

/*
 * Ideally this sensor resolution would not be needed since the ISP should be
 * able to downscale the 8Mpixel resolution to all other 4/3 resolutions.
 * However, this doesn't seem to work correctly at the moment. The preview
 * is all messed up. Using 2576x1936 as a workaround will create a visible
 * field of view change in the UI when switching resolution.
 *
 * 2576x1936 is large enough to cover 2560x1920, which has an aspect of 4/3
 */
static const struct imx_reg const imx111_PREVIEW_2576x1936_15fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340, 2504}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3440}, /* line_length_pck */
	{IMX_16BIT, 0x0344,  360}, /* x_addr_start */
	{IMX_16BIT, 0x0346,  312}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 2935}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 2247}, /* y_addr_end */
	{IMX_16BIT, 0x034C, 2576}, /* x_output_size */
	{IMX_16BIT, 0x034E, 1936}, /* y_output_size */
	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    1}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    1}, /* y_odd_inc */

	{IMX_8BIT, 0x303E, 0x41}, /* undocumented */
	{IMX_8BIT, 0x3048, 0x00}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x57}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x309B, 0x20}, /* RGDAFD SUMEN = 0, double vert anal gain */
	{IMX_8BIT, 0x30A1, 0x08}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x03}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x07}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x00}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x85}, /* HADCOEF0: 1 binning, 0x85 non-binning */
	{IMX_8BIT, 0x30D7, 0x2A}, /* HADCOEF1: 1 binning, 0x2A non-binning */
	{IMX_8BIT, 0x30D8, 0x64}, /* HADCOEF2 */
	{IMX_8BIT, 0x30D9, 0x89}, /* HADCOEF3 */
	{IMX_8BIT, 0x30DE, 0x00}, /* HADCOEF8: the binning factor, 0 non-bin */
	{IMX_8BIT, 0x3102, 0x10}, /* undocumented */
	{IMX_8BIT, 0x3103, 0x44}, /* undocumented */
	{IMX_8BIT, 0x3104, 0x40}, /* undocumented */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x01}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x09}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x08}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x0F}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x5C}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x5E}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x5D}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x62}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

static const struct imx_reg const imx111_PREVIEW_2064x1552_15fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340, 2504}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3440}, /* line_length_pck */
	{IMX_16BIT, 0x0344,  608}, /* x_addr_start */
	{IMX_16BIT, 0x0346,  456}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 2671}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 2007}, /* y_addr_end */
	{IMX_16BIT, 0x034C, 2064}, /* x_output_size */
	{IMX_16BIT, 0x034E, 1552}, /* y_output_size */
	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    1}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    1}, /* y_odd_inc */

	{IMX_8BIT, 0x3033, 0x00}, /* undocumented */
	{IMX_8BIT, 0x303D, 0x10}, /* undocumented */
	{IMX_8BIT, 0x303E, 0x40}, /* undocumented */
	{IMX_8BIT, 0x3040, 0x08}, /* undocumented */
	{IMX_8BIT, 0x3041, 0x97}, /* undocumented */
	{IMX_8BIT, 0x3048, 0x00}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x6F}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x304D, 0x03}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x3064, 0x12}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x3073, 0x00},
	{IMX_8BIT, 0x3074, 0x11},
	{IMX_8BIT, 0x3075, 0x11},
	{IMX_8BIT, 0x3076, 0x11},
	{IMX_8BIT, 0x3077, 0x11},
	{IMX_8BIT, 0x3079, 0x00},
	{IMX_8BIT, 0x307A, 0x00},
	{IMX_8BIT, 0x309B, 0x20},
	{IMX_8BIT, 0x309C, 0x13},
	{IMX_8BIT, 0x309E, 0x00},
	{IMX_8BIT, 0x30A0, 0x14},
	{IMX_8BIT, 0x30A1, 0x08},
	{IMX_8BIT, 0x30AA, 0x02},
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x07}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x00}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x85}, /* HADCOEF0: 1 binning, 0x85 non-binning */
	{IMX_8BIT, 0x30D7, 0x2A}, /* HADCOEF1: 1 binning, 0x2A non-binning */
	{IMX_8BIT, 0x30D8, 0x64}, /* HADCOEF2 */
	{IMX_8BIT, 0x30D9, 0x89}, /* HADCOEF3 */
	{IMX_8BIT, 0x30DE, 0x00}, /* HADCOEF8: the binning factor, 0 non-bin */
	{IMX_8BIT, 0x30DF, 0x20}, /* HADCOEF8: the binning factor, 0 non-bin */
	{IMX_8BIT, 0x3102, 0x08}, /* undocumented */
	{IMX_8BIT, 0x3103, 0x22}, /* undocumented */
	{IMX_8BIT, 0x3104, 0x20}, /* undocumented */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x87}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x03}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x02}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x03}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x9C}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x9B}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x9D}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x9C}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x62}, /* undocumented */
	{IMX_8BIT, 0x3348, 0xE0}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};


static const struct imx_reg const imx111_PREVIEW_3280x1852_15fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340, 2504}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3440}, /* line_length_pck */
	{IMX_16BIT, 0x0344,    8}, /* x_addr_start */
	{IMX_16BIT, 0x0346,  314}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 3287}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 2165}, /* y_addr_end */
	{IMX_16BIT, 0x034C, 3280}, /* x_output_size */
	{IMX_16BIT, 0x034E, 1852}, /* y_output_size */
	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    1}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    1}, /* y_odd_inc */

	{IMX_8BIT, 0x303E, 0x41}, /* undocumented */
	{IMX_8BIT, 0x3048, 0x00}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x57}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x309B, 0x20}, /* RGDAFD SUMEN = 0, double vert anal gain */
	{IMX_8BIT, 0x30A1, 0x08}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x03}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x07}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x00}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x85}, /* HADCOEF0: 1 binning, 0x85 non-binning */
	{IMX_8BIT, 0x30D7, 0x2A}, /* HADCOEF1: 1 binning, 0x2A non-binning */
	{IMX_8BIT, 0x30D8, 0x64}, /* HADCOEF2 */
	{IMX_8BIT, 0x30D9, 0x89}, /* HADCOEF3 */
	{IMX_8BIT, 0x30DE, 0x00}, /* HADCOEF8: the binning factor, 0 non-bin */
	{IMX_8BIT, 0x3102, 0x10}, /* undocumented */
	{IMX_8BIT, 0x3103, 0x44}, /* undocumented */
	{IMX_8BIT, 0x3104, 0x40}, /* undocumented */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x01}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x09}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x08}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x0F}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x5C}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x5E}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x5D}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x62}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

/* 3280x2646 is large enough to cover 3264x2448, which has an aspect of 4/3 */
static const struct imx_reg const imx111_PREVIEW_3280x2464_15fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */
	{IMX_8BIT,  0x0205, 30}, /* PLSTATIM: PLL stable wait */
	{IMX_16BIT, 0x0202, 2000}, /* frame_length_lines */
	{IMX_16BIT, 0x0340, 2490}, /* frame_length_lines */ // high fps:2504  low fps:3504
	{IMX_16BIT, 0x0342, 3850}, /* line_length_pck */    // high fps:3440  low fps:6440
	{IMX_16BIT, 0x0344, IMX111_FULL_SIZE_H_START}, /* x_addr_start */
	{IMX_16BIT, 0x0346, IMX111_FULL_SIZE_V_START}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 3287}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 2511}, /* y_addr_end */
	{IMX_16BIT, 0x034C, 3280}, /* x_output_size */
	{IMX_16BIT, 0x034E, 2464}, /* y_output_size */
	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    1}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    1}, /* y_odd_inc */

	{IMX_8BIT, 0x303E, 0x41}, /* undocumented */
	{IMX_8BIT, 0x3048, 0x00}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x57}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x309B, 0x20}, /* RGDAFD SUMEN = 0, double vert anal gain */
	{IMX_8BIT, 0x30A1, 0x08}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x03}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x07}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x00}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x85}, /* HADCOEF0: 1 binning, 0x85 non-binning */
	{IMX_8BIT, 0x30D7, 0x2A}, /* HADCOEF1: 1 binning, 0x2A non-binning */
	{IMX_8BIT, 0x30D8, 0x64}, /* HADCOEF2 */
	{IMX_8BIT, 0x30D9, 0x89}, /* HADCOEF3 */
	{IMX_8BIT, 0x30DE, 0x00}, /* HADCOEF8: the binning factor, 0 non-bin */
	{IMX_8BIT, 0x3102, 0x10}, /* undocumented */
	{IMX_8BIT, 0x3103, 0x44}, /* undocumented */
	{IMX_8BIT, 0x3104, 0x40}, /* undocumented */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x01}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x09}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x08}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x0F}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x5C}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x5E}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x5D}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x62}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

/* video */
static const struct imx_reg const imx111_VIDEO_1640x1232_30fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340, 1252}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3440}, /* line_length_pck */
	{IMX_16BIT, 0x0344,    8}, /* x_addr_start */
	{IMX_16BIT, 0x0346,   48}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 3287}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 2511}, /* y_addr_end */
	{IMX_16BIT, 0x034C, 1640}, /* x_output_size */
	{IMX_16BIT, 0x034E, 1232}, /* y_output_size */
	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    3}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    3}, /* y_odd_inc */

	{IMX_8BIT, 0x303E, 0x41},
	{IMX_8BIT, 0x3048, 0x01},
	{IMX_8BIT, 0x304C, 0x57},
	{IMX_8BIT, 0x309B, 0x28},
	{IMX_8BIT, 0x30A1, 0x09},
	{IMX_8BIT, 0x30AA, 0x03},
	{IMX_8BIT, 0x30B2, 0x03},
	{IMX_8BIT, 0x30D5, 0x09},
	{IMX_8BIT, 0x30D6, 0x01},
	{IMX_8BIT, 0x30D7, 0x01},
	{IMX_8BIT, 0x30D8, 0x64},
	{IMX_8BIT, 0x30D9, 0x89},
	{IMX_8BIT, 0x30DE, 0x02},
	{IMX_8BIT, 0x3102, 0x10},
	{IMX_8BIT, 0x3103, 0x44},
	{IMX_8BIT, 0x3104, 0x40},
	{IMX_8BIT, 0x3105, 0x00},
	{IMX_8BIT, 0x3106, 0x0D},
	{IMX_8BIT, 0x3107, 0x01},
	{IMX_8BIT, 0x3108, 0x09},
	{IMX_8BIT, 0x3109, 0x08},
	{IMX_8BIT, 0x310A, 0x0F},
	{IMX_8BIT, 0x315C, 0x5D},
	{IMX_8BIT, 0x315D, 0x5C},
	{IMX_8BIT, 0x316E, 0x5E},
	{IMX_8BIT, 0x316F, 0x5D},
	{IMX_8BIT, 0x3318, 0x73},
	{IMX_TOK_TERM, 0, 0}
};

/* 480p */
static const struct imx_reg const imx111_VIDEO_936x602_30fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340, 1600}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3760}, /* line_length_pck */
	{IMX_16BIT, 0x0344,  704}, /* x_addr_start */
	{IMX_16BIT, 0x0346,  630}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 2575}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 1833}, /* y_addr_end */
	{IMX_16BIT, 0x034C,  936}, /* x_output_size */
	{IMX_16BIT, 0x034E,  602}, /* y_output_size */
	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    3}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    3}, /* y_odd_inc */

	{IMX_8BIT, 0x3033, 0x00}, /* HDCONFIG: 0 */
	{IMX_8BIT, 0x303D, 0x10}, /* FRMSTAMODE: 1 */
	{IMX_8BIT, 0x303E, 0x41}, /* undocumented */
	{IMX_8BIT, 0x3040, 0x00}, /* OPB start address */
	{IMX_8BIT, 0x3041, 0x00}, /* OPB end address */
	{IMX_8BIT, 0x3048, 0x01}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x6F}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x304D, 0x03}, /* HCNTHALF = 0x036F */
	{IMX_8BIT, 0x3064, 0x12}, /* Gain related = default */
	{IMX_8BIT, 0x3073, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3074, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3075, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3076, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3077, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3079, 0x00}, /* undocumented */
	{IMX_8BIT, 0x307A, 0x00}, /* undocumented */
	{IMX_8BIT, 0x309B, 0x28}, /* RGDAFD SUMEN = 1, double vert anal gain */
	{IMX_8BIT, 0x309C, 0x13}, /* undocumented */
	{IMX_8BIT, 0x309E, 0x00}, /* undocumented */
	{IMX_8BIT, 0x30A0, 0x14}, /* vertical binning related = default */
	{IMX_8BIT, 0x30A1, 0x09}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x02}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x05}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x09}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x01}, /* HADCOEF0: 1 binning, 0x85 non-binning */
	{IMX_8BIT, 0x30D7, 0x01}, /* HADCOEF1: 1 binning, 0x2A non-binning */
	{IMX_8BIT, 0x30D8, 0x64}, /* HADCOEF2: 0 or 0x64 bin, 0x64 non-bin */
	{IMX_8BIT, 0x30D9, 0x89}, /* HADCOEF3: 0 or 0x89 bin, 0x89 non-bin */
	{IMX_8BIT, 0x30DA, 0x00}, /* HADCOEF4 */
	{IMX_8BIT, 0x30DB, 0x00}, /* HADCOEF5 */
	{IMX_8BIT, 0x30DC, 0x00}, /* HADCOEF6 */
	{IMX_8BIT, 0x30DD, 0x00}, /* HADCOEF7 */
	{IMX_8BIT, 0x30DE, 0x02}, /* HADCOEF8: 2 or 4 binning, 0x00 non-bin */
	{IMX_8BIT, 0x30DF, 0x20}, /* undocumented */
	{IMX_8BIT, 0x3102, 0x10}, /* undocumented */
	{IMX_8BIT, 0x3103, 0x44}, /* undocumented */
	{IMX_8BIT, 0x3104, 0x40}, /* undocumented */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x01}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x09}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x08}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x0F}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x5C}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x5E}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x3301, 0x00}, /* RGLANESEL = 2 lane output */
	{IMX_8BIT, 0x3304, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3305, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3306, 0x15}, /* undocumented */
	{IMX_8BIT, 0x3307, 0x02}, /* undocumented */
	{IMX_8BIT, 0x3308, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3309, 0x07}, /* undocumented */
	{IMX_8BIT, 0x330A, 0x09}, /* undocumented */
	{IMX_8BIT, 0x330B, 0x05}, /* undocumented */
	{IMX_8BIT, 0x330C, 0x08}, /* undocumented */
	{IMX_8BIT, 0x330D, 0x06}, /* undocumented */
	{IMX_8BIT, 0x330E, 0x03}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x70}, /* undocumented */
	{IMX_8BIT, 0x3322, 0x03}, /* undocumented */
	{IMX_8BIT, 0x3342, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3348, 0xE0}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

/* 720p */
static const struct imx_reg const imx111_VIDEO_1568x880_30fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340, 1600}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3760}, /* line_length_pck */
	{IMX_16BIT, 0x0344,   72}, /* x_addr_start */
	{IMX_16BIT, 0x0346,  352}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 3207}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 2111}, /* y_addr_end */
	{IMX_16BIT, 0x034C, 1568}, /* x_output_size */
	{IMX_16BIT, 0x034E,  880}, /* y_output_size */
	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    3}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    3}, /* y_odd_inc */

	{IMX_8BIT, 0x3033, 0x00}, /* HDCONFIG: 0 */
	{IMX_8BIT, 0x303D, 0x10}, /* FRMSTAMODE: 1 */
	{IMX_8BIT, 0x303E, 0x41}, /* undocumented */
	{IMX_8BIT, 0x3040, 0x00}, /* OPB start address */
	{IMX_8BIT, 0x3041, 0x00}, /* OPB end address */
	{IMX_8BIT, 0x3048, 0x01}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x6F}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x304D, 0x03}, /* HCNTHALF = 0x036F */
	{IMX_8BIT, 0x3064, 0x12}, /* Gain related = default */
	{IMX_8BIT, 0x3073, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3074, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3075, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3076, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3077, 0x11}, /* undocumented */
	{IMX_8BIT, 0x3079, 0x00}, /* undocumented */
	{IMX_8BIT, 0x307A, 0x00}, /* undocumented */
	{IMX_8BIT, 0x309B, 0x28}, /* RGDAFD SUMEN = 1, double vert anal gain */
	{IMX_8BIT, 0x309C, 0x13}, /* undocumented */
	{IMX_8BIT, 0x309E, 0x00}, /* undocumented */
	{IMX_8BIT, 0x30A0, 0x14}, /* vertical binning related = default */
	{IMX_8BIT, 0x30A1, 0x09}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x02}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x05}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x09}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x01}, /* HADCOEF0: 1 binning, 0x85 non-binning */
	{IMX_8BIT, 0x30D7, 0x01}, /* HADCOEF1: 1 binning, 0x2A non-binning */
	{IMX_8BIT, 0x30D8, 0x64}, /* HADCOEF2: 0 or 0x64 bin, 0x64 non-bin */
	{IMX_8BIT, 0x30D9, 0x89}, /* HADCOEF3: 0 or 0x89 bin, 0x89 non-bin */
	{IMX_8BIT, 0x30DA, 0x00}, /* HADCOEF4 */
	{IMX_8BIT, 0x30DB, 0x00}, /* HADCOEF5 */
	{IMX_8BIT, 0x30DC, 0x00}, /* HADCOEF6 */
	{IMX_8BIT, 0x30DD, 0x00}, /* HADCOEF7 */
	{IMX_8BIT, 0x30DE, 0x02}, /* HADCOEF8: 2 or 4 binning, 0x00 non-bin */
	{IMX_8BIT, 0x30DF, 0x20}, /* undocumented */
	{IMX_8BIT, 0x3102, 0x10}, /* undocumented */
	{IMX_8BIT, 0x3103, 0x44}, /* undocumented */
	{IMX_8BIT, 0x3104, 0x40}, /* undocumented */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x01}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x09}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x08}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x0F}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x5C}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x5E}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x3301, 0x00}, /* RGLANESEL = 2 lane output */
	{IMX_8BIT, 0x3304, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3305, 0x05}, /* undocumented */
	{IMX_8BIT, 0x3306, 0x15}, /* undocumented */
	{IMX_8BIT, 0x3307, 0x02}, /* undocumented */
	{IMX_8BIT, 0x3308, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3309, 0x07}, /* undocumented */
	{IMX_8BIT, 0x330A, 0x09}, /* undocumented */
	{IMX_8BIT, 0x330B, 0x05}, /* undocumented */
	{IMX_8BIT, 0x330C, 0x08}, /* undocumented */
	{IMX_8BIT, 0x330D, 0x06}, /* undocumented */
	{IMX_8BIT, 0x330E, 0x03}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x70}, /* undocumented */
	{IMX_8BIT, 0x3322, 0x03}, /* undocumented */
	{IMX_8BIT, 0x3342, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3348, 0xE0}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

/* 1080p */
static const struct imx_reg const imx111_VIDEO_2336x1320_30fps[] = {
	GROUPED_PARAMETER_HOLD_ENABLE,
	{IMX_8BIT, 0x0305,  2}, /* PREPLLCK_DIV: 1/N (N: 1,2,4 or 8) */
	{IMX_8BIT, 0x0307, 94}, /* PLL_MPY: PLL multiplier */
	{IMX_8BIT, 0x30A4,  2}, /* RGPLTD: PLL post div, 0=1/2, 1=1/4, 2=1 */
	{IMX_8BIT, 0x303C, 60}, /* PLSTATIM: PLL stable wait */

	{IMX_16BIT, 0x0340, 1600}, /* frame_length_lines */
	{IMX_16BIT, 0x0342, 3760}, /* line_length_pck */
	{IMX_16BIT, 0x0344,  472}, /* x_addr_start */
	{IMX_16BIT, 0x0346,  576}, /* y_addr_start */
	{IMX_16BIT, 0x0348, 2807}, /* x_addr_end */
	{IMX_16BIT, 0x034A, 1895}, /* y_addr_end */
	{IMX_16BIT, 0x034C, 2336}, /* x_output_size */
	{IMX_16BIT, 0x034E, 1320}, /* y_output_size */
	{IMX_16BIT, 0x0380,    1}, /* x_even_inc */
	{IMX_16BIT, 0x0382,    1}, /* x_odd_inc */
	{IMX_16BIT, 0x0384,    1}, /* y_even_inc */
	{IMX_16BIT, 0x0386,    1}, /* y_odd_inc */

	{IMX_8BIT, 0x303E, 0x41}, /* undocumented */
	{IMX_8BIT, 0x3048, 0x00}, /* VMODEFDS: binning operation by FD */
	{IMX_8BIT, 0x304C, 0x57}, /* HCNTHALF: 0x036F preview, 0x0357 still */
	{IMX_8BIT, 0x309B, 0x20}, /* RGDAFD SUMEN = 0, double vert anal gain */
	{IMX_8BIT, 0x30A1, 0x08}, /* undocumented 9 binning, 8 non-binning */
	{IMX_8BIT, 0x30AA, 0x03}, /* undocumented 2 in preview but 3 in still */
	/*
	 * Register 0x30B2:
	 * 3 when 1/4, 5 when 1/2 binning, 7 in full resolution
	 */
	{IMX_8BIT, 0x30B2, 0x07}, /* undocumented */
	{IMX_8BIT, 0x30D5, 0x00}, /* HADDEN = 0, HADCONFIG = 0 */
	{IMX_8BIT, 0x30D6, 0x85}, /* HADCOEF0: 1 binning, 0x85 non-binning */
	{IMX_8BIT, 0x30D7, 0x2A}, /* HADCOEF1: 1 binning, 0x2A non-binning */
	{IMX_8BIT, 0x30D8, 0x64}, /* HADCOEF2 */
	{IMX_8BIT, 0x30D9, 0x89}, /* HADCOEF3 */
	{IMX_8BIT, 0x30DE, 0x00}, /* HADCOEF8: 2 or 4 binning, 0x00 non-bin */
	{IMX_8BIT, 0x3102, 0x10}, /* undocumented */
	{IMX_8BIT, 0x3103, 0x44}, /* undocumented */
	{IMX_8BIT, 0x3104, 0x40}, /* undocumented */
	{IMX_8BIT, 0x3105, 0x00}, /* undocumented */
	{IMX_8BIT, 0x3106, 0x0D}, /* undocumented */
	{IMX_8BIT, 0x3107, 0x01}, /* undocumented */
	{IMX_8BIT, 0x3108, 0x09}, /* undocumented */
	{IMX_8BIT, 0x3109, 0x08}, /* undocumented */
	{IMX_8BIT, 0x310A, 0x0F}, /* undocumented */
	{IMX_8BIT, 0x315C, 0x5D}, /* undocumented */
	{IMX_8BIT, 0x315D, 0x5C}, /* undocumented */
	{IMX_8BIT, 0x316E, 0x5E}, /* undocumented */
	{IMX_8BIT, 0x316F, 0x5D}, /* undocumented */
	/*
	 * Register 0x3318:
	 * 0x79 when 1/4 but 0x70 when 1/2 binning in preview, but 0x73 when
	 * 1/2 binning in still settings and 0x62 in non-binning still
	 */
	{IMX_8BIT, 0x3318, 0x62}, /* undocumented */
	{IMX_TOK_TERM, 0, 0}
};

struct imx_resolution imx111_res_preview[] = {
	{
		.desc = "imx111_PREVIEW_820x464_30fps",
		.regs = imx111_PREVIEW_820x464_30fps,
		.width = 820,
		.height = 464,
		.fps = 60,
		.pixels_per_line = 3536, /* consistent with register arrays */
		.lines_per_frame = 628, /* consistent with register arrays */
		.bin_factor_x = 2,
		.bin_factor_y = 2,
		.used = 0,
	},
	{
		.desc = "imx111_PREVIEW_820x616_30fps",
		.regs = imx111_PREVIEW_820x616_30fps,
		.width = 820,
		.height = 616,
		.fps = 30,
		.pixels_per_line = 3536, /* consistent with register arrays */
		.lines_per_frame = 630, /* consistent with register arrays */
		.bin_factor_x = 2,
		.bin_factor_y = 2,
		.used = 0,
	},
	{
		.desc = "imx111_PREVIEW_1640x928_15fps",
		.regs = imx111_PREVIEW_1640x928_15fps,
		.width = 1640,
		.height = 928,
		.fps = 15,
		.pixels_per_line = 3536, /* consistent with register arrays */
		.lines_per_frame = 1250, /* consistent with register arrays */
		.bin_factor_x = 1,
		.bin_factor_y = 1,
		.used = 0,
	},
	{
		.desc = "imx111_PREVIEW_1640x1232_15fps",
		.regs = imx111_PREVIEW_1640x1232_15fps,
		.width = 1640,
		.height = 1232,
		.fps = 15,
		.pixels_per_line = 3536, /* consistent with register arrays */
		.lines_per_frame = 1250, /* consistent with register arrays */
		.bin_factor_x = 1,
		.bin_factor_y = 1,
		.used = 0,
	},
	{
		.desc = "imx111_PREVIEW_2576x1456_15fps",
		.regs = imx111_PREVIEW_2576x1456_15fps,
		.width = 2576,
		.height = 1456,
		.fps = 15,
		.pixels_per_line = 3536, /* consistent with register arrays */
		.lines_per_frame = 2490, /* consistent with register arrays */
		.bin_factor_x = 0,
		.bin_factor_y = 0,
		.used = 0,
	},
	{
		.desc = "imx111_PREVIEW_2064x1552_15fps",
		.regs = imx111_PREVIEW_2064x1552_15fps,
		.width = 2064,
		.height = 1552,
		.fps = 15,
		.pixels_per_line = 3440, /* consistent with register arrays */
		.lines_per_frame = 2504, /* consistent with register arrays */
		.bin_factor_x = 0,
		.bin_factor_y = 0,
		.used = 0,
	},
	{
		.desc = "imx111_PREVIEW_2576x1936_15fps",
		.regs = imx111_PREVIEW_2576x1936_15fps,
		.width = 2576,
		.height = 1936,
		.fps = 15,
		.pixels_per_line = 3440, /* consistent with register arrays */
		.lines_per_frame = 2504, /* consistent with register arrays */
		.bin_factor_x = 0,
		.bin_factor_y = 0,
		.used = 0,
	},
	{
		.desc = "imx111_PREVIEW_3280x1852_15fps",
		.regs = imx111_PREVIEW_3280x1852_15fps,
		.width = 3280,
		.height = 1852,
		.fps = 15,
		.pixels_per_line = 3440, /* consistent with register arrays */
		.lines_per_frame = 2504, /* consistent with register arrays */
		.bin_factor_x = 0,
		.bin_factor_y = 0,
		.used = 0,
	},
	{
		.desc = "imx111_PREVIEW_3280x2464_15fps",
		.regs = imx111_PREVIEW_3280x2464_15fps,
		.width = 3280,
		.height = 2464,
		.fps = 15,
		.pixels_per_line = 3850, /* consistent with register arrays */
		.lines_per_frame = 2490, /* consistent with register arrays */
		.bin_factor_x = 0,
		.bin_factor_y = 0,
		.used = 0,
	},
};

struct imx_resolution imx111_res_video[] = {
	{
		.desc = "imx111_VIDEO_936x602_30fps",
		.regs = imx111_VIDEO_936x602_30fps,
		.width = 936,
		.height = 602,
		.fps = 30,
		.pixels_per_line = 3760, /* consistent with register arrays */
		.lines_per_frame = 1600, /* consistent with register arrays */
		.bin_factor_x = 1,
		.bin_factor_y = 1,
		.used = 0,
	},
	{
		.desc = "imx111_VIDEO_1568x880_30fps",
		.regs = imx111_VIDEO_1568x880_30fps,
		.width = 1568,
		.height = 880,
		.fps = 30,
		.pixels_per_line = 3760, /* consistent with register arrays */
		.lines_per_frame = 1600, /* consistent with register arrays */
		.bin_factor_x = 1,
		.bin_factor_y = 1,
		.used = 0,
	},
	{
		.desc = "imx111_VIDEO_2336x1320_30fps",
		.regs = imx111_VIDEO_2336x1320_30fps,
		.width = 2336,
		.height = 1320,
		.fps = 30,
		.pixels_per_line = 3760, /* consistent with register arrays */
		.lines_per_frame = 1600, /* consistent with register arrays */
		.bin_factor_x = 0,
		.bin_factor_y = 0,
		.used = 0,
	},
};

struct imx_resolution imx111_res_video_nodvs[] = {
	{
		.desc = "imx111_VIDEO_1640x1232_30fps",
		.regs = imx111_VIDEO_1640x1232_30fps,
		.width = 1640,
		.height = 1232,
		.fps = 30,
		.pixels_per_line = 1720, /* consistent with register arrays */
		.lines_per_frame = 1250, /* consistent with register arrays */
		.bin_factor_x = 1,
		.bin_factor_y = 1,
		.used = 0,
	},
};

#endif
