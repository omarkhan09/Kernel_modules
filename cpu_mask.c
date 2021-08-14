/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/sched.h>
#include <linux/cpumask.h>
MODULE_LICENSE("GPL");


#define Number_of_CPUS 256

#define Number_of_Queues 5

cpumask_t cpumask_XPS ;   /* Remove those as using this as local variable compiler will give warning due to large variable size */
cpumask_t cpumask_TEMP ;  /*  Remove those */

int init_module(void)
{

	int i = 0, tmp_cpus = 0, cpus_per_Queue;


	
	cpumask_var_t cpumask_xps = &cpumask_XPS;  /* Use this: zalloc_cpumask_var(&cpumask_xps,GFP_KERNEL); check the return value as well, when using this use cpumask helpers instead of bitmap helpers */ 
	//cpumask_var_t cpumask_temp = &cpumask_TEMP; /*Use this: zalloc_cpumask_var(&cpumask_temp,GFP_KERNEL); check the return value as well, when using this use cpumask helpers instead of bitmap helpers  */
	
	//bitmap_set(cpumask_temp->bits, 0, cpus_per_Queue);
	
	
	//bitmap_set(cpumask_xps->bits, 0, cpus_per_Queue);
	
	for( i = 0; i < Number_of_Queues; i++) {
		
		
		cpus_per_Queue = ((Number_of_CPUS - tmp_cpus) / (Number_of_Queues - i)) ;
		
		printk("queue index = %d, cpus_per_Queue = %d\n",i,cpus_per_Queue);
		
		/*bitmap_shift_left(cpumask_bits(cpumask_xps), cpumask_bits(cpumask_temp), cpus_per_Queue,
			      NR_CPUS); */ /*Use this cpumask_shift_left(&cpumask_xps, &cpumask_temp,cpus_per_Queue); check this */
		
		bitmap_set(cpumask_xps->bits, tmp_cpus, cpus_per_Queue);	
		//memcpy(cpumask_temp, cpumask_xps, sizeof(*cpumask_xps)); /* Use this: cpumask_copy(&cpumask_temp, &cpumask_xps); */
		
		printk("mask[0] = 0x%lx, mask[1] = 0x%lx, mask[2] = 0x%lx, mask[3] = 0x%lx, mask[4] = 0x%lx, mask[5] = 0x%lx, mask[6] = 0x%lx, mask[7] = 0x%lx \n\n", cpumask_xps->bits[0], cpumask_xps->bits[1], cpumask_xps->bits[2], cpumask_xps->bits[3], cpumask_xps->bits[4], cpumask_xps->bits[5], cpumask_xps->bits[6], cpumask_xps->bits[7]);	
		
		#if 0
		err = netif_set_xps_queue(netdev, cpu_mask), i);

		if (err) {
			dev_err(&ssnic_priv->pdev->dev, "Unable to set XPS Map\\n");
			/* goto irq_clean; */
		}
		
		#endif	      
		
			     
		 bitmap_zero(cpumask_xps->bits, NR_CPUS); /* cpumask_clear(&cpumask_xps); Use this to clear actual memory used */
		 tmp_cpus += cpus_per_Queue;
	}
	
	/* Enable them when doing dynamic memory allocation */
	
	/* free_cpumask_var(cpumask_xps);
	free_cpumask_var(cpumask_temp); */ 
	
	i = nr_cpumask_bits;
	printk("nr_cpumask_bits =%d\n",i);
		
	
		
		/* Pass cpumask_xps to xps function */
		
	#if 0
		for (j = 0; j < CPUS_PER_QUEUE; j++ ) {
		
			
			cpumask_set_cpu(cpu++,&cpumask_xps);
		
		}
		
	#endif

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
