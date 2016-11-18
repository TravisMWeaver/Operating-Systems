#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/semaphore.h>
#include <linux/types.h>

/*
 * kernel/mysyscalls.c
 *
 * this is a template.
 * you are to complete the coding of the subroutines.
 * note: proj2 completed the coding of the ring buffer
 * subroutines; bring forward that code. 
 * for proj3 complete the coding of the monitor subroutine.
 *
 * author: Travis Weaver
 * date: October 21, 2016
 * update: October 21, 2016
 *
 * last update of template: 10 oct 2016
 *
 * submitted for grading
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

static DEFINE_SEMAPHORE(lock_sema);
static struct semaphore wait_sema = __SEMAPHORE_INITIALIZER(wait_sema, 0);
atomic_t wait_count = ATOMIC_INIT(0);

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

asmlinkage long sys_my_monitor(unsigned int op) 
{
	switch(op)
	{
		case MYMON_OP_RESET:
			sema_init(&lock_sema, 1);
			sema_init(&wait_sema, 0);
			atomic_set(&wait_count, 0);
			break;

		case MYMON_OP_ENTER:
			down_interruptible(&lock_sema);
			break;

		case MYMON_OP_LEAVE:
			up(&lock_sema);
			break;

		case MYMON_OP_WAIT:
			atomic_inc(&wait_count);
			up(&lock_sema);
			down_interruptible(&wait_sema);
			down_interruptible(&lock_sema);
			break;

		case MYMON_OP_NOTIFY:
			if(atomic_read(&wait_count) > 0)
			{
				atomic_dec(&wait_count);
				up(&wait_sema);
			}
			break;

		case MYMON_OP_NOTIFY_ALL:
			while(atomic_read(&wait_count) > 0)
			{
				atomic_dec(&wait_count);
				up(&wait_sema);
			}
			break;

		default:
			printk(KERN_DEBUG "my_monitor: undefined operation action");
	}

	return 0 ;
}

