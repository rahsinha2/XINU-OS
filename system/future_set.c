#include<future.h>

syscall future_set(future* var, int* val)
{
	if (var->state == FUTURE_EMPTY || var->state == FUTURE_WAITING)	{
		var->value = *val;
		var->state = FUTURE_VALID;
	}
	
	else if (var->state == FUTURE_VALID)	{
		return SYSERR;
	}
}
