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
ARCH	:= $(shell gcc -print-multiarch)

all: $(TARGET) $(OBJS)

DEFCFLAGS =	$(INCLUDE)	\
		$(LIBS)		\
		$(PKGCFG)	\
		-DARCH=\"$(ARCH)\"

DEFLDFLAGS = $(PKGCFG)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(DEFLDFLAGS) $(LDFLAGS)

clbiff.o: clbiff.c clbiff.h config.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c clbiff.c -o clbiff.o

subset.o: subset.c subset.h config.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c subset.c -o subset.o

info.o: info.c info.h config.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c info.c -o info.o

signal.o: signal.c signal.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c signal.c -o signal.o

string.o: string.c string.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c string.c -o string.o

file.o: file.c file.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c file.c -o file.o

memory.o: memory.c memory.h
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c memory.c -o memory.o

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $(TARGET) $(BINDIR)/

install: install-bin

.PHONY: clean
clean:
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)
