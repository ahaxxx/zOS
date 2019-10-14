; naskfunc
; TAB = 4
[FORMAT "WCOFF"]                                ; 目标文件生成模式
[INSTRSET "i486p"]                              ; 使用486命令
[BITS 32]                                       ; 32位机械语言


; 目标文件的信息
[FILE "naskfunc.nas"]

            GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt          	; hlt,cli,sti,gtihlt指令
            GLOBAL	_io_in8, _io_in16, _io_in32						; 8 16 32位输入
            GLOBAL	_io_out8, _io_out16, _io_out32				; 8 16 32位输出
            GLOBAL	_io_load_eflags, _io_store_eflags

; 实际的函数
[SECTION .text]

_io_hlt:                            ; void io_hlt(void);
        HLT                         ; 中断
        RET                         ; return

_io_cli:                            ; void io_cli(void);
        CLI                         ; 中断清零
        RET                         ; return

_io_sti:                            ; void io_sti(void);
        STI                         ; 中断设置
        RET                         ; return

_io_stihlt:							; void io_stihlt(void);
		STI 						; 中断设置
		HLT 						; 中断
		RET 						; return

_io_in8:							; int io_in8(int port);
		MOV		EDX,[ESP+4]			; port
		MOV		EAX,0
		IN 		AL,DX
		RET

_io_in16:							; int io_in16(int port);
		MOV		EDX,[ESP+4]			; port
		MOV		EAX,0
		IN 		AX,DX
		RET

_io_in32:							; int io_in32(int port);
		MOV		EDX,[ESP+4]			; port
		IN		EAX,DX
		RET

_io_out8:							; void io_out8(int port, int data);
		MOV		EDX,[ESP+4]			; port
		MOV		AL,[ESP+8]			; data
		OUT 	DX,AL
		RET

_io_out16:							; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]			; port
		MOV		EAX,[ESP+8]			; data
		OUT 	DX,AX
		RET

_io_out32:							; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]			; port
		MOV		EAX,[ESP+8]			; data
		OUT 	DX,EAX
		RET

_io_load_eflags:					; void io_load_eflags(void);
		PUSHFD						; PUSH EFLAGS
		POP		EAX
		RET

_io_store_eflags:					; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]
		PUSH 	EAX
		POPFD						; POP EFLAGS
		RET