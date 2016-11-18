#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

#include "pc-monitor.h"

/*
 * Consumer Template Author: Dr. Burt Rosenburg
 * Author: Travis Weaver
 * Date: November 5, 2016
 * Last Modified: November 10, 2016
 *
 * Submitted for help
 * Partially working behavior
 * Submitted for grading
 */

#define USAGE_MESSAGE "usage: consumer [-vM] [-r _num_] [-n _num_]"
#define REPEAT_TIME_DEFAULT 1
#define SLEEP_TIME_DEFAULT 1

int is_verbose = 0;
char * consumedWord;

int main(int argc, char * argv[]) 
{
	int ch;
	int repeat = REPEAT_TIME_DEFAULT;
	int is_mm = 0;
	int sleepTime = SLEEP_TIME_DEFAULT;
	int index, len, dequeueNum;

	printf("Test Line\n");
	
	/* usual getopt stuff */
	while ((ch = getopt(argc, argv, "vMn:r:")) != -1) {
		switch(ch) {
		case 'r':
			sleepTime = atoi(optarg);
			printf("%d = sleepTime for consumer\n", sleepTime);
			break ;
		case 'M':
			is_mm = 1;
			break ;
		case 'v':
			is_verbose = 1;
			break ;
		case 'n':
			repeat = atoi(optarg);
			printf("%d = repeat for consumer\n", repeat);
			break;
		default:
			printf("%s\n",USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	if(is_mm == 1)
	{
		for(index = 0; index < repeat; index++)
		{
			sleep(sleepTime);
			dequeueNum = consume(consumedWord, 0);
			printf("%d elements dequeued successfully to the buffer\n", dequeueNum);
		}
	}

	else
	{
		for(index = 0; index < repeat; index++)
		{
			sleep(sleepTime);
			dequeueNum = consume(consumedWord, 0);
			printf("%d elements dequeued successfully to the buffer\n", dequeueNum);
		}
	}

	//printf("%s = consumed word", consumedWord);
	
	return 0;
}
