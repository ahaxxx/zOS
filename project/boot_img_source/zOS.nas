; zOS
; TAB = 4
		ORG		0xc200		; 加载内存地址

		MOV		AL,0x13		; VGA显卡 320*200 8位色彩
		MOV		AH,0x00
		INT		0x10

fin:
		HLT
		JMP		fin
