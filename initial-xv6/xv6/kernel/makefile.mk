
# Kernel objects
KERNEL_OBJECTS := \
	bio.o\
	console.o\
	exec.o\
	file.o\
	fs.o\
	ide.o\
	ioapic.o\
	kalloc.o\
	kbd.o\
	lapic.o\
	main.o\
	mp.o\
	picirq.o\
	pipe.o\
	proc.o\
	spinlock.o\
	string.o\
	swtch.o\
	syscall.o\
	sysfile.o\
	sysproc.o\
	timer.o\
	trapasm.o\
	trap.o\
	uart.o\
	vectors.o\
	vm.o\

KERNEL_OBJECTS := $(addprefix kernel/, $(KERNEL_OBJECTS))

# Objects compiled in a special way for boot or alignment
KERNEL_SPECIAL_OBJECTS := \
	bootmain.o\
	bootasm.o\
	multiboot.o\
	data.o\
	bootother.o\
	initcode.o

KERNEL_SPECIAL_OBJECTS := $(addprefix kernel/, $(KERNEL_SPECIAL_OBJECTS))

# Make dependency files
KERNEL_DEPS := \
	$(KERNEL_OBJECTS:.o=.d) \
	$(KERNEL_SPECIAL_OBJECTS:.o=.d)

# All generated files
KERNEL_CLEAN := \
	$(KERNEL_OBJECTS) \
	$(KERNEL_SPECIAL_OBJECTS) \
	$(KERNEL_DEPS) \
	kernel/vectors.S\
	kernel/bootblock\
	kernel/bootblock.out\
	kernel/bootother.out\
	kernel/initcode.out\
	kernel/kernel\
	bootother\
	initcode\
	xv6.img

# add include dir to search path for headers
KERNEL_CPPFLAGS += -I include
# do not search standard system paths for headers
KERNEL_CPPFLAGS += -nostdinc
# disable PIC (position independent code)
KERNEL_CFLAGS += -fno-pic
# do not use GCC builtin funtions (used to optimize common functions)
KERNEL_CFLAGS += -fno-builtin
# disable some optimizations that assume one object will not alias an object
# of a different type
KERNEL_CFLAGS += -fno-strict-aliasing
# disable stack smashing check
KERNEL_CFLAGS += -fno-stack-protector
# generate code for 32-bit environment
KERNEL_CFLAGS += -m32

KERNEL_ASFLAGS += $(KERNEL_CFLAGS)

# FreeBSD ld wants ``elf_i386_fbsd''
KERNEL_LDFLAGS += -m $(shell $(LD) -V | grep elf_i386 2>/dev/null)
# do not link with the host standard library files
KERNEL_LDFLAGS += -nostdlib
# use simple contiguous section layout and do not use dynamic linking
KERNEL_LDFLAGS += --omagic

# bootable disk image
xv6.img: kernel/bootblock kernel/kernel
	dd if=/dev/zero of=xv6.img count=10000
	dd if=kernel/bootblock of=xv6.img conv=notrunc
	dd if=kernel/kernel of=xv6.img seek=1 conv=notrunc

kernel/kernel:	\
		$(KERNEL_OBJECTS) kernel/multiboot.o kernel/data.o bootother initcode
	$(LD) $(LDFLAGS) $(KERNEL_LDFLAGS) \
		--section-start=.text=0x100000 --entry=main --output=kernel/kernel \
		kernel/multiboot.o kernel/data.o $(KERNEL_OBJECTS) \
		-b binary initcode bootother

# bootblock is optimized for space
kernel/bootmain.o: kernel/bootmain.c
	$(CC) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(CFLAGS) $(KERNEL_CFLAGS) \
		 -Os -c -o $@ $<

kernel/bootblock: kernel/bootasm.o kernel/bootmain.o
	$(LD) $(LDFLAGS) $(KERNEL_LDFLAGS) \
		--entry=start --section-start=.text=0x7C00 \
		--output=kernel/bootblock.out kernel/bootasm.o kernel/bootmain.o
	$(OBJCOPY) -S -O binary -j .text kernel/bootblock.out kernel/bootblock
	./kernel/sign.pl kernel/bootblock

bootother: kernel/bootother.o
	$(LD) $(LDFLAGS) $(KERNEL_LDFLAGS) \
		--entry=start --section-start=.text=0x7000 \
		--output=kernel/bootother.out kernel/bootother.o
	$(OBJCOPY) -S -O binary kernel/bootother.out $@

initcode: kernel/initcode.o
	$(LD) $(LDFLAGS) $(KERNEL_LDFLAGS) \
		--entry=start --section-start=.text=0x0 \
		--output=kernel/initcode.out kernel/initcode.o
	$(OBJCOPY) -S -O binary kernel/initcode.out $@

kernel/vectors.S: kernel/vectors.pl
	perl kernel/vectors.pl > $@

# default recipe for object files
kernel/%.o: kernel/%.c
	$(CC) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(CFLAGS) $(KERNEL_CFLAGS) -c -o $@ $<

kernel/%.o: kernel/%.S
	$(AS) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(ASFLAGS) $(KERNEL_ASFLAGS) -c $< -o $@


# default recipes for dependency files
kernel/%.d: kernel/%.c
	$(CC) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(CFLAGS) $(KERNEL_CFLAGS) \
		-M -MG $< -MF $@ -MT $@ -MT $(<:.c=.o)

kernel/%.d: kernel/%.S
	$(AS) $(CPPFLAGS) $(KERNEL_CPPFLAGS) $(CFLAGS) $(KERNEL_CFLAGS) \
		-M -MG $< -MF $@ -MT $@ -MT $(<:.S=.o)

