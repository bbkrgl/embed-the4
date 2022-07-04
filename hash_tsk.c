#include "common.h"
#include "hash.h"

/**
 * Hash task
 */

TASK(HASH_TSK)
{
	while (1) {
		WaitEvent(HASH_GO);
		ClearEvent(HASH_GO);
		
		compute_hash(alert_string, hash);
		hash_done = 1;
	}

	TerminateTask();
}