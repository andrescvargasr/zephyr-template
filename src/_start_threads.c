// Parameters
#include "params.h"

// Threads
#ifdef THD_0
#include "thd_0.h"
#endif // THD_0
#ifdef THD_1
#include "thd_1.h"
#endif			  // THD_1
#ifdef THD_I2C /* CONFIG_I2C is directly from Zephyr */
#include "Thd_i2c.h"
#endif

#define THD_STACKSIZE 512 // Should always be a power of two.
#define THREAD0_PRIORITY 2
#define THREAD1_PRIORITY 3
#define THREAD_I2C_PRIORITY 4

/******************************************************************************/
/*                          Zephyr Workqueue Table	                          */
/******************************************************************************/
K_THREAD_STACK_DEFINE(my_stack_area, WORQ_THREAD_STACK_SIZE);

/******************************************************************************/
/*                          Zephyr Multithreading Table                       */
/******************************************************************************/
#ifdef THD_0 /* CONFIG_I2C is directly from Zephyr */
K_THREAD_DEFINE(thd0_id, THD_STACKSIZE, thread0, NULL, NULL, NULL,
				THREAD0_PRIORITY, 0, 0);
#endif	// End THD_0
#ifdef THD_1 /* CONFIG_I2C is directly from Zephyr */
K_THREAD_DEFINE(thd1_id, THD_STACKSIZE, thread1, NULL, NULL, NULL,
				THREAD1_PRIORITY, 0, 0);
#endif	// End THD_1
#ifdef THD_I2C /* CONFIG_I2C is directly from Zephyr */
K_THREAD_DEFINE(thd_i2c_id, THD_STACKSIZE, thread_i2c, NULL, NULL, NULL,
				THREAD_I2C_PRIORITY, 0, 0);
#endif	// End THD_I2C