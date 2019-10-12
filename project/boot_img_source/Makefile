TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/headfile_of_C/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
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

bootsource.gas : bootsource.c Makefile
	$(CC1) -o bootsource.gas bootsource.c

bootsource.nas : bootsource.gas Makefile
	$(GAS2NASK) bootsource.gas bootsource.nas

bootsource.obj : bootsource.nas Makefile
	$(NASK) bootsource.nas bootsource.obj bootsource.lst

bootsource.bim : bootsource.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootsource.bim stack:3136k map:bootsource.map \
		bootsource.obj
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

# 命令

asm :
	$(MAKE) ipl10.bin

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
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) bootsource.nas
	-$(DEL) bootsource.map
	-$(DEL) bootsource.bim
	-$(DEL) bootsource.hrb
	-$(DEL) zOS.sys

src_only :
	$(MAKE) clean
	-$(DEL) zOS.img