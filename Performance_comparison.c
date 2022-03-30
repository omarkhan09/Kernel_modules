#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
MODULE_LICENSE("GPL");


int init_module(void) 
{
	unsigned long delta_t, t1, i;
	int x = 0;
	
	printk("Module loaded\n");	
	
	preempt_disable();
	
	t1 = ktime_get_ns();
	for (i = 0; i < 10000000; i++) {
	
		#if 0
		x++;	
		if (unlikely (x == 1535))
			x = 0;
		#else
		x = (x + 1) % 1535;
		#endif
		
		//x = (!!((x + 1) ^ 1535) ) * (x +1);
	}

	
	delta_t = ktime_get_ns() - t1;

	printk("init module ends: delta T = %ld, i = %ld, x = %d\n",delta_t,i,x);
	preempt_enable();
	
	return 0;
}


void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}



