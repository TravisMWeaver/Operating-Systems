#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dos_filenames.h"

/*
 * code snippets
 * author: 
 * date: dec 2011
 *
 */

/*
 * TODO: this code needs to check for illegal characters in the FAT file system;
 *       or for illegal characters as restricted by our modified FAT.
 *       Illegal characters can be mapped to the 0x7e character, "~", because it is
 *       weird to just throw them away and cumbersome to return an "Illegal Filename"
 *       error to the user.
 *       - 16 nov 2014 -bjr
 */
 
char * dos_filename(char * name) {
/* 
 * converts string to 8+3 filename in dirent format.
 * returns static buffer, or NULL if illegal filename
 * beware: 
 *   does not check for illegal characters in dos filename
 *   does not check for illegal character in unix filename
 *   does not convert to uppercase
*/
	static char dos_name[11] ;
	char * s ;
	unsigned int i ;

	memset(dos_name, ' ', 11) ;
	s = strchr(name, '.') ;
	if (!s) {
		s = name + strlen(name) ; 
	}
	/* ASSERT *s == '.' || *s == '\0' */
	i = s-name ;
	if (i>8) return NULL ;
	memcpy(dos_name, name, i ) ;
	if ( *s=='.' ) {
		i = strlen(s+1) ;
		if ( i>3 ) return NULL ;
		memcpy(dos_name+8, s+1, i) ;	
	}
	return dos_name ;
}

char * unix_filename(char * name) {
/*
 * converts a 11 character buffer of a filename in
 * the format of a FAT directory entry to a string
 *
 * returns a pointer to a static buffer
 */

	static char ux_name[13] ;
	int i = 0 ;
	int j ;
	memset(ux_name, '\0', 13) ;
	while ( name[i]!=' ' && i<8 ) {
		ux_name[i] = name[i] ;
		i++ ;
	}
	j = i ;
	i = 8 ;
	if ( name[i] != ' ' ) {
		ux_name[j++] = '.' ;
		while ( name[i]!=' ' && i<11 ) {
			ux_name[j++] = name[i++] ;
		}
	}
	return ux_name ;
}


