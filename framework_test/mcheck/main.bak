/* Multithreaded test: each thread allocates and registers tagged allocations.
   Some allocations are freed, others intentionally leaked. Program forces plain-text detailed output. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#define THREAD_COUNT 4
#define ALLOCS_PER_THREAD 8

/* C API provided by the C++ memcheck module (defined in leaks.cpp) */
extern void memcheck_start(void);
extern void memcheck_stop(void);
extern void memcheck_report(void);

/* explicit registration API (no malloc/free interception) */
extern void memcheck_register_alloc_tag(void *p, size_t size, const char *tag);
extern void memcheck_register_free(void *p);
/* new: convenience free that unregisters then calls real free */
extern void memcheck_free(void *p);

static void *worker(void *arg)
{
	int tid = (int)(intptr_t)arg;
	char tagbuf[128];

	for (int i = 0; i < ALLOCS_PER_THREAD; ++i)
	{
		snprintf(tagbuf, sizeof(tagbuf), "thread-%d/alloc-%d", tid, i);
		/* allocate and register */
		size_t len = 64 + (i % 16);
		char *p = malloc(len);
		if (!p)
			continue;
		snprintf(p, len, "thread %d allocation %d", tid, i);
		memcheck_register_alloc_tag(p, len, tagbuf);
		/* print pointer for correlation */
		printf("[tid %d] allocated %p tag=%s\n", tid, (void *)p, tagbuf);

		/* previously leaked half the allocations; now always unregister & free */
		memcheck_free(p);
		printf("[tid %d] freed    %p\n", tid, (void *)p);

		/* small sleep to mix allocation order */
		usleep(1000 * (tid + 1));
	}
	return NULL;
}

__attribute__((weak))
int main(void)
{
	/* prefer plain text detailed report for quick inspection */
	setenv("MEMCHECK_JSON", "0", 1);
	/* show more frames and skip the top internal frame */
	setenv("MEMCHECK_FRAMES", "12", 1);
	setenv("MEMCHECK_SKIP", "1", 1);

	/* start tracking allocations */
	memcheck_start();

	/* spawn worker threads */
	pthread_t th[THREAD_COUNT];
	for (int t = 0; t < THREAD_COUNT; ++t)
		pthread_create(&th[t], NULL, worker, (void *)(intptr_t)t);

	for (int t = 0; t < THREAD_COUNT; ++t)
		pthread_join(th[t], NULL);

	/* extra deterministic allocations with tags */
	char *x1 = malloc(128);
	strcpy(x1, "extra-leak-1");
	memcheck_register_alloc_tag(x1, 128, "extra/leak-1");
	printf("[main] extra allocated %p tag=%s\n", (void *)x1, "extra/leak-1");

	char *x2 = malloc(200);
	strcpy(x2, "extra-leak-2");
	memcheck_register_alloc_tag(x2, 200, "extra/leak-2");
	printf("[main] extra allocated %p tag=%s\n", (void *)x2, "extra/leak-2");

	/* free both deterministic allocations */
	//memcheck_free(x1);
	printf("[main] freed    %p (extra/leak-1)\n", (void *)x1);

	/* do not unregister x2: leave it registered and unfreed to simulate a leak */
	/* to simulate a leak: do nothing here (keep x2 registered and unfreed) */
	memcheck_free(x2);
	printf("[main] leaked   %p (extra/leak-2)\n", (void *)x2);

	/* stop tracking and print plain-text report (should report no leaks) */
	memcheck_stop();
	memcheck_report();

	return 0;
}
