#include"bootsource.h"
#define FLAGS_OVERRUN		0x0001

void fifo_init(struct FIFO *fifo, int size,unsigned char *buf){
	fifo->size = size;
	fifo->buf = buf;
	fifo->free = size;
	fifo->flags = 0;
	fifo->p = 0;
	fifo->q = 0;
	return;
}

int fifo_put(struct FIFO *fifo,unsigned char data){
	if(fifo->free == 0){
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->p] = data;
	fifo->p++;
	if (fifo->p == fifo->size){
		fifo->p = 0;
	}
	fifo->free--;
	return 0;
}

int fifo_get(struct FIFO *fifo){
	int data;
	if(fifo->free == fifo->size){
		return -1;
	}
	data = fifo->buf[fifo->q];
	fifo->q++;
	if (fifo->q == fifo->size){
		fifo->q = 0;
	}
	fifo->free++;
	return data;
}

int fifo_status(struct FIFO *fifo){
	return fifo->size - fifo->free;
}
