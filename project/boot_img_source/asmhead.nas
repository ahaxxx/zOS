; zOS boot asm
; TAB=4

[INSTRSET "i486p"]

VBEMODE	EQU		0x105			;1280 x 1024 x 8

;	0x100 :  640 x  400 x 8bit
;	0x101 :  640 x  480 x 8bit
;	0x103 :  800 x  600 x 8bit
;	0x105 : 1024 x  768 x 8bit
;	0x107 : 1280 x 1024 x 8bit

BOTPAK	EQU		0x00280000		; bootsource
DSKCAC	EQU		0x00100000		; 磁盘缓存位置
DSKCAC0	EQU		0x00008000		; 磁盘缓存位置(实时模式)

; BOOT_INFO设置
CYLS	EQU		0x0ff0			; 启动扇区设置
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; 显示模式设置
SCRNX	EQU		0x0ff4			; x分辨率
SCRNY	EQU		0x0ff6			; y分辨率
VRAM	EQU		0x0ff8			; 开始地址

		ORG		0xc200			; 程序装载地址

; VBE存在确认

		MOV		AX,0x9000
		MOV		ES,AX
		MOV		DI,0
		MOV		AX,0x4f00
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320

; VBE版本检查

		MOV		AX,[ES:DI+4]
		CMP		AX,0x0200
		JB		scrn320	

; 取得画面模式信息

		MOV		CX,VBEMODE
		MOV		AX,0x4f01
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320

; 确认画面模式信息

		CMP		BYTE [ES:DI+0x19],8
		JNE		scrn320
		CMP		BYTE [ES:DI+0x1b],4
		JNE		scrn320
		MOV		AX,[ES:DI+0x00]
		AND		AX,0x0080
		JZ		scrn320	

; 画面模式切换

		MOV		BX,VBEMODE+0x4000
		MOV		AX,0x4f02
		INT		0x10
		MOV		BYTE [VMODE],8
		MOV		AX,[ES:DI+0x12]
		MOV		[SCRNX],AX
		MOV		AX,[ES:DI+0x14]
		MOV		[SCRNY],AX
		MOV		EAX,[ES:DI+0x28]
		MOV		[VRAM],EAX
		JMP		keystatus

scrn320:
		MOV		AL,0x13			
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8	
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

keystatus:
		MOV		AH,0x02
		INT		0x16 			; keyboard BIOS
		MOV		[LEDS],AL

; 使PIC不接受一切中断
; 根据AT兼容机初始化PIC

		MOV		AL,0xff
		OUT		0x21,AL
		NOP						
		OUT		0xa1,AL

		CLI						; CPU禁止中断

; 设定A20 gate，使CPU能够访问1MB以上的存储器

		CALL	waitkbdout
		MOV		AL,0xd1
		OUT		0x64,AL
		CALL	waitkbdout
		MOV		AL,0xdf			; enable A20
		OUT		0x60,AL
		CALL	waitkbdout

; 保护模式转移

[INSTRSET "i486p"]				; 486の命令まで使いたいという記述

		LGDT	[GDTR0]			; 暫定GDTを設定
		MOV		EAX,CR0
		AND		EAX,0x7fffffff	; bit31を0にする（ページング禁止のため）
		OR		EAX,0x00000001	; bit0を1にする（プロテクトモード移行のため）
		MOV		CR0,EAX
		JMP		pipelineflush
pipelineflush:
		MOV		AX,1*8			;  読み書き可能セグメント32bit
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX

; bootsource传送
;C语言写法 memcpy(bootsource , BOTPAK , 512*1024/4)
		MOV		ESI,bootsource	; 传送源
		MOV		EDI,BOTPAK		; 传送目标
		MOV		ECX,512*1024/4
		CALL	memcpy

; 硬盘数据复位

; 启动扇区
; C语言写法 memcpy(0x7c00 , DSKCAC , 512/4)
		MOV		ESI,0x7c00		; 传送源
		MOV		EDI,DSKCAC		; 传送目标
		MOV		ECX,512/4
		CALL	memcpy

; 剩余部分
; C语言写法 memcpy(DSKCAC0+512 ,DSKCAC+512 ,cyls * 512*18*2/4 - 512/4)
		MOV		ESI,DSKCAC0+512	; 传送源
		MOV		EDI,DSKCAC+512	; 传送目标
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4	; 柱面数转换为字节数/4
		SUB		ECX,512/4		; 减去IPL
		CALL	memcpy

; asmhead功能结束

; bootsource的启动

		MOV		EBX,BOTPAK
		MOV		ECX,[EBX+16]
		ADD		ECX,3			; ECX += 3;
		SHR		ECX,2			; ECX /= 4;
		JZ		skip			; 没有要传送的东西
		MOV		ESI,[EBX+20]	; 传送源
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]	; 传送目的地
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]	; 栈初始值
		JMP		DWORD 2*8:0x0000001b

waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout		; 如果相加结果不是0就跳转waitkbdout
		RET

memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy			; 如果相减结果不是0就跳转memcpy
		RET

		ALIGNB	16
GDT0:
		RESB	8				; NULL selector
		DW		0xffff,0x0000,0x9200,0x00cf	; 可以读写的段 32bit
		DW		0xffff,0x0000,0x9a28,0x0047	; 可以执行的段 32bit （bootsource）

		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0

		ALIGNB	16
bootsource:
