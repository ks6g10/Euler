/*Euler problem 5
  Copyright (C) 2012 Kim Svensson ks@linux.com
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#define NUM_THREADS     5
typedef uint32_t myint;

static const myint max = 20;
static myint min = UINT32_MAX;
static myint multi;
static myint acount;
static myint * usearray;
myint test(myint count);

/*Took a wrong approach on this one, 
  should have calculated prime factors of it,
  and then multiplied the highes prime factors
*/


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
