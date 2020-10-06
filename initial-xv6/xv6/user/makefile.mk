
# user programs
USER_PROGS := \
	cat\
	echo\
	forktest\
	grep\
	init\
	kill\
	hello\
	ln\
	ls\
	mkdir\
	rm\
	sh\
	stressfs\
	tester\
	usertests\
	wc\
	zombie

USER_PROGS := $(addprefix user/, $(USER_PROGS))

# user library files
USER_LIBS := \
	ulib.o\
	usys.o\
	printf.o\
	umalloc.o

USER_LIBS := $(addprefix user/, $(USER_LIBS))

USER_OBJECTS = $(USER_PROGS:%=%.o) $(USER_LIBS)

USER_DEPS := $(USER_OBJECTS:.o=.d)

USER_CLEAN := user/bin $(USER_PROGS) $(USER_OBJECTS) $(USER_DEPS)

# add include dir to search path for headers
USER_CPPFLAGS += -I include

# do not search standard system paths for headers
USER_CPPFLAGS += -nostdinc

# disable PIC (position independent code)
USER_CFLAGS += -fno-pic

# do not use GCC builtin funtions (used to optimize common functions)
USER_CFLAGS += -fno-builtin

# disable some optimizations that assume one object will not alias an object
# of a different type
USER_CFLAGS += -fno-strict-aliasing

# disable stack smashing check
USER_CFLAGS += -fno-stack-protector

# generate code for 32-bit environment
USER_CFLAGS += -m32

# generate code for 32-bit environment
USER_ASFLAGS := $(USER_CFLAGS)

# FreeBSD ld wants ``elf_i386_fbsd''
USER_LDFLAGS += -m $(shell $(LD) -V | grep elf_i386 2>/dev/null)

# do not link with the host standard library files
USER_LDFLAGS += -nostdlib

# use simple contiguous section layout and do not use dynamic linking
USER_LDFLAGS += --omagic

# where program execution should begin
USER_LDFLAGS += --entry=main

# location in memory where the program will be loaded
USER_LDFLAGS += --section-start=.text=0x0

user/bin:
	mkdir -p user/bin

# user programs
user/bin/%: user/%.o $(USER_LIBS) | user/bin
	$(LD) $(LDFLAGS) $(USER_LDFLAGS) --output=$@ $< $(USER_LIBS)

# forktest has less library code linked in - needs to be small
# in order to be able to max out the proc table.
user/bin/forktest: user/forktest.o user/ulib.o user/usys.o | user/bin
	$(LD) $(LDFLAGS) $(USER_LDFLAGS) --output=$@ $^

# default recipe for object files
user/%.o: user/%.c
	$(CC) $(CPPFLAGS) $(USER_CPPFLAGS) $(CFLAGS) $(USER_CFLAGS) -c -o $@ $<

user/%.o: user/%.S
	$(CC) $(CPPFLAGS) $(USER_CPPFLAGS) $(ASFLAGS) $(USER_ASFLAGS) -c $< -o $@

# default recipes for dependency files
user/%.d: user/%.c
	$(CC) $(CPPFLAGS) $(USER_CPPFLAGS) $(CFLAGS) $(USER_CFLAGS) \
		-M -MG $< -MF $@ -MT $@ -MT $(<:.c=.o)

user/%.d: user/%.S
	$(CC) $(CPPFLAGS) $(USER_CPPFLAGS) $(ASFLAGS) $(USER_ASFLAGS) \
		-M -MG $< -MF $@ -MT $@ -MT $(<:.S=.o)

