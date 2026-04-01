EXEC=app
SRCS=$(wildcard *.c)

TOOLCHAIN_PATH=/buildroot/output/host/usr/bin/
TOOLCHAIN=$(TOOLCHAIN_PATH)aarch64-linux-
CFLAGS+=-Wall -Wextra -g -c -O0 -MD -std=gnu11
CFLAGS+=-mcpu=cortex-a53 -funwind-tables

OBJDIR=.obj

CC=$(TOOLCHAIN)gcc
LD=$(TOOLCHAIN)gcc
AR=$(TOOLCHAIN)ar
STRIP=$(TOOLCHAIN)strip
OBJS+=$(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

.PHONY: all clean

$(OBJDIR)/%o: %c
	$(CC) $(CFLAGS) $< -o $@


all: $(OBJDIR)/ $(EXEC)

clean: $(EXTRA_CLEAN)
	rm -Rf .obj
	rm -Rf $(EXEC) *.map *~

$(EXEC): $(OBJS) $(LINKER_SCRIPT)
	$(LD) $(OBJS) $(LDFLAGS) -o $@

$(OBJDIR)/:
	mkdir -p $(OBJDIR)

-include $(OBJS:.o=.d)


