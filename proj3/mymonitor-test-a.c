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
 * created: oct 2015
 * lastupdate: 10 oct 2016
 *
 * you should not need to change this. this code
 * is provided to you to test your monitor.
 *
 */

int is_verbose = 0 ;

#define NUM_CHILDREN 4
#define TIME_SLEEP 3
#define USAGE_MESSAGE "usage: mymonitor-test-a -v "

void child_process(int id) {
	sleep(TIME_SLEEP) ;
#ifdef MONITOR
	my_monitor(MONITOR_OP_ENTER) ;
#endif
	printf("\tC-> monitor entered\n") ;
	sleep(TIME_SLEEP) ;
	printf("\tC-> monitor leave\n") ;
	fflush(NULL) ;
#ifdef MONITOR
	my_monitor(MONITOR_OP_LEAVE) ;
#endif
	exit(0) ;
}

int main( int argc, char * argv[]) {
	int ch ;
	int num_children = NUM_CHILDREN ;
	pid_t child_pid ;
	int child_status ;
	int i ;
	
	
	/* usual getopt stuff */
	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch(ch) {
		case 'v':
			is_verbose = 1 ;
			break ;
		case '?':
		default:
			printf("%s\n", USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	printf("P-> forking %d children\n",num_children) ; 	
	fflush(NULL) ;

	for (i=0;i<num_children;i++) {
		if ( !(child_pid = fork()) ) {
			/* child */
			child_process(i) ;
			assert(0==1) ; /* never get here */
		}
		if ( child_pid==-1 ) {
			printf("error: could not create child\n") ;
			return -1 ;
		}
	}

	for (i=0;i<num_children;i++) {
		child_pid = wait(&child_status) ;
	}

	sleep(TIME_SLEEP) ;
	printf("P-> exiting\n") ; 	
	fflush(NULL) ;
	return 0 ;
}
