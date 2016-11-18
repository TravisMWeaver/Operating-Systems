#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
  
#include "mysyscalls.h"
#include "pc-monitor.h"

extern int is_verbose;

/*

Template Author: Dr. Burt Rosenburg
Author: Travis Weaver
Date: November 5, 2016
Last Modified: November 10, 2016

Partially working behavior
Submitted for grading

DESCRIPTION

    Subroutines that are two entry points into a producer-consumer monitor.
    
    Produce enqueues on the ring buffer the sequence of len characters, taken from 
    the char array pointed to by s. 
    
    Consume dequeues from the ring buffer as sequence of up to len characters, and stores
    them in a char array pointed to by s.
    
    The character sequence need not be a string. The null character has no particular
    significance. 
    
    The ring buffer is queried for size. If len is larger than the size of the ring 
    buffer the additional produced characters are ignored, and the additional 
    consumed characters are discarded.
    
    Each call to produce contributes an atomic message to the ring buffer. All characters
    are enqueued together in the ring buffer, and a call to consume takes all characters
    from that message. That is, multiple consumers and producers working simultaneously
    do not confuse messages. 
    
    The procedures are thread-safe: each call enters and leaves the monitor as needed.
    Produce will wait until there is room in the ring buffer to enqueue the message;
    consume will wait until there is a message in the ring buffer to dequeue.

RETURN VALUE

    Returns the number of characters actually enqueued or dequeued, or -1 if error.

*/

int consume(char * s, int len) 
{
    //assert(s);
    int index, localCount, globalCount, bufferSize;

    while(syscall(RB_IOCTL, RB_OP_ISEMPTY))
    {
        syscall(MY_MONITOR, MYMON_OP_WAIT);
    }

    syscall(MY_MONITOR, MYMON_OP_ENTER);

    localCount = 0;
    globalCount = syscall(RB_IOCTL, RB_OP_COUNT);
    bufferSize = get_ringbuf_size();


    while((s[index] = syscall(RB_DEQUEUE)) != -1)
    {
        localCount++;
    }

    syscall(MY_MONITOR, MYMON_OP_NOTIFY_ALL);
    syscall(MY_MONITOR, MYMON_OP_LEAVE);

    return localCount;  
}

int produce(char * s, int len) 
{ 
    assert(s);
    int index, localCount, globalCount, bufferSize;

    while(syscall(RB_IOCTL, RB_OP_ISFULL))
    {
        syscall(MY_MONITOR, MYMON_OP_WAIT);
    }

    syscall(MY_MONITOR, MYMON_OP_ENTER);

    localCount = 0;
    globalCount = syscall(RB_IOCTL, RB_OP_COUNT);
    bufferSize = get_ringbuf_size();

    for(index = 0; index < len; index++)
    {
        syscall(RB_ENQUEUE, s[index]);
        localCount++;
    }

    syscall(MY_MONITOR, MYMON_OP_NOTIFY_ALL);
    syscall(MY_MONITOR, MYMON_OP_LEAVE);

    return localCount;
} 

int get_ringbuf_size() 
{
    return syscall(RB_IOCTL, RB_OP_SIZE);
}

