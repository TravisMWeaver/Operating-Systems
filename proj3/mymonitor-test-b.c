#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<sys/wait.h>

#include "mymonitor.h"

/*
 * monitor test program
 * author: bjr
 * created: november 2014
 * lastupdate: oct 10, 2016
 *
 * you should not need to change this code. this 
 * code is provided to you to test your monitor.
 *
 */

int is_verbose = 0 ;

#define USAGE_MESSAGE "usage: mymonitor-test-b -v "
#define BUF_SIZE 1024
#define ONE_SECOND_SLEEP 1 

#ifdef T1
#include "./test-commands-1.h"
#endif
#ifdef T2
#include "./test-commands-2.h"
#endif

int count_out_loud(char * s, int n, int count) {
	if ( count<0 ) return n ;
	while (count) {
		printf("%s %d\n", s, n ) ;
		fflush(NULL) ;
		sleep(ONE_SECOND_SLEEP) ;
		count-- ;
		n++ ;
	}
	return n ;
}

void child_process(struct Cmd * cmds, int cmd_count) {
	int child_count = 1 ;
	int step ;
	
	printf("\tC-> child entered\n") ;
	fflush(NULL) ;
	
	for (step=0; step<cmd_count; step++) {
		switch(cmds[step].op) {
		case 'z':
			child_count = count_out_loud("\tC-> ", child_count, cmds[step].arg ) ;
			break ;
		case 'e':
			printf("\tC-> request to enter monitor\n") ;
			fflush(NULL) ;
			my_monitor(MONITOR_OP_ENTER) ;
			printf("\tC-> monitor entered\n") ;
			fflush(NULL) ;
			break ;
		case 'w':
			printf("\tC-> monitor wait\n") ;
			fflush(NULL) ;
			my_monitor(MONITOR_OP_WAIT) ;
			printf("\tC-> wait over, re-entering monitor\n") ;
			fflush(NULL) ;
			break ;
		case 'l':
			my_monitor(MONITOR_OP_LEAVE) ;
			printf("\tC-> left monitor\n") ;
			fflush(NULL) ;
			break ;
		case 'n':
			printf("\tC-> monitor notify\n") ; 
			my_monitor(MONITOR_OP_NOTIFY) ; 	
			fflush(NULL) ;
			break ;	
		default:
			printf("\tC-> unknown command code\n") ;
			fflush(NULL) ;
		}
	}
	
	printf("\tC-> child exiting\n") ;
	exit(0) ;	
}

int main( int argc, char * argv[]) {
	char buf[BUF_SIZE] ;
	int ch ;
	pid_t child_pid ;
	int child_status ;
	int parent_count = 1 ;
	int step ;
	int cmd_count = PARENT_CMD_COUNT ;
	
	/* usual getopt stuff */
	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch(ch) {
		case 'v':
			is_verbose = 1 ;
			break ;
		case '?':
		default:
			printf("%s\n", USAGE_MESSAGE ) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;
    
	
	if ( !(child_pid = fork()) ) {
		/* child */
		child_process(child_cmds, CHILD_CMD_COUNT) ;
		assert(0==1) ; /* never get here */
	}
	if ( child_pid==-1 ) {
		printf("error: could not create child\n") ;
		return -1 ;
	}

	printf("P-> forked child with PID %d\n", child_pid) ;

	for (step=0; step<cmd_count; step++) {
		switch(parent_cmds[step].op) {
		case 'z':
			parent_count = count_out_loud("P-> ", parent_count, parent_cmds[step].arg ) ;
			break ;
		case 'e':
			printf("P-> request to enter monitor\n") ;
			fflush(NULL) ;
			my_monitor(MONITOR_OP_ENTER) ;
			printf("P-> monitor entered\n") ;
			fflush(NULL) ;
			break ;
		case 'w':
			printf("P-> monitor wait\n") ;
			fflush(NULL) ;
			my_monitor(MONITOR_OP_WAIT) ;
			printf("P-> wait over, re-entering monitor\n") ;
			fflush(NULL) ;
			break ;
		case 'l':
			my_monitor(MONITOR_OP_LEAVE) ;
			printf("P-> left monitor\n") ;
			fflush(NULL) ;
			break ;
		case 'n':
			printf("P-> monitor notify\n") ; 
			fflush(NULL) ;
			my_monitor(MONITOR_OP_NOTIFY) ; 	
			break ;	
		default:
			printf("P-> unknown command code\n") ;
			fflush(NULL) ;
		}
	}

	printf("P-> waiting for child to exit\n") ; 	
	fflush(NULL) ;
	child_pid = wait(&child_status) ;
	printf("P-> child %d exited %d\n", child_pid, WIFEXITED(child_status)) ;
	return 0 ;
}
