
/*
 * header file for mini-fat project
 * author: bjr
 * created: november 2014
 * last-update: 17 nov 2014
 */

#include <limits.h>
#include <stdio.h>

#define CLUSTER_SIZE 1024
#define CLUSTER_NUMBER 1024

#define FAT_TABLE_CLUSTER 1
#define ROOT_DIR_CLUSTER 5

/* int_32bit is typdef'ed according to architecture */

typedef int int_32bit ;

extern int is_verbose ;
extern void * vd_mm ; // base of virtual disk

/* 
   Note: you don't have to use all or even any of these structs.
   They are here to support the mini-fat documentation with examples.
*/

#define FAT_TABLE_EOF -1
#define FAT_TABLE_EOC -1
#define FAT_TABLE_FREE 0

struct Fat {
	int_32bit table[CLUSTER_NUMBER] ;
} ;

struct Cluster {
	char bytes[CLUSTER_SIZE] ;
} ;

struct Disk {
	struct Cluster clusters[CLUSTER_NUMBER] ;
} ;

#define DIRENT_ATTR_READ_ONLY 0x01 
#define DIRENT_ATTR_HIDDEN 0x02 
#define DIRENT_ATTR_DIRECTORY 0x10 

#define DIRENT_PARENT_NAME "..         "  /* warning! this is 12 bytes long, not 11 */

struct Dirent {
	char dir_name[11] ;
	char dir_attr ;
	char dir_reserved[12] ;
	int_32bit dir_firstcluster ;
	int_32bit dir_filesize ;
} ;

struct Directory {
	struct Dirent directories[1] ; /* actually a sequence of struct Dirents */
} ;

struct fs_state 
{
    FILE *logfile;
    char *rootdir;
};

#define FS_DATA ((struct fs_state *) fuse_get_context()->private_data)


