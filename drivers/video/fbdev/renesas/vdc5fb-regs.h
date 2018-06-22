/*
 * Copyright (C) 2013-2014 Renesas Solutions Corp.
 *
 * Based on drivers/video/ren_vdc4.c
 * Copyright (c) 2012 Renesas Electronics Europe Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef _VDC5FB_REGS_H_
#define _VDC5FB_REGS_H_

/* REGISTER INDEX */
enum {
	/* INPUT CONTROLLER */
	INP_UPDATE,
	INP_SEL_CNT,
	INP_EXT_SYNC_CNT,
	INP_VSYNC_PH_ADJ,
	INP_DLY_ADJ,

	/* IMAGE QUALITY ADJUSTMENT BLOCK */
	IMGCNT_UPDATE,
	IMGCNT_NR_CNT0,
	IMGCNT_NR_CNT1,
	IMGCNT_MTX_MODE,
	IMGCNT_MTX_YG_ADJ0,
	IMGCNT_MTX_YG_ADJ1,
	IMGCNT_MTX_CBB_ADJ0,
	IMGCNT_MTX_CBB_ADJ1,
	IMGCNT_MTX_CRR_ADJ0,
	IMGCNT_MTX_CRR_ADJ1,
	IMGCNT_DRC_REG,

	/* SCALER 0 */
	SC0_SCL0_UPDATE,
	SC0_SCL0_FRC1,
	SC0_SCL0_FRC2,
	SC0_SCL0_FRC3,
	SC0_SCL0_FRC4,
	SC0_SCL0_FRC5,
	SC0_SCL0_FRC6,
	SC0_SCL0_FRC7,
	SC0_SCL0_FRC9,
	SC0_SCL0_MON0,
	SC0_SCL0_INT,
	SC0_SCL0_DS1,
	SC0_SCL0_DS2,
	SC0_SCL0_DS3,
	SC0_SCL0_DS4,
	SC0_SCL0_DS5,
	SC0_SCL0_DS6,
	SC0_SCL0_DS7,
	SC0_SCL0_US1,
	SC0_SCL0_US2,
	SC0_SCL0_US3,
	SC0_SCL0_US4,
	SC0_SCL0_US5,
	SC0_SCL0_US6,
	SC0_SCL0_US7,
	SC0_SCL0_US8,
	SC0_SCL0_OVR1,
	SC0_SCL1_UPDATE,
	SC0_SCL1_WR1,
	SC0_SCL1_WR2,
	SC0_SCL1_WR3,
	SC0_SCL1_WR4,
	SC0_SCL1_WR5,
	SC0_SCL1_WR6,
	SC0_SCL1_WR7,
	SC0_SCL1_WR8,
	SC0_SCL1_WR9,
	SC0_SCL1_WR10,
	SC0_SCL1_WR11,
	SC0_SCL1_MON1,
	SC0_SCL1_PBUF0,
	SC0_SCL1_PBUF1,
	SC0_SCL1_PBUF2,
	SC0_SCL1_PBUF3,
	SC0_SCL1_PBUF_FLD,
	SC0_SCL1_PBUF_CNT,

	/* GRAPHICS 0 */
	GR0_UPDATE,
	GR0_FLM_RD,
	GR0_FLM1,
	GR0_FLM2,
	GR0_FLM3,
	GR0_FLM4,
	GR0_FLM5,
	GR0_FLM6,
	GR0_AB1,
	GR0_AB2,
	GR0_AB3,
	GR0_AB7,
	GR0_AB8,
	GR0_AB9,
	GR0_AB10,
	GR0_AB11,
	GR0_BASE,
	GR0_CLUT,
	GR0_MON,

	/* SCALER 1 */
	SC1_SCL0_UPDATE,
	SC1_SCL0_FRC1,
	SC1_SCL0_FRC2,
	SC1_SCL0_FRC3,
	SC1_SCL0_FRC4,
	SC1_SCL0_FRC5,
	SC1_SCL0_FRC6,
	SC1_SCL0_FRC7,
	SC1_SCL0_FRC9,
	SC1_SCL0_MON0,
	SC1_SCL0_INT,
	SC1_SCL0_DS1,
	SC1_SCL0_DS2,
	SC1_SCL0_DS3,
	SC1_SCL0_DS4,
	SC1_SCL0_DS5,
	SC1_SCL0_DS6,
	SC1_SCL0_DS7,
	SC1_SCL0_US1,
	SC1_SCL0_US2,
	SC1_SCL0_US3,
	SC1_SCL0_US4,
	SC1_SCL0_US5,
	SC1_SCL0_US6,
	SC1_SCL0_US7,
	SC1_SCL0_US8,
	SC1_SCL0_OVR1,
	SC1_SCL1_UPDATE,
	SC1_SCL1_WR1,
	SC1_SCL1_WR2,
	SC1_SCL1_WR3,
	SC1_SCL1_WR4,
	SC1_SCL1_WR5,
	SC1_SCL1_WR6,
	SC1_SCL1_WR7,
	SC1_SCL1_WR8,
	SC1_SCL1_WR9,
	SC1_SCL1_WR10,
	SC1_SCL1_WR11,
	SC1_SCL1_MON1,
	SC1_SCL1_PBUF0,
	SC1_SCL1_PBUF1,
	SC1_SCL1_PBUF2,
	SC1_SCL1_PBUF3,
	SC1_SCL1_PBUF_FLD,
	SC1_SCL1_PBUF_CNT,

	/* GRAPHICS 1 */
	GR1_UPDATE,
	GR1_FLM_RD,
	GR1_FLM1,
	GR1_FLM2,
	GR1_FLM3,
	GR1_FLM4,
	GR1_FLM5,
	GR1_FLM6,
	GR1_AB1,
	GR1_AB2,
	GR1_AB3,
	GR1_AB4,
	GR1_AB5,
	GR1_AB6,
	GR1_AB7,
	GR1_AB8,
	GR1_AB9,
	GR1_AB10,
	GR1_AB11,
	GR1_BASE,
	GR1_CLUT,
	GR1_MON,

	/* IMAGE QUALITY IMPROVER 0 */
	ADJ0_UPDATE,
	ADJ0_BKSTR_SET,
	ADJ0_ENH_TIM1,
	ADJ0_ENH_TIM2,
	ADJ0_ENH_TIM3,
	ADJ0_ENH_SHP1,
	ADJ0_ENH_SHP2,
	ADJ0_ENH_SHP3,
	ADJ0_ENH_SHP4,
	ADJ0_ENH_SHP5,
	ADJ0_ENH_SHP6,
	ADJ0_ENH_LTI1,
	ADJ0_ENH_LTI2,
	ADJ0_MTX_MODE,
	ADJ0_MTX_YG_ADJ0,
	ADJ0_MTX_YG_ADJ1,
	ADJ0_MTX_CBB_ADJ0,
	ADJ0_MTX_CBB_ADJ1,
	ADJ0_MTX_CRR_ADJ0,
	ADJ0_MTX_CRR_ADJ1,

	/* IMAGE QUALITY IMPROVER 1 */
	ADJ1_UPDATE,
	ADJ1_BKSTR_SET,
	ADJ1_ENH_TIM1,
	ADJ1_ENH_TIM2,
	ADJ1_ENH_TIM3,
	ADJ1_ENH_SHP1,
	ADJ1_ENH_SHP2,
	ADJ1_ENH_SHP3,
	ADJ1_ENH_SHP4,
	ADJ1_ENH_SHP5,
	ADJ1_ENH_SHP6,
	ADJ1_ENH_LTI1,
	ADJ1_ENH_LTI2,
	ADJ1_MTX_MODE,
	ADJ1_MTX_YG_ADJ0,
	ADJ1_MTX_YG_ADJ1,
	ADJ1_MTX_CBB_ADJ0,
	ADJ1_MTX_CBB_ADJ1,
	ADJ1_MTX_CRR_ADJ0,
	ADJ1_MTX_CRR_ADJ1,

	/* GRAPHICS 2 */
	GR2_UPDATE,
	GR2_FLM_RD,
	GR2_FLM1,
	GR2_FLM2,
	GR2_FLM3,
	GR2_FLM4,
	GR2_FLM5,
	GR2_FLM6,
	GR2_AB1,
	GR2_AB2,
	GR2_AB3,
	GR2_AB4,
	GR2_AB5,
	GR2_AB6,
	GR2_AB7,
	GR2_AB8,
	GR2_AB9,
	GR2_AB10,
	GR2_AB11,
	GR2_BASE,
	GR2_CLUT,
	GR2_MON,

	/* GRAPHICS 3 */
	GR3_UPDATE,
	GR3_FLM_RD,
	GR3_FLM1,
	GR3_FLM2,
	GR3_FLM3,
	GR3_FLM4,
	GR3_FLM5,
	GR3_FLM6,
	GR3_AB1,
	GR3_AB2,
	GR3_AB3,
	GR3_AB4,
	GR3_AB5,
	GR3_AB6,
	GR3_AB7,
	GR3_AB8,
	GR3_AB9,
	GR3_AB10,
	GR3_AB11,
	GR3_BASE,
	GR3_CLUT_INT,
	GR3_MON,

	/* VIN SYNTHESIZER */
	GR_VIN_UPDATE,
	GR_VIN_AB1,
	GR_VIN_AB2,
	GR_VIN_AB3,
	GR_VIN_AB4,
	GR_VIN_AB5,
	GR_VIN_AB6,
	GR_VIN_AB7,
	GR_VIN_BASE,
	GR_VIN_MON,

	/* OUTPUT IMAGE GENERATOR */
	OIR_SCL0_UPDATE,
	OIR_SCL0_FRC1,
	OIR_SCL0_FRC2,
	OIR_SCL0_FRC3,
	OIR_SCL0_FRC4,
	OIR_SCL0_FRC5,
	OIR_SCL0_FRC6,
	OIR_SCL0_FRC7,
	OIR_SCL0_DS1,
	OIR_SCL0_DS2,
	OIR_SCL0_DS3,
	OIR_SCL0_DS7,
	OIR_SCL0_US1,
	OIR_SCL0_US2,
	OIR_SCL0_US3,
	OIR_SCL0_US8,
	OIR_SCL0_OVR1,
	OIR_SCL1_UPDATE,
	OIR_SCL1_WR1,
	OIR_SCL1_WR2,
	OIR_SCL1_WR3,
	OIR_SCL1_WR4,
	OIR_SCL1_WR5,
	OIR_SCL1_WR6,
	OIR_SCL1_WR7,

	/* GRAPHICS OIR */
	GR_OIR_UPDATE,
	GR_OIR_FLM_RD,
	GR_OIR_FLM1,
	GR_OIR_FLM2,
	GR_OIR_FLM3,
	GR_OIR_FLM4,
	GR_OIR_FLM5,
	GR_OIR_FLM6,
	GR_OIR_AB1,
	GR_OIR_AB2,
	GR_OIR_AB3,
	GR_OIR_AB7,
	GR_OIR_AB8,
	GR_OIR_AB9,
	GR_OIR_AB10,
	GR_OIR_AB11,
	GR_OIR_BASE,
	GR_OIR_CLUT,
	GR_OIR_MON,

	/* GAMMA CORRECTION BLOCK */
	GAM_G_UPDATE,
	GAM_SW,
	GAM_G_LUT1,
	GAM_G_LUT2,
	GAM_G_LUT3,
	GAM_G_LUT4,
	GAM_G_LUT5,
	GAM_G_LUT6,
	GAM_G_LUT7,
	GAM_G_LUT8,
	GAM_G_LUT9,
	GAM_G_LUT10,
	GAM_G_LUT11,
	GAM_G_LUT12,
	GAM_G_LUT13,
	GAM_G_LUT14,
	GAM_G_LUT15,
	GAM_G_LUT16,
	GAM_G_AREA1,
	GAM_G_AREA2,
	GAM_G_AREA3,
	GAM_G_AREA4,
	GAM_G_AREA5,
	GAM_G_AREA6,
	GAM_G_AREA7,
	GAM_G_AREA8,
	GAM_B_UPDATE,
	GAM_B_LUT1,
	GAM_B_LUT2,
	GAM_B_LUT3,
	GAM_B_LUT4,
	GAM_B_LUT5,
	GAM_B_LUT6,
	GAM_B_LUT7,
	GAM_B_LUT8,
	GAM_B_LUT9,
	GAM_B_LUT10,
	GAM_B_LUT11,
	GAM_B_LUT12,
	GAM_B_LUT13,
	GAM_B_LUT14,
	GAM_B_LUT15,
	GAM_B_LUT16,
	GAM_B_AREA1,
	GAM_B_AREA2,
	GAM_B_AREA3,
	GAM_B_AREA4,
	GAM_B_AREA5,
	GAM_B_AREA6,
	GAM_B_AREA7,
	GAM_B_AREA8,
	GAM_R_UPDATE,
	GAM_R_LUT1,
	GAM_R_LUT2,
	GAM_R_LUT3,
	GAM_R_LUT4,
	GAM_R_LUT5,
	GAM_R_LUT6,
	GAM_R_LUT7,
	GAM_R_LUT8,
	GAM_R_LUT9,
	GAM_R_LUT10,
	GAM_R_LUT11,
	GAM_R_LUT12,
	GAM_R_LUT13,
	GAM_R_LUT14,
	GAM_R_LUT15,
	GAM_R_LUT16,
	GAM_R_AREA1,
	GAM_R_AREA2,
	GAM_R_AREA3,
	GAM_R_AREA4,
	GAM_R_AREA5,
	GAM_R_AREA6,
	GAM_R_AREA7,
	GAM_R_AREA8,

	/* TCON BLOCK */
	TCON_UPDATE,
	TCON_TIM,
	TCON_TIM_STVA1,
	TCON_TIM_STVA2,
	TCON_TIM_STVB1,
	TCON_TIM_STVB2,
	TCON_TIM_STH1,
	TCON_TIM_STH2,
	TCON_TIM_STB1,
	TCON_TIM_STB2,
	TCON_TIM_CPV1,
	TCON_TIM_CPV2,
	TCON_TIM_POLA1,
	TCON_TIM_POLA2,
	TCON_TIM_POLB1,
	TCON_TIM_POLB2,
	TCON_TIM_DE,

	/* OUTPUT CONTROLLER */
	OUT_UPDATE,
	OUT_SET,
	OUT_BRIGHT1,
	OUT_BRIGHT2,
	OUT_CONTRAST,
	OUT_PDTHA,
	OUT_CLK_PHASE,

	/* LVDS */
	LVDS_UPDATE,
	LVDSFCL,
	LCLKSELR,
	LPLLSETR,
	LPLLMONR,
	LPHYACC,

	/* SYSTEM CONTROLLER */
	SYSCNT_INT1,
	SYSCNT_INT2,
	SYSCNT_INT3,
	SYSCNT_INT4,
	SYSCNT_INT5,
	SYSCNT_INT6,
	SYSCNT_PANEL_CLK,
	SYSCNT_CLUT,

	VDC5FB_MAX_REGS,
};

/* IRQ INDEX */
enum {
	S0_VI_VSYNC = 0,
	S0_LO_VSYNC,
	S0_VSYNCERR,
	GR3_VLINE,
	S0_VFIELD,
	IV1_VBUFERR,
	IV3_VBUFERR,
	IV5_VBUFERR,
	IV6_VBUFERR,
	S0_WLINE,
	S1_VI_VSYNC,
	S1_LO_VSYNC,
	S1_VSYNCERR,
	S1_VFIELD,
	IV2_VBUFERR,
	IV4_VBUFERR,
	S1_WLINE,
	OIR_VI_VSYNC,
	OIR_LO_VSYNC,
	OIR_VLINE,
	OIR_VFIELD,
	IV7_VBUFERR,
	IV8_VBUFERR,
	VDC5FB_MAX_IRQS,
};

static unsigned long vdc5fb_offsets[VDC5FB_MAX_REGS] = {
	/* INPUT CONTROLLER */
	[INP_UPDATE]		= 0x0000,
	[INP_SEL_CNT]		= 0x0004,
	[INP_EXT_SYNC_CNT]	= 0x0008,
	[INP_VSYNC_PH_ADJ]	= 0x000C,
	[INP_DLY_ADJ]		= 0x0010,

	/* IMAGE QUALITY ADJUSTMENT BLOCK */
	[IMGCNT_UPDATE]		= 0x0080,
	[IMGCNT_NR_CNT0]	= 0x0084,
	[IMGCNT_NR_CNT1]	= 0x0088,
	[IMGCNT_MTX_MODE]	= 0x00A0,
	[IMGCNT_MTX_YG_ADJ0]	= 0x00A4,
	[IMGCNT_MTX_YG_ADJ1]	= 0x00A8,
	[IMGCNT_MTX_CBB_ADJ0]	= 0x00AC,
	[IMGCNT_MTX_CBB_ADJ1]	= 0x00B0,
	[IMGCNT_MTX_CRR_ADJ0]	= 0x00B4,
	[IMGCNT_MTX_CRR_ADJ1]	= 0x00B8,
	[IMGCNT_DRC_REG]	= 0x00C0,

	/* SCALER 0 */
	[SC0_SCL0_UPDATE]	= 0x0100,
	[SC0_SCL0_FRC1]		= 0x0104,
	[SC0_SCL0_FRC2]		= 0x0108,
	[SC0_SCL0_FRC3]		= 0x010C,
	[SC0_SCL0_FRC4]		= 0x0110,
	[SC0_SCL0_FRC5]		= 0x0114,
	[SC0_SCL0_FRC6]		= 0x0118,
	[SC0_SCL0_FRC7]		= 0x011C,
	[SC0_SCL0_FRC9]		= 0x0124,
	[SC0_SCL0_MON0]		= 0x0128,	/* 16bit */
	[SC0_SCL0_INT]		= 0x012A,	/* 16bit */
	[SC0_SCL0_DS1]		= 0x012C,
	[SC0_SCL0_DS2]		= 0x0130,
	[SC0_SCL0_DS3]		= 0x0134,
	[SC0_SCL0_DS4]		= 0x0138,
	[SC0_SCL0_DS5]		= 0x013C,
	[SC0_SCL0_DS6]		= 0x0140,
	[SC0_SCL0_DS7]		= 0x0144,
	[SC0_SCL0_US1]		= 0x0148,
	[SC0_SCL0_US2]		= 0x014C,
	[SC0_SCL0_US3]		= 0x0150,
	[SC0_SCL0_US4]		= 0x0154,
	[SC0_SCL0_US5]		= 0x0158,
	[SC0_SCL0_US6]		= 0x015C,
	[SC0_SCL0_US7]		= 0x0160,
	[SC0_SCL0_US8]		= 0x0164,
	[SC0_SCL0_OVR1]		= 0x016C,
	[SC0_SCL1_UPDATE]	= 0x0180,
	[SC0_SCL1_WR1]		= 0x0188,
	[SC0_SCL1_WR2]		= 0x018C,
	[SC0_SCL1_WR3]		= 0x0190,
	[SC0_SCL1_WR4]		= 0x0194,
	[SC0_SCL1_WR5]		= 0x019C,
	[SC0_SCL1_WR6]		= 0x01A0,
	[SC0_SCL1_WR7]		= 0x01A4,
	[SC0_SCL1_WR8]		= 0x01A8,
	[SC0_SCL1_WR9]		= 0x01AC,
	[SC0_SCL1_WR10]		= 0x01B0,
	[SC0_SCL1_WR11]		= 0x01B4,
	[SC0_SCL1_MON1]		= 0x01B8,
	[SC0_SCL1_PBUF0]	= 0x01BC,
	[SC0_SCL1_PBUF1]	= 0x01C0,
	[SC0_SCL1_PBUF2]	= 0x01C4,
	[SC0_SCL1_PBUF3]	= 0x01C8,
	[SC0_SCL1_PBUF_FLD]	= 0x01CC,
	[SC0_SCL1_PBUF_CNT]	= 0x01D0,

	/* GRAPHICS 0 */
	[GR0_UPDATE]		= 0x0200,
	[GR0_FLM_RD]		= 0x0204,
	[GR0_FLM1]		= 0x0208,
	[GR0_FLM2]		= 0x020C,
	[GR0_FLM3]		= 0x0210,
	[GR0_FLM4]		= 0x0214,
	[GR0_FLM5]		= 0x0218,
	[GR0_FLM6]		= 0x021C,
	[GR0_AB1]		= 0x0220,
	[GR0_AB2]		= 0x0224,
	[GR0_AB3]		= 0x0228,
	[GR0_AB7]		= 0x0238,
	[GR0_AB8]		= 0x023C,
	[GR0_AB9]		= 0x0240,
	[GR0_AB10]		= 0x0244,
	[GR0_AB11]		= 0x0248,
	[GR0_BASE]		= 0x024C,
	[GR0_CLUT]		= 0x0250,
	[GR0_MON]		= 0x0254,

	/* SCALER 1 */
	[SC1_SCL0_UPDATE]	= 0x0800,
	[SC1_SCL0_FRC1]		= 0x0804,
	[SC1_SCL0_FRC2]		= 0x0808,
	[SC1_SCL0_FRC3]		= 0x080C,
	[SC1_SCL0_FRC4]		= 0x0810,
	[SC1_SCL0_FRC5]		= 0x0814,
	[SC1_SCL0_FRC6]		= 0x0818,
	[SC1_SCL0_FRC7]		= 0x081C,
	[SC1_SCL0_FRC9]		= 0x0820,
	[SC1_SCL0_MON0]		= 0x0824,
	[SC1_SCL0_INT]		= 0x0828,
	[SC1_SCL0_DS1]		= 0x082C,
	[SC1_SCL0_DS2]		= 0x0830,
	[SC1_SCL0_DS3]		= 0x0834,
	[SC1_SCL0_DS4]		= 0x0838,
	[SC1_SCL0_DS5]		= 0x083C,
	[SC1_SCL0_DS6]		= 0x0840,
	[SC1_SCL0_DS7]		= 0x0844,
	[SC1_SCL0_US1]		= 0x0848,
	[SC1_SCL0_US2]		= 0x084C,
	[SC1_SCL0_US3]		= 0x0850,
	[SC1_SCL0_US4]		= 0x0854,
	[SC1_SCL0_US5]		= 0x0858,
	[SC1_SCL0_US6]		= 0x085C,
	[SC1_SCL0_US7]		= 0x0860,
	[SC1_SCL0_US8]		= 0x0864,
	[SC1_SCL0_OVR1]		= 0x086C,
	[SC1_SCL1_UPDATE]	= 0x0880,
	[SC1_SCL1_WR1]		= 0x0888,
	[SC1_SCL1_WR2]		= 0x088C,
	[SC1_SCL1_WR3]		= 0x0890,
	[SC1_SCL1_WR4]		= 0x0894,
	[SC1_SCL1_WR5]		= 0x089C,
	[SC1_SCL1_WR6]		= 0x08A0,
	[SC1_SCL1_WR7]		= 0x08A4,
	[SC1_SCL1_WR8]		= 0x08A8,
	[SC1_SCL1_WR9]		= 0x08AC,
	[SC1_SCL1_WR10]		= 0x08B0,
	[SC1_SCL1_WR11]		= 0x08B4,
	[SC1_SCL1_MON1]		= 0x08B8,
	[SC1_SCL1_PBUF0]	= 0x08BC,
	[SC1_SCL1_PBUF1]	= 0x08C0,
	[SC1_SCL1_PBUF2]	= 0x08C4,
	[SC1_SCL1_PBUF3]	= 0x08C9,
	[SC1_SCL1_PBUF_FLD]	= 0x08CC,
	[SC1_SCL1_PBUF_CNT]	= 0x08D0,

	/* GRAPHICS 1 */
	[GR1_UPDATE]		= 0x0900,
	[GR1_FLM_RD]		= 0x0904,
	[GR1_FLM1]		= 0x0908,
	[GR1_FLM2]		= 0x090C,
	[GR1_FLM3]		= 0x0910,
	[GR1_FLM4]		= 0x0914,
	[GR1_FLM5]		= 0x0918,
	[GR1_FLM6]		= 0x091C,
	[GR1_AB1]		= 0x0920,
	[GR1_AB2]		= 0x0924,
	[GR1_AB3]		= 0x0928,
	[GR1_AB4]		= 0x092C,
	[GR1_AB5]		= 0x0930,
	[GR1_AB6]		= 0x0934,
	[GR1_AB7]		= 0x0938,
	[GR1_AB8]		= 0x093C,
	[GR1_AB9]		= 0x0940,
	[GR1_AB10]		= 0x0944,
	[GR1_AB11]		= 0x0948,
	[GR1_BASE]		= 0x094C,
	[GR1_CLUT]		= 0x0950,
	[GR1_MON]		= 0x0954,

	/* IMAGE QUALITY IMPROVER 0 */
	[ADJ0_UPDATE]		= 0x0280,
	[ADJ0_BKSTR_SET]	= 0x0284,
	[ADJ0_ENH_TIM1]		= 0x0288,
	[ADJ0_ENH_TIM2]		= 0x028C,
	[ADJ0_ENH_TIM3]		= 0x0290,
	[ADJ0_ENH_SHP1]		= 0x0294,
	[ADJ0_ENH_SHP2]		= 0x0298,
	[ADJ0_ENH_SHP3]		= 0x029C,
	[ADJ0_ENH_SHP4]		= 0x02A0,
	[ADJ0_ENH_SHP5]		= 0x02A4,
	[ADJ0_ENH_SHP6]		= 0x02A8,
	[ADJ0_ENH_LTI1]		= 0x02AC,
	[ADJ0_ENH_LTI2]		= 0x02B0,
	[ADJ0_MTX_MODE]		= 0x02B4,
	[ADJ0_MTX_YG_ADJ0]	= 0x02B8,
	[ADJ0_MTX_YG_ADJ1]	= 0x02BC,
	[ADJ0_MTX_CBB_ADJ0]	= 0x02C0,
	[ADJ0_MTX_CBB_ADJ1]	= 0x02C4,
	[ADJ0_MTX_CRR_ADJ0]	= 0x02C8,
	[ADJ0_MTX_CRR_ADJ1]	= 0x02CC,

	/* IMAGE QUALITY IMPROVER 1 */
	[ADJ1_UPDATE]		= 0x0980,
	[ADJ1_BKSTR_SET]	= 0x0984,
	[ADJ1_ENH_TIM1]		= 0x0988,
	[ADJ1_ENH_TIM2]		= 0x098C,
	[ADJ1_ENH_TIM3]		= 0x0990,
	[ADJ1_ENH_SHP1]		= 0x0994,
	[ADJ1_ENH_SHP2]		= 0x0998,
	[ADJ1_ENH_SHP3]		= 0x099C,
	[ADJ1_ENH_SHP4]		= 0x09A0,
	[ADJ1_ENH_SHP5]		= 0x09A4,
	[ADJ1_ENH_SHP6]		= 0x09A8,
	[ADJ1_ENH_LTI1]		= 0x09A8,
	[ADJ1_ENH_LTI2]		= 0x09B0,
	[ADJ1_MTX_MODE]		= 0x09B4,
	[ADJ1_MTX_YG_ADJ0]	= 0x09B8,
	[ADJ1_MTX_YG_ADJ1]	= 0x09BC,
	[ADJ1_MTX_CBB_ADJ0]	= 0x09C0,
	[ADJ1_MTX_CBB_ADJ1]	= 0x09C4,
	[ADJ1_MTX_CRR_ADJ0]	= 0x09C8,
	[ADJ1_MTX_CRR_ADJ1]	= 0x09CC,

	/* GRAPHICS 2 */
	[GR2_UPDATE]		= 0x0300,
	[GR2_FLM_RD]		= 0x0304,
	[GR2_FLM1]		= 0x0308,
	[GR2_FLM2]		= 0x030C,
	[GR2_FLM3]		= 0x0310,
	[GR2_FLM4]		= 0x0314,
	[GR2_FLM5]		= 0x0318,
	[GR2_FLM6]		= 0x031C,
	[GR2_AB1]		= 0x0320,
	[GR2_AB2]		= 0x0324,
	[GR2_AB3]		= 0x0328,
	[GR2_AB4]		= 0x032C,
	[GR2_AB5]		= 0x0330,
	[GR2_AB6]		= 0x0334,
	[GR2_AB7]		= 0x0338,
	[GR2_AB8]		= 0x033C,
	[GR2_AB9]		= 0x0340,
	[GR2_AB10]		= 0x0344,
	[GR2_AB11]		= 0x0348,
	[GR2_BASE]		= 0x034C,
	[GR2_CLUT]		= 0x0350,
	[GR2_MON]		= 0x0354,

	/* GRAPHICS 3 */
	[GR3_UPDATE]		= 0x0380,
	[GR3_FLM_RD]		= 0x0384,
	[GR3_FLM1]		= 0x0388,
	[GR3_FLM2]		= 0x038C,
	[GR3_FLM3]		= 0x0390,
	[GR3_FLM4]		= 0x0394,
	[GR3_FLM5]		= 0x0398,
	[GR3_FLM6]		= 0x039C,
	[GR3_AB1]		= 0x03A0,
	[GR3_AB2]		= 0x03A4,
	[GR3_AB3]		= 0x03A8,
	[GR3_AB4]		= 0x03AC,
	[GR3_AB5]		= 0x03B0,
	[GR3_AB6]		= 0x03B4,
	[GR3_AB7]		= 0x03B8,
	[GR3_AB8]		= 0x03BC,
	[GR3_AB9]		= 0x03C0,
	[GR3_AB10]		= 0x03C4,
	[GR3_AB11]		= 0x03C8,
	[GR3_BASE]		= 0x03CC,
	[GR3_CLUT_INT]		= 0x03D0,
	[GR3_MON]		= 0x03D4,

	/* VIN SYNTHESIZER */
	[GR_VIN_UPDATE]		= 0x0A00,
	[GR_VIN_AB1]		= 0x0A20,
	[GR_VIN_AB2]		= 0x0A24,
	[GR_VIN_AB3]		= 0x0A28,
	[GR_VIN_AB4]		= 0x0A2C,
	[GR_VIN_AB5]		= 0x0A30,
	[GR_VIN_AB6]		= 0x0A34,
	[GR_VIN_AB7]		= 0x0A38,
	[GR_VIN_BASE]		= 0x0A4C,
	[GR_VIN_MON]		= 0x0A54,

	/* OUTPUT IMAGE GENERATOR */
	[OIR_SCL0_UPDATE]	= 0x0A80,
	[OIR_SCL0_FRC1]		= 0x0A84,
	[OIR_SCL0_FRC2]		= 0x0A88,
	[OIR_SCL0_FRC3]		= 0x0A8C,
	[OIR_SCL0_FRC4]		= 0x0A90,
	[OIR_SCL0_FRC5]		= 0x0A94,
	[OIR_SCL0_FRC6]		= 0x0A98,
	[OIR_SCL0_FRC7]		= 0x0A9C,
	[OIR_SCL0_DS1]		= 0x0AAC,
	[OIR_SCL0_DS2]		= 0x0AB0,
	[OIR_SCL0_DS3]		= 0x0AB4,
	[OIR_SCL0_DS7]		= 0x0AC4,
	[OIR_SCL0_US1]		= 0x0AC8,
	[OIR_SCL0_US2]		= 0x0ACC,
	[OIR_SCL0_US3]		= 0x0AD0,
	[OIR_SCL0_US8]		= 0x0AE4,
	[OIR_SCL0_OVR1]		= 0x0AEC,
	[OIR_SCL1_UPDATE]	= 0x0B00,
	[OIR_SCL1_WR1]		= 0x0B08,
	[OIR_SCL1_WR2]		= 0x0B0C,
	[OIR_SCL1_WR3]		= 0x0B10,
	[OIR_SCL1_WR4]		= 0x0B14,
	[OIR_SCL1_WR5]		= 0x0B1C,
	[OIR_SCL1_WR6]		= 0x0B20,
	[OIR_SCL1_WR7]		= 0x0B24,

	/* GRAPHICS OIR */
	[GR_OIR_UPDATE]		= 0x0B80,
	[GR_OIR_FLM_RD]		= 0x0B84,
	[GR_OIR_FLM1]		= 0x0B88,
	[GR_OIR_FLM2]		= 0x0B8C,
	[GR_OIR_FLM3]		= 0x0B90,
	[GR_OIR_FLM4]		= 0x0B94,
	[GR_OIR_FLM5]		= 0x0B98,
	[GR_OIR_FLM6]		= 0x0B9C,
	[GR_OIR_AB1]		= 0x0BA0,
	[GR_OIR_AB2]		= 0x0BA4,
	[GR_OIR_AB3]		= 0x0BA8,
	[GR_OIR_AB7]		= 0x0BB8,
	[GR_OIR_AB8]		= 0x0BBC,
	[GR_OIR_AB9]		= 0x0BC0,
	[GR_OIR_AB10]		= 0x0BC4,
	[GR_OIR_AB11]		= 0x0BC8,
	[GR_OIR_BASE]		= 0x0BCC,
	[GR_OIR_CLUT]		= 0x0BD0,
	[GR_OIR_MON]		= 0x0BD4,

	/* GAMMA CORRECTION BLOCK */
	[GAM_G_UPDATE]		= 0x0400,
	[GAM_SW]		= 0x0404,
	[GAM_G_LUT1]		= 0x0408,
	[GAM_G_LUT2]		= 0x040C,
	[GAM_G_LUT3]		= 0x0410,
	[GAM_G_LUT4]		= 0x0414,
	[GAM_G_LUT5]		= 0x0418,
	[GAM_G_LUT6]		= 0x041C,
	[GAM_G_LUT7]		= 0x0420,
	[GAM_G_LUT8]		= 0x0424,
	[GAM_G_LUT9]		= 0x0428,
	[GAM_G_LUT10]		= 0x042C,
	[GAM_G_LUT11]		= 0x0430,
	[GAM_G_LUT12]		= 0x0434,
	[GAM_G_LUT13]		= 0x0438,
	[GAM_G_LUT14]		= 0x043C,
	[GAM_G_LUT15]		= 0x0440,
	[GAM_G_LUT16]		= 0x0444,
	[GAM_G_AREA1]		= 0x0448,
	[GAM_G_AREA2]		= 0x044C,
	[GAM_G_AREA3]		= 0x0450,
	[GAM_G_AREA4]		= 0x0454,
	[GAM_G_AREA5]		= 0x0458,
	[GAM_G_AREA6]		= 0x045C,
	[GAM_G_AREA7]		= 0x0460,
	[GAM_G_AREA8]		= 0x0464,
	[GAM_B_UPDATE]		= 0x0480,
	[GAM_B_LUT1]		= 0x0488,
	[GAM_B_LUT2]		= 0x048C,
	[GAM_B_LUT3]		= 0x0490,
	[GAM_B_LUT4]		= 0x0494,
	[GAM_B_LUT5]		= 0x0498,
	[GAM_B_LUT6]		= 0x049C,
	[GAM_B_LUT7]		= 0x04A0,
	[GAM_B_LUT8]		= 0x04A4,
	[GAM_B_LUT9]		= 0x04A8,
	[GAM_B_LUT10]		= 0x04AC,
	[GAM_B_LUT11]		= 0x04B0,
	[GAM_B_LUT12]		= 0x04B4,
	[GAM_B_LUT13]		= 0x04B8,
	[GAM_B_LUT14]		= 0x04BC,
	[GAM_B_LUT15]		= 0x04C0,
	[GAM_B_LUT16]		= 0x04C4,
	[GAM_B_AREA1]		= 0x04C8,
	[GAM_B_AREA2]		= 0x04CC,
	[GAM_B_AREA3]		= 0x04D0,
	[GAM_B_AREA4]		= 0x04D4,
	[GAM_B_AREA5]		= 0x04D8,
	[GAM_B_AREA6]		= 0x04DC,
	[GAM_B_AREA7]		= 0x04E0,
	[GAM_B_AREA8]		= 0x04E4,
	[GAM_R_UPDATE]		= 0x0500,
	[GAM_R_LUT1]		= 0x0508,
	[GAM_R_LUT2]		= 0x050C,
	[GAM_R_LUT3]		= 0x0510,
	[GAM_R_LUT4]		= 0x0514,
	[GAM_R_LUT5]		= 0x0518,
	[GAM_R_LUT6]		= 0x051C,
	[GAM_R_LUT7]		= 0x0520,
	[GAM_R_LUT8]		= 0x0524,
	[GAM_R_LUT9]		= 0x0528,
	[GAM_R_LUT10]		= 0x052C,
	[GAM_R_LUT11]		= 0x0530,
	[GAM_R_LUT12]		= 0x0534,
	[GAM_R_LUT13]		= 0x0538,
	[GAM_R_LUT14]		= 0x053C,
	[GAM_R_LUT15]		= 0x0540,
	[GAM_R_LUT16]		= 0x0544,
	[GAM_R_AREA1]		= 0x0548,
	[GAM_R_AREA2]		= 0x054C,
	[GAM_R_AREA3]		= 0x0550,
	[GAM_R_AREA4]		= 0x0554,
	[GAM_R_AREA5]		= 0x0558,
	[GAM_R_AREA6]		= 0x055C,
	[GAM_R_AREA7]		= 0x0560,
	[GAM_R_AREA8]		= 0x0564,

	/* TCON BLOCK */
	[TCON_UPDATE]		= 0x0580,
	[TCON_TIM]		= 0x0584,
	[TCON_TIM_STVA1]	= 0x0588,
	[TCON_TIM_STVA2]	= 0x058C,
	[TCON_TIM_STVB1]	= 0x0590,
	[TCON_TIM_STVB2]	= 0x0594,
	[TCON_TIM_STH1]		= 0x0598,
	[TCON_TIM_STH2]		= 0x059C,
	[TCON_TIM_STB1]		= 0x05A0,
	[TCON_TIM_STB2]		= 0x05A4,
	[TCON_TIM_CPV1]		= 0x05A8,
	[TCON_TIM_CPV2]		= 0x05AC,
	[TCON_TIM_POLA1]	= 0x05B0,
	[TCON_TIM_POLA2]	= 0x05B4,
	[TCON_TIM_POLB1]	= 0x05B8,
	[TCON_TIM_POLB2]	= 0x05BC,
	[TCON_TIM_DE]		= 0x05C0,

	/* OUTPUT CONTROLLER */
	[OUT_UPDATE]		= 0x0600,
	[OUT_SET]		= 0x0604,
	[OUT_BRIGHT1]		= 0x0608,
	[OUT_BRIGHT2]		= 0x060C,
	[OUT_CONTRAST]		= 0x0610,
	[OUT_PDTHA]		= 0x0614,
	[OUT_CLK_PHASE]		= 0x0624,

	/* LVDS */
	[LVDS_UPDATE]		= 0x0630,
	[LVDSFCL]		= 0x0634,
	[LCLKSELR]		= 0x0650,
	[LPLLSETR]		= 0x0654,
	[LPLLMONR]		= 0x0658,
	[LPHYACC]		= 0x065C,

	/* SYSTEM CONTROLLER */
	[SYSCNT_INT1]		= 0x0680,
	[SYSCNT_INT2]		= 0x0684,
	[SYSCNT_INT3]		= 0x0688,
	[SYSCNT_INT4]		= 0x068C,
	[SYSCNT_INT5]		= 0x0690,
	[SYSCNT_INT6]		= 0x0694,
	[SYSCNT_PANEL_CLK]	= 0x0698, /* 16-bit */
	[SYSCNT_CLUT]		= 0x069A, /* 16-bit */
};

/* INTERRUPT NAME */
#if 0 /* interrupts not used */
static const char *irq_names[VDC5FB_MAX_IRQS] = {
	[S0_VI_VSYNC]		= "s0_vi_vsync",
	[S0_LO_VSYNC]		= "s0_lo_vsync",
	[S0_VSYNCERR]		= "s0_vsyncerr",
	[GR3_VLINE]		= "gr3_vline",
	[S0_VFIELD]		= "s0_vfield",
	[IV1_VBUFERR]		= "iv1_vbuferr",
	[IV3_VBUFERR]		= "iv3_vbuferr",
	[IV5_VBUFERR]		= "iv5_vbuferr",
	[IV6_VBUFERR]		= "iv6_vbuferr",
	[S0_WLINE]		= "s0_wline",
	[S1_VI_VSYNC]		= "s1_vi_vsync",
	[S1_LO_VSYNC]		= "s1_lo_vsync",
	[S1_VSYNCERR]		= "s1_vsyncerr",
	[S1_VFIELD]		= "s1_vfield",
	[IV2_VBUFERR]		= "iv2_vbuferr",
	[IV4_VBUFERR]		= "iv4_vbuferr",
	[S1_WLINE]		= "s1_wline",
	[OIR_VI_VSYNC]		= "oir_vi_vsync",
	[OIR_LO_VSYNC]		= "oir_lo_vsync",
	[OIR_VLINE]		= "oir_vline",
	[OIR_VFIELD]		= "oir_vfield",
	[IV7_VBUFERR]		= "iv7_vbuferr",
	[IV8_VBUFERR]		= "iv8_vbuferr",
};
#endif

/************************************************************************/
/* REGISTER BITS */

#define	RGB888(r, g, b)		\
	((((r) & 0xffu) << 16) | (((g) & 0xffu) << 8) | ((b) & 0xffu))
#define	GBR888(g, b, r)		\
	((((g) & 0xffu) << 16) | (((b) & 0xffu) << 8) | ((r) & 0xffu))
#define	AGBR(a, g, b, r)		\
	((((a) & 0xffu) << 24) | (((g) & 0xffu) << 16) \
	| (((b) & 0xffu) << 8) | ((r) & 0xffu))

/* SYSCNT_PANEL_CLK */
#define	PANEL_DCDR(x)		(((x) & 0x3fu) << 0)
#define PANEL_ICKEN		(1u << 8)
#define PANEL_OCKSEL(x)		(((x) & 0x3u) << 10)
#define PANEL_ICKSEL(x)         (((x) & 0x3u) << 12)

/* SCx_SCL0_FRC1, OIR_SCL0_FRC1 (x=0,1) */
#define	SC_RES_VMASK_ON		(1u << 0)
#define	SC_RES_VMASK(x)		(((x) & 0xffffu) << 16)
/* SCx_SCL0_FRC2, OIR_SCL0_FRC2 */
#define	SC_RES_VLACK_ON		(1u << 0)
#define	SC_RES_VLACK(x)		(((x) & 0xffffu) << 16)
/* SCx_SCL0_FRC3, OIR_SCL0_FRC3 */
#define	SC_RES_VS_SEL		(1u << 0)
#define	SC_RES_VS_IN_SEL	(1u << 8)		/* SC0, SC1 only */
#define	OIR_RES_EN		(1u << 16)		/* OIR only */
/* SCx_SCL0_FRC4, OIR_SCL0_FRC4 */
#define	SC_RES_FH(x)		(((x) & 0x7ffu) << 0)
#define	SC_RES_FV(x)		(((x) & 0x7ffu) << 16)
/* SCx_SCL0_FRC5, OIR_SCL0_FRC5 */
#define	SC_RES_VSDLY(x)		(((x) & 0xffu) << 0)
#define	SC_RES_FLD_DLY_SEL	(1u << 8)		/* SC0, SC1 only */
/* SCx_SCL0_FRC6, OIR_SCL0_FRC6 */
#define	SC_RES_F_VW(x)		(((x) & 0x7ffu) << 0)
#define	SC_RES_F_VS(x)		(((x) & 0x7ffu) << 16)
/* SCx_SCL0_FRC7, OIR_SCL0_FRC7 */
#define	SC_RES_F_HW(x)		(((x) & 0x7ffu) << 0)
#define	SC_RES_F_HS(x)		(((x) & 0x7ffu) << 16)
/* SCx_SCL0_OVR1, OIR_SCL0_OVR1 */
#define	D_SC_RES_BK_COL		RGB888(0, 0, 0)
/* SCx_SCL0_US8, OIR_SCL0_US8 */
#define	SC_RES_IBUS_SYNC_SEL	(1u << 4)

/* GRx_FLM_RD, GR_OIR_FLM_RD (x=0,1,2,3) */
#define	GR_R_ENB		(1u << 0)
/* GRx_FLM1, GR_OIR_FLM1 */
#define	GR_BST_MD		(1u << 0)
#define	GR_OIR_IMR_FLM_INV	(1u << 4)		/* GR_OIR only */
#define	GR_FLM_SEL(x)		(((x) & 0x3u) << 8)
#define	GR_LN_OFF_DIR		(1u << 16)
/* GRx_FLM3, GR_OIR_FLM3 */
#define	GR_FLM_NUM(x)		(((x) & 0x3ffu) << 0)
#define	GR_LN_OFF(x)		(((x) & 0x7fffu) << 16)
/* GRx_FLM4, GR_OIR_FLM4 */
#define	GR_FLM_OFF(x)		(((x) & 0x7fffffu) << 0)
/* GRx_FLM5, GR_OIR_FLM5 */
#define	GR_FLM_LOOP(x)		(((x) & 0x7ffu) << 0)
#define	GR_FLM_LNUM(x)		(((x) & 0x7ffu) << 16)
/* GRx_FLM6, GR_OIR_FLM6 */
//#define	GR_RDSWA(x)		(((x) & 0x7u) << 10)	/* moved to vdc5.h */
#define	GR_HW(x)		(((x) & 0x7ff) << 16)
//#define	GR_FORMAT(x)		(((x) & 0xfu) << 28)	/* moved to vdc5.h */
#define	D_GR_FLM6_RGB565	(GR_RDSWA(6) | GR_FORMAT(0))
#define	D_GR_FLM6_RGB888	(GR_RDSWA(4) | GR_FORMAT(1))
#define	D_GR_FLM6_ARGB8888	(GR_RDSWA(4) | GR_FORMAT(4))
#define	D_GR_FLM6_RGBA8888	(GR_RDSWA(4) | GR_FORMAT(11))
/* GRx_AB1, GR_VIN_AB1, GR_OIR_AB1 */
#define	GR_DISP_SEL(x)		(((x) & 0x3u) << 0)
#define	GR_VIN_SCL_UND_SEL	(1u << 2)		/* GR_VIN only */
#define	GR1_CUS_CON_ON		(1u << 28)		/* GR1 only */
#define	GR_AB1_MASK		0xeffffff4u
/* GRx_AB2, GR_VIN_AB2, GR_OIR_AB2 */
#define	GR_GRC_VW(x)		(((x) & 0x7ffu) << 0)
#define	GR_GRC_VS(x)		(((x) & 0x7ffu) << 16)
/* GRx_AB3, GR_VIN_AB3, GR_OIR_AB3 */
#define	GR_GRC_HW(x)		(((x) & 0x7ffu) << 0)
#define	GR_GRC_HS(x)		(((x) & 0x7ffu) << 16)
/* GRx_AB8, GR_VIN_AB8, GR_OIR_AB8 */
/* GRx_AB9, GR_VIN_AB9, GR_OIR_AB9 */
/* GRx_AB10, GR_VIN_AB10, GR_OIR_AB10 */
/* GRx_AB11, GR_VIN_AB11, GR_OIR_AB11 */
#define	D_GR_AB8		AGBR(0, 0, 0, 0)
#define	D_GR_AB9		AGBR(0xffu, 0, 0, 0)
#define	D_GR_AB10		AGBR(0xffu, 0, 0, 0)
#define	D_GR_AB11		AGBR(0xffu, 0, 0, 0)
/* GR0_BASE */
#define	D_GR_BASE		GBR888(0, 0, 0)

/* OUT_CLK_PHASE */
#define OUTCNT_LCD_EDGE(x)	(((x) & 0x1u) << 8)

/* OUT_BRIGHT1, OUT_BRIGHT2 */
#define	PBRT_G(x)		(((x) & 0x3ffu) << 0)
#define	PBRT_B(x)		(((x) & 0x3ffu) << 16)
#define	PBRT_R(x)		(((x) & 0x3ffu) << 0)
/* OUT_CONTRAST */
#define	CONT_R(x)		(((x) & 0xffu) << 0)
#define	CONT_B(x)		(((x) & 0xffu) << 8)
#define	CONT_G(x)		(((x) & 0xffu) << 16)
/* OUT_PDTHA */
#define	PDTHA_FORMAT(x)		(((x) & 0x3u) << 16)
#define	D_OUT_PDTHA		0x00003021u
/* OUT_SET */
#define	OUT_FORMAT(x)		(((x) & 0x3u) << 12)
#define	D_OUT_SET		0x001f0000u

/* TCON_TIM */
#define	TCON_OFFSET(x)          (((x) & 0x7ffu) << 0)
#define	TCON_HALF(x)            (((x) & 0x7ffu) << 16)
/* TCON_TIM_xxxx */
#define	TCON_VW(x)		(((x) & 0x7ffu) << 0)
#define	TCON_VS(x)		(((x) & 0x7ffu) << 16)
#define	TCON_SEL(x)		(((x) & 0x7u) << 0)
#define	TCON_INV		(0x1u << 4)
#define	TCON_HW(x)		(((x) & 0x7ffu) << 0)
#define	TCON_HS(x)		(((x) & 0x7ffu) << 16)
#define	TCON_SEL(x)		(((x) & 0x7u) << 0)
#define	TCON_INV		(0x1u << 4)
#define	TCON_HS_SEL		(0x1u << 8)
#define	TCON_MD(x)		(((x) & 0x3u) << 12)
#define	TCON_DE_INV		(0x1u << 0)

/* INP_UPDATE */
#define	INP_IMG_UPDATE		(1u << 0)
#define	INP_EXT_UPDATE		(1u << 4)
/* IMGCNT_UPDATE */
#define	IMGCNT_VEN		(1u << 0)
/* SCx_SCLx_UPDATE, GRx_UPADTE, OIR_SCLx_UPDATE (x=0,1) */
#define	SC_SCL0_VEN_A		(1u << 0)
#define	SC_SCL0_VEN_B		(1u << 4)
#define	SC_SCL0_UPDATE		(1u << 8)
#define	SC_SCL0_VEN_C		(1u << 12)	/* not OIR_SCL1_UPDATE */
#define	SC_SCL0_VEN_D		(1u << 13)	/* not OIR_SCL1_UPDATE */
#define	SC_SCL1_VEN_A		(1u << 0)
#define	SC_SCL1_VEN_B		(1u << 4)
#define	SC_SCL1_UPDATE_A	(1u << 16)
#define	SC_SCL1_UPDATE_B	(1u << 20)
/* ADJx_UPDATE */
#define	ADJ_VEN			(1u << 0)
/* GRx_UPDATE, GR_OIR_UPDATE (x=2,3) */
#define	GR_IBUS_VEN		(1u << 0)
#define	GR_P_VEN		(1u << 4)
#define	GR_UPDATE		(1u << 8)
/* GAM_x_UPDATE (x=G,B,R) */
#define	GAM_VEN			(1u << 0)
/* TCON_UPDATE */
#define	TCON_VEN		(1u << 0)
/* OUT_UPDATE */
#define	OUTCNT_VEN		(1u << 0)

/* LVDS */
#define LVDS_CLK_EN		(1u << 4)
#define LVDS_PLL_PD		(1u << 0)
#define LVDS_VDC_SEL		(1u << 1)
#define LVDS_PLL_LD		(1u << 0)
#define LVDS_LCLKSELR_MASK	(0x0703FF02u)
#define LVDS_LPLLSETR_MASK	(0x07FF1F30u)

#define LVDS_SET_IN_CLK_SEL(x)	(((x) & 0x7) << 24)
#define LVDS_SET_IDIV(x)	(((x) & 0x3) << 16)
#define LVDS_SET_TST(x)		(((x) & 0x3f) << 10)
#define LVDS_SET_ODIV(x)	(((x) & 0x3) << 8)
#define LVDS_SET_FD(x)		(((x) & 0x7ff) << 16)
#define LVDS_SET_RD(x)		(((x) & 0x1f) << 8)
#define LVDS_SET_OD(x)		(((x) & 0x3) << 4)

#endif /* _VDC5FB_REGS_H_ */
