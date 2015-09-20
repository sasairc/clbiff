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

install-zsh-compdef:
	@$(MAKE) -C ./compdef	$@

install: install-bin		\
	 install-zsh-compdef	\

.PHONY: all			\
	install			\
	install-bin		\
	install-zsh-compdef	\
	clean
