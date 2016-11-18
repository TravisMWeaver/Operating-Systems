#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "mymonitor.h"
#include "mysyscalls.h"

/*
 * this is a template.
 * you are to complete the code for the subroutine.
 * hint: the actual code for the monitor is in the kernel.
 *
 * author: Travis Weaver
 * date: October 21, 2016
 * update: October 21, 2016
 * pledge: this is my own work, unless otherwise noted
 *
 * last update of template: 10 oct 2016
 *
 * submitted for grading
 */

extern int is_verbose ;

int my_monitor(int op) 
{
	switch(op)
	{
		case MYMON_OP_RESET:
			return syscall(MY_MONITOR, op);
		case MYMON_OP_ENTER:
			return syscall(MY_MONITOR, op);
		case MYMON_OP_LEAVE:
			return syscall(MY_MONITOR, op);
		case MYMON_OP_WAIT:
			return syscall(MY_MONITOR, op);
		case MYMON_OP_NOTIFY:
			return syscall(MY_MONITOR, op);
		case MYMON_OP_NOTIFY_ALL:
			return syscall(MY_MONITOR, op);
		default:
			return syscall(MY_MONITOR, op);
	}

	return 0;
}