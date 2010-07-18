/*
 *      $COPYRIGHT$
 *
 *      Software for Humanity
 *      Public Domain
 *
 *      This program is freely distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	$Id: pty.c,v 6.3 1999/06/19 03:19:47 jsquyres Exp $
 *
 *	Function:	- pseudo terminal support
 *			- derived from Steven's "Advanced Programming
 *			  in the Unix Environment".
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "gucefCORE_pty.h"

/*
 * Some OS's don't seem to prototype these functions properly
 */
extern int grantpt( int __fd );
extern int unlockpt( int __fd );
extern char *ptsname( int __fd );


#if HAVE_SYSV_PTYS && HAVE_STROPTS_H

#include <stropts.h>

static int
ptym_open(char *pts_name)
{
    char	*ptr;
    int		fdm;

    strcpy(pts_name, "/dev/ptmx");
    if ((fdm = open(pts_name, O_RDWR)) < 0)
	return(-1);
    if (grantpt(fdm) < 0) {
	close(fdm);
	return(-1);
    }
    if (unlockpt(fdm) < 0) {
	close(fdm);
	return(-1);
    }
    if ((ptr = ptsname(fdm)) == NULL) {
	close(fdm);
	return(-1);
    }

    strcpy(pts_name, ptr);
    return(fdm);
}

static int
ptys_open(int fdm, char *pts_name)
{
    int		fds;

    if ((fds = open(pts_name, O_RDWR)) < 0) {
	close(fdm);
	return(-1);
    }
    if (ioctl(fds, I_PUSH, "ptem") < 0) {
	close(fdm);
	close(fds);
	return(-1);
    }
    if (ioctl(fds, I_PUSH, "ldterm") < 0) {
	close(fdm);
	close(fds);
	return(-1);
    }

    return(fds);
}

#elif HAVE_BSD_PTYS && HAVE_GRP_H

#include <grp.h>

static int
ptym_open(char *pts_name)
{
    int		fdm;
    char	*ptr1, *ptr2;

    strcpy(pts_name, "/dev/ptyXY");
    for (ptr1 = "pqrstuvwxyzPQRST"; *ptr1 != 0; ptr1++) {
	pts_name[8] = *ptr1;
	for (ptr2 = "0123456789abcdef"; *ptr2 != 0; ptr2++) {
	    pts_name[9] = *ptr2;

	    if ((fdm = open(pts_name, O_RDWR)) < 0) {
		if (errno == ENOENT)
		    return(-1);
		else
		    continue;
	    }

	    pts_name[5] = 't';
	    return(fdm);
	}
    }

    return(-1);
}

static int
ptys_open(int fdm, char *pts_name)
{
    struct group	*grptr;
    int			gid, fds;

    gid = (grptr = getgrnam("tty")) ? grptr->gr_gid : -1;
    chown(pts_name, getuid(), gid);
    chmod(pts_name, S_IRUSR | S_IWUSR | S_IWGRP);

    if ((fds = open(pts_name, O_RDWR)) < 0) {
	close(fdm);
	return(-1);
    }

    return(fds);
}

#else

static int
ptym_open(char *pts_name)
{
    return(-1);
}

static int
ptys_open(int fdm, char *pts_name)
{
    return(-1);
}

#endif

int
GUCEF_pty_open(int *fdm, int *fds)
{
    char	pts_name[20];

    if ((*fdm = ptym_open(pts_name)) < 0)
	return 1;

    if ((*fds = ptys_open(*fdm, pts_name)) < 0) {
	close(*fdm);
	return 1;
    }

    return 0;
}
