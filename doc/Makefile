#
#    Makefile for clbiff
#

install-man: clbiff.1
	install -pd $(MANDIR)
	install -pm 644 $< $(MANDIR)/

install-doc:
	@$(MAKE) -C ./polyaness	$@

.PHONY: install-man	\
	install-doc
