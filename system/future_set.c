#include<future.h>

syscall future_set(future* var, int* val)
{
	irqmask im;
	im = disable();
	if (var->state == FUTURE_WAITING)	{
		var->value = *val;
		var->state = FUTURE_VALID;
		resume(var->tid);
		restore(im);
		return OK;
	}
	
	if (var->state == FUTURE_EMPTY)	{
		var->value = *val;
		var->state = FUTURE_VALID;
		restore(im);
		return OK;
	}
	
	if (var->state == FUTURE_VALID)	{
		restore(im);
		return SYSERR;
	}
}
