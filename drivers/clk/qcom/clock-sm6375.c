// SPDX-License-Identifier: BSD-3-Clause

#include <clk-uclass.h>
#include <dm.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/bug.h>
#include <linux/bitops.h>
#include <dt-bindings/clock/qcom,sm6375-gcc.h>

#include "clock-qcom.h"

#define GPLL0_STATUS			0x0
#define GPLL0_ENA_VOTE			0x79000

#define GCC_QUPV3_WRAP0_S4_CMD_RCGR	0x1f608
#define GCC_SDCC1_APPS_CMD_RCGR		0x38028
#define GCC_SDCC2_APPS_CMD_RCGR		0x1e00c

static const struct freq_tbl ftbl_gcc_qupv3_wrap0_s0_clk_src[] = {
	F(7372800, CFG_CLK_SRC_GPLL0_EVEN, 1, 384, 15625),
	F(14745600, CFG_CLK_SRC_GPLL0_EVEN, 1, 768, 15625),
	F(19200000, CFG_CLK_SRC_CXO, 1, 0, 0),
	F(29491200, CFG_CLK_SRC_GPLL0_EVEN, 1, 1536, 15625),
	F(32000000, CFG_CLK_SRC_GPLL0_EVEN, 1, 8, 75),
	F(48000000, CFG_CLK_SRC_GPLL0_EVEN, 1, 4, 25),
	F(64000000, CFG_CLK_SRC_GPLL0_EVEN, 1, 16, 75),
	F(75000000, CFG_CLK_SRC_GPLL0_EVEN, 4, 0, 0),
	F(80000000, CFG_CLK_SRC_GPLL0_EVEN, 1, 4, 15),
	F(96000000, CFG_CLK_SRC_GPLL0_EVEN, 1, 8, 25),
	F(100000000, CFG_CLK_SRC_GPLL0_EVEN, 3, 0, 0),
	F(102400000, CFG_CLK_SRC_GPLL0_EVEN, 1, 128, 375),
	F(112000000, CFG_CLK_SRC_GPLL0_EVEN, 1, 28, 75),
	F(117964800, CFG_CLK_SRC_GPLL0_EVEN, 1, 6144, 15625),
	F(120000000, CFG_CLK_SRC_GPLL0_EVEN, 2.5, 0, 0),
	F(128000000, CFG_CLK_SRC_GPLL0, 1, 16, 75),
	{}
};

static const struct freq_tbl ftbl_gcc_sdcc2_apps_clk_src[] = {
	F(400000, CFG_CLK_SRC_CXO, 12, 1, 4),
	F(19200000, CFG_CLK_SRC_CXO, 1, 0, 0),
	F(25000000, CFG_CLK_SRC_GPLL0_EVEN, 12, 0, 0),
	F(50000000, CFG_CLK_SRC_GPLL0_EVEN, 6, 0, 0),
	F(100000000, CFG_CLK_SRC_GPLL0_EVEN, 3, 0, 0),
	F(202000000, CFG_CLK_SRC_GPLL0, 3, 0, 0),
	{}
};

static const struct pll_vote_clk gpll0_vote_clk = {
	.status = GPLL0_STATUS,
	.status_bit = BIT(31),
	.ena_vote = GPLL0_ENA_VOTE,
	.vote_bit = BIT(0),
};

static const struct gate_clk sm6375_clks[] = {
	GATE_CLK(GCC_AHB2PHY_USB_CLK,			0x1d008, BIT(0)),
	GATE_CLK(GCC_CFG_NOC_USB3_PRIM_AXI_CLK,		0x1a084, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP0_CORE_2X_CLK,		0x1f014, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP0_CORE_CLK,		0x1f00c, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP0_S0_CLK,		0x1f144, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP0_S1_CLK,		0x1f274, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP0_S2_CLK,		0x1f3a4, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP0_S3_CLK,		0x1f4d4, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP0_S4_CLK,		0x1f604, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP0_S5_CLK,		0x1f734, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP1_CORE_2X_CLK,		0x53014, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP1_CORE_CLK,		0x5300c, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP1_S0_CLK,		0x53018, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP1_S1_CLK,		0x53148, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP1_S2_CLK,		0x53278, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP1_S3_CLK,		0x533a8, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP1_S4_CLK,		0x534d8, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP1_S5_CLK,		0x53608, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP_0_M_AHB_CLK,		0x1f004, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP_0_S_AHB_CLK,		0x1f008, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP_1_M_AHB_CLK,		0x53004, BIT(0)),
	GATE_CLK(GCC_QUPV3_WRAP_1_S_AHB_CLK,		0x53008, BIT(0)),
	GATE_CLK(GCC_SDCC1_AHB_CLK,			0x38008, BIT(0)),
	GATE_CLK(GCC_SDCC1_APPS_CLK,			0x38004, BIT(0)),
	GATE_CLK(GCC_SDCC1_ICE_CORE_CLK,		0x3800c, BIT(0)),
	GATE_CLK(GCC_SDCC2_AHB_CLK,			0x1e008, BIT(0)),
	GATE_CLK(GCC_SDCC2_APPS_CLK,			0x1e004, BIT(0)),
	GATE_CLK(GCC_SYS_NOC_CPUSS_AHB_CLK,		0x2b06c, BIT(0)),
	GATE_CLK(GCC_SYS_NOC_UFS_PHY_AXI_CLK,		0x45098, BIT(0)),
	GATE_CLK(GCC_SYS_NOC_USB3_PRIM_AXI_CLK,		0x1a080, BIT(0)),
	GATE_CLK(GCC_UFS_MEM_CLKREF_CLK,		0x8c000, BIT(0)),
	GATE_CLK(GCC_UFS_PHY_AHB_CLK,			0x45014, BIT(0)),
	GATE_CLK(GCC_UFS_PHY_AXI_CLK,			0x45010, BIT(0)),
	GATE_CLK(GCC_UFS_PHY_ICE_CORE_CLK,		0x45044, BIT(0)),
	GATE_CLK(GCC_UFS_PHY_PHY_AUX_CLK,		0x45078, BIT(0)),
	GATE_CLK(GCC_UFS_PHY_RX_SYMBOL_0_CLK,		0x4501c, BIT(0)),
	GATE_CLK(GCC_UFS_PHY_TX_SYMBOL_0_CLK,		0x45018, BIT(0)),
	GATE_CLK(GCC_UFS_PHY_UNIPRO_CORE_CLK,		0x45040, BIT(0)),
	GATE_CLK(GCC_USB30_PRIM_MASTER_CLK,		0x1a010, BIT(0)),
	GATE_CLK(GCC_USB30_PRIM_MOCK_UTMI_CLK,		0x1a018, BIT(0)),
	GATE_CLK(GCC_USB30_PRIM_SLEEP_CLK,		0x1a014, BIT(0)),
	GATE_CLK(GCC_USB3_PRIM_CLKREF_CLK,		0x8c010, BIT(0)),
	GATE_CLK(GCC_USB3_PRIM_PHY_COM_AUX_CLK,		0x1a054, BIT(0)),
	GATE_CLK(GCC_USB3_PRIM_PHY_PIPE_CLK,		0x1a058, BIT(0)),
};

static ulong sm6375_set_rate(struct clk *clk, ulong rate)
{
	struct msm_clk_priv *priv = dev_get_priv(clk->dev);
	const struct freq_tbl *freq;

	if (clk->id < priv->data->num_clks)
		debug("%s: %s, requested rate=%ld\n", __func__,
		      priv->data->clks[clk->id].name, rate);

	switch (clk->id) {
	case GCC_QUPV3_WRAP0_S4_CLK:
		freq = qcom_find_freq(ftbl_gcc_qupv3_wrap0_s0_clk_src, rate);
		clk_rcg_set_rate_mnd(priv->base, GCC_QUPV3_WRAP0_S4_CMD_RCGR,
				     freq->pre_div, freq->m, freq->n, freq->src,
				     16);
		return freq->freq;
	case GCC_SDCC2_APPS_CLK:
		clk_enable_gpll0(priv->base, &gpll0_vote_clk);
		freq = qcom_find_freq(ftbl_gcc_sdcc2_apps_clk_src, rate);
		clk_rcg_set_rate_mnd(priv->base, GCC_SDCC2_APPS_CMD_RCGR,
				     freq->pre_div, freq->m, freq->n, freq->src,
				     8);
		return freq->freq;
	case GCC_SDCC1_APPS_CLK:
		return 384000000;
	default:
		return 0;
	}
}

static int sm6375_enable(struct clk *clk)
{
	struct msm_clk_priv *priv = dev_get_priv(clk->dev);

	if (priv->data->num_clks < clk->id) {
		debug("%s: unknown clk id %lu\n", __func__, clk->id);
		return 0;
	}

	debug("%s: clk %s\n", __func__, sm6375_clks[clk->id].name);

	switch (clk->id) {
	case GCC_USB30_PRIM_MASTER_CLK:
		qcom_gate_clk_en(priv, GCC_USB3_PRIM_PHY_COM_AUX_CLK);
		qcom_gate_clk_en(priv, GCC_USB3_PRIM_CLKREF_CLK);
		break;
	}

	return qcom_gate_clk_en(priv, clk->id);
}

static const struct qcom_reset_map sm6375_gcc_resets[] = {
	[GCC_QUSB2PHY_PRIM_BCR] = { 0x1c000 },
	[GCC_QUSB2PHY_SEC_BCR] = { 0x1c004 },
	[GCC_SDCC1_BCR] = { 0x38000 },
	[GCC_SDCC2_BCR] = { 0x1e000 },
	[GCC_UFS_PHY_BCR] = { 0x45000 },
	[GCC_USB30_PRIM_BCR] = { 0x1a000 },
	[GCC_USB_PHY_CFG_AHB2PHY_BCR] = { 0x1d000 },
	[GCC_USB3_DP_PHY_PRIM_BCR] = { 0x1b020 },
	[GCC_USB3_PHY_PRIM_SP0_BCR] = { 0x1b000 },
	[GCC_VCODEC0_BCR] = { 0x58094 },
	[GCC_VENUS_BCR] = { 0x58078 },
	[GCC_VIDEO_INTERFACE_BCR] = { 0x6e000 },
	[GCC_QUPV3_WRAPPER_0_BCR] = { 0x1f000 },
	[GCC_QUPV3_WRAPPER_1_BCR] = { 0x53000 },
	[GCC_PDM_BCR] = { 0x20000 },
	[GCC_GPU_BCR] = { 0x36000 },
	[GCC_MMSS_BCR] = { 0x17000 },
	[GCC_CAMSS_TFE_BCR] = { 0x52000 },
	[GCC_CAMSS_OPE_BCR] = { 0x55000 },
	[GCC_CAMSS_TOP_BCR] = { 0x58000 },
};

static const struct qcom_power_map sm6375_gdscs[] = {
	[USB30_PRIM_GDSC] = { 0x1a004 },
	[UFS_PHY_GDSC] = { 0x45004 },
};

static struct msm_clk_data sm6375_gcc_data = {
	.resets = sm6375_gcc_resets,
	.num_resets = ARRAY_SIZE(sm6375_gcc_resets),
	.clks = sm6375_clks,
	.num_clks = ARRAY_SIZE(sm6375_clks),
	.power_domains = sm6375_gdscs,
	.num_power_domains = ARRAY_SIZE(sm6375_gdscs),

	.enable = sm6375_enable,
	.set_rate = sm6375_set_rate,
};

static const struct udevice_id gcc_sm6375_of_match[] = {
	{
		.compatible = "qcom,gcc-sm6375",
		.data = (ulong)&sm6375_gcc_data,
	},
	{}
};

U_BOOT_DRIVER(gcc_sm6375) = {
	.name = "gcc_sm6375",
	.id = UCLASS_NOP,
	.of_match = gcc_sm6375_of_match,
	.bind = qcom_cc_bind,
	.flags = DM_FLAG_PRE_RELOC,
};
