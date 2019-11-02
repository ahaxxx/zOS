#include<stdio.h>
#include"bootsource.h"

void HariMain(void)
{	
	struct Bootinfo *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40],mouse[256];
	int cx,cy;

	init_gdtidt();
	init_pic();
	io_sti(); 
	
	init_palette();
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
	cx = (binfo->scrnx - 16) / 2;
	cy = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mouse, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, cx, cy, mouse, 16);
	sprintf(s, "(%d, %d)", cx, cy);
	putfonts(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef); 
	while(1){
		io_hlt();
	}
}

