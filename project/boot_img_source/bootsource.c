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
/*-----以上函数存在于bootsource.c-------*/

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

struct Bootinfo {
	//引导信息
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};

void HariMain(void)
{	
	struct Bootinfo *bootinfo;
	extern char hankaku[4096];

	init_palette();

	bootinfo = (struct Bootinfo *) 0x0ff0;

	init_screen(bootinfo->vram, bootinfo->scrnx, bootinfo->scrny);

	putfont8(bootinfo->vram, bootinfo->scrnx,  8, 8, COL8_FFFFFF, hankaku + 'H' * 16);
	putfont8(bootinfo->vram, bootinfo->scrnx, 16, 8, COL8_FFFFFF, hankaku + 'E' * 16);
	putfont8(bootinfo->vram, bootinfo->scrnx, 24, 8, COL8_FFFFFF, hankaku + 'L' * 16);
	putfont8(bootinfo->vram, bootinfo->scrnx, 32, 8, COL8_FFFFFF, hankaku + 'L' * 16);
	putfont8(bootinfo->vram, bootinfo->scrnx, 40, 8, COL8_FFFFFF, hankaku + 'O' * 16);
	putfont8(bootinfo->vram, bootinfo->scrnx, 56, 8, COL8_FFFFFF, hankaku + 'z' * 16);
	putfont8(bootinfo->vram, bootinfo->scrnx, 64, 8, COL8_FFFFFF, hankaku + 'O' * 16);
	putfont8(bootinfo->vram, bootinfo->scrnx, 72, 8, COL8_FFFFFF, hankaku + 'S' * 16);
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