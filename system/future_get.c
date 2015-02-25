#include<future.h>

syscall future_get(future* var, int* val)
{
	if (var->state == FUTURE_EMPTY || var->state == FUTURE_VALID)	{ 
		var->tid = gettid();
		var->state = FUTURE_WAITING;
		while (var->state != FUTURE_VALID) {
			printf("");
		}
		*val = var->value;	// or int *
		var->state = FUTURE_EMPTY;
		return OK;
	}	
	if (var->state == FUTURE_WAITING)
		return SYSERR;	

return OK;
}
