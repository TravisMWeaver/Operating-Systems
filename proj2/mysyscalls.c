#include <linux/kernel.h>
#include <linux/syscalls.h>

/*
 * kernel/mysyscalls.c
 *
 * this is a template of the file that is in the kernel and implements
 * the four new system calls: 
 *     my_syslog, rb_ioctl, rb_enqueue, and rb_dequeue
 *
 * author: Travis Weaver
 * date: October 2, 2016
 * update: October 3, 2016
 * Ring buffer implemented, rebuilding the kernel with the new file
 * Working kernel ring buffer, submitted for grading
 *
 */

#include "mysyscalls.h"

#define RINGBUF_SIZE 16

struct RingBuf {
	int head ;
	int tail ;
	int is_full ;
	char ringbuf[RINGBUF_SIZE] ;
} ;

struct RingBuf rb ;

int count = 0;

asmlinkage long sys_my_syslog(const char * msg) {
	printk(KERN_ALERT "my_syslog: %s\n", msg) ;
	return 0 ;
}

asmlinkage long sys_rb_ioctl(unsigned int op) 
{
	switch (op) 
	{
		case RB_OP_SIZE:
			return RINGBUF_SIZE ;

		case RB_OP_ISEMPTY:
			if(count == 0)
			{
				return 1;
			}

			else
			{
				return 0;
			}

		case RB_OP_ISFULL:
			if(count < RINGBUF_SIZE)
			{
				return 0;
			}
			
			else
			{
				return 1;
			}

		case RB_OP_COUNT:
			return count;

		default:
			return -1;
	}

	return 0 ;
}

asmlinkage long sys_rb_enqueue(char c) 
{
	if(count < RINGBUF_SIZE)
	{
		rb.ringbuf[rb.head] = c;
		rb.head = (rb.head + 1) % RINGBUF_SIZE;
		count++;

		return c;
	}

	else
	{
		return -1;
	}
}

asmlinkage long sys_rb_dequeue(void) 
{
	if(count > 0)
	{
		int temp;
		temp = rb.ringbuf[rb.tail];
		rb.tail = (rb.tail + 1) % RINGBUF_SIZE;
		count--;

		return temp;
	}

	else
	{
		return -1 ;
	}
}

