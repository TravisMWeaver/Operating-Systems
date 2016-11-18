#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "ringbuf.h"
#include "mysyscalls.h"

/*
 * author: Travis Weaver
 * date: October 2, 2016
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. change "author" and continue work
 * Hint: see mysyslog.c to see how to use syscall( syscall-number, arguments ... ) 
 *
 * update: October 3, 2016
 * Working user interface for the kernel ringbuffer 
 * Submitted for grading
 */

int g_debug = 0 ;
int is_verbose = 0 ;

int rb_ioctl(int op) {
	switch (op) {
	case RB_Q_SIZE:
		return syscall(RB_IOCTL, op);
	case RB_IS_EMPTY:
		return syscall(RB_IOCTL, op);
	case RB_IS_FULL:
		return syscall(RB_IOCTL, op);
	case RB_Q_COUNT:
		return syscall(RB_IOCTL, op);
	default:
		perror("rb_ioctl: Invalid function call. Ring buffer size, is_empty, is_full, and count currently supported.");
		return -1;
	}
	return 0 ;
}

int rb_enqueue(int ele) {
	return syscall(RB_ENQUEUE, ele);
}

int rb_dequeue(void) {
	return syscall(RB_DEQUEUE);
}