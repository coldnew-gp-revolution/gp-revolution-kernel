/* langwell_gpio.c Moorestown platform Langwell chip GPIO driver
 * Copyright (c) 2008 - 2009,  Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Supports:
 * Moorestown platform Langwell chip.
 * Medfield platform Penwell chip.
 * Whitney point.
 */

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/stddef.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/lnw_gpio.h>
#include <linux/pm_runtime.h>
#include <asm/intel-mid.h>

#ifdef CONFIG_INTEL_EXTENDED_SUPPORT
#include <linux/seq_file.h>
#include <asm/intel_scu_ipc.h>

#define GPAON_AF_STR(gpio,afi1,afi2,afi3,afo1,afo2,afo3) \
	        [gpio] = {"gp_aon_"#gpio,  afi1, afi2, afi3, afo1, afo2, afo3 }
#define GPCORE_AF_STR(gpio,afi1,afi2,afi3,afo1,afo2,afo3) \
	        [gpio+96] = {"gp_core_"#gpio, afi1, afi2, afi3, afo1, afo2, afo3 }
#define GPAON_FLIS_OFF(gpio, flis_offset) \
		[gpio] = flis_offset
#define GPCOR_FLIS_OFF(gpio, flis_offset) \
		[gpio+96] = flis_offset

u8* flis=NULL;
u8 flis_offset[192]={
	GPAON_FLIS_OFF(83,0x5),
	GPAON_FLIS_OFF(82,0x6),
	GPAON_FLIS_OFF(81,0x7),
	GPAON_FLIS_OFF(80,0x8),
	GPAON_FLIS_OFF(36,0x20),
	GPAON_FLIS_OFF(37,0x21),
	GPAON_FLIS_OFF(38,0x22),
	GPAON_FLIS_OFF(39,0x23),
	GPAON_FLIS_OFF(40,0x24),
	GPAON_FLIS_OFF(41,0x25),
	GPAON_FLIS_OFF(42,0x26),
	GPAON_FLIS_OFF(43,0x27),
	GPAON_FLIS_OFF(44,0x28),
	GPAON_FLIS_OFF(45,0x29),

	GPCOR_FLIS_OFF(77,0x30),
	GPCOR_FLIS_OFF(78,0x31),
	GPCOR_FLIS_OFF(79,0x32),
	GPCOR_FLIS_OFF(80,0x33),
	GPCOR_FLIS_OFF(81,0x34),
	GPCOR_FLIS_OFF(39,0x35),
	GPCOR_FLIS_OFF(38,0x36),
	GPCOR_FLIS_OFF(41,0x37),
	GPCOR_FLIS_OFF(40,0x38),
	GPCOR_FLIS_OFF( 6,0x39),
	GPCOR_FLIS_OFF(44,0x40),
	GPCOR_FLIS_OFF(45,0x41),
	GPCOR_FLIS_OFF(46,0x42),
	GPCOR_FLIS_OFF(47,0x43),
	GPCOR_FLIS_OFF(48,0x44),
	GPCOR_FLIS_OFF(49,0x45),
	GPCOR_FLIS_OFF(52,0x46),
	GPCOR_FLIS_OFF(63,0x47),
	GPCOR_FLIS_OFF(64,0x48),
	GPCOR_FLIS_OFF(65,0x49),
	GPCOR_FLIS_OFF(15,0x50),
	GPCOR_FLIS_OFF(16,0x51),
	GPCOR_FLIS_OFF(17,0x52),
	GPCOR_FLIS_OFF(18,0x53),
	GPCOR_FLIS_OFF(19,0x54),
	GPCOR_FLIS_OFF(20,0x55),
	GPCOR_FLIS_OFF(21,0x56),
	GPCOR_FLIS_OFF(22,0x57),
	GPCOR_FLIS_OFF(26,0x58),
	GPCOR_FLIS_OFF(27,0x59),

	GPAON_FLIS_OFF(76,0x60),
	GPAON_FLIS_OFF(77,0x61),

	GPCOR_FLIS_OFF(60,0x67),
	GPCOR_FLIS_OFF(59,0x68),

	GPAON_FLIS_OFF(70,0x69),

	GPCOR_FLIS_OFF(56,0x70),
	GPCOR_FLIS_OFF(57,0x71),
	GPCOR_FLIS_OFF(58,0x72),

	GPAON_FLIS_OFF( 0,0x73),
	GPAON_FLIS_OFF( 1,0x74),
	GPAON_FLIS_OFF( 2,0x75),
	GPAON_FLIS_OFF( 3,0x76),
	GPAON_FLIS_OFF(72,0x77),
	GPAON_FLIS_OFF(73,0x78),
	GPAON_FLIS_OFF(74,0x79),
	GPAON_FLIS_OFF( 9,0x80),
	GPAON_FLIS_OFF(11,0x81),
	GPAON_FLIS_OFF(10,0x82),
	GPAON_FLIS_OFF(12,0x83),
	GPAON_FLIS_OFF(13,0x84),
	GPAON_FLIS_OFF(59,0x86),
	GPAON_FLIS_OFF(58,0x87),
	GPAON_FLIS_OFF(57,0x88),
	GPAON_FLIS_OFF(55,0x89),
	GPAON_FLIS_OFF(62,0x90),
	GPAON_FLIS_OFF(63,0x91),
	GPAON_FLIS_OFF(60,0x92),
	GPAON_FLIS_OFF(61,0x93),
	GPAON_FLIS_OFF(64,0x94),
	GPAON_FLIS_OFF(66,0x95),
	GPAON_FLIS_OFF(65,0x96),
	GPAON_FLIS_OFF(67,0x97),
	GPAON_FLIS_OFF(68,0x98),

	GPCOR_FLIS_OFF(67,0x99),

	GPAON_FLIS_OFF(30,0x1A),
	GPAON_FLIS_OFF(31,0x1B),
	GPAON_FLIS_OFF(32,0x1C),
	GPAON_FLIS_OFF(33,0x1D),
	GPAON_FLIS_OFF(34,0x1E),
	GPAON_FLIS_OFF(35,0x1F),
	GPAON_FLIS_OFF(46,0x2A),
	GPAON_FLIS_OFF(47,0x2B),
	GPAON_FLIS_OFF(48,0x2C),
	GPAON_FLIS_OFF(49,0x2D),

	GPCOR_FLIS_OFF(82,0x2E),
	GPCOR_FLIS_OFF(76,0x2F),
	GPCOR_FLIS_OFF( 7,0x3A),

	GPAON_FLIS_OFF(69,0x3B),

	GPCOR_FLIS_OFF(51,0x3C),
	GPCOR_FLIS_OFF(50,0x3D),
	GPCOR_FLIS_OFF(42,0x3E),
	GPCOR_FLIS_OFF(43,0x3F),
	GPCOR_FLIS_OFF(66,0x4A),

	GPAON_FLIS_OFF(78,0x4B),
	GPAON_FLIS_OFF(79,0x4C),

	GPCOR_FLIS_OFF(12,0x4D),
	GPCOR_FLIS_OFF(13,0x4E),
	GPCOR_FLIS_OFF(14,0x4F),
	GPCOR_FLIS_OFF(28,0x5A),
	GPCOR_FLIS_OFF(29,0x5B),
	GPCOR_FLIS_OFF(30,0x5C),
	GPCOR_FLIS_OFF(31,0x5D),
	GPCOR_FLIS_OFF(32,0x5E),
	GPCOR_FLIS_OFF(33,0x5F),
	GPCOR_FLIS_OFF(53,0x6A),
	GPCOR_FLIS_OFF(54,0x6B),
	GPCOR_FLIS_OFF(55,0x6C),
	GPCOR_FLIS_OFF(62,0x6D),
	GPCOR_FLIS_OFF(61,0x6E),

	GPAON_FLIS_OFF(71,0x6F),
	GPAON_FLIS_OFF(75,0x7A),
	GPAON_FLIS_OFF( 4,0x7B),
	GPAON_FLIS_OFF( 5,0x7C),
	GPAON_FLIS_OFF( 7,0x7D),
	GPAON_FLIS_OFF( 6,0x7E),
	GPAON_FLIS_OFF( 8,0x7F),
	GPAON_FLIS_OFF(56,0x8A),
	GPAON_FLIS_OFF(54,0x8B),
	GPAON_FLIS_OFF(53,0x8C),
	GPAON_FLIS_OFF(52,0x8D),
	GPAON_FLIS_OFF(50,0x8E),
	GPAON_FLIS_OFF(51,0x8F),

	GPCOR_FLIS_OFF(68,0x9A),
	GPCOR_FLIS_OFF(71,0x9B),
	GPCOR_FLIS_OFF(69,0xA2),
	GPCOR_FLIS_OFF(72,0xA3),
	GPCOR_FLIS_OFF(70,0xAC),
	GPCOR_FLIS_OFF(73,0xAD),
	GPCOR_FLIS_OFF(74,0xAE),
	GPCOR_FLIS_OFF(75,0xAF),
	GPCOR_FLIS_OFF(37,0xB0),
	GPCOR_FLIS_OFF(35,0xB1),
	GPCOR_FLIS_OFF(36,0xB2),

	GPAON_FLIS_OFF(25,0xB3),
	GPAON_FLIS_OFF(24,0xB4),
	GPAON_FLIS_OFF(27,0xB5),
	GPAON_FLIS_OFF(26,0xB6),
	GPAON_FLIS_OFF(29,0xB7),
	GPAON_FLIS_OFF(28,0xB8),
	GPAON_FLIS_OFF(23,0xB9),
	GPAON_FLIS_OFF(22,0xBA),
	GPAON_FLIS_OFF(21,0xBB),
	GPAON_FLIS_OFF(16,0xBC),
	GPAON_FLIS_OFF(17,0xBD),
	GPAON_FLIS_OFF(18,0xBE),
	GPAON_FLIS_OFF(19,0xBF),
	GPAON_FLIS_OFF(20,0xC0),
};

char* af_list[192][7]={
	GPAON_AF_STR(0,"coms_int0","","","","",""),
	GPAON_AF_STR(1,"coms_int1","","","","",""),
	GPAON_AF_STR(2,"coms_int2","","","","",""),
	GPAON_AF_STR(3,"coms_int3","","","","",""),
	GPAON_AF_STR(4,"i2s_0_clk","","","i2s_0_clk","",""),
	GPAON_AF_STR(5,"i2s_0_fs","","","i2s_0_fs","",""),
	GPAON_AF_STR(6,"","","","i2s_0_txd","",""),
	GPAON_AF_STR(7,"i2s_0_rxd","","","","",""),
	GPAON_AF_STR(8,"i2s_1_clk","mslim_2_bclk","","i2s_1_clk","mslim_2_bclk",""),
	GPAON_AF_STR(9,"i2s_1_fs","mslim_2_bdat","","i2s_1_fs","mslim_2_bdat",""),
	GPAON_AF_STR(10,"","","","i2s_1_txd","",""),
	GPAON_AF_STR(11,"i2s_1_rxd","","","","",""),
	GPAON_AF_STR(12,"mslim_1_bclk","","","mslim_1_bclk","",""),
	GPAON_AF_STR(13,"mslim_1_bdat","","","mslim_1_bdat","",""),
	GPAON_AF_STR(14,"uart_2_rx","","","","",""),
	GPAON_AF_STR(15,"","","","","",""),
	GPAON_AF_STR(16,"","","","spi_1_ss0","",""),
	GPAON_AF_STR(17,"","","","spi_1_ss1","",""),
	GPAON_AF_STR(18,"","","","spi_1_ss2","spi_0_ss3a",""),
	GPAON_AF_STR(19,"","","","spi_1_ss3","spi_0_ss2",""),
	GPAON_AF_STR(20,"","","","spi_1_ss4","spi_0_ss1",""),
	GPAON_AF_STR(21,"","","","spi_1_sdo","",""),
	GPAON_AF_STR(22,"spi_1_sdi","","","","",""),
	GPAON_AF_STR(23,"","","","spi_1_clk","",""),
	GPAON_AF_STR(24,"i2c_0_sda","","","i2c_0_sda","",""),
	GPAON_AF_STR(25,"i2c_0_scl","","","i2c_0_scl","",""),
	GPAON_AF_STR(26,"i2c_1_sda","","","i2c_1_sda","",""),
	GPAON_AF_STR(27,"i2c_1_scl","","","i2c_1_scl","",""),
	GPAON_AF_STR(28,"i2c_2_sda","","","i2c_2_sda","",""),
	GPAON_AF_STR(29,"i2c_2_scl","","","i2c_2_scl","",""),
	GPAON_AF_STR(30,"kbd_dkin0","","","","",""),
	GPAON_AF_STR(31,"kbd_dkin1","","","","",""),
	GPAON_AF_STR(32,"kbd_dkin2","","","","",""),
	GPAON_AF_STR(33,"kbd_dkin3","","","","",""),
	GPAON_AF_STR(34,"kbd_mkin0","","","","",""),
	GPAON_AF_STR(35,"kbd_mkin1","","","","",""),
	GPAON_AF_STR(36,"kbd_mkin2","","","","",""),
	GPAON_AF_STR(37,"kbd_mkin3","","","","",""),
	GPAON_AF_STR(38,"kbd_mkin4","","","","",""),
	GPAON_AF_STR(39,"kbd_mkin5","","","","",""),
	GPAON_AF_STR(40,"kbd_mkin6","","","","",""),
	GPAON_AF_STR(41,"kbd_mkin7","","","","",""),
	GPAON_AF_STR(42,"","","","kbd_mkout0","",""),
	GPAON_AF_STR(43,"","","","kbd_mkout1","",""),
	GPAON_AF_STR(44,"","","","kbd_mkout2","",""),
	GPAON_AF_STR(45,"","","","kbd_mkout3","",""),
	GPAON_AF_STR(46,"","","","kbd_mkout4","",""),
	GPAON_AF_STR(47,"","","","kbd_mkout5","",""),
	GPAON_AF_STR(48,"","","","kbd_mkout6","",""),
	GPAON_AF_STR(49,"","","","kbd_mkout7","",""),
	GPAON_AF_STR(50,"spi_3_ss0","","","spi_3_ss0","",""),
	GPAON_AF_STR(51,"","","","","",""),
	GPAON_AF_STR(52,"","","","spi_3_sdo","",""),
	GPAON_AF_STR(53,"spi_3_sdi","","","","",""),
	GPAON_AF_STR(54,"spi_3_clk","","","spi_3_clk","",""),
	GPAON_AF_STR(55,"","","","spi_2_ss0","",""),
	GPAON_AF_STR(56,"","","","spi_2_ss1","spi_0_ss3b",""),
	GPAON_AF_STR(57,"","","","spi_2_sdo","",""),
	GPAON_AF_STR(58,"spi_2_sdi","","","","",""),
	GPAON_AF_STR(59,"","","","spi_2_clk","",""),
	GPAON_AF_STR(60,"uart_0_rx","","","","",""),
	GPAON_AF_STR(61,"","","","uart_0_tx","",""),
	GPAON_AF_STR(62,"uart_0_cts","","","","",""),
	GPAON_AF_STR(63,"","","","uart_0_rts","",""),
	GPAON_AF_STR(64,"uart_1_rx","","","","",""),
	GPAON_AF_STR(65,"","","","uart_1_tx","",""),
	GPAON_AF_STR(66,"","","","uart_1_sd","uart_1_rts",""),
	GPAON_AF_STR(67,"uart_2_rx","","","","",""),
	GPAON_AF_STR(68,"uart_1_cts","","","","uart_2_tx",""),
	GPAON_AF_STR(69,"stio_0_cd_b","","","","",""),
	GPAON_AF_STR(70,"stio_1_dat0","","","stio_1_dat0","",""),
	GPAON_AF_STR(71,"stio_2_dat1","","","stio_2_dat1","",""),
	GPAON_AF_STR(72,"batt_charge","","","batt_charge","",""),
	GPAON_AF_STR(73,"","","","uart_2_tx","",""),
	GPAON_AF_STR(74,"","","","uart_2_rts","",""),
	GPAON_AF_STR(75,"uart_2_cts","","","","",""),
	GPAON_AF_STR(76,"","","","","",""),
	GPAON_AF_STR(77,"","","","","",""),
	GPAON_AF_STR(78,"","","","","",""),
	GPAON_AF_STR(79,"","","","","",""),
	GPAON_AF_STR(80,"","","","spi_0_ss0","",""),
	GPAON_AF_STR(81,"","","","spi_0_sdo","",""),
	GPAON_AF_STR(82,"spi_0_sdi","","","","",""),
	GPAON_AF_STR(83,"","","","spi_0_clk","",""),
	GPAON_AF_STR(84,"","","","","",""),
	GPAON_AF_STR(85,"","","","","",""),
	GPAON_AF_STR(86,"","","","","",""),
	GPAON_AF_STR(87,"","","","","",""),
	GPAON_AF_STR(88,"","","","","",""),
	GPAON_AF_STR(89,"","","","","",""),
	GPAON_AF_STR(90,"","","","","",""),
	GPAON_AF_STR(91,"","","","","",""),
	GPAON_AF_STR(92,"","","","","",""),
	GPAON_AF_STR(93,"","","","","",""),
	GPAON_AF_STR(94,"","","","","",""),
	GPAON_AF_STR(95,"","","","","",""),

	GPCORE_AF_STR(0,"","","","","",""),
	GPCORE_AF_STR(1,"","","","","",""),
	GPCORE_AF_STR(2,"","","","","",""),
	GPCORE_AF_STR(3,"","","","","",""),
	GPCORE_AF_STR(4,"","","","","",""),
	GPCORE_AF_STR(5,"","","","","",""),
	GPCORE_AF_STR(6,"intd_dsi_te1","","","","",""),
	GPCORE_AF_STR(7,"intd_dsi_te2","","","","",""),
	GPCORE_AF_STR(8,"","","","","",""),
	GPCORE_AF_STR(9,"","","","","",""),
	GPCORE_AF_STR(10,"","","","","",""),
	GPCORE_AF_STR(11,"","","","","",""),
	GPCORE_AF_STR(12,"","","","","",""),
	GPCORE_AF_STR(13,"","","","","",""),
	GPCORE_AF_STR(14,"","","","","",""),
	GPCORE_AF_STR(15,"","","","","",""),
	GPCORE_AF_STR(16,"","","","","",""),
	GPCORE_AF_STR(17,"","","","","",""),
	GPCORE_AF_STR(18,"","","","","",""),
	GPCORE_AF_STR(19,"","","","","",""),
	GPCORE_AF_STR(20,"","","","","",""),
	GPCORE_AF_STR(21,"","","","","",""),
	GPCORE_AF_STR(22,"","","","","",""),
	GPCORE_AF_STR(23,"","","","","",""),
	GPCORE_AF_STR(24,"","","","","",""),
	GPCORE_AF_STR(25,"","","","","",""),
	GPCORE_AF_STR(26,"uart_0_rx","","","","",""),
	GPCORE_AF_STR(27,"","","","uart_0_tx","",""),
	GPCORE_AF_STR(28,"uart_0_cts","","","","",""),
	GPCORE_AF_STR(29,"","","","uart_0_rts","",""),
	GPCORE_AF_STR(30,"uart_1_rx","","","","",""),
	GPCORE_AF_STR(31,"","","","uart_1_tx","",""),
	GPCORE_AF_STR(32,"","","","uart_1_sd","uart_1_rts",""),
	GPCORE_AF_STR(33,"uart_1_cts","","","","uart_2_tx",""),
	GPCORE_AF_STR(34,"","","","","",""),
	GPCORE_AF_STR(35,"i2c_3_scl_hdmi_ddc","","","i2c_3_scl_hdmi_ddc","",""),
	GPCORE_AF_STR(36,"i2c_3_sda_hdmi_ddc","","","i2c_3_sda_hdmi_ddc","",""),
	GPCORE_AF_STR(37,"hdmi_cec","","","hdmi_cec","",""),
	GPCORE_AF_STR(38,"i2c_4_sda","","","i2c_4_sda","",""),
	GPCORE_AF_STR(39,"i2c_4_scl","","","i2c_4_scl","",""),
	GPCORE_AF_STR(40,"i2c_5_sda","","","i2c_5_sda","",""),
	GPCORE_AF_STR(41,"i2c_5_scl","","","i2c_5_scl","",""),
	GPCORE_AF_STR(42,"stio_0_dat0","","","stio_0_dat0","",""),
	GPCORE_AF_STR(43,"stio_0_dat1","","","stio_0_dat1","",""),
	GPCORE_AF_STR(44,"stio_0_dat2","","","stio_0_dat2","",""),
	GPCORE_AF_STR(45,"stio_0_dat3","","","stio_0_dat3","",""),
	GPCORE_AF_STR(46,"stio_0_dat4","","","stio_0_dat4","",""),
	GPCORE_AF_STR(47,"stio_0_dat5","","","stio_0_dat5","",""),
	GPCORE_AF_STR(48,"stio_0_dat6","","","stio_0_dat6","",""),
	GPCORE_AF_STR(49,"stio_0_dat7","","","stio_0_dat7","",""),
	GPCORE_AF_STR(50,"stio_0_cmd","","","","",""),
	GPCORE_AF_STR(51,"","","","stio_0_clk","",""),
	GPCORE_AF_STR(52,"stio_0_wp_b","","","","",""),
	GPCORE_AF_STR(53,"stio_1_dat0","","","stio_1_dat0","",""),
	GPCORE_AF_STR(54,"stio_1_dat2","","","stio_1_dat2","",""),
	GPCORE_AF_STR(55,"stio_1_dat3","","","stio_1_dat3","",""),
	GPCORE_AF_STR(56,"stio_2_dat0","","","stio_2_dat0","",""),
	GPCORE_AF_STR(57,"stio_2_dat2","","","stio_2_dat2","",""),
	GPCORE_AF_STR(58,"stio_2_dat3","","","stio_2_dat3","",""),
	GPCORE_AF_STR(59,"stio_1_cmd","","","stio_1_cmd","",""),
	GPCORE_AF_STR(60,"","","","stio_1_clk","",""),
	GPCORE_AF_STR(61,"stio_2_cmd","","","stio_2_cmd","",""),
	GPCORE_AF_STR(62,"","","","stio_2_clk","",""),
	GPCORE_AF_STR(63,"camerasb0","","","camerasb0","",""),
	GPCORE_AF_STR(64,"camerasb1","","","camerasb1","",""),
	GPCORE_AF_STR(65,"camerasb2","","","camerasb2","",""),
	GPCORE_AF_STR(66,"camerasb3","","","camerasb3","",""),
	GPCORE_AF_STR(67,"","","","","",""),
	GPCORE_AF_STR(68,"","","","","",""),
	GPCORE_AF_STR(69,"","","","","",""),
	GPCORE_AF_STR(70,"","","","","",""),
	GPCORE_AF_STR(71,"","","","","",""),
	GPCORE_AF_STR(72,"","","","","",""),
	GPCORE_AF_STR(73,"uart_2_rx","","","","",""),
	GPCORE_AF_STR(74,"","","","uart_2_tx","",""),
	GPCORE_AF_STR(75,"","","","","",""),
	GPCORE_AF_STR(76,"camerasb4","","","camerasb4","",""),
	GPCORE_AF_STR(77,"camerasb5","","","camerasb5","",""),
	GPCORE_AF_STR(78,"camerasb6","","","camerasb6","",""),
	GPCORE_AF_STR(79,"camerasb7","","","camerasb7","",""),
	GPCORE_AF_STR(80,"camerasb8","","","camerasb8","",""),
	GPCORE_AF_STR(81,"camerasb9","","","camerasb9","",""),
	GPCORE_AF_STR(82,"","","","","",""),
	GPCORE_AF_STR(83,"","","","","",""),
	GPCORE_AF_STR(84,"","","","","",""),
	GPCORE_AF_STR(85,"","","","","",""),
	GPCORE_AF_STR(86,"","","","","",""),
	GPCORE_AF_STR(87,"","","","","",""),
	GPCORE_AF_STR(88,"","","","","",""),
	GPCORE_AF_STR(89,"","","","","",""),
	GPCORE_AF_STR(90,"","","","","",""),
	GPCORE_AF_STR(91,"","","","","",""),
	GPCORE_AF_STR(92,"","","","","",""),
	GPCORE_AF_STR(93,"","","","","",""),
	GPCORE_AF_STR(94,"","","","","",""),
	GPCORE_AF_STR(95,"","","","","",""),

};
#endif

#define IRQ_TYPE_EDGE	(1 << 0)
#define IRQ_TYPE_LEVEL	(1 << 1)

/*
 * Langwell chip has 64 pins and thus there are 2 32bit registers to control
 * each feature, while Penwell chip has 96 pins for each block, and need 3 32bit
 * registers to control them, so we only define the order here instead of a
 * structure, to get a bit offset for a pin (use GPDR as an example):
 *
 * nreg = ngpio / 32;
 * reg = offset / 32;
 * bit = offset % 32;
 * reg_addr = reg_base + GPDR * nreg * 4 + reg * 4;
 *
 * so the bit of reg_addr is to control pin offset's GPDR feature
*/

enum GPIO_REG {
	GPLR = 0,	/* pin level read-only */
	GPDR,		/* pin direction */
	GPSR,		/* pin set */
	GPCR,		/* pin clear */
	GRER,		/* rising edge detect */
	GFER,		/* falling edge detect */
	GEDR,		/* edge detect result */
	GAFR,		/* alt function */
	GFBR = 9,       /* glitch filter bypas */
	GPIT,		/* interrupt type */
	GPIP = GFER,	/* level interrupt polarity */
	GPIM = GRER,	/* level interrupt mask */

	/* the following registers only exist on MRFLD */
	GFBR_TNG = 6,
	GIMR,		/* interrupt mask */
	GISR,		/* interrupt source */
};

enum GPIO_CONTROLLERS {
	LINCROFT_GPIO,
	PENWELL_GPIO_AON,
	PENWELL_GPIO_CORE,
	CLOVERVIEW_GPIO_AON,
	CLOVERVIEW_GPIO_CORE,
	TANGIER_GPIO,
};

static int platform;	/* Platform type */

/* langwell gpio driver data */
struct lnw_gpio_ddata_t {
	u16 ngpio;		/* number of gpio pins */
	u32 gplr_offset;	/* offset of first GPLR register from base */
	u32 flis_base;		/* base address of FLIS registers */
	u32 flis_len;		/* length of FLIS registers */
	u32 (*get_flis_offset)(int gpio);
	u32 chip_irq_type;	/* chip interrupt type */
};

struct gpio_flis_pair {
	int gpio;	/* gpio number */
	int offset;	/* register offset from FLIS base */
};

/*
 * The following mapping table lists the pin and flis offset pair
 * of some key gpio pins, the offset of other gpios can be calculated
 * from the table.
 */
static struct gpio_flis_pair gpio_flis_mapping_table[] = {
	{ 0,	0x2900 },
	{ 12,	0x2544 },
	{ 14,	0x0958 },
	{ 16,	0x2D18 },
	{ 17,	0x1D10 },
	{ 19,	0x1D00 },
	{ 23,	0x1D18 },
	{ 31,	-EINVAL }, /* No GPIO 31 in pin list */
	{ 32,	0x1508 },
	{ 44,	0x3500 },
	{ 64,	0x2534 },
	{ 68,	0x2D1C },
	{ 70,	0x1500 },
	{ 72,	0x3D00 },
	{ 77,	0x0D00 },
	{ 97,	0x0954 },
	{ 98,	-EINVAL }, /* No GPIO 98-101 in pin list */
	{ 102,	0x1910 },
	{ 120,	0x1900 },
	{ 124,	0x2100 },
	{ 136,	-EINVAL }, /* No GPIO 136 in pin list */
	{ 137,	0x2D00 },
	{ 143,	-EINVAL }, /* No GPIO 143-153 in pin list */
	{ 154,	0x092C },
	{ 164,	0x3900 },
	{ 177,	0x2500 },
	{ 190,	0x2D50 },
};

static u32 get_flis_offset_by_gpio(int gpio)
{
	int i;
	int start;
	u32 offset = -EINVAL;

	for (i = 0; i < ARRAY_SIZE(gpio_flis_mapping_table) - 1; i++) {
		if (gpio >= gpio_flis_mapping_table[i].gpio
			&& gpio < gpio_flis_mapping_table[i + 1].gpio)
			break;
	}

	start = gpio_flis_mapping_table[i].gpio;

	if (gpio_flis_mapping_table[i].offset != -EINVAL) {
		offset = gpio_flis_mapping_table[i].offset
				+ (gpio - start) * 4;
	}

	return offset;
}

static struct lnw_gpio_ddata_t lnw_gpio_ddata[] = {
	[LINCROFT_GPIO] = {
		.ngpio = 64,
	},
	[PENWELL_GPIO_AON] = {
		.ngpio = 96,
		.chip_irq_type = IRQ_TYPE_EDGE,
	},
	[PENWELL_GPIO_CORE] = {
		.ngpio = 96,
		.chip_irq_type = IRQ_TYPE_EDGE,
	},
	[CLOVERVIEW_GPIO_AON] = {
		.ngpio = 96,
		.chip_irq_type = IRQ_TYPE_EDGE | IRQ_TYPE_LEVEL,
	},
	[CLOVERVIEW_GPIO_CORE] = {
		.ngpio = 96,
		.chip_irq_type = IRQ_TYPE_EDGE,
	},
	[TANGIER_GPIO] = {
		.ngpio = 192,
		.gplr_offset = 4,
		.flis_base = 0xFF0C0000,
		.flis_len = 0x8000,
		.get_flis_offset = get_flis_offset_by_gpio,
		.chip_irq_type = IRQ_TYPE_EDGE,
	},
};

static struct lnw_gpio_ddata_t *ddata;

struct lnw_gpio {
	struct gpio_chip		chip;
	void				*reg_base;
	void				*reg_gplr;
	void				*flis_base;
	spinlock_t			lock;
	unsigned			irq_base;
	int				wakeup;
	struct pci_dev			*pdev;
	u32				(*get_flis_offset)(int gpio);
	u32				chip_irq_type;
};

static void __iomem *gpio_reg(struct gpio_chip *chip, unsigned offset,
			enum GPIO_REG reg_type)
{
	struct lnw_gpio *lnw = container_of(chip, struct lnw_gpio, chip);
	unsigned nreg = chip->ngpio / 32;
	u8 reg = offset / 32;
	void __iomem *ptr;

#ifdef CONFIG_INTEL_EXTENDED_SUPPORT
	ptr = reg_type==GAFR?((void __iomem *)(lnw->reg_base + reg_type * nreg * 4 + (offset>>4) * 4))\
		: ( (void __iomem *)(lnw->reg_base + reg_type * nreg * 4 + reg * 4));
#else
	ptr = (void __iomem *)(lnw->reg_gplr + reg_type * nreg * 4 + reg * 4);
#endif
	return ptr;
}

void lnw_gpio_set_alt(int gpio, int alt)
{
	struct lnw_gpio *lnw;
	u32 __iomem *mem;
	int reg;
	int bit;
	u32 offset;
	u32 value;
	unsigned long flags;

	/* use this trick to get memio */
	lnw = irq_get_chip_data(gpio_to_irq(gpio));
	if (!lnw) {
		dev_err(lnw->chip.dev, "langwell_gpio: can not find pin %d\n",
			gpio);
		return;
	}
	if (gpio < lnw->chip.base || gpio >= lnw->chip.base + lnw->chip.ngpio) {
		dev_err(lnw->chip.dev,
			"langwell_gpio: wrong pin %d to config alt\n", gpio);
		return;
	}
	if (lnw->irq_base + gpio - lnw->chip.base != gpio_to_irq(gpio)) {
		dev_err(lnw->chip.dev,
			"langwell_gpio: wrong chip data for pin %d\n", gpio);
		return;
	}
	gpio -= lnw->chip.base;

	if (platform != INTEL_MID_CPU_CHIP_TANGIER) {
		reg = gpio / 16;
		bit = gpio % 16;

#ifdef CONFIG_INTEL_EXTENDED_SUPPORT
	mem = gpio_reg(&lnw->chip, gpio, GAFR);
	spin_lock_irqsave(&lnw->lock, flags);
	value = readl(mem);
	value &= ~(3 << (bit * 2));
	value |= (alt & 3) << (bit * 2);
	writel(value, mem);
	spin_unlock_irqrestore(&lnw->lock, flags);
	dev_dbg(lnw->chip.dev, "ALT: writing 0x%x to %p\n", value, mem);
#else
		mem = gpio_reg(&lnw->chip, 0, GAFR);
		spin_lock_irqsave(&lnw->lock, flags);
		value = readl(mem + reg);
		value &= ~(3 << (bit * 2));
		value |= (alt & 3) << (bit * 2);
		writel(value, mem + reg);
		spin_unlock_irqrestore(&lnw->lock, flags);
		dev_dbg(lnw->chip.dev, "ALT: writing 0x%x to %p\n",
			value, mem + reg);
#endif
	} else {
		offset = lnw->get_flis_offset(gpio);
		if (WARN(offset == -EINVAL, "invalid pin %d\n", gpio))
			return;

		mem = (void __iomem *)(lnw->flis_base + offset);

		spin_lock_irqsave(&lnw->lock, flags);
		value = readl(mem);
		value &= ~7;
		value |= (alt & 7);
		writel(value, mem);
		spin_unlock_irqrestore(&lnw->lock, flags);
		dev_dbg(lnw->chip.dev, "ALT: writing 0x%x to %p\n", value, mem);
	}
}
EXPORT_SYMBOL_GPL(lnw_gpio_set_alt);

static int lnw_gpio_set_debounce(struct gpio_chip *chip, unsigned offset,
				 unsigned debounce)
{
	struct lnw_gpio *lnw = container_of(chip, struct lnw_gpio, chip);
	void __iomem *gfbr;
	unsigned long flags;
	u32 value;
	enum GPIO_REG reg_type;

	reg_type = platform == INTEL_MID_CPU_CHIP_TANGIER ? GFBR_TNG : GFBR;
	gfbr = gpio_reg(chip, offset, reg_type);

	if (lnw->pdev)
		pm_runtime_get(&lnw->pdev->dev);

	spin_lock_irqsave(&lnw->lock, flags);
	value = readl(gfbr);
	if (debounce) {
		/* debounce bypass disable */
		value &= ~BIT(offset % 32);
	} else {
		/* debounce bypass enable */
		value |= BIT(offset % 32);
	}
	writel(value, gfbr);
	spin_unlock_irqrestore(&lnw->lock, flags);

	if (lnw->pdev)
		pm_runtime_put(&lnw->pdev->dev);

	return 0;
}

static void __iomem *gpio_reg_2bit(struct gpio_chip *chip, unsigned offset,
				   enum GPIO_REG reg_type)
{
	struct lnw_gpio *lnw = container_of(chip, struct lnw_gpio, chip);
	unsigned nreg = chip->ngpio / 32;
	u8 reg = offset / 16;
	void __iomem *ptr;

	ptr = (void __iomem *)(lnw->reg_base + reg_type * nreg * 4 + reg * 4);
	return ptr;
}

static int lnw_gpio_request(struct gpio_chip *chip, unsigned offset)
{
	void __iomem *gafr = gpio_reg_2bit(chip, offset, GAFR);
	u32 value = readl(gafr);
	int shift = (offset % 16) << 1, af = (value >> shift) & 3;

	if (af) {
		value &= ~(3 << shift);
		writel(value, gafr);
	}
	return 0;
}

static int lnw_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	void __iomem *gplr = gpio_reg(chip, offset, GPLR);

	return readl(gplr) & BIT(offset % 32);
}

static void lnw_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	void __iomem *gpsr, *gpcr;

	if (value) {
		gpsr = gpio_reg(chip, offset, GPSR);
		writel(BIT(offset % 32), gpsr);
	} else {
		gpcr = gpio_reg(chip, offset, GPCR);
		writel(BIT(offset % 32), gpcr);
	}
}

static int lnw_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	struct lnw_gpio *lnw = container_of(chip, struct lnw_gpio, chip);
	void __iomem *gpdr = gpio_reg(chip, offset, GPDR);
	u32 value;
	unsigned long flags;

	if (lnw->pdev)
		pm_runtime_get(&lnw->pdev->dev);

	spin_lock_irqsave(&lnw->lock, flags);
	value = readl(gpdr);
	value &= ~BIT(offset % 32);
	writel(value, gpdr);
	spin_unlock_irqrestore(&lnw->lock, flags);

	if (lnw->pdev)
		pm_runtime_put(&lnw->pdev->dev);

	return 0;
}

static int lnw_gpio_direction_output(struct gpio_chip *chip,
			unsigned offset, int value)
{
	struct lnw_gpio *lnw = container_of(chip, struct lnw_gpio, chip);
	void __iomem *gpdr = gpio_reg(chip, offset, GPDR);
	unsigned long flags;

	lnw_gpio_set(chip, offset, value);

	if (lnw->pdev)
		pm_runtime_get(&lnw->pdev->dev);

	spin_lock_irqsave(&lnw->lock, flags);
	value = readl(gpdr);
	value |= BIT(offset % 32);
	writel(value, gpdr);
	spin_unlock_irqrestore(&lnw->lock, flags);

	if (lnw->pdev)
		pm_runtime_put(&lnw->pdev->dev);

	return 0;
}

#ifdef CONFIG_INTEL_EXTENDED_SUPPORT
static int lnw_gpio_get_direction(struct gpio_chip *chip,
						   unsigned offset)
{
	void __iomem *gpdr = gpio_reg(chip, offset-chip->base, GPDR);

	if (readl(gpdr) & BIT(offset % 32))
		return GPIOF_DIR_OUT;
	return GPIOF_DIR_IN;
}

void lnw_gpio_arch_dbg_info(struct seq_file *s, struct gpio_chip *chip,
				unsigned offset)
{
	void __iomem *gafr = gpio_reg(chip, offset-chip->base, GAFR);
	int af;
	int direction = lnw_gpio_get_direction(chip, offset);
	int ret = 0;
	int i;

	/* put ipc read mip header here since intel_scu_ipc init
	 * is later than gpio */
	if( unlikely(!flis) )
	{
		flis = kzalloc(192, GFP_KERNEL);
		if( flis )
		{
			ret = intel_scu_ipc_read_mip(flis, 0xc0 , 0x374, 1);
			if( ret )
			{
				printk("%s: read flis failed\n", __func__);
			}else{
				dev_dbg(chip->dev, "Begin dump FLIS header\n");
				for(i=0;i<192;i++)
				{
					dev_dbg(chip->dev, "%02x ",flis[i]);
					if( (i%16)==15 )
						dev_dbg(chip->dev,"\n");
				}
			}
		}else
			printk("%s: malloc flis buffer failed\n",__func__);
	}

	af = (readl(gafr) >> (((offset-chip->base)%16) * 2)) & 0x3 ;

	seq_printf(s, " af(%-2d,%-3s,%-12s)", af,direction?"in":"out", af_list[offset][af?((1-direction)*3+af):af]);
	if ( flis )
	{
		seq_printf(s, " flis(%-2.2x)", flis[flis_offset[offset]]);
	}
}
#endif

static int lnw_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	struct lnw_gpio *lnw = container_of(chip, struct lnw_gpio, chip);
	return lnw->irq_base + offset;
}

static int lnw_irq_type(struct irq_data *d, unsigned type)
{
	struct lnw_gpio *lnw = irq_data_get_irq_chip_data(d);
	u32 gpio = d->irq - lnw->irq_base;
	unsigned long flags;
	u32 value;
	int ret = 0;
	void __iomem *grer = gpio_reg(&lnw->chip, gpio, GRER);
	void __iomem *gfer = gpio_reg(&lnw->chip, gpio, GFER);
	void __iomem *gpit, *gpip;

	if (gpio >= lnw->chip.ngpio)
		return -EINVAL;

	if (lnw->pdev)
		pm_runtime_get(&lnw->pdev->dev);

	/* chip supports level interrupt has extra GPIT registers */
	if (lnw->chip_irq_type & IRQ_TYPE_LEVEL) {
		gpit = gpio_reg(&lnw->chip, gpio, GPIT);
		gpip = gpio_reg(&lnw->chip, gpio, GPIP);

		spin_lock_irqsave(&lnw->lock, flags);
		if (type & IRQ_TYPE_LEVEL_MASK) {
			value = readl(gpit) | BIT(gpio % 32);
			writel(value, gpit);

			if (type & IRQ_TYPE_LEVEL_LOW)
				value = readl(gpip) | BIT(gpio % 32);
			else
				value = readl(gpip) & (~BIT(gpio % 32));
			writel(value, gpip);

			__irq_set_handler_locked(d->irq, handle_level_irq);
		} else if (type & IRQ_TYPE_EDGE_BOTH) {
			value = readl(gpit) & (~BIT(gpio % 32));
			writel(value, gpit);

			if (type & IRQ_TYPE_EDGE_RISING)
				value = readl(grer) | BIT(gpio % 32);
			else
				value = readl(grer) & (~BIT(gpio % 32));
			writel(value, grer);

			if (type & IRQ_TYPE_EDGE_FALLING)
				value = readl(gfer) | BIT(gpio % 32);
			else
				value = readl(gfer) & (~BIT(gpio % 32));
			writel(value, gfer);

			__irq_set_handler_locked(d->irq, handle_edge_irq);
		}
		spin_unlock_irqrestore(&lnw->lock, flags);
	} else {
		if (type & IRQ_TYPE_LEVEL_MASK) {
			ret = -EINVAL;
		} else if (type & IRQ_TYPE_EDGE_BOTH) {
			spin_lock_irqsave(&lnw->lock, flags);

			if (type & IRQ_TYPE_EDGE_RISING)
				value = readl(grer) | BIT(gpio % 32);
			else
				value = readl(grer) & (~BIT(gpio % 32));
			writel(value, grer);

			if (type & IRQ_TYPE_EDGE_FALLING)
				value = readl(gfer) | BIT(gpio % 32);
			else
				value = readl(gfer) & (~BIT(gpio % 32));
			writel(value, gfer);

			spin_unlock_irqrestore(&lnw->lock, flags);
		}
	}

	if (lnw->pdev)
		pm_runtime_put(&lnw->pdev->dev);

	return ret;
}

static int lnw_set_maskunmask(struct irq_data *d, enum GPIO_REG reg_type,
				unsigned unmask)
{
	struct lnw_gpio *lnw = irq_data_get_irq_chip_data(d);
	u32 gpio = d->irq - lnw->irq_base;
	unsigned long flags;
	u32 value;
	void __iomem *gp_reg;

	gp_reg = gpio_reg(&lnw->chip, gpio, reg_type);

	spin_lock_irqsave(&lnw->lock, flags);

	if (unmask) {
		/* enable interrupt from GPIO input pin */
		value = readl(gp_reg) | BIT(gpio % 32);
	} else {
		/* disable interrupt from GPIO input pin */
		value = readl(gp_reg) & (~BIT(gpio % 32));
	}

	writel(value, gp_reg);

	spin_unlock_irqrestore(&lnw->lock, flags);

	return 0;
}

static void lnw_irq_unmask(struct irq_data *d)
{
	struct lnw_gpio *lnw = irq_data_get_irq_chip_data(d);
	u32 gpio = d->irq - lnw->irq_base;
	void __iomem *gpit;

	if (gpio >= lnw->chip.ngpio)
		return;

	if (platform == INTEL_MID_CPU_CHIP_CLOVERVIEW &&
		(lnw->chip_irq_type & IRQ_TYPE_LEVEL)) {
			gpit = gpio_reg(&lnw->chip, gpio, GPIT);

			/* if it's level trigger, unmask GPIM */
			if (readl(gpit) & BIT(gpio % 32))
				lnw_set_maskunmask(d, GPIM, 1);

	} else if (platform == INTEL_MID_CPU_CHIP_TANGIER) {
		lnw_set_maskunmask(d, GIMR, 1);
	}
}

static void lnw_irq_mask(struct irq_data *d)
{
	struct lnw_gpio *lnw = irq_data_get_irq_chip_data(d);
	u32 gpio = d->irq - lnw->irq_base;
	void __iomem *gpit;

	if (gpio >= lnw->chip.ngpio)
		return;

	if (platform == INTEL_MID_CPU_CHIP_CLOVERVIEW &&
		(lnw->chip_irq_type & IRQ_TYPE_LEVEL)) {
			gpit = gpio_reg(&lnw->chip, gpio, GPIT);

			/* if it's level trigger, mask GPIM */
			if (readl(gpit) & BIT(gpio % 32))
				lnw_set_maskunmask(d, GPIM, 0);

	} else if (platform == INTEL_MID_CPU_CHIP_TANGIER) {
		lnw_set_maskunmask(d, GIMR, 0);
	}
}

static int lnw_irq_wake(struct irq_data *d, unsigned on)
{
	return 0;
}

static void lnw_irq_ack(struct irq_data *d)
{
}

static void lnw_irq_shutdown(struct irq_data *d)
{
	struct lnw_gpio *lnw = irq_data_get_irq_chip_data(d);
	u32 gpio = d->irq - lnw->irq_base;
	unsigned long flags;
	u32 value;
	void __iomem *grer = gpio_reg(&lnw->chip, gpio, GRER);
	void __iomem *gfer = gpio_reg(&lnw->chip, gpio, GFER);

	spin_lock_irqsave(&lnw->lock, flags);
	value = readl(grer) & (~BIT(gpio % 32));
	writel(value, grer);
	value = readl(gfer) & (~BIT(gpio % 32));
	writel(value, gfer);
	spin_unlock_irqrestore(&lnw->lock, flags);
};

static struct irq_chip lnw_irqchip = {
	.name		= "LNW-GPIO",
	.flags		= IRQCHIP_SET_TYPE_MASKED,
	.irq_mask	= lnw_irq_mask,
	.irq_unmask	= lnw_irq_unmask,
	.irq_set_type	= lnw_irq_type,
	.irq_set_wake	= lnw_irq_wake,
	.irq_ack	= lnw_irq_ack,
	.irq_shutdown	= lnw_irq_shutdown,
};

static DEFINE_PCI_DEVICE_TABLE(lnw_gpio_ids) = {   /* pin number */
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x080f),
		.driver_data = LINCROFT_GPIO },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x081f),
		.driver_data = PENWELL_GPIO_AON },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x081a),
		.driver_data = PENWELL_GPIO_CORE },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x08eb),
		.driver_data = CLOVERVIEW_GPIO_AON },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x08f7),
		.driver_data = CLOVERVIEW_GPIO_CORE },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, 0x1199),
		.driver_data = TANGIER_GPIO },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, lnw_gpio_ids);

static void lnw_irq_handler(unsigned irq, struct irq_desc *desc)
{
	struct irq_data *data = irq_desc_get_irq_data(desc);
	struct lnw_gpio *lnw = irq_data_get_irq_handler_data(data);
	struct irq_chip *chip = irq_data_get_irq_chip(data);
	u32 base, gpio, mask;
	unsigned long pending;
	void __iomem *gp_reg;
	enum GPIO_REG reg_type;

	reg_type = platform == INTEL_MID_CPU_CHIP_TANGIER ? GISR : GEDR;

	/* check GPIO controller to check which pin triggered the interrupt */
	for (base = 0; base < lnw->chip.ngpio; base += 32) {
		gp_reg = gpio_reg(&lnw->chip, base, reg_type);
		pending = readl(gp_reg);
		while (pending) {
			gpio = __ffs(pending);
			if (!lnw->wakeup)
				dev_info(&lnw->pdev->dev,
				"wakeup source: gpio %d\n", base + gpio);
			mask = BIT(gpio);
			pending &= ~mask;
			/* Clear before handling so we can't lose an edge */
			writel(mask, gp_reg);
			generic_handle_irq(lnw->irq_base + base + gpio);
			/* Read again to check if same irq comes */
			pending = readl(gp_reg);
		}
	}

	chip->irq_eoi(data);
}

#ifdef CONFIG_PM
static int lnw_gpio_suspend_noirq(struct device *dev)
{
	struct pci_dev *pdev = container_of(dev, struct pci_dev, dev);
	struct lnw_gpio *lnw = pci_get_drvdata(pdev);

	lnw->wakeup = 0;

	return 0;
}

static int lnw_gpio_resume_noirq(struct device *dev)
{
	struct pci_dev *pdev = container_of(dev, struct pci_dev, dev);
	struct lnw_gpio *lnw = pci_get_drvdata(pdev);

	lnw->wakeup = 1;

	return 0;
}

static int lnw_gpio_runtime_resume(struct device *dev)
{
	return 0;
}

static int lnw_gpio_runtime_suspend(struct device *dev)
{
	return 0;
}

static int lnw_gpio_runtime_idle(struct device *dev)
{
	int err = pm_schedule_suspend(dev, 500);

	if (!err)
		return 0;

	return -EBUSY;
}

#else
#define lnw_gpio_suspend_noirq		NULL
#define lnw_gpio_resume_noirq		NULL
#define lnw_gpio_runtime_suspend	NULL
#define lnw_gpio_runtime_resume		NULL
#define lnw_gpio_runtime_idle		NULL
#endif

static const struct dev_pm_ops lnw_gpio_pm_ops = {
	.suspend_noirq = lnw_gpio_suspend_noirq,
	.resume_noirq = lnw_gpio_resume_noirq,
	.runtime_suspend = lnw_gpio_runtime_suspend,
	.runtime_resume = lnw_gpio_runtime_resume,
	.runtime_idle = lnw_gpio_runtime_idle,
};

static int __devinit lnw_gpio_probe(struct pci_dev *pdev,
			const struct pci_device_id *id)
{
	void *base;
	int i;
	resource_size_t start, len;
	struct lnw_gpio *lnw;
	u32 irq_base;
	u32 gpio_base;
	int retval = 0;
	int pid;

	pid = id->driver_data;
	ddata = &lnw_gpio_ddata[pid];

	retval = pci_enable_device(pdev);
	if (retval)
		goto done;

	retval = pci_request_regions(pdev, "langwell_gpio");
	if (retval) {
		dev_err(&pdev->dev, "error requesting resources\n");
		goto err2;
	}
	/* get the irq_base from bar1 */
	start = pci_resource_start(pdev, 1);
	len = pci_resource_len(pdev, 1);
	base = ioremap_nocache(start, len);
	if (!base) {
		dev_err(&pdev->dev, "error mapping bar1\n");
		goto err3;
	}
	irq_base = *(u32 *)base;
	gpio_base = *((u32 *)base + 1);
	/* release the IO mapping, since we already get the info from bar1 */
	iounmap(base);
	/* get the register base from bar0 */
	start = pci_resource_start(pdev, 0);
	len = pci_resource_len(pdev, 0);
	base = ioremap_nocache(start, len);
	if (!base) {
		dev_err(&pdev->dev, "error mapping bar0\n");
		retval = -EFAULT;
		goto err3;
	}

	lnw = kzalloc(sizeof(struct lnw_gpio), GFP_KERNEL);
	if (!lnw) {
		dev_err(&pdev->dev, "can't allocate langwell_gpio chip data\n");
		retval = -ENOMEM;
		goto err4;
	}
	lnw->reg_base = base;
	lnw->reg_gplr = lnw->reg_base + ddata->gplr_offset;
	lnw->irq_base = irq_base;
	lnw->wakeup = 1;
	lnw->get_flis_offset = ddata->get_flis_offset;
	lnw->chip_irq_type = ddata->chip_irq_type;
	lnw->chip.label = dev_name(&pdev->dev);
	lnw->chip.request = lnw_gpio_request;
	lnw->chip.direction_input = lnw_gpio_direction_input;
	lnw->chip.direction_output = lnw_gpio_direction_output;
	lnw->chip.get = lnw_gpio_get;
	lnw->chip.set = lnw_gpio_set;
	lnw->chip.to_irq = lnw_gpio_to_irq;
	lnw->chip.base = gpio_base;
	lnw->chip.ngpio = ddata->ngpio;
	lnw->chip.can_sleep = 0;
	lnw->chip.set_debounce = lnw_gpio_set_debounce;
#ifdef CONFIG_INTEL_EXTENDED_SUPPORT
	lnw->chip.get_direction = lnw_gpio_get_direction;
	lnw->chip.arch_dbg_info = lnw_gpio_arch_dbg_info;
#endif
	lnw->pdev = pdev;
	pci_set_drvdata(pdev, lnw);
	retval = gpiochip_add(&lnw->chip);
	if (retval) {
		dev_err(&pdev->dev, "langwell gpiochip_add error %d\n", retval);
		goto err5;
	}
	lnw->irq_base = irq_alloc_descs(-1, lnw->irq_base, lnw->chip.ngpio,
					NUMA_NO_NODE);
	if (lnw->irq_base < 0) {
		dev_err(&pdev->dev, "langwell irq_alloc_desc failed %d\n",
			lnw->irq_base);
		goto err5;
	}
	irq_set_handler_data(pdev->irq, lnw);
	irq_set_chained_handler(pdev->irq, lnw_irq_handler);
	for (i = 0; i < lnw->chip.ngpio; i++) {
		irq_set_chip_and_handler_name(i + lnw->irq_base, &lnw_irqchip,
					      handle_edge_irq, "demux");
		irq_set_chip_data(i + lnw->irq_base, lnw);
	}

	if (ddata->flis_base) {
		lnw->flis_base = ioremap_nocache(ddata->flis_base,
					ddata->flis_len);
		if (!lnw->flis_base) {
			dev_err(&pdev->dev, "error mapping flis base\n");
			retval = -EFAULT;
			goto err6;
		}
	}

	spin_lock_init(&lnw->lock);

	pm_runtime_put_noidle(&pdev->dev);
	pm_runtime_allow(&pdev->dev);

	goto done;
err6:
	irq_free_descs(lnw->irq_base, lnw->chip.ngpio);
err5:
	kfree(lnw);
err4:
	iounmap(base);
err3:
	pci_release_regions(pdev);
err2:
	pci_disable_device(pdev);
done:
	return retval;
}

static struct pci_driver lnw_gpio_driver = {
	.name		= "langwell_gpio",
	.id_table	= lnw_gpio_ids,
	.probe		= lnw_gpio_probe,
	.driver		= {
		.pm	= &lnw_gpio_pm_ops,
	},
};


static int __devinit wp_gpio_probe(struct platform_device *pdev)
{
	struct lnw_gpio *lnw;
	struct gpio_chip *gc;
	struct resource *rc;
	int retval = 0;

	rc = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!rc)
		return -EINVAL;

	lnw = kzalloc(sizeof(struct lnw_gpio), GFP_KERNEL);
	if (!lnw) {
		dev_err(&pdev->dev,
			"can't allocate whitneypoint_gpio chip data\n");
		return -ENOMEM;
	}
	lnw->reg_base = ioremap_nocache(rc->start, resource_size(rc));
	if (lnw->reg_base == NULL) {
		retval = -EINVAL;
		goto err_kmalloc;
	}
	spin_lock_init(&lnw->lock);
	gc = &lnw->chip;
	gc->label = dev_name(&pdev->dev);
	gc->owner = THIS_MODULE;
	gc->direction_input = lnw_gpio_direction_input;
	gc->direction_output = lnw_gpio_direction_output;
	gc->get = lnw_gpio_get;
	gc->set = lnw_gpio_set;
	gc->to_irq = NULL;
	gc->base = 0;
	gc->ngpio = 64;
	gc->can_sleep = 0;
	retval = gpiochip_add(gc);
	if (retval) {
		dev_err(&pdev->dev, "whitneypoint gpiochip_add error %d\n",
								retval);
		goto err_ioremap;
	}
	platform_set_drvdata(pdev, lnw);
	return 0;
err_ioremap:
	iounmap(lnw->reg_base);
err_kmalloc:
	kfree(lnw);
	return retval;
}

static int __devexit wp_gpio_remove(struct platform_device *pdev)
{
	struct lnw_gpio *lnw = platform_get_drvdata(pdev);
	int err;
	err = gpiochip_remove(&lnw->chip);
	if (err)
		dev_err(&pdev->dev, "failed to remove gpio_chip.\n");
	iounmap(lnw->reg_base);
	kfree(lnw);
	platform_set_drvdata(pdev, NULL);
	return 0;
}

static struct platform_driver wp_gpio_driver = {
	.probe		= wp_gpio_probe,
	.remove		= __devexit_p(wp_gpio_remove),
	.driver		= {
		.name	= "wp_gpio",
		.owner	= THIS_MODULE,
	},
};

static int __init lnw_gpio_init(void)
{
	int ret;

	platform = intel_mid_identify_cpu();
	if (platform == 0)
		return -ENODEV;

	ret =  pci_register_driver(&lnw_gpio_driver);
	if (ret < 0)
		return ret;
	ret = platform_driver_register(&wp_gpio_driver);
	if (ret < 0)
		pci_unregister_driver(&lnw_gpio_driver);
	return ret;
}

fs_initcall(lnw_gpio_init);
