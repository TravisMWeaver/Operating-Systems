#
# proj4 makefile
# producer consumer project
# csc421-171, oct 2016
#

KHR=/usr/src/linux-source-4.4.0
LS=linux-source-4.4.0

all:
	make producer
	make consumer
	
producer: producer.c pc-monitor.o pc-monitor.h
	cc ${COPTS} -o $@ $< pc-monitor.o
	
consumer: consumer.c pc-monitor.o pc-monitor.h
	cc ${COPTS} -o $@ $< pc-monitor.o
	
pc-monitor.o: pc-monitor.c pc-monitor.h
	cc ${COPTS} -c -o $@ $<


test1: producer consumer
	./producer -v -n 1 a & ./consumer -v -n 1 & wait

test2: producer consumer
	./producer -vn1 abc & ./consumer -vn3 & wait
	./producer -vn1 abc & ./consumer -vn1 & ./consumer -vn1 & ./consumer -vn1 & wait 

test3: producer consumer
	./producer -vn1 -M hello & ./consumer -vn1 -M & wait

test4: producer consumer
	./producer -vn1 -M hello.world & ./consumer -vn2 -M & wait

test5: producer consumer
	./producer -vMn1 hello.world & ./consumer -vMn1 & ./consumer -vMn1 & wait

clean:
	-rm pc-monitor.o producer consumer

submit:
	svn commit -m "submitted for grading"

