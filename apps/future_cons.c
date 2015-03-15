#include<future.h>


uint future_cons(future *fut) {
  int i, status;
  status = future_get(fut, &i);
if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }

        irqmask im;
	im=disable();
  printf("it produced %d\n", i);
  restore(im);
	return OK;
}
