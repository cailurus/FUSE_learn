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
