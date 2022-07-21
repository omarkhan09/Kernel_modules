#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/kallsyms.h>
MODULE_LICENSE("GPL");


int init_module(void) 
{
	unsigned long j0,j1,delay;
	delay = msecs_to_jiffies(10000); /* 10 second delay */
	j0 = jiffies;
	j1 = j0 + delay;
	
	printk("Now we wait for 10 secs\n");	
	while (time_before(jiffies, j1));
	printk("We waited for 10 secs\n");
			
	return 0;
}


void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
