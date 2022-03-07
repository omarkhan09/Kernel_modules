/* On kernels 5.7+ kallsyms_lookup_name is not exported anymore, so it is not usable in kernel modules.
* The address of this function is visible via /proc/kallsyms
* but since the address is randomized on reboot, hardcoding a value is not possible.
* A kprobe replaces the first instruction of a kernel function
* and saves cpu registers into a struct pt_regs *regs and then a handler
* function is executed with that struct as parameter.
* The saved value of the instruction pointer in regs->ip, is the address of probed function + 1.
* A kprobe on kallsyms_lookup_name can read the address in the handler function.
* Internally register_kprobe calls kallsyms_lookup_name, which is visible for this code, so,
* planting a second kprobe, allow us to get the address of kallsyms_lookup_name without waiting
* and then we can call this address via a function pointer, to use kallsyms_lookup_name in our module.
*
* example for _x86_64.
*/

#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/kprobes.h>
#include <linux/dma-map-ops.h>
#include <linux/pci.h>
//#include <asm/dma-mapping.h>
#include <linux/dma-direction.h>
#include "print_name.h"
#include <linux/msi.h>
MODULE_LICENSE("GPL");


struct kprobe p = {0};
int (*print_name_ptr) (char *name, char *name2);

#define KPROBE_PRE_HANDLER(fname) static int __kprobes fname(struct kprobe *p, struct pt_regs *regs)

long unsigned int kln_addr = 0;
unsigned long (*kln_pointer)(const char *name) = NULL;

static struct kprobe kp0, kp1;

KPROBE_PRE_HANDLER(handler_pre0)
{
  kln_addr = (--regs->ip);
  
  return 0;
}

KPROBE_PRE_HANDLER(handler_pre1)
{
  return 0;
}

static int do_register_kprobe(struct kprobe *kp, char *symbol_name, void *handler)
{
  int ret;
  
  kp->symbol_name = symbol_name;
  kp->pre_handler = handler;
  
  ret = register_kprobe(kp);
  if (ret < 0) {
    pr_err("register_probe() for symbol %s failed, returned %d\n", symbol_name, ret);
    return ret;
  }
  
  pr_info("Planted kprobe for symbol %s at %p\n", symbol_name, kp->addr);
  
  return ret;
}

static int kprobe_pre_handler_test (struct kprobe *kp, struct pt_regs *pt_regs)
{
	static int i = 0;
	i++;
	struct msi_desc *entry =(struct msi_desc *) pt_regs->di;
	struct msi_msg *msg = (struct msi_msg *) pt_regs->si;
	printk("Kprobe Pre handler irq %d, msg->address_lo 0x%x, msg->address_hi 0x%x, msg->data = 0x%x\n ",entry->irq,msg->address_lo, msg->address_hi, msg->data);

//	char *str0  = (char *) pt_regs->di;
//	char *str1 = (char *) pt_regs->si;
//	printk("Kprobe Pre handler str0 = %s , str1 = %s\n",str0,str1);
	printk("__pci_write_msi_msg called ith time = %d\n",i);
	return 0;

}

/*static int prsyms_print_symbol(void *data, const char *namebuf,
                               struct module *module, unsigned long address)
{

	if(strcmp(namebuf, "print_my_name") == 0) {
    	pr_info("### %lx\t%s\n", address, namebuf);
		print_name_ptr = (typeof(print_name_ptr))address;
		return 1;
	}
    return 0;
}*/

int init_module(void) 
{
	int err;
	unsigned long addr;
	int ret;

	/*kallsyms_on_each_symbol(prsyms_print_symbol, NULL);*/

	pr_info("kprobe module loaded\n");
  
	ret = do_register_kprobe(&kp0, "kallsyms_lookup_name", handler_pre0);
	if (ret < 0)
		return ret;
	
	ret = do_register_kprobe(&kp1, "kallsyms_lookup_name", handler_pre1);
	if (ret < 0) {
		unregister_kprobe(&kp0);
		return ret;
	}
	
	unregister_kprobe(&kp0);
	unregister_kprobe(&kp1);
	
	pr_info("kallsyms_lookup_name address = 0x%lx\n", kln_addr);
	
	kln_pointer = (unsigned long (*)(const char *name)) kln_addr;
	
	pr_info("kallsyms_lookup_name address = 0x%lx\n", kln_pointer("kallsyms_lookup_name"));
	
	addr = kln_pointer("__pci_write_msi_msg");
	if (!addr) {
		printk("addr is null: exiting\n");
		return -1;
	}

	printk("print_name_addr: %llx\n",(u64)addr);

	p.addr = (kprobe_opcode_t *)addr;
	/*p.symbol_name = "xen_swiotlb_sync_single_for_cpu";*/
	p.pre_handler = kprobe_pre_handler_test;
	p.post_handler = NULL;
	
	err = register_kprobe(&p);
	if (err < 0) {
		printk("kprobe registeration failed\n");
	}

//	print_name_ptr = (int (*) (char *name,char *name2))addr;
//	print_name_ptr("Tayyab\n","Omar\n");
	return 0;
}


void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
	//symbol_put(print_name);
	 unregister_kprobe(&p);
}
