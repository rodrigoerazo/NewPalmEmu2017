#ifndef MINIFUNC
#define MINIFUNC

#include "types.h"

#ifdef _DBGMODE
#define DEBUG(x) x
#define dbgprintf	printf
#else
#define DEBUG(x)
#define dbgprintf(x,...)
#endif

#define TEMPHACK

//meaningful words
#define findme 0
#define notrequired 0
#define ISSPECIAL(x) ((x) > 0xFFFFFF00)
#define INTERCEPT 0xFFFFFFFF


//basic misc functions

//this needs to go
#define inc_for(var,cmp) for(var = 0;var < cmp;var++)


#define IS_EVEN(x)		(((x) & 1) == 0)

#define nextboundry(x)	(((x) + 1) & ~0x1)

#define bit(x)			(1 << (x))

inline long smallest(long val1,long val2){
	return val1 < val2 ? val1 : val2;
}

inline long largest(long val1,long val2){
	return val1 > val2 ? val1 : val2;
}

inline long absdiffrence(long var1,long var2){
	return largest(var1,var2) - smallest(var1,var2);
}

#endif