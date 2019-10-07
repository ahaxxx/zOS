; zOS
; TAB = 4
; BOOT_INFO
CYLS	EQU		0x0ff0		; 启动区
LEDS	EQU		0x0ff1		; 键盘指示灯
VMODE	EQU		0x0ff2		; 显示颜色位数
SCRNX	EQU		0x0ff4		; x分辨率
SCRNY	EQU		0x0ff6		; y分辨率
VRAM	EQU		0x0ff8		; 图像缓冲区开始位置

		ORG		0xc200		; 加载内存地址

		MOV		AL,0x13		; VGA显卡 320*200 8位色彩
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8		; 8位色彩
		MOV		WORD [SCRNX],320	; 分辨率320*200	
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

; 获取键盘状态灯状态

		MOV		AH,0x02
		INT		0x16
		MOV		[LEDS],AL

; PIC初始化
		MOV		AL,0xff
		OUT		0x21,AL
		NOP		
		OUT		0xa1,AL
		CIL

fin:
		HLT
		JMP		fin
