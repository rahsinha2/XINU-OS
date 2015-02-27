#include<future.h>

syscall future_free(future* var)
{
	int ret = 0;
	irqmask im;
	im = disable();
	ret = memfree(var, sizeof(future));
	var = NULL;
	restore(im);
	if (SYSERR == ret)
		return SYSERR;
	return OK;
}
