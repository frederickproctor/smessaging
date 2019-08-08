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

#ifndef SERDES_H
#define SERDES_H

#define SOM 0xAB		/* start of message */
#define EOM 0xCD		/* end of message */
#define PAD 0xEF		/* pad around SOM, EOM */

/* normally this would be defined by your application */
#define MSG_MAX 10		/* max size of a message to encode */

#if 1
/* printable chars make debugging easier */
#undef SOM
#undef EOM
#undef PAD
#define SOM '>'
#define EOM '<'
#define PAD '!'
#endif

typedef struct {
  char * encptr;	/* where we last left their input */
  char * decptr;	/* where we last left our output */
  char * decbad;	/* where we can't write our output */
  enum {
    NOMSG = 0,			/* not in a message */
    SOM1,			/* saw a PAD char while in NOMSG */
    SOM2,			/* saw SOM char after PAD */
    MSG,		      /* saw PAD char after SOM, now in msg */
    INPAD,
    INSOM,		 /* saw a SOM char in msg, supress the first*/
    INEOM,
    INEOM1,		       /* saw a PAD char while in a message */
    INEOM2			/* saw an EOM char after a PAD */
  } state;
  int count;
} serdes_decode_state;

/*
  For a given uncoded message length 'msglen', returns how many bytes
  to allocate for its worst-case encoded version.
 */
#define serdes_encode_size(msglen) ((3 * (msglen) + 1)/2 + 3 + 3 + 10)

/*
  For a given encoded message length 'enclen', returns how many bytes
  to allocate for its worst-case decoded version.
 */
#define serdes_decode_size(enclen) ((enclen) - 3 - 3 + 10)

/*
  Given a message in 'msgbuf', and its length 'msglen', encodes it
  into 'encbuf'.

  Returns the length of the encoded message if it doesn't exceed
  'encsize', otherwise -1.
*/
extern int			/* the encoded length */
serdes_encode(const char * msgbuf, /* to be encoded */
	    int msglen,	/* how long the msg in 'msgbuf' is */
	    char * encbuf, /* the encoded result */
	    int encsize);	/* allocated size of 'encbuf' */

/*
  Initializes the decoder state. Returns 0 if successful, otherwise
  non-zero.
*/
extern int
serdes_decode_state_init(serdes_decode_state * state,
		       char * encbuf, /* ptr to encoded buffer */
		       char * decbuf, /* ptr to decoded buffer */
		       int encsize, /* allocated size of encbuf */
		       int decsize); /* allocated size of decbuf */


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
extern int
serdes_decode(char * encbuf, /* the encoded message fragment*/
	    int * enclen,	/* length of encoded message */
	    char * decbuf, /* where the decoded message will go */
	    serdes_decode_state * st); /* saved state between calls */

#endif /* SERDES_H */
