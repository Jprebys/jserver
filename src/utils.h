#ifndef _JP_UTILS_H
#define _JP_UTILS_H

#include <errno.h>
#include <stdlib.h>


void error_and_die(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

#endif  // _JP_UTILS_H