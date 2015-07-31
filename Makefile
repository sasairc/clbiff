#
#    Makefile for clbiff
#

PREFIX	:= /usr/local
BINDIR	:= $(PREFIX)/bin
INCLUDE :=
LIBS	:=
MAKE	:= make
CC	:= cc
RM	:= rm
CFLAGS	:= -O2 -g -Wall -fno-strict-aliasing
LDFLAGS	:=
export

all install install-bin clean:
	@$(MAKE) -C ./src	$@

.PHONY: all install install-bin clean
