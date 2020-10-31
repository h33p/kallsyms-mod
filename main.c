#include <linux/types.h>
#include <linux/version.h>
#include <linux/module.h>
#include "kallsyms.h"
#include "ksyms.h"
#include <linux/printk.h>

MODULE_DESCRIPTION("Access to kallsyms_lookup_symbol through kernel livepatch interface");
MODULE_AUTHOR("Heep");
MODULE_LICENSE("GPL");

static int example_init(void);
static void example_exit(void);

module_init(example_init);
module_exit(example_exit);

#define mprintk(format, ...) printk(KBUILD_MODNAME": "format, ##__VA_ARGS__)

KSYMDEF(kvm_lock);
KSYMDEF(vm_list);

static int example_init(void)
{
	int r;

	if ((r = init_kallsyms()))
		return r;

	KSYMINIT_FAULT(kvm_lock);
	KSYMINIT_FAULT(vm_list);

	if (r)
		return r;

	mprintk("initialized\n");

	mprintk("kvm_lock: %p\n", _kvm_lock);
	mprintk("vm_list: %p\n", _vm_list);

	return 0;
}

static void example_exit(void)
{
	mprintk("uninitialized\n");
}
