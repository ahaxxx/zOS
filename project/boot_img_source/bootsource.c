void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
/*-----以上函数存在于naskfunc.nas中-----*/

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
/*-----以上函数存在于bootsource.c-------*/

void HariMain(void)
{	
	int i;
	char *p;

	init_palette();

	p = (char *) 0xa0000;

	for(i = 0; i <= 0xaffff; i++){
		p[i] = i & 0x0f;
	}
	while(1){
		io_hlt();
	}
}

void init_palette(void){
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
