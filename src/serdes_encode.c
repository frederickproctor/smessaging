/*
  This software is in the public domain.

  DISCLAIMER:
  This software was produced by the National Institute of Standards
  and Technology (NIST), an agency of the U.S. government, and by
  statute is not subject to copyright in the United States. Recipients
  of this software assume all responsibility associated with its
  operation, modification, maintenance, and subsequent redistribution.

  See NIST Administration Manual 4.09.07 b and Appendix I. 
*/

/*!
  \file serdes_encode.c

  \brief Encodes stdin and writes to stdout.

  \code CVS Status:
  $Author: proctor $
  $Revision: 1.1.1.1 $
  $Date: 2010/03/15 19:32:55 $
  \endcode

  \author Fred Proctor
*/

#include <stdio.h>		/* fprintf, stderr */
#include <string.h>		/* strerror */
#include <stddef.h>		/* NULL */
#include <errno.h>		/* errno */
#include <ulapi.h>		/* ulapi_fd_new, etc */
#include "serdes.h"		/* these decls */

/*
  Usage:

  Reads from stdin, writes to stdout, e.g., 

  ./serdes_encode < infile | ./serdes_decode > outfile
  diff infile outfile
  # should be identical
*/

int main(int argc, char * argv[])
{
  void *inptr;
  void *outptr;
  enum {READ_SIZE = MSG_MAX};	/* how big a block to read */
  char readbuf[READ_SIZE];
  char writebuf[serdes_encode_size(READ_SIZE)];
  int readlen;
  int writelen;

  ulapi_init();

  inptr = ulapi_fd_new();
  if (NULL == inptr) {
    fprintf(stderr, "Can't create standard input\n");
    return 1;
  }
  if (ULAPI_OK != ulapi_std_open(ULAPI_STDIN, inptr)) {
    fprintf(stderr, "Can't open standard input\n");
    return 1;
  }

  outptr = ulapi_fd_new();
  if (NULL == outptr) {
    fprintf(stderr, "Can't create standard output\n");
    return 1;
  }
  if (ULAPI_OK != ulapi_std_open(ULAPI_STDOUT, outptr)) {
    fprintf(stderr, "Can't open standard output\n");
    return 1;
  }

  for (;;) {
    readlen = ulapi_fd_read(inptr, readbuf, READ_SIZE);
    if (0 == readlen) break;	/* end of file */
    if (0 > readlen) {
      fprintf(stderr, "Error reading input: %s\n", strerror(errno));
      return 1;
    }

    writelen = serdes_encode(readbuf, readlen, writebuf, sizeof(writebuf));
    if (writelen > 0) {
      ulapi_fd_write(outptr, writebuf, writelen);
    }
  }

  return 0;
}
