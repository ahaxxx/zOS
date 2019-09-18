; zOS
; TAB=4

		ORG		0x7c00			; 程序加?位置

; 以下?FAT12引?

		DB		0xeb, 0x4e, 0x90
		DB		"zOS__IPL"		; 扇区名称(8字符)
		DW		512				; 扇区大小（512字?）
		DB		1				; cluster的个数(必??1)
		DW		1				; FAT起始位置（一般?1）
		DB		2				; FAT个数（必??2）
		DW		224				; 根目?大小（一般?224）
		DW		2880			; 磁?大小（必?是2880扇区）
		DB		0xf0			; 磁???（必??0xf0）
		DW		9				; FAT?度(必??9)
		DW		18				; 1个磁道有几个扇区（必??18）
		DW		2				; 磁?数（必??2）
		DD		0				; 不使用分区（必??0）
		DD		2880			; 重写磁?大小
		DB		0,0,0x29		; 
		DD		0xffffffff		; 卷?号
		DB		"zOS-OS     "	; 磁?名
		DB		"FAT12   "		; 硬?格式
		RESB	18				; 空出18字?

; 程序主体

entry:
		MOV		AX,0			; 初始化寄存器
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
		MOV		ES,AX

		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			; ?ST+1
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; ?示文字
		MOV		BX,15			; 字符?色
		INT		0x10			; ?用??BIOS
		JMP		putloop
fin:
		HLT						; CPU停止等待指令
		JMP		fin				; 死循?

msg:
		DB		0x0a, 0x0a		; ?行2次
		DB		"zOS is in development."
		DB		0x0a			; ?行
		DB		"Last updata is 2019.09.18 19:39"
		DB		0

		RESB	0x7dfe-$		

		DB		0x55, 0xaa

; ??区之外?出

		DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		RESB	4600
		DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		RESB	1469432
