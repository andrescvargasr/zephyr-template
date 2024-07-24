// Parameters
#include "params.h"

// Threads
#ifdef THD_0
#include "thd_0.h"
#endif // THD_0
#ifdef THD_1
#include "thd_1.h"
#endif // THD_1

// Define stack area used by workqueue thread
K_THREAD_STACK_DEFINE(my_stack_area, WORQ_THREAD_STACK_SIZE);

K_THREAD_DEFINE(thread0_id, THD_STACKSIZE, thread0, NULL, NULL, NULL,
		THREAD0_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, THD_STACKSIZE, thread1, NULL, NULL, NULL,
		THREAD1_PRIORITY, 0, 0);