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
  \file serdes.c

  \brief Definitions of serialization-deserialization functions that
  build delimited messages that ensure data transparency.

  \code CVS Status:
  $Author: proctor $
  $Revision: 1.1.1.1 $
  $Date: 2010/03/15 19:32:55 $
  \endcode

  \author Fred Proctor
*/

#include "serdes.h"		/* these decls */

/*
  Encoding adds a header PAD SOM PAD, the message, then a
  trailer PAD EOM PAD. These delimiters flag the start and end
  of the message for decoding.

  If the pattern PAD SOM+ PAD or PAD EOM+ PAD occurs naturally in the
  message to be encoded, where + means one or more occurrences, it is
  replaced with PAD SOM SOM+ PAD or PAD EOM EOM+ PAD, respectively.
  For example, PAD EOM PAD is replaced with PAD EOM EOM PAD;
  PAD SOM SOM PAD is replaced with PAD SOM SOM SOM PAD. This makes
  it possible to distinguish the header and trailer from any that
  happen to be in the message. The message will be expanded upon
  encoding.

  Letting 0 denote PAD and 1 denote SOM or EOM, we can see message
  expansions

  010 -> 0110
  0110 -> 01110
  01010 -> 0110110
  0110110 -> 011101110

  with the worst case happening for the message 01010..010. For
  n bytes, there will be (n-1)/2 extra bytes added. The expansion
  ratio is then

  ((n-1)/2)/n, or 1/2 - 1/(2n). This approaches 1/2 for large n, so
  the maximum message expansion is 50%, plus the header and trailer.

  50% extra is a factor of 3/2, or rounding up, (3n+1)2, plus 3 bytes
  each for the header and trailer.
*/

/*
  Given a message in 'msgbuf', and its length 'msglen', encodes it
  into 'encbuf'.

  Returns the length of the encoded message if it doesn't exceed
  'encsize', otherwise -1.
*/
int				/* the encoded length */
serdes_encode(const char * msgbuf, /* to be encoded */
	    int msglen,	/* how long the msg in 'msgbuf' is */
	    char * encbuf, /* the encoded result */
	    int encsize)	/* allocated size of 'encbuf' */
{
  const char * msgptr;
  char * encptr;
  char ch;
  enum {INMSG = 0, INPAD, INSOM, INEOM} state;

  /* this aborts if the encoded message length is less than
     the worst-case length, which is more strict than necessary
     but eliminates having to check every byte write */
  if (encsize < serdes_encode_size(msglen)) {
    return -1;
  }

  msgptr = msgbuf;
  encptr = encbuf;
  state = INMSG;

  *encptr++ = PAD;
  *encptr++ = SOM;
  *encptr++ = PAD;

  while (msglen > 0) {
    ch = *msgptr++, msglen--;
    switch (state) {
    case INMSG:
      if (PAD == ch) {
	state = INPAD;
      }
      break;
    case INPAD:
      if (SOM == ch) {
	state = INSOM;
      } else if (EOM == ch) {
	state = INEOM;
      } else if (PAD != ch) {
	state = INMSG;
      }
      break;
    case INSOM:
      if (PAD == ch) {
	*encptr++ = SOM;	/* write an extra one */
	state = INPAD;
      } else if (SOM != ch) {
	state = INMSG;
      }
      break;
    case INEOM:
      if (PAD == ch) {
	*encptr++ = EOM;	/* write an extra one */
	state = INPAD;
      } else if (EOM != ch) {
	state = INMSG;
      }
      break;
    default:
      state = INMSG;
      break;
    }
    *encptr++ = ch;
  }

  /* if the last char was SOM or EOM, dup it so that the first
     PAD of the terminal PAD EOM PAD won't make it look like
     the end of message prematurely */
  if (INSOM == state) *encptr++ = SOM;
  else if (INEOM == state) *encptr++ = EOM;

  *encptr++ = PAD;
  *encptr++ = EOM;
  *encptr++ = PAD;

  return encptr - encbuf;
}

/*
  Initializes the decoder state. Returns 0 if successful, otherwise
  non-zero.
*/
int
serdes_decode_state_init(serdes_decode_state * state,
		       char * encbuf, /* ptr to encoded buffer */
		       char * decbuf, /* ptr to decoded buffer */
		       int encsize, /* allocated size of encbuf */
		       int decsize) /* allocated size of decbuf */
{
  /* no need to save buffer starts, these are passed each decode call */
  state->encptr = encbuf;
  state->decptr = decbuf;
  state->decbad = decbuf + decsize;
  state->state = NOMSG;
  state->count = 0;

  return 0;
}

/*
  serdes_decode takes a fragment of an encoded message stream in
  'encbuf', and its length 'enclen', and decodes the message into
  'decbuf'. If a full message is not found, then it returns 0, and a
  new fragment should be read and this function called again. If a
  full message is found, its length is returned. In cases where the
  encoded buffer contains many messages, it will be left where it was,
  with its state kept in 'st'. Therefore, if the returned value is
  greater than 0, this function should be called again immediately,
  without changing 'encbuf', to get the next message. Only when the
  function returns 0 should a new 'encbuf' be read in.

  Returns 0 if no message has been formed, a positive number for the
  length of the message stored in 'decbuf', or a negative number on
  error.
*/
int
serdes_decode(char * encbuf, /* the encoded message fragment*/
	    int * enclen,	/* length of encoded message */
	    char * decbuf, /* where the decoded message will go */
	    serdes_decode_state * st) /* saved state between calls */
{
  char ch;
  int retval;

  while (*enclen > 0) {
    if (st->decptr >= st->decbad) return -1;
    ch = *(st->encptr)++, (*enclen)--;
    switch (st->state) {
    case NOMSG:
      if (PAD == ch) {
	st->state = INSOM1;
      }
      break;
    case INSOM1:
      if (SOM == ch) {
	st->state = INSOM2;
      } else {
	st->state = NOMSG;
      }
      break;
    case INSOM2:
      if (PAD == ch) {
	st->state = INMSG;
      } else {
	st->state = NOMSG;
      }
      break;
    case INMSG:
      if (PAD == ch) {
	st->state = INPAD;
      } else {
	*(st->decptr)++ = ch;
      }
      break;
    case INPAD:
      if (PAD == ch) {
	*(st->decptr)++ = PAD;
      } else if (SOM == ch) {
	st->count = 1;
	st->state = INSOM;
      } else if (EOM == ch) {
	st->state = INEOM1;
      } else {
	*(st->decptr)++ = PAD;
	*(st->decptr)++ = ch;
	st->state = INMSG;
      }
      break;
    case INSOM:
      if (PAD == ch) {
	*(st->decptr++) = PAD;
	while (st->count-- > 1) *(st->decptr++) = SOM;
	st->state = INPAD;
      } else if (SOM == ch) {
	st->count++;
      } else {
	*(st->decptr++) = PAD;
	while (st->count-- > 0) *(st->decptr++) = SOM;
	*(st->decptr++) = ch;
	st->state = INMSG;
      }
      break;
    case INEOM1:
      if (PAD == ch) {
	/* we're done */
	retval = st->decptr - decbuf;
	st->decptr = decbuf;
	st->state = NOMSG;
	return retval;
      } else if (EOM == ch) {
	st->count = 2;
	st->state = INEOM2;
      } else {
	*(st->decptr++) = PAD;
	*(st->decptr++) = EOM;
	*(st->decptr++) = ch;
	st->state = INMSG;
      }
      break;
    case INEOM2:
      if (PAD == ch) {
	*(st->decptr++) = PAD;
	while (st->count-- > 1) *(st->decptr++) = EOM;
	st->state = INPAD;
      } else if (EOM == ch) {
	st->count++;
      } else {
	*(st->decptr++) = PAD;
	while (st->count-- > 0) *(st->decptr++) = EOM;
	*(st->decptr++) = ch;
	st->state = INMSG;
      }
      break;
    default:
      st->state = NOMSG;
      break;
    }
  }

  /*
    got to the end of the encoded fragment, so reset the encoded
    message pointer to the beginning for the next fragment, but leave
    intact the decoded buffer
  */
  st->encptr = encbuf;

  return 0;
}
