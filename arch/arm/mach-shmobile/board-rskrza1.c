/* for heartbeat */
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kthread.h>

/* Pin Function Controller */
#include "pfc-rza1.c"

static int __init rskrza1_pinmux_init(void)
{
	struct device_node *root;
	struct device_node *np;


	/* Only execute this function on the RSK boards */
	if (!of_machine_is_compatible("renesas,rskrza1"))
		return 0;

	//printk("=== %s ===\n",__func__);

	root = of_find_node_by_path("/");

	r7s72100_pinmux_setup();

	/* ------------ Pin setup section ---------------*/
	r7s72100_pfc_pin_assign(P1_15, ALT1, DIIO_PBDC_EN);	/* AD7 */

	/* RIIC ch 0 (Touchscreen) */
	r7s72100_pfc_pin_assign(P1_0, ALT1, DIIO_PBDC_EN);	/* I2C SCL0 */
	r7s72100_pfc_pin_assign(P1_1, ALT1, DIIO_PBDC_EN);	/* I2C SDA0 */

	/* RIIC ch 3 (Port Expander, EEPROM (MAC Addr), Audio Codec) */
	r7s72100_pfc_pin_assign(P1_6, ALT1, DIIO_PBDC_EN);	/* I2C SCL3 */
	r7s72100_pfc_pin_assign(P1_7, ALT1, DIIO_PBDC_EN);	/* I2C SDA3 */

	/* Audio */
	r7s72100_pfc_pin_assign(P4_4, ALT5, DIIO_PBDC_EN);	/* SSISCK0 */
	r7s72100_pfc_pin_assign(P4_5, ALT5, DIIO_PBDC_EN);	/* SSIWS0 */
	r7s72100_pfc_pin_assign(P4_6, ALT5, DIIO_PBDC_EN);	/* SSIRxD0 */
	r7s72100_pfc_pin_assign(P4_7, ALT5, SWIO_OUT_PBDCEN);	/* SSITxD0 */

	/* NOTE: USB pins are dedicated */

	/* MMC pins */
	np = of_find_node_by_path("/mmc@e804c800");
	if (np) {
		if (of_device_is_available(np)) {
			/* MMC on CN1 */
			printk("=== MMC Enabled on CN1 ===\n");
			r7s72100_pfc_pin_assign(P3_8, ALT8, DIIO_PBDC_DIS);	/* MMC CD */
			r7s72100_pfc_pin_assign(P3_10, ALT8, DIIO_PBDC_DIS);	/* MMC DAT1 */
			r7s72100_pfc_pin_assign(P3_11, ALT8, DIIO_PBDC_DIS);	/* MMC DAT0 */
			r7s72100_pfc_pin_assign(P3_12, ALT8, DIIO_PBDC_DIS);	/* MMC CLK */
			r7s72100_pfc_pin_assign(P3_13, ALT8, DIIO_PBDC_DIS);	/* MMC CMD */
			r7s72100_pfc_pin_assign(P3_14, ALT8, DIIO_PBDC_DIS);	/* MMC DAT3*/
			r7s72100_pfc_pin_assign(P3_15, ALT8, DIIO_PBDC_DIS);	/* MMC DAT2 */
			r7s72100_pfc_pin_assign(P4_0, ALT8, DIIO_PBDC_DIS);	/* MMC DAT4 */
			r7s72100_pfc_pin_assign(P4_1, ALT8, DIIO_PBDC_DIS);	/* MMC DAT5 */
			r7s72100_pfc_pin_assign(P4_2, ALT8, DIIO_PBDC_DIS);	/* MMC DAT6*/
			r7s72100_pfc_pin_assign(P4_3, ALT8, DIIO_PBDC_DIS);	/* MMC DAT7 */
		}
		of_node_put(np);
	}
	
	/* SDHI ch 0 pins */
	//np = of_find_node_by_path("/sd@e804e000");

	/* SDHI ch 1 pins */
	np = of_find_node_by_path("/sd@e804e800");
	if (np) {
		if (of_device_is_available(np)) {
			/* SHDI ch1 on CN1 */
			printk("=== SDHI Enabled on CN1 ===\n");
			r7s72100_pfc_pin_assign(P3_8, ALT7, DIIO_PBDC_DIS);	/* SDHI1 CD */
			r7s72100_pfc_pin_assign(P3_9, ALT7, DIIO_PBDC_DIS);	/* SDHI1 WP */
			r7s72100_pfc_pin_assign(P3_10, ALT7, DIIO_PBDC_EN);	/* SDHI1 DAT1 */
			r7s72100_pfc_pin_assign(P3_11, ALT7, DIIO_PBDC_EN);	/* SDHI1 DAT0 */
			r7s72100_pfc_pin_assign(P3_12, ALT7, DIIO_PBDC_DIS);	/* SDHI1 CLK */
			r7s72100_pfc_pin_assign(P3_13, ALT7, DIIO_PBDC_EN);	/* SDHI1 CMD */
			r7s72100_pfc_pin_assign(P3_14, ALT7, DIIO_PBDC_EN);	/* SDHI1 DAT3*/
			r7s72100_pfc_pin_assign(P3_15, ALT7, DIIO_PBDC_EN);	/* SDHI1 DAT2 */
		}
		of_node_put(np);
	}


	/* RSPI4 */
	/* Add "rspi4-testing" to the device tree to enable this code */
	/* Make sure you execute "io_mux c" in u-boot before booting */
	if (of_property_read_bool(root, "rspi-testing")) {

		/* Enable SPI4 pins */
		/* RSKRZA1 Board SPI4 is on CN15 (but that means you can't use Ethernet) */
		printk("== RSPI4 enabled on CN15 ==\n");
		r7s72100_pfc_pin_assign(P2_8, ALT8, DIIO_PBDC_EN);	/* RSPCK4 */
		r7s72100_pfc_pin_assign(P2_9, ALT8, DIIO_PBDC_EN);	/* SSL40 */
		r7s72100_pfc_pin_assign(P2_10, ALT8, DIIO_PBDC_EN);	/* MOSI4 */
		r7s72100_pfc_pin_assign(P2_11, ALT8, DIIO_PBDC_EN);	/* MISO4 */
	}

	/* SCIF 1 pins */
	np = of_find_node_by_path("/serial@e8007800");
	if (np) {
		if (of_device_is_available(np)) {
#if 1
			/* SCIF1 on JA6 */
			printk("=== SCIF1 Enabled on JA6 ===\n");
			r7s72100_pfc_pin_assign(P4_12, ALT7, DIIO_PBDC_DIS);	/* SCIF1 TX */
			r7s72100_pfc_pin_assign(P4_13, ALT7, DIIO_PBDC_DIS);	/* SCIF1 RX */
#else
			/* SCIF1 with flow control on JA1 (used for Bluetooth testing) */
			printk("=== SCIF1 with CTS/RTS Enabled on JA1 ===\n");
			r7s72100_pfc_pin_assign(P2_3, ALT6, DIIO_PBDC_DIS);	/* SCIF1 CTS1 */
			r7s72100_pfc_pin_assign(P2_5, ALT6, DIIO_PBDC_DIS);	/* SCIF1 TXD1 */
			r7s72100_pfc_pin_assign(P2_6, ALT6, DIIO_PBDC_DIS);	/* SCIF1 RXD1 */
			r7s72100_pfc_pin_assign(P2_7, ALT6, DIIO_PBDC_DIS);	/* SCIF1 RTS1 */
			r7s72100_pfc_pin_assign(P2_4, PMODE, PORT_OUT_LOW);	/* BT chip reset */
			mdelay(6);
			r7s72100_pfc_pin_assign(P2_4, PMODE, PORT_OUT_HIGH);	/* BT chip reset */
#endif
		}
		of_node_put(np);
	}

	if (root)
		of_node_put(root);

	return 0;
}
early_initcall(rskrza1_pinmux_init);
/* HINT: When you declare a function with early_initcall, that function
 * automatically be run early in the boot process before device drivers
 * have started loading.
 */

static int __init rskrza1_init_early(void)
{
	//printk("=== %s ===\n",__func__);
	
	return 0;
}
early_initcall(rskrza1_init_early);
/* HINT: When you declare a function with early_initcall, that function
 * automatically be run early in the boot process before device drivers
 * have started loading.
 */


/* Write to I2C device */
/* Based off of board-sx1.c */
int rza1_i2c_write_byte(u8 ch, u8 devaddr, u8 regoffset, u8 value)
{
	struct i2c_adapter *adap;
	int err;
	struct i2c_msg msg[1];
	unsigned char data[2];

	adap = i2c_get_adapter(ch);
	if (!adap)
		return -ENODEV;
	msg->addr = devaddr;	/* I2C address of chip */
	msg->flags = 0;
	msg->len = 2;
	msg->buf = data;
	data[0] = regoffset;	/* register num */
	data[1] = value;		/* register data */
	err = i2c_transfer(adap, msg, 1);
	i2c_put_adapter(adap);
	if (err >= 0)
		return 0;
	return err;
}

/* Read from I2C device */
/* Based off of board-sx1.c */
int rza1_i2c_read_byte(u8 ch, u8 devaddr, u8 regoffset, u8 *value)
{
	struct i2c_adapter *adap;
	int err;
	struct i2c_msg msg[1];
	unsigned char data[2];

	adap = i2c_get_adapter(ch);
	if (!adap)
		return -ENODEV;

	msg->addr = devaddr;	/* I2C address of chip */
	msg->flags = 0;
	msg->len = 1;
	msg->buf = data;
	data[0] = regoffset;	/* register num */
	err = i2c_transfer(adap, msg, 1);
	if (err < 0)
		return err;

	msg->addr = devaddr;	/* I2C address */
	msg->flags = I2C_M_RD;
	msg->len = 1;
	msg->buf = data;
	err = i2c_transfer(adap, msg, 1);
	*value = data[0];
	i2c_put_adapter(adap);

	if (err >= 0)
		return 0;
	return err;
}

static int heartbeat(void * data)
{
	u8 index = 0;
	u8 value;
	int ret;
	static const u8 pattern[8] = {7,6,5,3,7,3,5,6};

	while(1) {
		/* Register address 1 is the Output Control register */
		ret = rza1_i2c_read_byte(3, 0x20, 0x01, &value);
		value &= ~0x7;
		value |= pattern[index++];
		index &= 0x7;

		if( !ret )
			rza1_i2c_write_byte(3, 0x20, 0x01, value);

		msleep_interruptible(250);
		//msleep(250);
	}

	return 0;
}

static int __init rskrza1_init_late(void)
{
	struct device_node *root = of_find_node_by_path("/");

	/* Only execute this function on the RSK boards */
	if (!of_machine_is_compatible("renesas,rskrza1"))
		return 0;

	//printk("=== %s ===\n",__func__);

	/* Add "no-heartbeat" to the device tree to disable heartbeat */
	if (!of_property_read_bool(root, "no-heartbeat")) {
		/* Start heartbeat kernel thread */
		kthread_run(heartbeat, NULL,"heartbeat");
	}

	if (root)
		of_node_put(root);

	return 0;
}
late_initcall(rskrza1_init_late);
/* HINT: When you declare a function with late_initcall, that function
 * automatically be run at the end of kernel boot after all the drivers
 * have been loaded, but before the file system is mounted.
 */
