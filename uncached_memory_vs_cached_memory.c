/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/sched.h>
#include <asm/set_memory.h>
#include <linux/mm.h>
MODULE_LICENSE("GPL");

/*

    int set_memory_uc(unsigned long addr, int numpages); //uncached
    int set_memory_wc(unsigned long addr, int numpages); // write-combine
    int set_memory_wb(unsigned long addr, int numpages); // writeback (ca

*/


struct page * page;
volatile int xx = 0;
int init_module(void)
{
	int rv, i = 0;
	u64 t0_ns, delta_ns;
	volatile u32 *ptr;
	page = alloc_page(GFP_KERNEL);
	if(!page) {
		printk("Can't allocate page\n");
		return -1;
	}
	ptr = page_address(page);
	rv =  set_memory_uc(ptr, 1); /*made the page uncached*/
	printk("rv %d\n",rv);
	
	local_irq_disable();
	
	t0_ns = ktime_get_ns();
	for (; i< 100000000; i++) {
		
		xx = *(ptr + (i & (1023))); // i % 1024
		
	}
	
	delta_ns = ktime_get_ns() - t0_ns;
	
	local_irq_enable();
	
	//55426350 : 7045620725
	
	printk("delta_ns %lld\n",delta_ns);
	 
	return 0;
}

void cleanup_module(void)
{
	int rv;
	rv =  set_memory_wb(page_address(page), 1);
	__free_pages(page,0);
	printk("rv %d\n",rv);
}
