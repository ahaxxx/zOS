OBJS_BOOTPACK = bootsource.obj naskfunc.obj hankaku.obj gui.obj dsctbl.obj int.obj fifo.obj
TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/headfile_of_C/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)headfile_of_C/headfile_of_C.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# 默认动作

default :
	$(MAKE) img

# 文件生成规则



ipl10.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl10.bin ipl10.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

bootsource.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootsource.bim stack:3136k map:bootsource.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

bootsource.hrb : bootsource.bim Makefile
	$(BIM2HRB) bootsource.bim bootsource.hrb 0

zOS.sys : asmhead.bin bootsource.hrb Makefile
	copy /B asmhead.bin+bootsource.hrb zOS.sys

zOS.img : ipl10.bin zOS.sys Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:zOS.sys to:@: \
		imgout:zOS.img

#一般规则

%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

# 命令

img :
	$(MAKE) zOS.img

run :
	$(MAKE) img
	$(COPY) zOS.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: zOS.img


clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) bootsource.map
	-$(DEL) bootsource.bim
	-$(DEL) bootsource.hrb
	-$(DEL) zOS.sys

src_only :
	$(MAKE) clean
	-$(DEL) zOS.img