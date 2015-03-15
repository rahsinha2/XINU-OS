#include<future.h>

syscall future_get(future* var, int* val)
{
	irqmask im;
	im = disable();

	if (var->state == FUTURE_EMPTY && var->flag == FUTURE_EXCLUSIVE)	{ 
		var->tid = gettid();
		var->state = FUTURE_WAITING;
		suspend(gettid());
		printf("");
		*val = var->value;	
		restore(im);
		return OK;
	}	
	if (var->state == FUTURE_VALID && var->flag == FUTURE_EXCLUSIVE)	{ 
		var->tid = gettid();
		printf("");
		*val = var->value;	// or int *
		restore(im);
		return OK;
	}	
	if (var->state == FUTURE_WAITING && var->flag == FUTURE_EXCLUSIVE)	{
		restore(im);
		return SYSERR;	
	}
	if (var->state == FUTURE_VALID && var->flag == FUTURE_SHARED)	{ 
		var->tid = gettid();
		*val = var->value;	// or int *
		var->state = FUTURE_VALID;
		restore(im);
		return OK;
	}	
	if ((var->state == FUTURE_EMPTY || var->state == FUTURE_WAITING) && var->flag == FUTURE_SHARED)	{ 
		var->tid = gettid();
		var->state = FUTURE_WAITING;
		enQ(var->get_queue, gettid(), &(var->head_getq), &(var->tail_getq));
		suspend(gettid());
		printf("");
		*val = var->value;	// or int *
		restore(im);
		return OK;
	}
	
	if(var->flag == FUTURE_QUEUE)	{
		if(isQempty(&(var->head_getq),&(var->tail_getq)) && isQempty(&(var->head_setq),&(var->tail_setq)) )	{
			enQ(var->get_queue, gettid(), &(var->head_getq), &(var->tail_getq));
			var->state = FUTURE_WAITING;
			var->tid = gettid();
			suspend(gettid()); // BOTH EMPTY
			printf("");
			*val = var->value;	
			restore(im);
			return OK;
		}
					//GETQ EMPTY  && SETQ !EMPTY
		if(isQempty(&(var->head_getq),&(var->tail_getq)) && !isQempty(&(var->head_setq),&(var->tail_setq)) )	{
			resume(var->set_queue[var->tail_setq]);
			deQ(var->set_queue, &(var->head_setq), &(var->tail_setq));
			var->state = FUTURE_WAITING;
			var->tid = gettid();
			printf("");
			*val = var->value;	
			restore(im);
			return OK;
			
		}			// BOTH ARE NOT EMPTY
		if(!isQempty(&(var->head_getq),&(var->tail_getq)) && !isQempty(&(var->head_setq),&(var->tail_setq)) )	{
			enQ(var->get_queue, gettid(), &(var->head_getq), &(var->tail_getq));
			var->state = FUTURE_WAITING;
			var->tid = gettid();
			suspend(gettid()); 
			printf("");
			*val = var->value;
			restore(im);
			return OK;
		}
	
					//GET !EMPTY   and SETQ EMPTY	
		if(!isQempty(&(var->head_getq),&(var->tail_getq)) && isQempty(&(var->head_setq),&(var->tail_setq)) )	{
			enQ(var->get_queue, gettid(), &(var->head_getq), &(var->tail_getq));
			suspend(gettid()); 
			resume(var->set_queue[var->tail_setq]);
			deQ(var->set_queue, &(var->head_setq), &(var->tail_setq));
			printf("");
			var->state = FUTURE_WAITING;
			var->tid = gettid();
			*val = var->value;	
			restore(im);
			return OK;
			
		}

	}

}	
