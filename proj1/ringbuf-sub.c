#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "ringbuf.h"

/*
 * author: Travis Weaver
 * date: N/A
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. change "author" and continue work
 *
 * update: September 18, 2016
 * submitted for grading
 */


/* static variables (zeroed) */

int g_debug = 0 ;
int is_verbose = 0 ;
struct RingBuf rb ;
int count = 0;

int rb_ioctl(int op) 
{
	switch (op) 
	{
		case RB_Q_SIZE:
			return RINGBUF_SIZE ;

		case RB_IS_EMPTY:
			if(count == 0)
			{
				return 1;
			}

			else
			{
				return 0;
			}

		case RB_IS_FULL:
			if(count < RINGBUF_SIZE)
			{
				return 0;
			}
			
			else
			{
				return 1;
			}

		case RB_Q_COUNT:
			return count;

		default:
			perror("rb_ioctl: Invalid function call. Ring buffer size, is_empty, is_full, and count currently supported.");
			return -1;
	}

	return 0 ;
}

int rb_enqueue(int ele) 
{
	if(count < RINGBUF_SIZE)
	{
		rb.ringbuf[rb.head] = ele;
		rb.head = (rb.head + 1) % RINGBUF_SIZE;
		count++;

		return ele;
	}

	else
	{
		return -1;
	}
}

int rb_dequeue(void) 
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

