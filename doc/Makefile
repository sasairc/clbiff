#
#    Makefile for clbiff
#

install-man: clbiff.1
ifeq ($(LANG),ja_JP.UTF-8)
	install -pd $(MANDIR)
	install -pm 644 $< $(MANDIR)/
endif

install-doc:
	@$(MAKE) -C ./polyaness	$@

.PHONY: install-man	\
	install-doc
