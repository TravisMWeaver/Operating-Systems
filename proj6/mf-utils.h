/*
 * header file mf-utils.h
 * author: bjr
 * created: 17 nov 2016
 * last-update: 17 nov 2016
 */

struct DirIter {
	int cur_clust ;
	int dir_index ;
} ;

char * dos_filename(char * name) ;
char * unix_filename(char * name) ;
int dos_compare(char * sa, char * sb ) ;

struct DirIter  * create_dir_iter(void) ;
struct Dirent * first_dir(struct DirIter * di, int dir_cluster ) ;
struct Dirent * next_dir(struct DirIter * di) ;
void walk_dir(int clust) ;
