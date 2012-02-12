# Makefile

.SUFFIXES: .txt .html
.PRECIOUS:

SRC = \
	download.txt \
	index.txt \
	links.txt \
	moreinfo.txt \
	project.txt \
	releasehistory.txt \
	roadmap.txt \
	screenshots.txt \
	wanted.txt \
	de_download.txt \
	de_index.txt \
	de_links.txt \
	de_moreinfo.txt \
	de_project.txt \
	de_releasehistory.txt \
	de_roadmap.txt \
	de_screenshots.txt \
	de_wanted.txt \

HTML = $(SRC:.txt=.html)

all: $(HTML)

clean:
	rm -f $(HTML)

.txt.html:
	./make_html Scidb.txt $*.txt > $*.html

download.html:			make_html Scidb.txt download.txt
index.html:				make_html Scidb.txt index.txt
links.html:				make_html Scidb.txt links.txt
moreinfo.html:			make_html Scidb.txt moreinfo.txt
project.html:			make_html Scidb.txt project.txt
releasehistory.html:	make_html Scidb.txt releasehistory.txt
roadmap.html: 			make_html Scidb.txt roadmap.txt
screenshots.html:		make_html Scidb.txt screenshots.txt
wanted.html: 			make_html Scidb.txt wanted.txt

de_download.html:			make_html Scidb.txt de_download.txt
de_index.html:				make_html Scidb.txt de_index.txt
de_links.html:				make_html Scidb.txt de_links.txt
de_moreinfo.html:			make_html Scidb.txt de_moreinfo.txt
de_project.html:			make_html Scidb.txt de_project.txt
de_releasehistory.html:	make_html Scidb.txt de_releasehistory.txt
de_roadmap.html: 			make_html Scidb.txt de_roadmap.txt
de_screenshots.html:		make_html Scidb.txt de_screenshots.txt
de_wanted.html: 			make_html Scidb.txt de_wanted.txt

# vi:set ts=3 sw=3:
