// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Illya Miheev");
MODULE_DESCRIPTION("Hello2 module");
MODULE_LICENSE("Dual BSD/GPL");

// Параметр для кількості повторів
static int repeat = 1;
module_param(repeat, int, 0444);
MODULE_PARM_DESC(repeat, "The number of times to print 'Hello, world!'");

static int __init hello2_init(void)
{
	if (repeat > 10) {
		pr_err("Error: repeat > 10\n");
		return -EINVAL;
	} else if (repeat == 0 || (repeat >= 5 && repeat <= 10)) {
		pr_warn("Warning: repeat is 0 or between 5 and 10\n");
	}

	pr_info("Hello2 module calling print_hello\n");
	print_hello(repeat);
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("Hello2 module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
