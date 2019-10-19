#include<stdio.h>

/*-----------颜色的声明-----------*/
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
/*-----以上函数存在于naskfunc.nas中-----*/

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
/*-----以上函数存在于bootsource.c-------*/



struct Bootinfo {
	//引导信息
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};

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

void init_palette(void){
	//RGB颜色初始化函数，定义了RGB色值
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:黑 */
		0xff, 0x00, 0x00,	/*  1:红 */
		0x00, 0xff, 0x00,	/*  2:绿 */
		0xff, 0xff, 0x00,	/*  3:黄 */
		0x00, 0x00, 0xff,	/*  4:蓝 */
		0xff, 0x00, 0xff,	/*  5:紫 */
		0x00, 0xff, 0xff,	/*  6:浅蓝 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xc6, 0xc6, 0xc6,	/*  8:灰 */
		0x84, 0x00, 0x00,	/*  9:暗红 */
		0x00, 0x84, 0x00,	/* 10:暗绿 */
		0x84, 0x84, 0x00,	/* 11:暗黄 */
		0x00, 0x00, 0x84,	/* 12:暗蓝 */
		0x84, 0x00, 0x84,	/* 13:暗紫 */
		0x00, 0x84, 0x84,	/* 14:暗浅蓝 */
		0x84, 0x84, 0x84	/* 15:暗灰 */
	};
	set_palette(0,15,table_rgb);
	return;
}

void set_palette(int start,int end,unsigned char *rgb){
	int i,eflags;
	eflags = io_load_eflags();
	io_cli();
	io_out8(0x03c8, start);
	for(i = start; i <= end; i++){
		io_out8(0x03c9,rgb[0] / 4);
		io_out8(0x03c9,rgb[1] / 4);
		io_out8(0x03c9,rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);
	return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char color, int x0, int y0, int x1, int y1){
	//填充一个以(x0,y0)为基准点，横向为x1-x0，纵向为y1-y0的矩形
	int x,y;
	for(y = y0; y <= y1; y++){
		for(x = x0; x<= x1; x++){
			vram[y * xsize + x] = color;
		}
	}
	return;
}

void init_screen(char *vram, int xsize, int ysize){
	boxfill8(vram, xsize, COL8_840084,   0,         0,          xsize -  1, ysize - 29);
	boxfill8(vram, xsize, COL8_C6C6C6,   0,         ysize - 28, xsize -  1, ysize - 28);
	boxfill8(vram, xsize, COL8_FFFFFF,   0,         ysize - 27, xsize -  1, ysize - 27);
	boxfill8(vram, xsize, COL8_C6C6C6,   0,         ysize - 26, xsize -  1, ysize -  1);
	//桌面图案
	boxfill8(vram, xsize, COL8_FF0000,  20,  				20, 	   150, 	   120);
	boxfill8(vram, xsize, COL8_00FF00,  70,  				50, 	   180, 	   140);
	boxfill8(vram, xsize, COL8_0000FF, 120,  				80, 	   210, 	   150);
	//以下为左下角按钮
	boxfill8(vram, xsize, COL8_FFFFFF,  3,         ysize - 24, 59,         ysize - 24);
	boxfill8(vram, xsize, COL8_FFFFFF,  2,         ysize - 24,  2,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484,  3,         ysize -  4, 59,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484, 59,         ysize - 23, 59,         ysize -  5);
	boxfill8(vram, xsize, COL8_000000,  2,         ysize -  3, 59,         ysize -  3);
	boxfill8(vram, xsize, COL8_000000, 60,         ysize - 24, 60,         ysize -  3);
	//以下为右下角按钮
	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize -  3, ysize - 24, xsize -  3, ysize -  3);
	return;
}

void putfont8(char *vram, int xsize, int x, int y, char color, char *font){
	int i;
	char *p, d;
	for(i = 0; i < 16; i++){
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = color; }
		if ((d & 0x40) != 0) { p[1] = color; }
		if ((d & 0x20) != 0) { p[2] = color; }
		if ((d & 0x10) != 0) { p[3] = color; }
		if ((d & 0x08) != 0) { p[4] = color; }
		if ((d & 0x04) != 0) { p[5] = color; }
		if ((d & 0x02) != 0) { p[6] = color; }
		if ((d & 0x01) != 0) { p[7] = color; }
	}
	return;
}

void putfonts(char *vram, int xsize, int x, int y, char c, unsigned char *s){
	extern char hankaku[4096];
	for(; *s != 0x00; s++){
		putfont8(vram,xsize,x,y,c,hankaku + *s * 16);
		x +=8 ;
	}
}

void init_mouse_cursor8(char *mouse, char bc){
	static char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}

void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize){
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}