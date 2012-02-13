# Makefile

.SUFFIXES: .txt .html

TARGETS = $(SOURCES:.txt=.html)

all:
	@echo "SOURCES = `ls *.txt | sed ':a;N;$$!ba;s/\n/ /g'`" >  Makefile.tmp
	@echo "include ../Makefile" >> Makefile.tmp
	@for f in `ls *.txt`; do \
		echo "`basename $$f .txt`.html: $$f ../make_html ../Scidb.txt" >> Makefile.tmp; \
	done
	@make -f Makefile.tmp targets
	@rm -f Makefile.tmp

targets: $(TARGETS)

.txt.html:
	@echo "Generating $@"
	@../make_html ../Scidb.txt $*.txt > $*.html

clean:
	rm -f Makefile.tmp

# vi:set ts=3 sw=3:
