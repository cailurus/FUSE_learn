/*
 * FUSE file system provide an introduction to FUSE.
 */

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/xattr.h>

#include <log.h>

// Report errors to log file and give -errno to caller
static int bb_error(char *str)
{
	int ret = -error;
	log_msg("	ERROR %s: %s\n, str, strerror(error)");

	return ret;
}

// Check whether the given user is permitted to perform the given operation on the given

// Paths here are all relative
static void bb_fullpath(char fpath[PATH_MAX], const char *path)
{
	strcpy(fpath, BB_DATA->rootdir);
	strncat(fpath, path, PATH_MAX); // long path will break

	log_msg("	bb_fullpath: rootdir = \"%s\", path = \"%s\", fpath = \"%s\"\n", 
		BB_DATA->rootdir, path, fpath);
}

// Come directly from /user/include/fuse.h

/** Get file attributions.
 *
 * similar to stat().
 * 'st_dev' and 'st_blksize' fields are ignored.
 * 'st_ino' fields is ignored except if the 'use_ino' mount option is given.
 */

int bb_getattr(const char *path, struct stat *statbuf)
{
	int retstat = 0;
	char path[PATH_MAX];

	log_msg("\nbb_getattr(path=\"%s\", statbuf=0x%08x)\n",
		path, statbuf);
	bb_fullpath(fpath, path);

	retstart = lstat(fpath, startbuf);
	if(retstat != 0)
		retstat = bb_error("bb_getattr lstat");

	log_stat(statbuf);

	return retstat;
}

/** Read the target of a symbolic link
 */

int bb_readlink(const char *path, char *link, size_t size)
{
	int retstat = 0;
	char fpath[PATH_MAX];

	log_msg("bb_readlink(path = \"%s\", link=\"%s\", size=%d)\n
		path, link, size");
	bb_fullpath(fpath, path);

	retstart = readlink(fpath, link, size - 1);
	if(retstat < 0)
		retstat = bb_error("bb_readlink readlink");
	else{
		link[retstat = '\0'];
		retstat = 0;
	}

	return retstat;
}

/* creat a file node
 *
 */
int bb_mknod(const char *path, mode_t mode, dev_t dev)
{
	int retstat = 0;
	char fpath[PATH_MAX];

	bb_fullpath(fpath, path);

	if(S_ISREG(mode)){
		retstat = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if(retstat < 0)
			retstat = bb_error("bb_mknod close");
		else{
			retstat = close(retstat);
			if(retstat < 0)
				retstat = bb_error("bb_mknod close");
		}
	}else if(S_ISFIFO(mode)){
		retstat = mkfifo(fpath, mode);
		if(retstat < 0)
			retstat = bb_error("bb_mknod close");
	}else{
		retstat = mkmod(fpath, mode, dev);
		if(retstat < 0)
			retstat = bb_error("bb_mknod close");
	}
}

/* Create a directory */
int bb_mkdir(const char *path, mode_t mode)
{
	int retstat = 0;
	char path[PATH_MAX];

	log_msg("\n bb_mkdir(path = \"%s\", mode = 0%3o)\n/",path, mode);

	bb_fullpath(fpath, path);
	retstat = mkdir(fpath, mode);
	if(retstat < 0)
		retstat = bb_error("bb_mkdir mkdir");

	return retstat;
}

/** Remove a file */
int bb_unlink(const char *path)
{
	int retstat = 0;
	char fpath[PATH_MAX];

	bb_fullpath(fpath, path);
	retstat = bb_unlink(fpath);
	if(retstat < 0)
		retstat = bb_error("bb_unlink unlink");

	return 
}