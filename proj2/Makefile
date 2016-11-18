#!/bin/bash

#
# Makefile for Project 2
# csc421-171
# author: bjr
# last revision: 13 sep 2016
#

KHR=/usr/src/linux-source-4.4.0
LS=linux-source-4.4.0


all:
	make clean
	make mysyslog
	make test-mysyslog
	make ringbuf
	make test-ringbuf
	make evidence


mysyslog: mysyslog.c mysyscalls.h
	cc -o $@ $<

test-mysyslog: mysyslog
	./mysyslog "hello kernel!"
	echo $$? > mysyslog.out
	tail /var/log/syslog | grep "my_syslog:" | tail -n 1 >> mysyslog.out
	cat mysyslog.out

ringbuf: ringbuf.c ringbuf-sub.o ringbuf.h
	cc -o $@ ringbuf-sub.o $<

ringbuf-sub.o: ringbuf-sub.c ringbuf.h mysyscalls.h
	cc -c -o $@ $<

test-ringbuf: ringbuf
	make empty-rb
	./ringbuf +abcdefghijklmno+pq-rs--tuv+ > ringbuf.out
	diff ringbuf.out ringbuf.ref

empty-rb:
	./ringbuf -- -------------------------

evidence:
	make test-mysyslog
	make test-ringbuf
	cat mysyslog.out ringbuf.out > evidence.out
	tar cvf evidence.tar evidence.out -C ${KHR} \
		${LS}/kernel/Makefile \
		${LS}/include/linux/syscalls.h \
		${LS}/arch/x86/entry/syscalls/syscall_32.tbl \
		${LS}/kernel/mysyscalls.c ${LS}/kernel/mysyscalls.h

untar:
	tar xvf evidence.tar

clean:
	-rm evidence.tar evidence.out 
	-rm mysyslog ringbuf ringbuf-sub.o
	-rm -rf ${LS}
	-rm ringbuf.out mysyslog.out

submit:
	svn commit -m "submit for grading"

