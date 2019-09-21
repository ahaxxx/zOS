# zOS开发笔记
## 启动
引导启动zOS系统盘
```
; zOS
; TAB=4

; 以下为FAT12引导

		DB		0xeb, 0x4e, 0x90
		DB		"zOS__IPL"		; 扇区名称(8字符)
		DW		512				; 扇区大小（512字节）
		DB		1				; cluster的个数(必须为1)
		DW		1				; FAT起始位置（一般为1）
		DB		2				; FAT个数（必须为2）
		DW		224				; 根目录大小（一般为224）
		DW		2880			; 磁盘大小（必须是2880扇区）
		DB		0xf0			; 磁盘种类（必须为0xf0）
		DW		9				; FAT?度(必须为9)
		DW		18				; 1个磁道有几个扇区（必须为18）
		DW		2				; 磁头数（必须为2）
		DD		0				; 不使用分区（必须为0）
		DD		2880			; 重写磁盘大小
		DB		0,0,0x29		; 
		DD		0xffffffff		; 卷标号
		DB		"zOS-OS     "	; 磁盘名
		DB		"FAT12   "		; 硬盘格式
		RESB	18				; 空出18字节
; 程序主体

		DB		0xb8, 0x00, 0x00, 0x8e, 0xd0, 0xbc, 0x00, 0x7c
		DB		0x8e, 0xd8, 0x8e, 0xc0, 0xbe, 0x74, 0x7c, 0x8a
		DB		0x04, 0x83, 0xc6, 0x01, 0x3c, 0x00, 0x74, 0x09
		DB		0xb4, 0x0e, 0xbb, 0x0f, 0x00, 0xcd, 0x10, 0xeb
		DB		0xee, 0xf4, 0xeb, 0xfd

; 信息显示部分

		DB		0x0a, 0x0a		; 换行符
		DB		"zOS is in development"
		DB		0x0a			; 换行
		DB		0

		RESB	0x1fe-$			; 填写0x00到0x001fe位置

		DB		0x55, 0xaa

; 启动区之外输出

		DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		RESB	4600
		DB		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
		RESB	1469432

```


 * DB指令
   * 可以用来直接写字符串
 * DW指令
   * 16位占位符
 * DD指令
   * 32位占位符

## 汇编语言与makefile

汇编语言版本的zOS引导镜像如下
```
; zOS

; TAB=4

        ORG     0x7c00          ; 程序加载位置

; 以下为FAT12引导

        DB      0xeb, 0x4e, 0x90

        DB      "zOS__IPL"      ; 扇区名称(8字符)

        DW      512             ; 扇区大小（512字节）

        DB      1               ; cluster的个数(必须为1)

        DW      1               ; FAT起始位置（一般为1）

        DB      2               ; FAT个数（必须为2）

        DW      224             ; 根目录大小（一般为224）

        DW      2880            ; 磁盘大小（必须是2880扇区）

        DB      0xf0            ; 磁盘种类（必须为0xf0）

        DW      9               ; FAT?度(必须为9)

        DW      18              ; 1个磁道有几个扇区（必须为18）

        DW      2               ; 磁头数（必须为2）

        DD      0               ; 不使用分区（必须为0）

        DD      2880            ; 重写磁盘大小

        DB      0,0,0x29        ; 

        DD      0xffffffff      ; 卷标号

        DB      "zOS-OS     "   ; 磁盘名

        DB      "FAT12   "      ; 硬盘格式

        RESB    18              ; 空出18字节

; 程序主体

entry:

        MOV     AX,0            ; 初始化寄存器

        MOV     SS,AX

        MOV     SP,0x7c00

        MOV     DS,AX

        MOV     ES,AX

        MOV     SI,msg

putloop:

        MOV     AL,[SI]

        ADD     SI,1            ; 给ST+1

        CMP     AL,0

        JE      fin

        MOV     AH,0x0e         ; 显示文字

        MOV     BX,15           ; 字符颜色

        INT     0x10            ; 调用显卡BIOS

        JMP     putloop

fin:

        HLT                     ; CPU停止等待指令

        JMP     fin             ; 死循环

msg:

        DB      0x0a, 0x0a      ; 换行2次

        DB      "hello, world"

        DB      0x0a            ; 换行

        DB      0

        RESB    0x7dfe-$        

        DB      0x55, 0xaa

; 启动区之外输出

        DB      0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00

        RESB    4600

        DB      0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00

        RESB    1469432
```

 * ORG 
   * 指定开始地址
 * JMP
   * 跳转至某标签，类似于C语言的goto
   * `JMP A`意思就是跳转到A标签所在的语句
 * **:
   * 标签语句，类似于C语言中的A:
 * MOV
   * 赋值语句，前一个参数是操作的变量名，后一个参数是要赋的值
   * `MOV AX,0`这句的意思就是给AX赋值为0

### 寄存器
CPU中的储存电路，相当于变量，比较有代表性的有如下8个
| 寄存器标识 | 全拼 | 中文名 |
| --------- | ---- | ----- |
| AX |  accumulator  | 累加寄存器  |
| CX | counter     | 计数寄存器      |
|DX|data|数据寄存器|
|BX|base|基址寄存器|
|SP|stack pointer| 栈指针寄存器      |
|BP|base pointer|基址指针寄存器|
|SI|source index|源变址寄存器|
|DI|destination index|目的变址寄存器|
以上寄存器均为16位寄存器，即代表16位二进制数，也就是2字节

还有8个8位寄存器
| 寄存器标识 | 全拼 | 中文名 |
| --------- | ---- | ----- |
| AL |  accumulator low | 累加寄存器低位  |
| CL | counter low    | 计数寄存器低位      |
|DL|data low|数据寄存器低位|
|BL|base low|基址寄存器低位|
| AH |  accumulator high | 累加寄存器高位  |
| CH | counter high   | 计数寄存器高位      |
|DH|data high|数据寄存器高位|
|BH|base high|基址寄存器高位|

16位寄存器名前加E _(extend)_即为32位寄存器
此外还存在段寄存器
### 内存

`MOV     AL,[SI]`在这里[SI]是指SI所在的内存地址
`MOV`指令的传送源和传送目的可以是寄存器、常数，也可以是内存地址。

`BYTE`、`WORD`和 `DWORD`
这三个数据大小分别是占用一字节，占用两字节，占用四字节。

占用的内存为倒序插入数据，即大标号内存地址储存低位数据。

MOV指令的原则是源数据和目的数据位数必须相同

只有`BX` `BP` `SI` `DI` 可以用来储存数据地址

### 汇编语言语法

汇编语言的基本语句为
`命令 操作的寄存器 值`

`ADD`加法指令
`CMP`指令是判断语句，类似于高级语言中的`if`
`JE`是条件跳转语句
 * 汇编语言写法

```
CMP AL,0
JE fin
```

 * 高级语言写法
```
if(AL == 0){
  goto fin;
}
```

   `INT` 指令，中断操作的意思，配合BIOS指令集使用，类似于`system(“pause”);`
   `HLT`指令 CPU停止（线程挂起），外部环境没有变化时就会挂起线程，节约系统资源，iOS也是这样设计？
