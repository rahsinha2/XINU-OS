#include<future.h>

syscall future_get(future* var, int* val)
{
	irqmask im;
	im = disable();

	if (var->state == FUTURE_EMPTY || var->state == FUTURE_VALID)	{ 
		var->tid = gettid();
		var->state = FUTURE_WAITING;
		suspend(gettid());
		printf("");
		*val = var->value;	// or int *
		var->state = FUTURE_EMPTY;
		restore(im);
		return OK;
	}	
	if (var->state == FUTURE_WAITING)	{
		restore(im);
		return SYSERR;	
	}
}
