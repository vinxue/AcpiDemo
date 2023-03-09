// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  ACPI demo driver
 *
 *  Copyright (C) 2023 Gavin Xue
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/acpi.h>
#include <acpi/acpi_bus.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gavin Xue");
MODULE_ALIAS("acpi*:DEMO0001:*");

static const struct acpi_device_id demo_device_ids[] = {
	{"DEMO0001", 0},
	{"", 0},
};

static void acpi_demo_notify(struct acpi_device *acpi_dev, u32 event)
{
	if (event != 0x80) {
		printk("acpi-demo: received unknown event (0x%x)\n", event);
		return;
	}

	/* Not implemented yet */
}

static int acpi_demo_add(struct acpi_device *device)
{
	printk("acpi-demo: acpi_demo_add\n");

	return 0;
}

static int acpi_demo_remove(struct acpi_device *device)
{
	printk("acpi-demo: acpi_demo_remove\n");

	return 0;
}

static struct acpi_driver acpi_demo_driver = {
	.name	= "acpi-demo",
	.owner	= THIS_MODULE,
	.ids	= demo_device_ids,
	.ops	= {
		.add	= acpi_demo_add,
		.remove	= acpi_demo_remove,
		.notify	= acpi_demo_notify,
	},
};

module_acpi_driver(acpi_demo_driver);
