#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "boom.h"

/*
 * author: Travis Weaver
 * date: 24 aug 2015
 * pledge: this is my own work, unless otherwise noted
 *
 * this is a template. change name and continue work
 *
 * update: 5 September 2016
 * ready for grading!
 */


/* globals go here */
int g_debug = 0 ;

/* defines go here, in SHOUTY_CASE */
#define N_DEFAULT 1
#define U_DEFAULT 0
#define USAGE_MESSAGE "usage: boom [-vu] [-n stepsize] count" 

int main(int argc, char * argv[]) {

	int is_verbose = 0 ;
	int ch ;
	int n = N_DEFAULT ;
	int u = U_DEFAULT;
	int steps, index;

	while ((ch = getopt(argc, argv, "D:n:vu")) != -1) {
		switch(ch) {

		/*
                 * modify or add to these case statements
                 * added cases 'n' and 'u' from original file
		 */
            case 'n':
            	n = atoi(optarg);
            	break;
            case 'u':
            	u = 1;
            	break;
			case 'D':
				g_debug = atoi(optarg) ;
				break ;
			case 'v':
				is_verbose = 1 ;
				break ;
			default:
				printf("%s\n", USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	/* example of an assertion */
	assert(n>0) ;

	/* 
	 * your code here
	 *
	 */

	 steps = atoi(*argv);
	 //printf("%d\n", steps);

	 assert(steps >= 0);

	 if(u == 1)
	 {
	 	for(index = 0; index <= steps; index++)
	 	{
	 		printf("%d\n", index);
	 		index += (n - 1);
	 	}

	 	printf("Boom!\n");
	 }

	 else
	 {
	 	for(index = steps; index >= 0; index--)
	 	{
	 		printf("%d\n", index);
	 		index -= (n - 1);
	 	}

	 	printf("Boom!\n");
	 }

	return 0 ;
}

