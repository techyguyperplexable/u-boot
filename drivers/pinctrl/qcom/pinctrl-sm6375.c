// SPDX-License-Identifier: GPL-2.0+
/*
 * Qualcomm SM6375 pinctrl
 *
 * (C) Copyright 2024 Linaro Ltd.
 *
 */

#include <dm.h>

#include "pinctrl-qcom.h"

#define MAX_PIN_NAME_LEN 32
static char pin_name[MAX_PIN_NAME_LEN] __section(".data");

static const struct pinctrl_function msm_pinctrl_functions[] = {
	{ "qup04", 1 },
	{ "gpio", 0 },
};

/* SM6375 has no tile offsets - single tile */
static const unsigned int sm6375_pin_offsets[] = {
	[0] = 0, [1] = 0, [2] = 0, [3] = 0, [4] = 0, [5] = 0, [6] = 0, [7] = 0,
	[8] = 0, [9] = 0, [10] = 0, [11] = 0, [12] = 0, [13] = 0, [14] = 0, [15] = 0,
	[16] = 0, [17] = 0, [18] = 0, [19] = 0, [20] = 0, [21] = 0, [22] = 0, [23] = 0,
	[24] = 0, [25] = 0, [26] = 0, [27] = 0, [28] = 0, [29] = 0, [30] = 0, [31] = 0,
	[32] = 0, [33] = 0, [34] = 0, [35] = 0, [36] = 0, [37] = 0, [38] = 0, [39] = 0,
	[40] = 0, [41] = 0, [42] = 0, [43] = 0, [44] = 0, [45] = 0, [46] = 0, [47] = 0,
	[48] = 0, [49] = 0, [50] = 0, [51] = 0, [52] = 0, [53] = 0, [54] = 0, [55] = 0,
	[56] = 0, [57] = 0, [58] = 0, [59] = 0, [60] = 0, [61] = 0, [62] = 0, [63] = 0,
	[64] = 0, [65] = 0, [66] = 0, [67] = 0, [68] = 0, [69] = 0, [70] = 0, [71] = 0,
	[72] = 0, [73] = 0, [74] = 0, [75] = 0, [76] = 0, [77] = 0, [78] = 0, [79] = 0,
	[80] = 0, [81] = 0, [82] = 0, [83] = 0, [84] = 0, [85] = 0, [86] = 0, [87] = 0,
	[88] = 0, [89] = 0, [90] = 0, [91] = 0, [92] = 0, [93] = 0, [94] = 0, [95] = 0,
	[96] = 0, [97] = 0, [98] = 0, [99] = 0, [100] = 0, [101] = 0, [102] = 0, [103] = 0,
	[104] = 0, [105] = 0, [106] = 0, [107] = 0, [108] = 0, [109] = 0, [110] = 0, [111] = 0,
	[112] = 0, [113] = 0, [114] = 0, [115] = 0, [116] = 0, [117] = 0, [118] = 0, [119] = 0,
	[120] = 0, [121] = 0, [122] = 0, [123] = 0, [124] = 0, [125] = 0, [126] = 0, [127] = 0,
	[128] = 0, [129] = 0, [130] = 0, [131] = 0, [132] = 0, [133] = 0, [134] = 0, [135] = 0,
	[136] = 0, [137] = 0, [138] = 0, [139] = 0, [140] = 0, [141] = 0, [142] = 0, [143] = 0,
	[144] = 0, [145] = 0, [146] = 0, [147] = 0, [148] = 0, [149] = 0, [150] = 0, [151] = 0,
	[152] = 0, [153] = 0, [154] = 0, [155] = 0,
	/* Special pins */
	[156] = 0x0ae000, /* ufs_reset */
	[157] = 0x0a1000, /* sdc1_rclk */
	[158] = 0x0a0000, /* sdc1_clk */
	[159] = 0x0a0000, /* sdc1_cmd */
	[160] = 0x0a0000, /* sdc1_data */
	[161] = 0x0a2000, /* sdc2_clk */
	[162] = 0x0a2000, /* sdc2_cmd */
	[163] = 0x0a2000, /* sdc2_data */
};

static const char *sm6375_get_function_name(struct udevice *dev, unsigned int selector)
{
	return msm_pinctrl_functions[selector].name;
}

static const char *sm6375_get_pin_name(struct udevice *dev, unsigned int selector)
{
	static const char *special_pins_names[] = {
		"ufs_reset", "sdc1_rclk", "sdc1_clk", "sdc1_cmd",
		"sdc1_data", "sdc2_clk", "sdc2_cmd", "sdc2_data",
	};

	if (selector >= 156 && selector <= 163)
		snprintf(pin_name, MAX_PIN_NAME_LEN, "%s", special_pins_names[selector - 156]);
	else
		snprintf(pin_name, MAX_PIN_NAME_LEN, "gpio%u", selector);

	return pin_name;
}

static int sm6375_get_function_mux(__maybe_unused unsigned int pin, unsigned int selector)
{
	return msm_pinctrl_functions[selector].val;
}

static struct msm_pinctrl_data sm6375_data = {
	.pin_data = {
		.pin_offsets = sm6375_pin_offsets,
		.pin_count = ARRAY_SIZE(sm6375_pin_offsets),
		.special_pins_start = 156,
	},
	.functions_count = ARRAY_SIZE(msm_pinctrl_functions),
	.get_function_name = sm6375_get_function_name,
	.get_function_mux = sm6375_get_function_mux,
	.get_pin_name = sm6375_get_pin_name,
};

static const struct udevice_id msm_pinctrl_ids[] = {
	{
		.compatible = "qcom,sm6375-tlmm",
		.data = (ulong)&sm6375_data
	},
	{ /* Sentinel */ }
};

U_BOOT_DRIVER(pinctrl_sm6375) = {
	.name = "pinctrl_sm6375",
	.id = UCLASS_NOP,
	.of_match = msm_pinctrl_ids,
	.ops = &msm_pinctrl_ops,
	.bind = msm_pinctrl_bind,
};
