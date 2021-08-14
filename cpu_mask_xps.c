/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/sched.h>
#include <linux/cpumask.h>
MODULE_LICENSE("GPL");


#define Number_of_CPUS 48

#define Number_of_Queues 10


int init_module(void)
{

	int i = 0, tmp_cpus = 0, cpus_per_Queue;
	bool check;

	
	cpumask_var_t cpumask_xps;
	
	check = alloc_cpumask_var(&cpumask_xps,GFP_KERNEL);
	
	if (!check) {
		pr_err("Low mem: can't allocate cpumask structure\n");
		return -1;
	}
	
	for( i = 0; i < Number_of_Queues; i++) {
		
		cpus_per_Queue = (Number_of_CPUS - tmp_cpus) / (Number_of_Queues - i);
		
		printk("queue index = %d, cpus_per_Queue = %d , tmp_cpus =%d \n",i,cpus_per_Queue,tmp_cpus);
		
		bitmap_set(cpumask_xps->bits, tmp_cpus, cpus_per_Queue);	
		
		printk("mask[0] = 0x%lx \n\n", cpumask_xps->bits[0]);
		
			#if 0
		err = netif_set_xps_queue(netdev, cpu_mask), i);

		if (err) {
			dev_err(&ssnic_priv->pdev->dev, "Unable to set XPS Map\\n");
			/* goto irq_clean; */
		}
		
		#endif	
		
			     
		  cpumask_clear(cpumask_xps); /* Use this to clear actual memory used */
		  
		  tmp_cpus += cpus_per_Queue;
	}
	

	free_cpumask_var(cpumask_xps);
	
	
	printk(KERN_INFO "module loaded\n");

	
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
