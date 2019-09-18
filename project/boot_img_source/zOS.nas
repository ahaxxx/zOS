; zOS
; TAB=4

		ORG		0x7c00			; ๖ม?สu

; ศบ?FAT12๘?

		DB		0xeb, 0x4e, 0x90
		DB		"zOS__IPL"		; ๎ๆผฬ(8)
		DW		512				; ๎ๆๅฌi512?j
		DB		1				; clusterIข(K??1)
		DW		1				; FATNnสui๊ส?1j
		DB		2				; FATขiK??2j
		DW		224				; ชฺ?ๅฌi๊ส?224j
		DW		2880			; ฅ?ๅฌiK?ฅ2880๎ๆj
		DB		0xf0			; ฅ???iK??0xf0j
		DW		9				; FAT?x(K??9)
		DW		18				; 1ขฅนL{ข๎ๆiK??18j
		DW		2				; ฅ?iK??2j
		DD		0				; sgpชๆiK??0j
		DD		2880			; dสฅ?ๅฌ
		DB		0,0,0x29		; 
		DD		0xffffffff		; ษ?
		DB		"zOS-OS     "	; ฅ?ผ
		DB		"FAT12   "		; d?iฎ
		RESB	18				; ๓o18?

; ๖ๅฬ

entry:
		MOV		AX,0			; nป๑ถํ
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
		MOV		AH,0x0e			; ?ฆถ
		MOV		BX,15			; ?F
		INT		0x10			; ?p??BIOS
		JMP		putloop
fin:
		HLT						; CPUโ~าw฿
		JMP		fin				; z?

msg:
		DB		0x0a, 0x0a		; ?s2
		DB		"zOS is in development."
		DB		0x0a			; ?s
		DB		"Last updata is 2019.09.18 19:39"
		DB		0

		RESB	0x7dfe-$		

		DB		0x55, 0xaa

; ??ๆVO?o

		DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		RESB	4600
		DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		RESB	1469432
