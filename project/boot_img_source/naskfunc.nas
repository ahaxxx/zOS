; naskfunc
; TAB = 4
[FORMAT "WCOFF"]                                ; 目标文件生成模式
[INSTRSET "i486p"]                              ; 使用486命令
[BITS 32]                                       ; 32位机械语言


; 目标文件的信息
[FILE "naskfunc.nas"]

            GLOBAL _io_hlt,_write_mem8          ; 文件中包含全局函数io_hlt


; 实际的函数
[SECTION .text]

_io_hlt:                            ; void io_hlt(void);
        HLT                         ; 中断
        RET                         ; return

_write_mem8:                        ; void write_mem8(int addr, int data);
        MOV     ECX,[ESP+4]         ; 将[ESP+4]中的地址读入ECX
        MOV     AL,[ESP+8]          ; 将[ESP+8]中的地址读入AL
        MOV     [ECX],AL
        RET