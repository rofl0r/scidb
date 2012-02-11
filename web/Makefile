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

download.html:			Scidb.txt download.txt
index.html:				Scidb.txt index.txt
links.html:				Scidb.txt links.txt
moreinfo.html:			Scidb.txt moreinfo.txt
project.html:			Scidb.txt project.txt
releasehistory.html:	Scidb.txt releasehistory.txt
roadmap.html: 			Scidb.txt roadmap.txt
screenshots.html:		Scidb.txt screenshots.txt
wanted.html: 			Scidb.txt wanted.txt

de_download.html:			Scidb.txt de_download.txt
de_index.html:				Scidb.txt de_index.txt
de_links.html:				Scidb.txt de_links.txt
de_moreinfo.html:			Scidb.txt de_moreinfo.txt
de_project.html:			Scidb.txt de_project.txt
de_releasehistory.html:	Scidb.txt de_releasehistory.txt
de_roadmap.html: 			Scidb.txt de_roadmap.txt
de_screenshots.html:		Scidb.txt de_screenshots.txt
de_wanted.html: 			Scidb.txt de_wanted.txt

# vi:set ts=3 sw=3:
