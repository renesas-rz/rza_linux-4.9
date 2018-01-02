// SPDX-License-Identifier: GPL-1.0+
/*
 * Renesas USB driver RZ/A initialization and power control
 *
 * Copyright (C) 2018 Renesas Electronics Corporation
 */

#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_device.h>
#include "common.h"
#include "rza.h"

static int usbhs_rza1_hardware_init(struct platform_device *pdev)
{
	struct usbhs_priv *priv = usbhs_pdev_to_priv(pdev);

	/* Input Clock Selection (NOTE: ch0 controls both ch0 and ch1) */
	if (of_property_read_bool(priv->pdev->dev.of_node, "xtal-12mhz")) {
		/* Select 12MHz XTAL */
		usbhs_bset(priv, SYSCFG, UCKSEL, UCKSEL);
	}

	/* Enable USB PLL (NOTE: ch0 controls both ch0 and ch1) */
	usbhs_bset(priv, SYSCFG, UPLLE, UPLLE);
	udelay(1000);
	usbhs_bset(priv, SUSPMODE0, SUSPM, SUSPM);

	return 0;
}

static int usbhs_rza1_hardware_exit(struct platform_device *pdev)
{
	return 0;
}

static int usbhs_rza1_power_ctrl(struct platform_device *pdev,
				void __iomem *base, int enable)
{
	return 0;
}

static int usbhs_rza_get_id(struct platform_device *pdev)
{
	return USBHS_GADGET;
}

const struct renesas_usbhs_platform_callback usbhs_rza1_ops = {
	.hardware_init = usbhs_rza1_hardware_init,
	.hardware_exit = usbhs_rza1_hardware_exit,
	.power_ctrl = usbhs_rza1_power_ctrl,
	.get_id = usbhs_rza_get_id,
};


