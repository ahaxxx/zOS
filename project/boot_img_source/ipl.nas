; zOS 
; TAB=4

		ORG		0x7c00			; 程序加载位置


; 以下为FAT12引导
		JMP		entry

		DB		0x90
		DB		"zOS__IPL"		; 扇区名称(8字符)
		DW		512				; 扇区大小（512字节）
		DB		1				; cluster的个数(必须为1)
		DW		1				; FAT起始位置（一般为1）
		DB		2				; FAT个数（必须为2）
		DW		224				; 根目录大小（一般为224）
		DW		2880			; 磁盘大小（必须是2880扇区）
		DB		0xf0			; 磁盘种类（必须为0xf0）
		DW		9				; FAT长度(必须为9)
		DW		18				; 1个磁道有几个扇区（18）
		DW		2				; 磁头数（必须为2）
		DD		0				; 不使用分区（必为00）
		DD		2880			; 重写磁道大小
		DB		0,0,0x29		; 
		DD		0xffffffff		; 卷号
		DB		"zOS-OS     "	; 磁盘名
		DB		"FAT12   "		; 硬盘格式
		RESB	18				; 空出18字符

; 程序主体

entry:
		MOV		AX,0			; 初始化寄存器
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
; 读取磁盘
		MOV		AX,0x0820
		MOV		ES,AX			
		MOV		CH,0			; 柱面0
		MOV		DH,0			; 磁头2
		MOV		CL,2			; 扇区2

		MOV		AH,0x02			; AH=0x02 载入磁盘
		MOV		AL,1			; 1个扇区
		MOV 	BX,0
		MOV 	DL,0x00			; A驱动器
		INT		0x13			; 调用磁盘BIOS
		JC		error

fin:
		HLT						; CPU待命
		JMP		fin				; 无限循环

error:
		MOV		SI,msg

putloop:
		MOV		AL,[SI]
		ADD		SI,1			; SI+1
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 显示文字
		MOV		BX,15			; 字符颜色
		INT		0x10			; 调用BIOS命令
		JMP		putloop

msg:
		DB		0x0a, 0x0a		; 换行
		DB		"zOS is in development."
		DB		0x0a			; 换行
		DB		"Last updata is 2019.10.02 19:39"
		DB		0

		RESB	0x7dfe-$		; 填充0x00直到0x7dfe

		DB		0x55, 0xaa
