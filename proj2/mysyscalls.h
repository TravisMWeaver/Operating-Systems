
/*
 * mysyscalls.h
 *
 * this is the header file for the system calls: 
 *    my_syslog, rb_ioctl, rb_enqueue, and rb_dequeue
 *
 * author: burt
 * date: 13 sep 2016
 *
 */


#define MY_SYSLOG 377
#define RB_IOCTL  378
#define RB_ENQUEUE 379
#define RB_DEQUEUE 380

#define RB_OP_SIZE 0
#define RB_OP_ISEMPTY 1
#define RB_OP_ISFULL 2
#define RB_OP_COUNT 3


