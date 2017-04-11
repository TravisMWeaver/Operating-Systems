  
# makefile for proj6
# author: bjr
# create: 12 nov 2016
# last update: 24 nov 2016

ZERO= /dev/zero
VD= vd.dmg
MNT= mnt

DOT=  | head -2 | tail -n +2
NODOT= | tail -n +2

minifat: minifat.c mf-utils.o fuseops.o minifat.h
	cc $< `pkg-config fuse --cflags --libs` -o $@ mf-utils.o fuseops.o

mf-utils.o: mf-utils.c mf-utils.h minifat.h fuseops.h
	cc `pkg-config fuse --cflags --libs` -o $@ -c $<

fuseops.o: fuseops.c mf-utils.h minifat.h fuseops.h
	cc `pkg-config fuse --cflags --libs` -o $@ -c $<

tone-gen: tone-gen.c 
	cc -o $@ $<
	./tone-gen -dn 32 | wc -c

${VD}:
	-rm ${VD}
	dd if=${ZERO} of=${VD} count=1024 bs=1024

run: minifat tone-gen
	-fusermount -u ${MNT}
	-mkdir ${MNT}
	./minifat -v ${MNT} ${VD}
	hexdump -C ${VD}

test-start: 
	make clean
	make minifat
	make tone-gen
	mkdir ${MNT}
	echo make ${VD}
	./minifat -c ${MNT} ${VD}

test-stage1-basic:
	touch ${MNT}/myfile.txt
	touch ${MNT}/a.c
	touch ${MNT}/b.h
	rm ${MNT}/a.c
	echo "-" > test-stage1.out
	ls -la ${MNT} ${DOT} >> test-stage1.out
	ls -l ${MNT} ${NODOT} >> test-stage1.out
	mkdir ${MNT}/mydir.dir
	mkdir ${MNT}/adir
	mkdir ${MNT}/bdir
	rmdir ${MNT}/adir
	echo "-" >> test-stage1.out
	ls -la ${MNT} ${DOT} >> test-stage1.out
	ls -l ${MNT} ${NODOT} >> test-stage1.out

test-stage1-dirlarge:
	cd ${MNT}; touch a b c d e f g h i j k l m n o p q r s t u v w x y z 
	cd ${MNT}; touch a.a b.b c.c d.d e.e f.f
	echo "-" >> test-stage1.out
	ls -la ${MNT} ${DOT} >> test-stage1.out
	ls -l ${MNT} ${NODOT} >> test-stage1.out
	cd ${MNT}; rm a.a b.b c.c d.d e.e f.f
	echo "-" >> test-stage1.out
	ls -la ${MNT} ${DOT} >> test-stage1.out
	ls -l ${MNT} ${NODOT} >> test-stage1.out

test-stage2-basic:
	echo "-" > test-stage2.out
	touch ${MNT}/empty
	echo "hello world" > ${MNT}/testfile.txt
	cat ${MNT}/testfile.txt | hexdump -C >> test-stage2.out
	truncate -s 5 ${MNT}/testfile.txt
	echo "" >> ${MNT}/testfile.txt
	cat ${MNT}/testfile.txt | hexdump -C >> test-stage2.out
	truncate -s 8 ${MNT}/testfile.txt
	cat ${MNT}/testfile.txt | hexdump -C >> test-stage2.out
	echo "-" >> test-stage2.out
	ls -l ${MNT} ${NODOT} >> test-stage2.out
	hexdump -C ${VD} >> test-stage2.out
	rm ${MNT}/testfile.txt
	ls -l ${MNT} ${NODOT} >> test-stage2.out
	hexdump -C ${VD} >> test-stage2.out
	
test-stage1:
	make test-start
	make test-stage1-basic
	make test-stage1-dirlarge
	diff test-stage1.ref test-stage1.out

test-stage2:
	make test-start
	make test-stage2-basic
	diff test-stage2.ref test-stage2.out

clean:
	-fusermount -u ${MNT}
	-rmdir ${MNT}
	-rm minifat ${VD} fuseops.o mf-utils.o tone-gen
	-rm test-stage1.out test-stage2.out

submit:
	svn commit -m "submitted for credit"

installfuse:
	sudo apt-get install libfuse-dev
	sudo apt-get install pkg-config

