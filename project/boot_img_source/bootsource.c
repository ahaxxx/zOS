#include<stdio.h>
#include"bootsource.h"

void HariMain(void)
{	
	struct Bootinfo *bootinfo;
	char s1[40],s2[40],mouse[256];
	int cx,cy;

	init_palette();
	bootinfo = (struct Bootinfo *) 0x0ff0;
	init_screen(bootinfo->vram, bootinfo->scrnx, bootinfo->scrny);
	
	cx = (bootinfo->scrnx - 16) / 2;
	cy = (bootinfo->scrny - 28 - 16) / 2;

	putfonts(bootinfo->vram, bootinfo->scrnx,  8, 8, COL8_FFFFFF, "hello zOS");
	putfonts(bootinfo->vram, bootinfo->scrnx,  8, 31, COL8_FFFFFF, "2019.10.19 updata");
	sprintf(s1, "x resolution is %d", bootinfo->scrnx);
	sprintf(s2, "y resolution is %d", bootinfo->scrny);
	putfonts(bootinfo->vram, bootinfo->scrnx, 16, 64, COL8_FFFFFF, s1);
	putfonts(bootinfo->vram, bootinfo->scrnx, 16, 96, COL8_FFFFFF, s2);
	init_mouse_cursor8(mouse, COL8_008484);
	putblock8_8(bootinfo->vram, bootinfo->scrnx, 16, 16, cx, cy, mouse, 16);
	while(1){
		io_hlt();
	}
}

