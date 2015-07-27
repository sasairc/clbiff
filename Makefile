#
#    Makefile for clbiff
#

TARGET	= clbiff
PREFIX	:= /usr/local
BINDIR	:= $(PREFIX)/bin
INCLUDE :=
LIBS	:=
PKGCFG	:= 
MAKE	:= make
CC	:= cc
RM	:= rm
CFLAGS	:= -O2 -g -Wall -fno-strict-aliasing
LDFLAGS	:=

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:.c=.o)
ARCH	= $(shell gcc -print-multiarch)

DEFCFLAGS = -DARCH=\"$(ARCH)\" \
		$(INCLUDE)     \
		$(LIBS)        \
		$(PKGCFG)

DEFLDFLAGS = $(PKGCFG)

all: $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(DEFLDFLAGS) $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c $< -o $@

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $(TARGET) $(BINDIR)/

install: install-bin

clean:
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)

.PHONY: all install clean
