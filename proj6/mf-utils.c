#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "minifat.h"
#include "mf-utils.h"

extern void * vd_mm ; // base of virtual disk

/*
 * code snippets
 * author: burt
 * date: dec 2011
 * last update: 17 nov 2016
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


int dos_compare(char * sa, char * sb ) {
	return 0 ;
}


/*
 * implement iterator to walk a MiniFat directory
 */

static char * cluster_cs(int clust) {
	return ((char *)vd_mm) + CLUSTER_SIZE*clust ;
}

struct  DirIter  * create_dir_iter(void) {
	struct DirIter * di ;
	di = (struct DirIter *) malloc(sizeof(struct DirIter)) ;
	if ( !di ) return NULL ;
	di->cur_clust = 0 ;
	di->dir_index = 0 ;
	return di ;
}

struct Dirent * first_dir(struct DirIter * di, int dir_cluster ) {
	di->cur_clust = dir_cluster ;
	di->dir_index = 0 ;
	return (struct Dirent *) (
		cluster_cs(di->cur_clust) + sizeof(struct Dirent) * di->dir_index ) ;
}

struct Dirent * next_dir(struct DirIter * di) {

	// increment dir_index; 
	// if equal to CLUSTER_SIZE/sizeof(struct Dirent) then 
	//    update cur_clust
	//    set dir_index to 0
	
	return (struct Dirent *) (
		cluster_cs(di->cur_clust) + sizeof(struct Dirent) * di->dir_index ) ;
}

void walk_dir(int clust) {
	/* illustratation of the DirIter interator */
	struct DirIter di ;
	struct Dirent * de ;
	de = first_dir(&di, clust) ;
	while (de) {
		de = next_dir(&di) ;
	}
	return ;
}


