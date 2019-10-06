TOOLPATH = ../z_tools/
MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
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

zOS.sys : zOS.nas Makefile
	$(NASK) zOS.nas zOS.sys zOS.lst

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
	-$(DEL) ipl10.bin
	-$(DEL) ipl10.lst

src_only :
	$(MAKE) clean
	-$(DEL) zOS.img