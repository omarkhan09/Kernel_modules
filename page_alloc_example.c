/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/sched.h>
#include <linux/cpumask.h>
#include <linux/page_ref.h>
#include <linux/slab.h>
MODULE_LICENSE("GPL");

struct page * page;

int init_module(void)
{
	int count;
	page = alloc_page(GFP_KERNEL);
	if(!page) {
		printk("Can't allocate page\n");
		return -1;
	}
	
	
	
	
	//printk("ARCH_KMALLOC_MINALIGN = %d\n",ARCH_KMALLOC_MINALIGN);
	
	count = page_ref_count(page);
	printk(KERN_INFO "module loaded, count = %d\n",count);
	
	page_ref_add(page,1);
	
	printk("After reference counting, count = %d\n",page_ref_count(page));



	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
	
	printk("Before freeing, count = %d\n",page_ref_count(page));
	
	__free_pages(page,0);
	
	printk("After freeing for first time, count = %d\n",page_ref_count(page));
	
	__free_pages(page,0);
	
	printk("After freeing for second time, count = %d\n",page_ref_count(page));
}
