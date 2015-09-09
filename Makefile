#
#    Makefile for clbiff
#

TARGET	= clbiff
PREFIX	:= /usr/local
BINDIR	:= $(PREFIX)/bin
INCLUDE :=
LIBS	:=
MAKE	:= make
CC	:= cc
RM	:= rm
CFLAGS	:= -O2 -g
LDFLAGS	:=
CMDLINE	:= 0
export

all install-bin clean:
	@$(MAKE) -C ./src	$@

install: install-bin

.PHONY: all		\
	install		\
	install-bin	\
	clean
