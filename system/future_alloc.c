#include<future.h>

future* future_alloc(int future_flags)
{
	irqmask im;
	im = disable();
	future* var = (future*) memget(sizeof(future));
	if (SYSERR == (int)var)	{
		restore(im);
		return NULL;
	}
	else	{ 
		var->flag = future_flags;
		var->state = FUTURE_EMPTY;
		restore(im);
		return var;
	}
}
