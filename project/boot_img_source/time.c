#include "bootsource.h"
#define PIL_CTRL		0x0043
#define PIL_CNT0		0x0040

struct TIMECTL timectl;

void init_pit(void){
	io_out8(PIL_CTRL,0x34);
	io_out8(PIL_CNT0,0x9c);
	io_out8(PIL_CNT0,0x2e);
	return;
}

void inthandler20(int *esp){
	io_out8(PIC1_OCW2,0x60);
	return;
}
