#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/clk.h>

/* Pin Function Controller */
#include "pfc-rza1.c"

static int __init genmai_pinmux_init(void)
{
	struct device_node *root;
	struct device_node *np;

	/* Only execute this function on the RSK boards */
	if (!of_machine_is_compatible("renesas,genmai"))
		return 0;

	//printk("=== %s ===\n",__func__);

	root = of_find_node_by_path("/");

	r7s72100_pinmux_setup();

	/* ------------ Pin setup section ---------------*/
	r7s72100_pfc_pin_assign(P1_15, ALT1, DIIO_PBDC_DIS);	/* AD7 */

	/* Set up IRQ for touchscreen */
	{
		/* Manually set IRQ1 for 'low level' trigger in Interrupt Control Register 1 */
		enum { LOW_LEVEL=0, FALLING_EDGE, RISING_EDGE, BOTH_EDGE };
		#define TOUCHSCREEN_IRQ 1

		void __iomem *irc1 = ioremap_nocache(arch/arm/configs/0xfcfef802, 0x2);
		u16 val;
		val = readw(irc1);
		val &= ~(0x3 << (TOUCHSCREEN_IRQ * 2));
		val |= LOW_LEVEL;
		writew(val, irc1);
		iounmap(irc1);
		r7s72100_pfc_pin_assign(P4_9, ALT8, DIIO_PBDC_DIS);  /* IRQ1 */
	}


	if (root)
		of_node_put(root);

	return 0;
}
early_initcall(genmai_pinmux_init);
/* HINT: When you declare a function with early_initcall, that function
 * automatically be run early in the boot process before device drivers
 * have started loading.
 */

static int __init genmai_init_early(void)
{
	//printk("=== %s ===\n",__func__);

	return 0;
}
early_initcall(genmai_init_early);
/* HINT: When you declare a function with early_initcall, that function
 * automatically be run early in the boot process before device drivers
 * have started loading.
 */


static int heartbeat(void * data)
{
	u8 index = 0;
	u8 value1, value2;
	index = 0;
	while(1) {
		switch (index)
		{
			case 0: value1 = 1; break;
			case 1: value1 = 0;
				value2 = 1; break;
			case 2: value2 = 0; break;
			default:	    break;
		}
		index = (index + 1) & 7;
		if( value1 )
			r7s72100_pfc_pin_assign(P4_10, PMODE, PORT_OUT_LOW);
		else
			r7s72100_pfc_pin_assign(P4_10, PMODE, PORT_OUT_HIGH);
		if( value2 )
			r7s72100_pfc_pin_assign(P4_11, PMODE, PORT_OUT_LOW);
		else
			r7s72100_pfc_pin_assign(P4_11, PMODE, PORT_OUT_HIGH);

		msleep_interruptible(125);
	}

	return 0;
}

static int __init genmai_init_late(void)
{
	struct device_node *root = of_find_node_by_path("/");
	struct clk *clk;

	/* Only execute this function on the RSK boards */
	if (!of_machine_is_compatible("renesas,genmai"))
		return 0;

	/* Keep XIP QSPI Alive
	 * At the end of kernel boot, the function clk_disable_unused will
	 * get called which will disable peripheral clocks that are not
	 * being used. However, since we set up XIP QSPI in u-boot, the
	 * kernel does not know that we should not power down the QSPI
	 * interface. Therefore, we will re-enable the clock here which will
	 * inform the kernel that at least someone is using it.
	 */
	clk = clk_get_sys(NULL,"spibsc0");
	if (IS_ERR(clk))
		pr_err("%s: Error: Could not find QSPI clock\n", __func__);
	else
		clk_prepare_enable(clk);

	/* Add "no-heartbeat" to the device tree to disable heartbeat */
	if (!of_property_read_bool(root, "no-heartbeat")) {
		/* Start heartbeat kernel thread */
		kthread_run(heartbeat, NULL,"heartbeat");
	}

	if (root)
		of_node_put(root);

	return 0;
}
late_initcall(genmai_init_late);
/* HINT: When you declare a function with late_initcall, that function
 * automatically be run at the end of kernel boot after all the drivers
 * have been loaded, but before the file system is mounted.
 */
