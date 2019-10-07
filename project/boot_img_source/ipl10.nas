; zOS 
; TAB=4

CYLS	EQU		10

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

readloop:						; 读取至18扇区
		MOV		SI,0			; 此寄存器用于记录失败次数

retry:
		MOV		AH,0x02			; AH=0x02 载入磁盘
		MOV		AL,1			; 1个扇区
		MOV 	BX,0
		MOV 	DL,0x00			; A驱动器
		INT		0x13			; 调用磁盘BIOS
		JNC		next			; 如果没有报错就执行next
		ADD		SI,1			; 失败次数+1
		CMP		SI,5			; 判断失败次数是否为5
		JAE		error			; SI>=5 时报错 JAE的意思是>=
		MOV		AH,0x00
		MOV		DL,0x00			; A驱动器
		INT		0x13			; 驱动器重置
		JMP		retry

next:
		MOV		AX,ES			; 内存地址后移0x200
		ADD		AX,0x0020
		MOV		ES,AX			
		ADD		CL,1			; CL用来记录移动的扇区次数
		CMP		CL,18			; 比较CL和18
		JBE		readloop		; CL <= 18时继续读盘
		MOV		CL,1
		ADD		DH,1
		CMP		DH,2
		JB 		readloop		; DH < 2 跳转readloop
		MOV		DH,0
		ADD		CH,1
		CMP		CH,CYLS
		JB		readloop		; CH < CYLS 跳转readloop

		MOV		[0x0ff0],CH		; 读取zOS.sys
		JMP		0xc200

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
		
fin:
		HLT						; CPU待命
		JMP		fin				; 无限循环

msg:
		DB		0x0a, 0x0a		; 换行
		DB		"load error...zOS is in development."
		DB		0x0a			; 换行
		DB		"Last updata is 2019.10.04 14:52"
		DB		0

		RESB	0x7dfe-$		; 填充0x00直到0x7dfe

		DB		0x55, 0xaa