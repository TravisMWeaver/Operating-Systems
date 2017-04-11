#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <assert.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>

/*
#define USE_FUSE_VERSION must come before #include<fuse.h>
*/

#define FUSE_USE_VERSION 30
#include <fuse.h>

#include "minifat.h"
#include "fuseops.h"

/*
 * program to implement fuse ops 
 *
 * author: Travis Weaver
 * created: 17 nov 2016
 * last-update: 12/6/16
 */
 
extern int is_verbose ;
extern int errno ;

static void fs_fullpath(char fullpath[PATH_MAX], const char *path)
{
	strcpy(fullpath, FS_DATA -> rootdir);
	strncat(fullpath, path, PATH_MAX);
}

int fs_getattr(const char * path, struct stat * stbuf) 
{	
	int ret;
	char fullpath[PATH_MAX];

	fs_fullpath(fullpath, path);
	ret = lstat(fullpath, statbuf);

	return ret;
}

int fs_readdir(const char * path, void * buf, 
	fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi ) 
{
	struct dirent * dEnt;
	int ret = 0;
	DIR * dp;

	dp = (DIR *) (uintptr_t) fi -> fh;
	dEnt = readdir(dp);

	if(dEnt == 0)
	{
		return ret;
	}

	do
	{
		if(filler(buf, dEnt -> d_name, NULL, 0) != 0)
		{
			return -ENOMEM;
		}
	} while((dEnt = readdir(dp)) != NULL);

	return ret;
}

int fs_mkdir( const char * path, mode_t mode ) 
{
	int ret = 0;
	char fullpath[PATH_MAX];
	fs_fullpath(fullpath, path);
	mkdir(fullpath, mode);

	return ret;
}

int fs_rmdir( const char * path ) 
{
	int ret = 0;
	char fullpath[PATH_MAX];
	fs_fullpath(fullpath, path);
	rmdir(fullpath);

	return ret;
}

int fs_create( const char * path, mode_t m, struct fuse_file_info * fi ) 
{
	int ret = 0;
	char fullpath[PATH_MAX];
	fs_fullpath(fullpath, path);
	int fd = mknod(fullpath, m, fi -> flags);

	if(ret < 0)
	{
		return -errno;
	}

	fi -> fh = fd;

	return ret;
}

int fs_unlink( const char * path ) 
{
	int ret = 0;
	char fullpath[PATH_MAX];
	fs_fullpath(fullpath, path);
	unlink(fullpath);

	return ret;
}

// touch prefers this implemented 
int fs_utime( const char * path, const struct timespec tv[2] ) 
{
	int ret = 0;
	char fullpath[PATH_MAX];
	fs_fullpath(fullpath, path);

	return ret;
}

int fs_truncate(const char * path, off_t offset) 
{	
	int ret = 0;
	char fullpath[PATH_MAX];
	fs_fullpath(fullpath, path);
	truncate(fullpath, offset);

	return ret;
}
	
int fs_open(const char * path, struct fuse_file_info * fi) 
{	
	int ret = 0;
	int fd;
	char fullpath[PATH_MAX];
	fs_fullpath(fullpath, path);

	fd = open(fullpath, fi -> flags);
	fi -> fh = fd;

	return ret;
}

int fs_read(const char * path, char * buf, size_t size, off_t offset, 
	struct fuse_file_info * fi) 
{
	int ret = 0;
	pread(fi -> fh, buf, size, offset);

	return ret;
}

int fs_write(const char * path, const char * buf, size_t size, off_t offset,
	struct fuse_file_info * fi) 
{
	int ret = 0;
	pwrite(fi -> fh, buf, size, offset);

	return ret;
}

