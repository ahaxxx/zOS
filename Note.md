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

## 汇编语言

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

        DW      9               ; FAT长度(必须为9)

        DW      18              ; 1个磁道有几个扇区（18）

        DW      2               ; 磁头数（必须为2）

        DD      0               ; 不使用分区（必为00）

        DD      2880            ; 重写磁道大小

        DB      0,0,0x29        ; 

        DD      0xffffffff      ; 卷号

        DB      "zOS-OS     "   ; 磁盘名

        DB      "FAT12   "      ; 硬盘格式

        RESB    18              ; 空出18字符

; 程序本体

entry:

        MOV     AX,0            ; 初始化寄存器

        MOV     SS,AX

        MOV     SP,0x7c00

        MOV     DS,AX

        MOV     ES,AX

        MOV     SI,msg

putloop:

        MOV     AL,[SI]

        ADD     SI,1            ; SI+1

        CMP     AL,0

        JE      fin

        MOV     AH,0x0e         ; 显示文字

        MOV     BX,15           ; 字符颜色

        INT     0x10            ; 调用BIOS命令

        JMP     putloop

fin:

        HLT                     ; 暂停等待输入命令

        JMP     fin             ; 无限循环

msg:

        DB      0x0a, 0x0a      ; 换行

        DB      "zOS is in development."

        DB      0x0a            ; 换行

        DB      "Last updata is 2019.09.18 19:39"

        DB      0

        RESB    0x7dfe-$        ; 填充0x00直到0x7dfe

        DB      0x55, 0xaa
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
`JC`进位标志
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

## Makefile
### 介绍
make官方的描述如下：
* make是一个可以从程序源文件控制生成可执行文件或者其他非源文件的工具。
* make可以从`makefile`文件中获取到文件的生成规则，`makefile`列出了所有的非源程序和如何从其他文件生成非源程序的规则。
* 当创建一个项目时，应该为它创建一个`makefile`

### makefile规则

Makefile里主要包含了五个东西：显示规则、隐晦规则、变量定义、文件指示和注释。

*   显示规则
    显示规则说明了如何生成一个或者多个目标文件。这是由Malefile的书写者明显指出，要生成的文件，文件的依赖关系，生成的命令。

*   隐晦规则
    由于我们的make有自动推导的功能，所以隐晦的规则可以让我们比较粗糙地简略地书写Makefile。

*   变量的定义
    在Makefile中我们需要定义一系列的变量，变量一般都是字符串，这个有点像c语言里面的宏定义，当Makefile被执行时，其中的变量都会被扩展到相应的引用位置上。

*   文件指示
    包括了三个部分，一个是在一个Makefile中引用另一个Makefile，就像c语言里面的include一样；另一个是根据某些情况指定Makefile中的有效部分，就像c语言中的#if一样；还有就是定义一个多行的命令。

*   注释
    Makefile中只有行注释，注释使用`#`。

### 本人的理解
makefile在这里是将一大串用于生成各种文件的批处理命令整合到了一起，通过make的命令可以很便捷的将这些语句调用，减少开发过程中的繁琐操作。