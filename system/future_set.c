#include<future.h>

syscall future_set(future* var, int* val)
{
	irqmask im;
	im = disable();
	if (var->state == FUTURE_WAITING && var->flag == FUTURE_EXCLUSIVE)	{
		var->value = *val;
		var->state = FUTURE_VALID;
		resume(var->tid);
		restore(im);
		return OK;
	}
	
	if (var->state == FUTURE_EMPTY && (var->flag == FUTURE_EXCLUSIVE || var->flag == FUTURE_SHARED))	{
		var->value = *val;
		var->state = FUTURE_VALID;
		restore(im);
		return OK;
	}
	
	if (var->state == FUTURE_VALID && (var->flag == FUTURE_EXCLUSIVE || var->flag == FUTURE_SHARED))	{
		restore(im);
		return SYSERR;
	}
	
	if (var->state == FUTURE_WAITING && var->flag == FUTURE_SHARED)	{
		var->value = *val;
		var->state = FUTURE_VALID;
		if ((var->head_getq) > -1 && (var->tail_getq) >-1 && (var->head_getq)<QSIZE && (var->tail_getq)<QSIZE)	{
		int counter = var->head_getq;
			while(counter<=(var->tail_getq))	{
				resume(var->get_queue[counter]);
				counter++;
			}
		
		}	

		restore(im);
		return OK;
	}
	
	if(var->flag == FUTURE_QUEUE) {
		if(isQempty(&(var->head_getq),&(var->tail_getq)) && isQempty(&(var->head_setq),&(var->tail_setq)) )    {
                        *val = var->value;      
                        var->state = FUTURE_VALID;
			enQ(var->set_queue, gettid(), &(var->head_setq), &(var->tail_setq));
                        suspend(gettid()); // BOTH EMPTY
                        resume(var->get_queue[var->head_getq]);
                        printf("");
			restore(im);
			return OK;
		}
			//only setq empty	
		if(!isQempty(&(var->head_getq),&(var->tail_getq)) && isQempty(&(var->head_setq),&(var->tail_setq)) )    {
                        *val = var->value;     
                        var->state = FUTURE_VALID;
                        resume(var->get_queue[var->head_getq]);
			printf("");
                        deQ(var->get_queue, &(var->head_getq), &(var->tail_getq));
			restore(im);
			return OK;
		}
			//only getq empty
		if(isQempty(&(var->head_getq),&(var->tail_getq)) && !isQempty(&(var->head_setq),&(var->tail_setq)) )    {
			enQ(var->set_queue, gettid(), &(var->head_setq), &(var->tail_setq));
                        suspend(gettid()); 
                        resume(var->get_queue[var->head_getq]);
			printf("");
                        *val = var->value;     
                        var->state = FUTURE_VALID;
			restore(im);
			return OK;
		}
	
		if(!isQempty(&(var->head_getq),&(var->tail_getq)) && !isQempty(&(var->head_setq),&(var->tail_setq)) )    {
			enQ(var->set_queue, gettid(), &(var->head_setq), &(var->tail_setq));
                        suspend(gettid()); // BOTH not EMPTY
                        printf("");
                        resume(var->get_queue[var->head_getq]);
                        *val = var->value;      
                        var->state = FUTURE_VALID;
			restore(im);
			return OK;
		}
	}
}	
