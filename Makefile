#
#    Makefile for clbiff
#

TARGET	= clbiff
PREFIX	:= /usr/local
BINDIR	:= $(PREFIX)/bin
MANDIR	:= $(PREFIX)/share/man/ja/man1
DOCDIR	:= $(PREFIX)/share/doc/clbiff
MAKE	:= make
CC	:= cc
RM	:= rm
CFLAGS	:= -O2 -g
LDFLAGS	:=
CMDLINE	:= 0
export

all install-bin clean:
	@$(MAKE) -C ./src	$@

install-man:
	@$(MAKE) -C ./doc	$@

install-doc:
	@$(MAKE) -C ./doc	$@

install-zsh-compdef:
	@$(MAKE) -C ./compdef	$@

install: install-bin		\
	 install-man		\
	 install-doc		\
	 install-zsh-compdef	\

.PHONY: all			\
	install			\
	install-bin		\
	install-man		\
	install-doc		\
	install-zsh-compdef	\
	clean
