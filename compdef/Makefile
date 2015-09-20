#
#    Makefile for clbiff
#

install-zsh-compdef: _$(TARGET).zsh
	install -pd $(PREFIX)/share/$(TARGET)/compdef
	install -pm 644 $< $(PREFIX)/share/$(TARGET)/compdef

.PHONY: install-zsh-compdef
