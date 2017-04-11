/*
#define USE_FUSE_VERSION must come before #include<fuse.h>
*/

#define FUSE_USE_VERSION 30
#include <fuse.h>
#define _XOPEN_SOURCE 700

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

#define LOGFILE_NAME "logfile"
#define EIGHT_PLUS_THREE 11

#include "minifat.h"
#include "dos_filenames.h"

/*
 * program to implement minifat 
 *
 * author: 
 * created: November 12, 2016
 * last-update: November 21, 2016
 */
 
// int is_verbose = 0;
extern int errno; 
  
static struct
{
	void * vd_mm;
	FILE * logfile;
	int is_verbose;
} g;

static int fs_getattr(const char * path, struct stat * stbuf) 
{
	int retValue = 0;

	if (g.is_verbose) 
	{
		fprintf(g.logfile, "getattr: %s\n", path ) ;
		fflush(g.logfile) ;
	}

	memset(stbuf, 0, sizeof(struct stat));

	if(strcmp(path, "/") == 0) 
	{
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }

    else
    {
    	stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = 0;
    }

	retValue = lstat(path, stbuf);

	if(retValue < 0) 
	{
		return -errno;
	}

	return retValue;
}

static int fs_readdir(const char * path, void * buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
	DIR *dp;
	struct dirent *dent;
	char *b;
	int retValue = 0;

	(void) offset;

	if (g.is_verbose) 
	{
		fprintf(g.logfile, "readdir: %s\n", path );
		fflush(g.logfile);
	}

	// b = ((char *) g.vd_mm) + ROOT_DIR_CLUSTER * CLUSTER_SIZE ;
	// dent = (struct dirent *) b ;

	dp = (DIR *) (uintptr_t) fi -> fh;
	dent = readdir(dp);


	if(dent == 0)
	{
		return -errno; 
	}

	while((dent = readdir(dp)) != NULL)
	{
		if(filler(buf, dent -> d_name, NULL, 0) != 0)
		{
			break;
		}
	}

	//closedir(dp);

	return retValue;
}

static int fs_mkdir( const char * path, mode_t mode ) 
{
	int retValue = 0;

	if (g.is_verbose) 
	{
		fprintf(g.logfile, "mkdir: %s\n", path );
		fflush(g.logfile);
	}

	retValue = mkdir(path, mode);

	if(retValue < 0)
	{
		return -errno;
	}

	return retValue;
}

static int fs_rmdir( const char * path ) 
{
	int retValue = 0;

	if (g.is_verbose) 
	{
		fprintf(g.logfile, "rmdir: %s\n", path );
		fflush(g.logfile);
	}

	retValue = rmdir(path);

	if(retValue < 0)
	{
		return -errno;
	}

	return retValue;
}

// open() method used instead of create()

static int fs_create( const char * path, mode_t m, struct fuse_file_info * fi ) 
{
	int retValue = 0;
	int fd;
	char dos_name;

	if (g.is_verbose) 
	{
		fprintf(g.logfile, "create: %s\n", path );
		fflush(g.logfile);
	}

	fd = mknod(path, m, fi -> flags);

	if(retValue < 0)
	{
		return -errno;
	}

	fi -> fh = fd;

	return retValue;
}

static int fs_unlink( const char * path ) 
{
	int retValue = 0;

	if (g.is_verbose) 
	{
		fprintf(g.logfile, "unlink: %s\n", path );
		fflush(g.logfile);
	}

	retValue = unlink(path);

	if(retValue < 0)
	{
		return -errno;
	}

	return retValue;
}

/* Touch prefers this implemented
 * utimensat used instead of utime/utimes because they follow symbolic links
 */

static int fs_utimens( const char * path, const struct timespec tv[2] ) 
{
	if (g.is_verbose) 
	{
		fprintf(g.logfile, "utimens: %s\n", path ) ;
		fflush(g.logfile) ;
	}

	int retValue = 0;

	retValue = utimensat(0, path, tv, AT_SYMLINK_NOFOLLOW);

	if(retValue < 0)
	{
		return -errno;
	}

	return retValue;
}

static struct fuse_operations fuse_oper = 
{
	// call backs for fuse
	.getattr = fs_getattr,
	.readdir = fs_readdir,
	.mkdir   = fs_mkdir,
	.rmdir   = fs_rmdir,
	.create  = fs_create,
	.unlink  = fs_unlink,
	.utimens = fs_utimens,
} ;

void print_usage(void) 
{
	printf("Usage: minifat -cv -d _directory_ _virtual_disk_name_\n");
	return ;
}

/*
void initialize_vd(int vd_id, void * g_vd_mm) 
{
	struct Fat * v_fat;
	int i;
	char * b;
	struct dirent * dent;
	
	// clear FAT table
	v_fat = (struct Fat *) (((char *) g_vd_mm) + FAT_TABLE_CLUSTER * CLUSTER_SIZE);

	for (i=0; i<CLUSTER_NUMBER; i++ ) 
	{
		v_fat->table[i] = 0 ;
	}
	
	// claim block ROOT_DIR_CLUSTER in fat table
	v_fat->table[ROOT_DIR_CLUSTER] = (int_32bit) -1;
	
	// clear root directory
	b = ((char *) g_vd_mm) + ROOT_DIR_CLUSTER * CLUSTER_SIZE;
	for(i=0; i<CLUSTER_SIZE; i++) b[i] = (char) 0;
	
	// make first directory entry
	// directories no longer have parent pointers, nov 2015
	dent = (struct dirent *) b;

	for (i=0; i<sizeof(dent->d_name); i++) 
	{
	  dent->d_name[i] = DIRENT_PARENT_NAME[i];
	}

	//dent->d_attr = DIRENT_ATTR_DIRECTORY;
	//dent->d_firstcluster = (int_32bit) ROOT_DIR_CLUSTER;

	return;
}
*/


int main(int argc, char * argv[]) 
{
	int ch, vd_id;
	char * mount_dir = NULL;
	int is_create = 0;
	char * lf_name = LOGFILE_NAME;
	char * v_disk_name;
	
	assert(sizeof(int_32bit)==4);
	
	while ((ch = getopt(argc, argv, "vcd:")) != -1) 
	{
		switch(ch) 
		{
			case 'v':
				g.is_verbose = 1;
				break;
			case 'c':
				is_create = 1;
				break;
			case 'd':
				mount_dir = strdup(optarg);
				break;
			default:
				print_usage();
				return 0;
		}
	}
	argc -= optind;
	argv += optind;

	if(argc < 1 || !mount_dir) 
	{
		print_usage();
		return 0;
	}

	/*
	if(g.is_verbose) 
	{
		g.logfile = fopen(lf_name,"w");
	}

	vd_id = open(v_disk_name, O_RDWR | O_CREAT, 0777);

	if (vd_id == -1) 
	{
		printf("Error: failed to open file %s, exiting.\n", v_disk_name);
		return 0;
	}

	ftruncate(vd_id, CLUSTER_NUMBER * CLUSTER_SIZE );
	
	g.vd_mm = mmap(NULL, CLUSTER_NUMBER * CLUSTER_SIZE, 
		PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, vd_id, 0 );

	if (g.vd_mm == MAP_FAILED) 
	{
		perror("Error in mmap");
		return 0;
	}
	
	if(is_create) 
	{
		// initialize the virtual disk
		initialize_vd(vd_id, g.vd_mm);
	}
	*/

	if (g.is_verbose) 
	{
		char * argv_fuse[3]  = { "", "-d", mount_dir, NULL };
		return fuse_main(3, argv_fuse, &fuse_oper, NULL);	
	}

	else 
	{
		char * argv_fuse[2]  = { "", mount_dir, NULL };
		return fuse_main(2, argv_fuse, &fuse_oper, NULL);	
	}
	assert(0==1);
}
