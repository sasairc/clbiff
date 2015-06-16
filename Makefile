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
SRCS	= clbiff.c
OBJS	= $(SRCS:.c=.o)

all: $(TARGET) $(OBJS)

DEFCFLAGS = 	$(INCLUDE)		  \
		$(LIBS)		 	  \
		$(PKGCFG)

DEFLDFLAGS = $(PKGCFG)

$(TARGET): $(OBJS)
	$(CC) $(DEFLDFLAGS) $(LDFLAGS) $(OBJS) -o $(TARGET)

clbiff.o: clbiff.c
	$(CC) $(DEFCFLAGS) $(CFLAGS) -c clbiff.c -o clbiff.o

install-bin: $(TARGET)
	install -pd $(BINDIR)
	install -pm 755 $(TARGET) $(BINDIR)/

install-cows:
	install -pd $(COWPATH)
	install -pm 644 ./cows/* $(COWPATH)/

install: install-bin

.PHONY: clean
clean:
	-$(RM) -f $(OBJS)
	-$(RM) -f $(TARGET)
