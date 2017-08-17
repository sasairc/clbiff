#
#    Makefile for clbiff
#

SRCS      = $(wildcard *.c)
OBJS      = $(SRCS:.c=.o)
DEPOBJS   = ./libbenly/src/*.o ./libpolyaness/src/*.o
ARCH      = $(shell gcc -print-multiarch)
DEFCFLAGS = -DARCH=\"$(ARCH)\"	\
	    -DLOCALE=\"\"	\
	    $(INCLUDE)		\
	    $(LIBS)
export

all: $(TARGET) $(OBJS)

$(TARGET): build-dep $(OBJS)
ifeq ($(CMDLINE), 0)
	@echo "  BUILD   $@"
	@$(CC) $(LDFLAGS) $(OBJS) $(DEPOBJS) -o $@
else
	$(CC) $(LDFLAGS) $(OBJS) $(DEPOBJS) -o $@
endif

%.o: %.c %.h
ifeq ($(CMDLINE), 0)
	@echo "  CC      $@"
	@$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
else
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
endif

build-dep:
	@$(MAKE) -C ./libbenly		all
	@$(MAKE) -C ./libpolyaness	all

clean-dep:
	@$(MAKE) -C ./libbenly		clean
	@$(MAKE) -C ./libpolyaness	clean

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $< $(BINDIR)/

clean: clean-dep
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)

.PHONY: all		\
	build-dep	\
	clean-dep	\
        install-bin	\
	clean
