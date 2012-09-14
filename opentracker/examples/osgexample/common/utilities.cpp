#ifdef WIN32
#include <sys/timeb.h> 
#else 
#include <time.h> 
#endif
#include <stdlib.h>
#include "utilities.h"


//
// Random number generator 
// 
void randInit()
{
#ifdef WIN32
	struct timeb tb;
	ftime(&tb);
	srand(tb.millitm);
#else
	/* Init random generators */
	struct timeval t;
	gettimeofday(&t, NULL);
	srand((unsigned int) t.tv_usec);
#endif
}


float
RandFloat(void)
{
  return (float)(drand48());
}

int
RandInt(int i)
{
  return (int)(i*(drand48()));
}