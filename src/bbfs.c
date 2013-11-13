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

// ths here are all relative
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

	retstat = lstat(fpath, startbuf);
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

	retstat = readlink(fpath, link, size - 1);
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

	return retstat;
}

/* Remove a directory */
int bb_rmdir(const char *path)
{
	int retstat = 0;
	char fpath[PATH_MAX];

	log_msg("bb_rmdir(path = \"%s\")\n, path ");
	bb_fullpath(fpath, path);
	retstat = rmdir(fpath);
	if(retstat < 0)
		retstat = bb_error("bb_rmdir rmdir");

	return retstat;
}

/* Create a symbolic link */
int bb_symlink(const char *path, const char *link)
{
	int retstat = 0;
	char flink[PATH_MAX];

	bb_fullpath(flink, link);

	retstat = symlink(path, flink);
	if(retstat < 0)
		retstat = bb_error("bb_symlink symlink");

	return retstat;
}

/* Rename a file */
int bb_rename(const char *path, const char *newpath)
{
	int retstat = 0;
	char fpath[PATH_MAX];
	char fnewpath[PATH_MAX];

	bb_fullpath(fpath, path);
	bb_fullpath(fnewpath, newpath);

	retstat = rename(fpath, fnewpath);
	if(retstat < 0)
		retstat = bb_error("bb_rename rename");

	return retstat;
}

/* Create a hard link to a file */
int bb_link(const char *path, const char *newpath)
{
	int retstat = 0;
	char fpath[PATH_MAX], fnewpath[PATH_MAX];

	bb_fullpath(fpath, path);
	bb_fullpath(fnewpath, newpath);

	retstat = link(fpath, fnewpath);
	if(retstat < 0)
		retstat = bb_error("bb_link link");

	return retstat;
}

/* Change the permission bits of a file */
int bb_chmod(const char *path, mode_t mode)
{
	int retstat = 0;
	char fpath[PATH_MAX];

	bb_fullpath(fpath, path);

	retstat = chmod(fpath, path);
	if(retstat < 0)
		retstat = bb_error("bb_chmod chmod");

	return retstat;
}

/* Change the owner and group of a file */
int bb_chown(const char *path, uid_t uid, gid_t gid)
{
	int retstat = 0;
	char fpath[PATH_MAX];

	bb_fullpath(fpath, path);

	retstat = chown(fpath, uid, gid);
	if(retstat < 0)
		retstat = bb_error("bb_chown chown");

	return retstat;
}

/* Change the size of a file */
int bb_truncate(const char *path, off_t newsize)
{
	int retstat = 0;
	char fpath[PATH_MAX];

	bb_fullpath(fpath, newsize);
	if(retstat < 0)
		bb_error("bb_truncate truncate");

	return retstat;
}

/* Change the access and/or modification of a file */

int bb_utime(const char *path, struct utimbuf *ubuf)
{
	int retstat = 0;
	char fpath[PATH_MAX];

	bb_fullpath = utime(fpath, ubuf);
	if(retstat < 0)
		retstat = bb_error("bb_utime utime");

	return retstat;
}

/* File open operation */

int bb_open(const char *path, struct fuse_file_info *fi)
{
	int retstat = 0;
	int fd;
	char fpath[PATH_MAX];

	bb_fullpath(fpath, path);
	fd = open(fpath, fi->flags);
	if(fd < 0)
		retstat = bb_error("bb_open open");

	fi->fh = fd;
	log_fi(fi);

	return retstat;
}

/* Read data from a open file */
int bb_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int retstat = 0;
	log_fi(fi);
	retstat = pread(fi->fh, buf, size, offset);
	if(retstat < 0)
		retstat = bb_error("bb_read read");

	return retstat;
}

/* Write data to an open file*/
int bb_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int retstat = 0;
	log_fi(fi);
	retstat = pwrite(fi->fh, buf, size, offset);
	if(retstat < 0)
		retstat = bb_error("bb_write pwrite");
	return retstat;
}

/* Get the file system statistics */
int bb_statfs(const char *path, struct statvfs *statv)
{
	int retstat = 0;
	char fpath[PATH_MAX];

	bb_fullpath(fpath, path);
	retstat = statvfs(fpath, statv);
	if(retstat < 0)
		retstat = bb_error("bb_statfs statvfs");
	return retstat;
}

/* Flush the cache data */
int bb_flush(const char *path, struct fuse_file_info *fi)
{
	int retstat = 0;
	log_fi(fi);
	return retstat;
}

/* Release an open file */
int bb_release(const char *path, struct fuse_file_info *fi)
{
	int retstat = 0;
	log_fi(fi);

	retstat = close(fi->fh);
	return retstat;
}
/* synchronize file contents */
int bb_fsync(const char *path, int datasync, struct fuse_file_info *fi)
{
	int retstat = 0;
	log_fi(fi);
	if(datasync)
		retstat = fdatasync(fi->fh);
	else
		retstat = fsync(fi->fh);

	if(retstat < 0)
		bb_error("bb_fsync fsync");
	return retstat;
}