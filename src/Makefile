#
#    Makefile for clbiff
#

include ../config.mk

SRCS      = $(wildcard *.c)
OBJS      = $(SRCS:.c=.o)
DEPSRCS   = $(wildcard ./libbenly/src/*.c ./libpolyaness/src/*.c)
DEPOBJS   = $(DEPSRCS:.c=.o)
ARCH      = $(shell gcc -print-multiarch)
DEFCFLAGS = -DARCH=\"$(ARCH)\"	\
	    -DLOCALE=\"\"

# use shared libraries (default: disable)
ifeq ($(WITH_SHARED), 1)
DEFCFLAGS   += -DWITH_SHARED
# WITH_SHARED
endif
ifeq ($(WITH_ADD_INFO), 1)
DEFCFLAGS   += -DWITH_ADD_INFO
# WITH_ADD_INFO
endif
ifeq ($(WITH_USLEEP), 1)
DEFCFLAGS   += -DWITH_USLEEP
# WITH_USLEEP
endif
ifdef DEFAULT_TMSEC
DEFCFLAGS   += -DDEFAULT_TMSEC=$(DEFAULT_TMSEC)
# DEFAULT_TMSEC
endif
ifdef DEFAULT_EXEC
DEFCFLAGS   += -DDEFAULT_EXEC=\"$(DEFAULT_EXEC)\"
# DEFAULT_EXEC
endif
ifdef INBOX_MBOX
DEFCFLAGS   += -DINBOX_MBOX=\"$(INBOX_MBOX)\"
# INBOX_MBOX
endif
ifdef INBOX_MDIR
DEFCFLAGS   += -DINBOX_MDIR=\"$(INBOX_MDIR)\"
# INBOX_MDIR
endif
# threshold of lines (sizeof(char*) * TH_LINES)
ifdef TH_LINES
DEFCFLAGS   += -DTH_LINES=$(TH_LINES)
# TH_LINES
endif
# threshold of string (sizeof(char) * TH_LENGTH)
ifdef TH_LENGTH
DEFCFLAGS   += -DTH_LENGTH=$(TH_LENGTH)
# TH_LENGTH
endif
export

all: $(TARGET)

ifeq ($(WITH_SHARED), 0)
$(TARGET): $(DEPOBJS) $(OBJS)
ifeq ($(CMDLINE), 0)
	@echo "  BUILD   $@"
	@$(CC) $(LDFLAGS) $^ -o $@
else
	$(CC) $(LDFLAGS) $^ -o $@
# CMDLINE
endif
else
DEFLDFLAGS = -lbenly_cmd -lbenly_env -lbenly_file -lbenly_memory -lbenly_signal -lbenly_string -lpolyaness
$(TARGET): $(OBJS)
ifeq ($(CMDLINE), 0)
	@echo "  BUILD   $@"
	@$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
else
	$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
# CMDLINE
endif
# WITH_SHARED
endif

%.o: %.c %.h
ifeq ($(CMDLINE), 0)
	@echo "  CC      $@"
	@$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
else
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
endif

build-dep $(DEPOBJS):
	@$(MAKE) -C ./libbenly     CFLAGS="$(CFLAGS)"	all
	@$(MAKE) -C ./libpolyaness CFLAGS="$(CFLAGS)"	all

clean-dep:
	-$(MAKE) -C ./libpolyaness	clean
	-$(MAKE) -C ./libbenly		clean

install-dep:
	@$(MAKE) -C ./libpolyaness	install
	@$(MAKE) -C ./libbenly		install

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $< $(BINDIR)/

clean: clean-dep
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)

.PHONY: all		\
        build-dep       \
	clean-dep       \
	install-dep	\
	install-bin	\
	clean
