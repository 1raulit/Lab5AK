// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Illya Miheev");
MODULE_DESCRIPTION("Hello1 module");
MODULE_LICENSE("Dual BSD/GPL");

// Структура для списку
struct hello_time {
	struct list_head list;
	ktime_t start_time;
	ktime_t end_time;
};

// Голова списку
static LIST_HEAD(hello_list);

// Функція друку привітання
void print_hello(int times)
{
	struct hello_time *new_node;
	int i;

	for (i = 0; i < times; i++) {
		new_node = kmalloc(sizeof(*new_node), GFP_KERNEL);
		if (!new_node) {
			pr_err("Memory allocation failed\n");
			return;
		}

		new_node->start_time = ktime_get();
		pr_info("Hello, world!\n");
		new_node->end_time = ktime_get();

		list_add_tail(&new_node->list, &hello_list);
	}
}
EXPORT_SYMBOL(print_hello);

// Вивантаження списку
static void cleanup_list(void)
{
	struct hello_time *current_node, *tmp;

	list_for_each_entry_safe(current_node, tmp, &hello_list, list) {
		pr_info("Duration: %llu ns\n",
			ktime_to_ns(ktime_sub(current_node->end_time, current_node->start_time)));
		list_del(&current_node->list);
		kfree(current_node);
	}
}

// Ініціалізація модуля
static int __init hello1_init(void)
{
	pr_info("Hello1 module loaded\n");
	return 0;
}

// Вивантаження модуля
static void __exit hello1_exit(void)
{
	cleanup_list();
	pr_info("Hello1 module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
