#
# Name:
# Date:
# 
# Note: Makefiles begin with a captal M
#   Indentation MUST be with a tab
#
# tagets build, test, clean and submit are required.
# 

COPTS=

build:
	make boom

boom: boom.c boom.h
	cc ${COPTS} -o $@ $<
	
test: boom
	-rm boom.out
	./boom 10  > boom.out
	./boom -u -n 2 10  >> boom.out
	diff boom.ref boom.out

clean:
	-rm boom boom.out

submit:
	@echo svn add your work
	svn commit -m "submitted for grading"


