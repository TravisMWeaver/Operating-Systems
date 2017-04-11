
/*
 * header file fuseops.h
 * author: bjr
 * created: 17 nov 2016
 * last-update: 17 nov 2016
 */

// stage one operations

int fs_getattr(const char * path, struct stat * stbuf) ;
int fs_readdir(const char * path, void * buf, 
	fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi ) ;
int fs_mkdir( const char * path, mode_t mode ) ;
int fs_rmdir( const char * path ) ;
int fs_create( const char * path, mode_t m, struct fuse_file_info * fi ) ;
int fs_unlink( const char * path ) ;
// touch prefers this implemented 
int fs_utimens( const char * path, const struct timespec tv[2] );

// stage two operations

int fs_truncate(const char *, off_t) ;
int fs_open(const char *, struct fuse_file_info *) ;
int fs_read(const char *, char *, size_t, off_t, struct fuse_file_info *) ;
int fs_write(const char *, const char *, size_t, off_t,struct fuse_file_info *) ;


