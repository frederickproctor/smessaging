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
  \file serdes_decode.c

  \brief Test application that generates a random stream of SOM, EOM
  and PAD characters to try and break the serdes_encode and
  serdes_decode applications.

  \code CVS Status:
  $Author: proctor $
  $Revision: 1.1.1.1 $
  $Date: 2010/03/15 19:32:55 $
  \endcode

  \author Fred Proctor
*/

#include <stdio.h>		/* fprintf, stderr */
#include <stdlib.h>		/* atoi, rand */
#include <stddef.h>		/* NULL */
#include <unistd.h>		/* read, write, STDIN,OUT_FILENO */
#include "serdes.h"		/* these decls */

/*
  Usage:

  serdes_gen {<number of chars to generate>}

  Writes to stdout.
*/

int main(int argc, char * argv[])
{
  int num;
  int r;
  char ch;

  if (argc < 2) num = 1000000;
  else num = atoi(argv[1]);

  for (; num > 0; num--) {
    r = rand() % 3;
    if (0 == r) ch = PAD;
    else if (1 == r) ch = SOM;
    else ch = EOM;
    write(STDOUT_FILENO, &ch, 1);
  }
    
  return 0;
}
