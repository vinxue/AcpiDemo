// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  File operation for ACPI demo device.
 *
 *  Copyright (C) 2023 Gavin Xue
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/acpi.h>
#include <acpi/acpi_bus.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gavin Xue");

#define DEVICE_NAME_DEM0        "\\_SB.DEM0"
#define METHOD_NAME_SETM        "SETM"
#define METHOD_NAME_RETM        "RETM"

#define PROC_DEMO                "demo"

extern struct proc_dir_entry *acpi_root_dir;

static ssize_t acpi_demo_read(struct file *filp, char __user *buffer,
				size_t count, loff_t *off )
{
	acpi_status status = AE_OK;
	acpi_handle handle;
	acpi_integer retrun_data = 0;

	/* get the handle of the ACPI device */
	status = acpi_get_handle(NULL, DEVICE_NAME_DEM0, &handle);
	if (ACPI_FAILURE(status)) {
		printk("acpi_demo_op: Cannot get handle.\n");
		return -ENODEV;
	}

	status = acpi_evaluate_integer(handle, METHOD_NAME_RETM, NULL, &retrun_data);
	printk("acpi_demo_op: return data: 0x%llX\n", retrun_data);

	return 0;
}

static ssize_t acpi_demo_write(struct file *filp, const char __user *buffer,
				size_t count, loff_t *data )
{
	acpi_status status = AE_OK;
	acpi_handle handle;
	unsigned long val;
	union acpi_object param;
	struct acpi_object_list input;
	char buf[32];
	ssize_t len;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, buffer, len))
		return -EFAULT;

	buf[len] = '\0';
	if (kstrtoul(buf, 16, &val)) {
		printk("kstrtoul fail.\n");
		return -EINVAL;
	}

	printk("acpi_demo_op: Write data is: 0x%lX\n", val);

	/* get the handle of the ACPI device */
	status = acpi_get_handle(NULL, DEVICE_NAME_DEM0, &handle);
	if (ACPI_FAILURE(status)) {
		printk("acpi_demo_op: Cannot get handle.\n");
		return -ENODEV;
	}

	/* call the method */
	param.type = ACPI_TYPE_INTEGER;
	param.integer.value = val;
	input.count = 1;
	input.pointer = &param;

	status = acpi_evaluate_object(handle, METHOD_NAME_SETM, &input, NULL);
	if (ACPI_FAILURE(status))
		printk("Error set data: %d\n", status);

	printk("acpi_demo_op: set successfully\n");

	return count;
}

static struct proc_ops acpi_demo_operations = {
	.proc_read	= acpi_demo_read,
	.proc_write	= acpi_demo_write,
};

/** module initialization function */
static int __init init_acpi_demo_op(void)
{
	struct proc_dir_entry *p;

	p = proc_create(PROC_DEMO, 0660, acpi_root_dir, &acpi_demo_operations);
	if (!p) {
		printk("Unable to initialize proc entry\n");
		return -ENOMEM;
	}

	printk ("acpi_demo_op: Module loaded successfully\n");

	return 0;
}

static void __exit unload_acpi_demo_op(void)
{
	remove_proc_entry("demo", acpi_root_dir);

	printk(KERN_INFO "acpi_demo_op: Module unloaded successfully\n");
}

module_init(init_acpi_demo_op);
module_exit(unload_acpi_demo_op);
