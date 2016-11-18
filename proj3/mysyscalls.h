/*
 * mysyscalls.h
 *
 * this is the header file for the system calls: 
 *    my_syslog, rb_ioctl, rb_enqueue, and rb_dequeue
 *
 * author: burt
 * date: 8 oct 2016
 *
 */


#define MY_SYSLOG 377
#define RB_IOCTL  378
#define RB_ENQUEUE 379
#define RB_DEQUEUE 380
#define MY_MONITOR 381

#define RB_OP_SIZE 0
#define RB_OP_ISEMPTY 1
#define RB_OP_ISFULL 2
#define RB_OP_COUNT 3

#define MYMON_OP_RESET 0
#define MYMON_OP_ENTER 1
#define MYMON_OP_LEAVE 2
#define MYMON_OP_WAIT 3
#define MYMON_OP_NOTIFY 4
#define MYMON_OP_NOTIFY_ALL 5