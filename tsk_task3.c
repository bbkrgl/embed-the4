#include "common.h"
#include "hash.h"

TASK(TASK3)
{
	while (1) {
		WaitEvent(HASH_GO);
		ClearEvent(HASH_GO);
		
		compute_hash(alert_string, hash);
		hash_done = 1;
		money_opportunity = 0;
	}

	TerminateTask();
}