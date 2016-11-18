#!/bin/bash

#
# Makefile for Project 3
# csc421-171
# author: bjr
# last revision: 8 oct 2016
#

KHR=/usr/src/linux-source-4.4.0
LS=linux-source-4.4.0


all:
	make test-mymonitor


# monitor

mymonitor-test-a: mymonitor-test-a.c  mymonitor-sub.o mymonitor.h 
	cc ${COPTS} -D MONITOR -o $@ $< mymonitor-sub.o

mymonitor-test-b1: mymonitor-test-b.c mymonitor-sub.o mymonitor.h test-commands-1.h
	cc ${COPTS} -D T1 -o $@ $< mymonitor-sub.o

mymonitor-test-b2: mymonitor-test-b.c mymonitor-sub.o mymonitor.h  test-commands-2.h
	cc ${COPTS} -D T2 -o $@ $< mymonitor-sub.o 

mymonitor-sub.o: mymonitor-sub.c mymonitor.h mysyscalls.h
	cc -c -o $@ $<

test-mymonitor: mymonitor-test-a mymonitor-test-b1 mymonitor-test-b2
	-rm mymonitor-test-a.out
	-rm mymonitor-test-b.out
	@echo NOTE: These tests might take time
	./mymonitor-test-a > mymonitor-test-a.out
	./mymonitor-test-b1 > mymonitor-test-b.out
	./mymonitor-test-b2 >> mymonitor-test-b.out
	diff mymonitor-test-a.out mymonitor-test-a.ref
	@echo NOTE: diff will fail because of PID
	diff mymonitor-test-b.out mymonitor-test-b.ref



# mysyslog

mysyslog: mysyslog.c mysyscalls.h
	cc -o $@ $<

test-mysyslog: mysyslog
	./mysyslog "hello kernel!"
	echo $$? > mysyslog.out
	tail /var/log/syslog | grep "my_syslog:" | tail -n 1 >> mysyslog.out
	cat mysyslog.out



# ring buffer

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



# maintenance tasks

evidence:
	tar cvf evidence.tar -C ${KHR} \
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
	-rm mymonitor-test-a mymonitor-test-b1 mymonitor-test-b2 mymonitor-sub.o
	-rm mymonitor-test-a.out mymonitor-test-b.out

submit:
	svn commit -m "submit for grading"

