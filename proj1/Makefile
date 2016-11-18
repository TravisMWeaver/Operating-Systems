#
# Name: bjr
# Date: 5 sep 2016
# 
# Note: Makefiles begin with a captal M
#   Indentation MUST be with a tab
#
# tagets build, test, clean and submit are required.
# 

COPTS=

build:
	make ringbuf

ringbuf: ringbuf.c ringbuf-sub.o ringbuf.h
	cc ${COPTS} -o $@ ringbuf-sub.o $<

ringbuf-sub.o: ringbuf-sub.c ringbuf.h
	cc ${COPTS} -c -o $@ $<
	
test: ringbuf
	-rm ringbuf.out
	./ringbuf +abcdefghijklmno+pq-rs--tuv+ > ringbuf.out
	diff ringbuf.out ringbuf.ref

clean:
	-rm ringbuf ringbuf-sub.o ringbuf.out

submit:
	@echo svn add your work
	svn commit -m "submitted for grading"


