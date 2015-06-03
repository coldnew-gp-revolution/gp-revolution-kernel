/*
 * Support for Medifield PNW Camera Imaging ISP subsystem.
 *
 * Copyright (c) 2010 Intel Corporation. All Rights Reserved.
 *
 * Copyright (c) 2010 Silicon Hive www.siliconhive.com.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */
#ifndef	__ATOMISP_TABLES_H__
#define	__ATOMISP_TABLES_H__

#include "sh_css_params.h"

/*Sepia image effect table*/
static const struct sh_css_cc_config sepia_cc_config = {
	.fraction_bits  = 8,
	.matrix	 = {141, 18, 68, -40, -5, -19, 35, 4, 16},
};

/*Negative image effect table*/
static const struct sh_css_cc_config nega_cc_config = {
	.fraction_bits  = 8,
	.matrix	 = {255, 29, 120, 0, 374, 342, 0, 672, -301},
};

/*Mono image effect table*/
static const struct sh_css_cc_config mono_cc_config = {
	.fraction_bits  = 8,
	.matrix	 = {255, 29, 120, 0, 0, 0, 0, 0, 0},
};

/*Skin whiten image effect table*/
static struct sh_css_macc_table skin_low_macc_table = {
	.data = {
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	7168, 0, 2048, 8192,
	5120, -1024, 2048, 8192,
	8192, 2048, -1024, 5120,
	8192, 2048, 0, 7168,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192
	}
};

static struct sh_css_macc_table skin_medium_macc_table = {
	.data = {
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	5120, 0, 6144, 8192,
	3072, -1024, 2048, 6144,
	6144, 2048, -1024, 3072,
	8192, 6144, 0, 5120,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192
	}
};

static struct sh_css_macc_table skin_high_macc_table = {
	.data = {
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	4096, 0, 8192, 8192,
	0, -2048, 4096, 6144,
	6144, 4096, -2048, 0,
	8192, 8192, 0, 4096,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192
	}
};

/*Blue enhencement image effect table*/
static struct sh_css_macc_table blue_macc_table = {
	.data = {
	9728, -3072, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	9728, 0, -3072, 8192,
	12800, 1536, -3072, 8192,
	11264, 0, 0, 11264,
	9728, -3072, 0, 11264
	}
};

/*Green enhencement image effect table*/
static struct sh_css_macc_table green_macc_table = {
	.data = {
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	10240, 4096, 0, 8192,
	10240, 4096, 0, 12288,
	12288, 0, 0, 12288,
	14336, -2048, 4096, 8192,
	10240, 0, 4096, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192,
	8192, 0, 0, 8192
	}
};

#ifdef CONFIG_X86_MRFLD
/* this table is from CSS1.5 default_ctc_table(20121003) */
static struct sh_css_ctc_table vivid_ctc_table = {
	.data = {
	0,  384,  837,  957, 1011, 1062, 1083, 1080,
	1078, 1077, 1053, 1039, 1012,  992,  969,  951,
	929,  906,  886,  866,  845,  823,  809,  790,
	772,  758,  741,  726,  711,  701,  688,  675,
	666,  656,  648,  639,  633,  626,  618,  612,
	603,  594,  582,  572,  557,  545,  529,  516,
	504,  491,  480,  467,  459,  447,  438,  429,
	419,  412,  404,  397,  389,  382,  376,  368,
	363,  357,  351,  345,  340,  336,  330,  326,
	321,  318,  312,  308,  304,  300,  297,  294,
	291,  286,  284,  281,  278,  275,  271,  268,
	261,  257,  251,  245,  240,  235,  232,  225,
	223,  218,  213,  209,  206,  204,  199,  197,
	193,  189,  186,  185,  183,  179,  177,  175,
	172,  170,  169,  167,  164,  164,  162,  160,
	158,  157,  156,  154,  154,  152,  151,  150,
	149,  148,  146,  147,  146,  144,  143,  143,
	142,  141,  140,  141,  139,  138,  138,  138,
	137,  136,  136,  135,  134,  134,  134,  133,
	132,  132,  131,  130,  131,  130,  129,  128,
	129,  127,  127,  127,  127,  125,  125,  125,
	123,  123,  122,  120,  118,  115,  114,  111,
	110,  108,  106,  105,  103,  102,  100,   99,
	97,   97,   96,   95,   94,   93,   93,   91,
	91,   91,   90,   90,   89,   89,   88,   88,
	89,   88,   88,   87,   87,   87,   87,   86,
	87,   87,   86,   87,   86,   86,   84,   84,
	82,   80,   78,   76,   74,   72,   70,   68,
	67,   65,   62,   60,   58,   56,   55,   54,
	53,   51,   49,   49,   47,   45,   45,   45,
	41,   40,   39,   39,   34,   33,   34,   32,
	25,   23,   24,   20,   13,    9,   12,    0,
	0
	}
};
#else
/*Color enhancement image effect table*/
static struct sh_css_ctc_table vivid_ctc_table = {
	.data = {
	876, 872, 869, 865, 861, 858, 854, 850,
	847, 843, 839, 835, 832, 828, 824, 821,
	817, 813, 810, 806, 802, 799, 795, 791,
	788, 784, 780, 777, 773, 769, 766, 762,
	758, 754, 751, 747, 743, 740, 736, 732,
	729, 725, 721, 718, 714, 710, 707, 703,
	699, 696, 692, 688, 685, 681, 677, 673,
	670, 666, 662, 659, 655, 651, 648, 644,
	640, 637, 633, 629, 626, 622, 618, 615,
	611, 607, 604, 600, 596, 592, 589, 585,
	581, 578, 574, 570, 567, 563, 559, 556,
	552, 548, 545, 541, 539, 537, 536, 534,
	533, 531, 530, 528, 527, 525, 524, 522,
	521, 519, 518, 516, 515, 514, 512, 511,
	509, 508, 506, 505, 503, 502, 500, 499,
	497, 496, 494, 493, 491, 490, 488, 487,
	485, 484, 482, 481, 479, 478, 476, 475,
	473, 472, 470, 469, 467, 466, 464, 463,
	461, 460, 458, 457, 455, 454, 452, 451,
	449, 448, 446, 445, 443, 442, 440, 439,
	437, 436, 434, 433, 431, 430, 428, 427,
	425, 424, 422, 421, 419, 418, 417, 415,
	414, 412, 411, 409, 408, 406, 405, 403,
	402, 400, 399, 397, 396, 394, 393, 392,
	392, 391, 391, 390, 389, 389, 388, 388,
	387, 387, 386, 385, 385, 384, 384, 383,
	383, 382, 381, 381, 380, 380, 379, 379,
	378, 377, 377, 376, 376, 375, 375, 374,
	374, 373, 372, 372, 371, 371, 370, 370,
	369, 368, 368, 367, 367, 366, 366, 365,
	364, 364, 363, 363, 362, 362, 361, 360,
	360, 359, 359, 358, 358, 357, 356, 356,
	355, 355, 354, 354, 353, 352, 352, 351,
	351, 350, 350, 349, 348, 348, 347, 347,
	346, 346, 345, 344, 344, 343, 343, 342,
	342, 341, 340, 340, 339, 339, 338, 338,
	337, 336, 336, 335, 335, 334, 334, 333,
	333, 332, 331, 331, 330, 330, 329, 329,
	328, 327, 327, 326, 326, 325, 325, 324,
	323, 323, 322, 322, 321, 321, 320, 319,
	319, 318, 318, 317, 317, 316, 315, 315,
	314, 314, 313, 313, 312, 311, 311, 310,
	310, 309, 309, 308, 307, 307, 306, 306,
	305, 305, 304, 303, 303, 302, 302, 301,
	301, 300, 299, 299, 298, 298, 297, 297,
	296, 296, 295, 294, 294, 293, 293, 292,
	292, 291, 290, 290, 289, 289, 288, 288,
	287, 286, 286, 285, 285, 284, 284, 283,
	282, 282, 281, 281, 280, 280, 279, 278,
	278, 277, 277, 276, 276, 275, 274, 274,
	273, 273, 272, 272, 271, 270, 270, 269,
	269, 268, 268, 267, 266, 266, 265, 265,
	264, 264, 263, 262, 262, 261, 261, 260,
	260, 259, 259, 258, 257, 257, 256, 256,
	255, 255, 254, 253, 253, 252, 252, 251,
	251, 250, 249, 249, 248, 248, 247, 247,
	246, 245, 245, 244, 244, 243, 243, 242,
	241, 241, 240, 240, 239, 239, 238, 237,
	237, 236, 236, 235, 235, 234, 233, 233,
	232, 232, 231, 231, 230, 229, 229, 228,
	228, 227, 227, 226, 225, 225, 224, 224,
	223, 223, 222, 221, 221, 220, 220, 219,
	219, 218, 218, 217, 216, 216, 215, 215,
	214, 214, 213, 212, 212, 211, 211, 210,
	210, 209, 208, 208, 207, 207, 206, 206,
	205, 204, 204, 203, 203, 202, 202, 201,
	200, 200, 199, 199, 198, 198, 197, 196,
	196, 195, 195, 194, 194, 193, 192, 192,
	191, 191, 190, 190, 189, 188, 188, 187,
	187, 186, 186, 185, 184, 184, 183, 183,
	182, 182, 181, 181, 180, 179, 179, 178,
	178, 177, 177, 176, 175, 175, 174, 174,
	173, 173, 172, 171, 171, 170, 170, 169,
	169, 168, 167, 167, 166, 166, 165, 165,
	164, 163, 163, 163, 162, 162, 161, 161,
	161, 160, 160, 159, 159, 159, 158, 158,
	158, 157, 157, 156, 156, 156, 155, 155,
	155, 154, 154, 153, 153, 153, 152, 152,
	152, 151, 151, 150, 150, 150, 149, 149,
	149, 148, 148, 147, 147, 147, 146, 146,
	145, 145, 145, 144, 144, 144, 143, 143,
	142, 142, 142, 141, 141, 141, 140, 140,
	139, 139, 139, 138, 138, 138, 137, 137,
	136, 136, 136, 135, 135, 134, 134, 134,
	133, 133, 133, 132, 132, 131, 131, 131,
	130, 130, 130, 129, 129, 128, 128, 128,
	127, 127, 127, 126, 126, 125, 125, 125,
	124, 124, 123, 123, 123, 122, 122, 122,
	121, 121, 120, 120, 120, 119, 119, 119,
	118, 118, 117, 117, 117, 116, 116, 116,
	115, 115, 114, 114, 114, 113, 113, 112,
	112, 112, 111, 111, 111, 110, 110, 109,
	109, 109, 108, 108, 108, 107, 107, 106,
	106, 106, 105, 105, 105, 104, 104, 103,
	103, 103, 102, 102, 101, 101, 101, 100,
	100, 100, 99, 99, 98, 98, 98, 97,
	97, 97, 96, 96, 95, 95, 95, 94,
	94, 94, 93, 93, 92, 92, 92, 91,
	91, 91, 90, 90, 89, 89, 89, 88,
	88, 87, 87, 87, 86, 86, 86, 85,
	85, 84, 84, 84, 83, 83, 83, 82,
	82, 81, 81, 81, 80, 80, 80, 79,
	79, 78, 78, 78, 77, 77, 76, 76,
	76, 75, 75, 75, 74, 74, 73, 73,
	73, 72, 72, 72, 71, 71, 70, 70,
	70, 69, 69, 69, 68, 68, 67, 67,
	67, 66, 66, 65, 65, 65, 64, 64,
	64, 63, 63, 61, 61, 61, 61, 61,
	61, 60, 60, 58, 58, 58, 58, 58,
	58, 57, 57, 56, 56, 56, 55, 55,
	54, 54, 54, 53, 53, 53, 51, 51,
	51, 51, 51, 50, 50, 50, 48, 48,
	48, 48, 48, 47, 47, 47, 45, 45,
	45, 45, 45, 44, 44, 42, 42, 42,
	42, 42, 42, 41, 41, 40, 40, 40,
	39, 39, 39, 38, 38, 37, 37, 37,
	35, 35, 35, 35, 35, 34, 34, 34,
	32, 32, 32, 32, 32, 31, 31, 31,
	29, 29, 29, 29, 29, 28, 28, 28,
	26, 26, 25, 25, 25, 25, 25, 25,
	24, 24, 22, 22, 22, 22, 22, 22,
	21, 21, 19, 19, 19, 19, 19, 18,
	18, 18, 16, 16, 16, 16, 16, 15,
	15, 15, 13, 13, 13, 12, 12, 12,
	12, 12, 10, 10, 10, 9, 9, 9,
	9, 9, 8, 8, 6, 6, 6, 6,
	6, 6, 5, 5, 3, 3, 3, 3,
	3, 3, 2, 2, 0, 0, 0, 0,
	}
};
#endif
#endif
