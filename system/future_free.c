#include<future.h>

syscall future_free(future* var)
{
	return (memfree(var, sizeof(future)));
}
