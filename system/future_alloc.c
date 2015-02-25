#include<future.h>

future* future_alloc(int future_flags)
{
	future* var = (future*) memget(sizeof(future));
	if (SYSERR == (int)var)
		return NULL;
	else	{ 
		var->flag = future_flags;
		var->state = FUTURE_EMPTY;
		return var;
	}
}
