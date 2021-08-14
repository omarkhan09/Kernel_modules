/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/sched.h>
MODULE_LICENSE("GPL");

int init_module(void)
{
	wait_queue_head_t mbox_tx_waitq;
	
	init_waitqueue_head(&mbox_tx_waitq);
	
	printk(KERN_INFO "before waitqueue\n");
	
	wait_event_interruptible_timeout(mbox_tx_waitq,0,msecs_to_jiffies(10000));
	
	printk(KERN_INFO "after waitqueue\n");

	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

void cleanup_module(void)
{
	
	printk(KERN_INFO "Goodbye world 1.\n");
}
