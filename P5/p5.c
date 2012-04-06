#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#define NUM_THREADS     1
typedef uint32_t myint;

static const myint max = 20;
static myint min = UINT32_MAX;
static multi;
static myint acount;
static myint * usearray;
//20 40 60 80 100 120
myint test(myint count);
void *run(void * count)
{
	register myint icount =  *((myint *) count);
	while(__builtin_expect(test(icount),1))
	{
		if(__builtin_expect(icount>min,0))
			return;
		icount += multi;
	}
	if(icount < min)
		min = icount;
	else
		return;

}

myint test(myint count) 
{
//	myint scount = count;
	register myint i;		
	/* i = 1, for skip of biggest divisor as we multiply by it during increment of icount*/
	for( i = 1;i<acount; i++)
	{
		if(count%usearray[i])
			return 1;
	}

	return 0;
}

void main() {
	pthread_t threads[NUM_THREADS];
       	myint thread_args[NUM_THREADS];	
	myint rc;
	myint * remnumbers = calloc(max+1,sizeof(myint));
	myint small = 2;
	myint big = max;
	myint n;
	myint i;


	multi =	max*NUM_THREADS;
	usearray = calloc(max,sizeof(myint ));

	for(i = max; i>small && (remnumbers[i] == 0);i--)
	{
		for(n = small; n < i;n++)
		{
			if(!(i%n))
				remnumbers[n]++;

		}
		*(usearray+acount) = i;
		acount++;
		printf("count %d\n",i);
	}


	free(remnumbers);
//	return;
	for (i=0; i<NUM_THREADS; ++i) 
	{
		thread_args[i] = (i+1)*max;
		rc = pthread_create(&threads[i], NULL, run, (void *) &thread_args[i]);
		assert(0 == rc);
	}
	for (i=0; i<NUM_THREADS; ++i) {
		rc = pthread_join(threads[i], NULL);
		assert(0 == rc);
	}
	printf("min number %u\n",min);
	exit(EXIT_SUCCESS);
}
