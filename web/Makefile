# Makefile

all:
	@$(MAKE) -C en
	@if [ $$? != 0 ]; then exit 1; fi
	@cp -f en/*.html .
	@$(MAKE) -C de

# vi:set ts=3 sw=3:
