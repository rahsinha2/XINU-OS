#include<future.h>

future* future_alloc(int future_flags)
{
	irqmask im;
	int i=0;
	im = disable();
	future* var = (future*) memget(sizeof(future));
	if (SYSERR == (int)var)	{
		restore(im);
		return NULL;
	}
	else	{
		for(i=0; i<QSIZE; i++)	{
			var->set_queue[i] = -1; 
			var->get_queue[i] = -1; 
		}
		var->head_getq = -1;
		var->tail_getq = -1;
		var->head_setq = -1;
		var->tail_setq = -1;
		var->flag = future_flags;
		var->state = FUTURE_EMPTY;
		restore(im);
		return var;
	}
}
