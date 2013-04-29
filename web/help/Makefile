# Makefile

all:
	@for lang in en de; do \
		for src in `ls ../../tcl/help/$$lang/*.html`; do \
			dst="$$lang/`basename $$src`"; \
			echo "Make $$dst"; \
			./make_html.tcl $$src $$dst; \
		done; \
	done

# vi:set ts=3 sw=3:
