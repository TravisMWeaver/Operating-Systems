#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

/*
 * tone-gen - making "test tones" in terms of character strings
 * author: bjr
 * created: 22 nov 2016
 * last update: 22 nov 2016
 *
 * Usage: tone-gen [-hd] n
 *        -h in hexadecimal
 *        -d in decimal
 * 	  -n no newline
 *        n lenght of string
 */

#define PN "tone-gen"

void print_usage(char * name) {
	printf("Usage: %s [-hdn] _num_\n", name ) ;
	return ;
}

int main(int argc, char * argv[]) {
	int ch ;
	int is_verbose = 0 ;
	int is_hexi = 1 ;
	int is_deci = 0 ;
	int is_nl = 1 ;
	int n ;
	int i, j ;
	int base ;
	char * cp ;
	
	while ( (ch = getopt(argc, argv, "hdn")) != -1 ) {
		switch(ch) {
		case 'v':
			is_verbose = 1 ;
			break ;
		case 'h':
			is_hexi = 1 ;
			is_deci = 0 ;
			break ;
		case 'd':
			is_deci = 1 ;
			is_hexi = 0 ;
			break ;
		case 'n':
			is_nl = 0 ;
			break ;
		default:
			print_usage(PN) ;
			return 0 ;
		}
	}
	argc -= optind ;
	argv += optind ;	
	if ( argc!=1 ) {
		print_usage(PN) ;
		return 0 ;
	}
	n = atoi(argv[0]) ;

	base = ( is_hexi ) ? 16 : 10 ;

	cp = ( is_hexi ) 
		? "__._.__%06x->!" 
		: "_%06d->!" ;
	for (i=0; i<n/base; i++ ) {
		printf( cp, (i+1)*base ) ;
	}

	cp = ( is_hexi )
		? "%1x" 
		: "%1d" ;
	for (i=0; i<n%base; i++) {
		printf(cp, (i+1)) ;
	}

	if ( is_nl ) printf("\n") ;

	return 0 ;
}

