#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

#include "pc-monitor.h"

/*
 * Producer Template Author: Dr. Burt Rosenburg
 * Author: Travis Weaver
 * Date: November 5, 2016
 * Last Modified: November 10, 2016
 *
 * Partially working behavior
 * Submitted for grading
 */

#define USAGE_MESSAGE "usage: producer [-vM] [-r _num_] [-n _num_] _message_"
#define REPEAT_TIME_DEFAULT 1
#define SLEEP_TIME_DEFAULT 1

#define WORD_DELIMIT '.'

int is_verbose = 0 ;

int main(int argc, char * argv[]) 
{
	int ch;
	int repeat = REPEAT_TIME_DEFAULT;
	int is_mm = 0; 
	int num = 0;
	int index, len, enqueueNum;
	int sleepTime = SLEEP_TIME_DEFAULT;
	char * splitWord;
	char * word;
	
	/* usual getopt stuff */
	while ((ch = getopt(argc, argv, "vMn:r:")) != -1) {
		switch(ch) {
		case 'r':
			sleepTime = atoi(optarg);
			printf("%d = sleepTime for producer\n", sleepTime);
			break ;
		case 'M':
			is_mm = 1;
			break ;
		case 'v':
			is_verbose = 1;
			break ;
		case 'n':
			repeat = atoi(optarg);
			printf("%d = repeat for producer\n", repeat);
			break ;
		default:
			printf("%s\n",USAGE_MESSAGE) ;
			return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	word = argv[0];

	if(is_mm == 1)
	{
		for(index = 0; index < repeat; index++)
		{
			while(splitWord = strsep(&word, "."))
			{
				len = strlen(splitWord);
				sleep(sleepTime);
				enqueueNum = produce(splitWord, len);
				printf("%d elements enqueued successfully to the buffer\n", enqueueNum);
			}
		}
	}

	else
	{
		for(index = 0; index < repeat; index++)
		{
			len = strlen(word);
			sleep(sleepTime);
			enqueueNum = produce(word, len);
			printf("%d elements enqueued successfully to the buffer\n", enqueueNum);
		}
	}

	return 0;
}
