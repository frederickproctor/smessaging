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

#include <stdio.h>		/* sprintf */
#include <stddef.h>		/* size_t, sizeof */
#include <stdarg.h>		/* va_list */
#include <string.h>		/* memset */
#include <math.h>		/* M_PI */
#include <stdlib.h>		/* malloc, free */
#include <ulapi.h>
#include "serdes.h"		/* encoding, decoding */
#include "smsg.h"

const char *smsg_id_to_string(int id) {
  switch (id) {
  case SMSG_CODE_REQUEST_DYNREG: return "REQUEST_DYNREG";
  case SMSG_CODE_REPLY_DYNREG: return "REPLY_DYNREG";
  case SMSG_CODE_QUERY_DYNREG: return "QUERY_DYNREG";
  case SMSG_CODE_REPORT_DYNREG: return "REPORT_DYNREG";
  case SMSG_CODE_QUERY_ALLREG: return "QUERY_ALLREG";
  case SMSG_CODE_REPORT_ALLREG: return "REPORT_ALLREG";
  case SMSG_CODE_OPEN_CLIENT_CONNECTION: return "OPEN_CLIENT_CONNECTION";
  case SMSG_CODE_RETURN_CLIENT_CONNECTION: return "RETURN_CLIENT_CONNECTION";
  case SMSG_CODE_CLOSE_CLIENT_CONNECTION: return "CLOSE_CLIENT_CONNECTION";
  case SMSG_CODE_OPEN_SERVER_CONNECTION: return "OPEN_SERVER_CONNECTION";
  case SMSG_CODE_RETURN_SERVER_CONNECTION: return "RETURN_SERVER_CONNECTION";
  case SMSG_CODE_CLOSE_SERVER_CONNECTION: return "CLOSE_SERVER_CONNECTION";
  case SMSG_CODE_QUERY_TEST: return "QUERY_TEST";
  case SMSG_CODE_REPORT_TEST: return "REPORT_TEST";
  default: return "?";
  }
  return "?";
}

int smsg_message_to_request_dynreg(smsg_byte * msg, smsg_request_dynreg_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);
  T_FR_B(&smsg_msg->component_id, msg);
  T_FR_B(&smsg_msg->instance_id, msg);
  T_FR_B(&smsg_msg->node_id, msg);
  T_FR_B(&smsg_msg->subsystem_id, msg);

  return smsg_msg->identifier != SMSG_CODE_REQUEST_DYNREG;
}

int smsg_request_dynreg_to_message(smsg_request_dynreg_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_REQUEST_DYNREG;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);
  T_TO_B(&smsg_msg->component_id, msg);
  T_TO_B(&smsg_msg->instance_id, msg);
  T_TO_B(&smsg_msg->node_id, msg);
  T_TO_B(&smsg_msg->subsystem_id, msg);

  return msg - start;
}

int smsg_message_to_reply_dynreg(smsg_byte * msg, smsg_reply_dynreg_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);
  T_FR_B(&smsg_msg->component_id, msg);
  T_FR_B(&smsg_msg->instance_id, msg);
  T_FR_B(&smsg_msg->node_id, msg);
  T_FR_B(&smsg_msg->subsystem_id, msg);
  T_FR_B(&smsg_msg->address, msg);
  T_FR_B(&smsg_msg->port, msg);

  return smsg_msg->identifier != SMSG_CODE_REPLY_DYNREG;
}

int smsg_reply_dynreg_to_message(smsg_reply_dynreg_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_REPLY_DYNREG;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);
  T_TO_B(&smsg_msg->component_id, msg);
  T_TO_B(&smsg_msg->instance_id, msg);
  T_TO_B(&smsg_msg->node_id, msg);
  T_TO_B(&smsg_msg->subsystem_id, msg);
  T_TO_B(&smsg_msg->address, msg);
  T_TO_B(&smsg_msg->port, msg);

  return msg - start;
}

int smsg_message_to_query_dynreg(smsg_byte * msg, smsg_query_dynreg_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);
  T_FR_B(&smsg_msg->component_id, msg);
  T_FR_B(&smsg_msg->instance_id, msg);
  T_FR_B(&smsg_msg->node_id, msg);
  T_FR_B(&smsg_msg->subsystem_id, msg);

  return smsg_msg->identifier != SMSG_CODE_QUERY_DYNREG;
}

int smsg_query_dynreg_to_message(smsg_query_dynreg_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_QUERY_DYNREG;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);
  T_TO_B(&smsg_msg->component_id, msg);
  T_TO_B(&smsg_msg->instance_id, msg);
  T_TO_B(&smsg_msg->node_id, msg);
  T_TO_B(&smsg_msg->subsystem_id, msg);

  return msg - start;
}

int smsg_message_to_report_dynreg(smsg_byte * msg, smsg_report_dynreg_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);
  T_FR_B(&smsg_msg->component_id, msg);
  T_FR_B(&smsg_msg->instance_id, msg);
  T_FR_B(&smsg_msg->node_id, msg);
  T_FR_B(&smsg_msg->subsystem_id, msg);
  T_FR_B(&smsg_msg->address, msg);
  T_FR_B(&smsg_msg->port, msg);

  return smsg_msg->identifier != SMSG_CODE_REPORT_DYNREG;
}

int smsg_report_dynreg_to_message(smsg_report_dynreg_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_REPORT_DYNREG;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);
  T_TO_B(&smsg_msg->component_id, msg);
  T_TO_B(&smsg_msg->instance_id, msg);
  T_TO_B(&smsg_msg->node_id, msg);
  T_TO_B(&smsg_msg->subsystem_id, msg);
  T_TO_B(&smsg_msg->address, msg);
  T_TO_B(&smsg_msg->port, msg);

  return msg - start;
}

int smsg_message_to_query_allreg(smsg_byte * msg, smsg_query_allreg_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);

  return smsg_msg->identifier != SMSG_CODE_QUERY_ALLREG;
}

int smsg_query_allreg_to_message(smsg_query_allreg_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_QUERY_ALLREG;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);

  return msg - start;
}

int smsg_message_to_report_allreg(smsg_byte * msg, smsg_report_allreg_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);
  T_FR_B(&smsg_msg->component_id, msg);
  T_FR_B(&smsg_msg->instance_id, msg);
  T_FR_B(&smsg_msg->node_id, msg);
  T_FR_B(&smsg_msg->subsystem_id, msg);
  T_FR_B(&smsg_msg->address, msg);
  T_FR_B(&smsg_msg->port, msg);

  return smsg_msg->identifier != SMSG_CODE_REPORT_ALLREG;
}

int smsg_report_allreg_to_message(smsg_report_allreg_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_REPORT_ALLREG;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);
  T_TO_B(&smsg_msg->component_id, msg);
  T_TO_B(&smsg_msg->instance_id, msg);
  T_TO_B(&smsg_msg->node_id, msg);
  T_TO_B(&smsg_msg->subsystem_id, msg);
  T_TO_B(&smsg_msg->address, msg);
  T_TO_B(&smsg_msg->port, msg);

  return msg - start;
}

int smsg_message_to_open_client_connection(smsg_byte * msg, smsg_open_client_connection_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);
  T_FR_B(&smsg_msg->address, msg);
  T_FR_B(&smsg_msg->port, msg);

  return smsg_msg->identifier != SMSG_CODE_OPEN_CLIENT_CONNECTION;
}

int smsg_open_client_connection_to_message(smsg_open_client_connection_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_OPEN_CLIENT_CONNECTION;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);
  T_TO_B(&smsg_msg->address, msg);
  T_TO_B(&smsg_msg->port, msg);

  return msg - start;
}

int smsg_message_to_return_client_connection(smsg_byte * msg, smsg_return_client_connection_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);
  T_FR_B(&smsg_msg->connection, msg);

  return smsg_msg->identifier != SMSG_CODE_RETURN_CLIENT_CONNECTION;
}

int smsg_return_client_connection_to_message(smsg_return_client_connection_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_RETURN_CLIENT_CONNECTION;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);
  T_TO_B(&smsg_msg->connection, msg);

  return msg - start;
}

int smsg_message_to_open_server_connection(smsg_byte * msg, smsg_open_server_connection_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);
  T_FR_B(&smsg_msg->address, msg);
  T_FR_B(&smsg_msg->port, msg);

  return smsg_msg->identifier != SMSG_CODE_OPEN_SERVER_CONNECTION;
}

int smsg_open_server_connection_to_message(smsg_open_server_connection_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_OPEN_SERVER_CONNECTION;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);
  T_TO_B(&smsg_msg->address, msg);
  T_TO_B(&smsg_msg->port, msg);

  return msg - start;
}

int smsg_message_to_return_server_connection(smsg_byte * msg, smsg_return_server_connection_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);
  T_FR_B(&smsg_msg->connection, msg);

  return smsg_msg->identifier != SMSG_CODE_RETURN_SERVER_CONNECTION;
}

int smsg_return_server_connection_to_message(smsg_return_server_connection_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_RETURN_SERVER_CONNECTION;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);
  T_TO_B(&smsg_msg->connection, msg);

  return msg - start;
}

int smsg_message_to_query_test(smsg_byte * msg, smsg_query_test_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);

  return smsg_msg->identifier != SMSG_CODE_QUERY_TEST;
}

int smsg_query_test_to_message(smsg_query_test_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_QUERY_TEST;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);

  return msg - start;
}

int smsg_message_to_report_test(smsg_byte * msg, smsg_report_test_t * smsg_msg)
{
  T_FR_B(&smsg_msg->identifier, msg);
  T_FR_B(&smsg_msg->sequence_number, msg);
  T_FR_B(&smsg_msg->count, msg);
  T_FR_B(&smsg_msg->time, msg);

  return smsg_msg->identifier != SMSG_CODE_REPORT_TEST;
}

int smsg_report_test_to_message(smsg_report_test_t * smsg_msg, smsg_byte * msg)
{
  smsg_byte *start = msg;
  smsg_byte identifier = SMSG_CODE_REPORT_TEST;

  T_TO_B(&identifier, msg);
  T_TO_B(&smsg_msg->sequence_number, msg);
  T_TO_B(&smsg_msg->count, msg);
  T_TO_B(&smsg_msg->time, msg);

  return msg - start;
}

unsigned int smsg_debug_mask = 0;
char * smsg_debug_name = "Smsg";

/* sets the local included copy of smsg_debug_mask */
void
smsg_set_debug_mask(unsigned int mask)
{
  smsg_debug_mask = mask;
}

void
smsg_set_debug_name(char * name)
{
  smsg_debug_name = name;
}

void
smsg_print_debug(unsigned int mask, const char * fmt, ...)
{
  va_list ap;

  if (mask & smsg_debug_mask) {
    va_start(ap, fmt);
    fprintf(stderr, "%s: ", smsg_debug_name);
    vfprintf(stderr, fmt, ap);
    fflush(stderr);
    va_end(ap);
  }
}

void my_free(void * ptr)
{
  free(ptr);
  smsg_print_debug(SMSG_DEBUG_MEM, "free(0x%X)\n", (int) ptr);
}

void * my_malloc(size_t size)
{
  void * ret;

  ret = malloc(size);

  smsg_print_debug(SMSG_DEBUG_MEM, "malloc(%d) = 0x%X\n", (int) size, (int) ret);

  return ret;
}

void * my_realloc(void * ptr, size_t size)
{
  void * ret;

  ret = realloc(ptr, size);
  smsg_print_debug(SMSG_DEBUG_MEM, "realloc(0x%X, %d) = 0x%X\n", (int) ptr, (int) size, (int) ret);

  return ret;
}

/*
  Given:

  int i = 0xABCDEFGH;
  char * c[sizeof(i)] = &i;

  Big-endian is

  c[3] c[2] c[1] c[0]
  0xGH 0xEF 0xCD 0xAB

  Little-endian is

  c[3] c[2] c[1] c[0]
  0xAB 0xCD 0xEF 0xGH
*/

void * fwdcpy(void * dest, const void * src, size_t n)
{
  size_t i;

  for (i = 0; i < n; i++) ((char *) dest)[i] = ((char *) src)[i];

  return dest;
}

void * revcpy(void * dest, const void * src, size_t n)
{
  size_t i, nm1;

  for (i = 0, nm1 = n-1; i < n; i++, nm1--) ((char *) dest)[i] = ((char *) src)[nm1];

  return dest;
}

int
db_init(component_db_t * db)
{
  db->mutex = ulapi_mutex_new(0);
  ulapi_mutex_give(db->mutex);
  db->entries = my_malloc(sizeof(component_db_t));
  db->size = 1;
  db->index = 0;
  db->port = SMSG_PORT_BASE;

  return 0;
}

int
db_free(component_db_t * db)
{
  if (NULL != db->entries) {
    my_free(db->entries);
    db->entries = NULL;
  }
  db->size = 0;
  db->index = 0;		/* no value is good here */
  ulapi_mutex_delete(db->mutex);
  return 0;
}

/*
  Fill in entry.component,instance,node,subsystem_id and pass pointer.
  If it's in the DB, the rest will be filled in and the non-negative
  index of the entry is returned.  Otherwise, a negative value is
  returned and entry is left alone.
*/
int
db_find(component_db_t * db, component_entry_t * entry)
{
  int index, retval;

  retval = -1;

  ulapi_mutex_take(db->mutex);
  for (index = 0; index < db->index; index++) {
    if (db->entries[index].component_id == entry->component_id &&
	db->entries[index].instance_id == entry->instance_id &&
	db->entries[index].node_id == entry->node_id &&
	db->entries[index].subsystem_id == entry->subsystem_id) {
      entry->address = db->entries[index].address;
      entry->port = db->entries[index].port;
      entry->fd = db->entries[index].fd;
      retval = index;
      break;
    }
  }
  ulapi_mutex_give(db->mutex);

  return retval;
}

/*
  Given 'fd', look up the entry at that fd.  If it's in the DB,
  'entry' will be filled in and the non-negative index of the entry is
  returned.  Otherwise, a negative value is returned and entry is left
  alone.
*/
int
db_find_fd(component_db_t * db, int fd, component_entry_t * entry)
{
  int index, retval;

  retval = -1;

  ulapi_mutex_take(db->mutex);
  for (index = 0; index < db->index; index++) {
    if (db->entries[index].fd == fd) {
      *entry = db->entries[index];
      retval = index;
      break;
    }
  }
  ulapi_mutex_give(db->mutex);

  return retval;
}

/*
  Add this entry to the database. If it's already in the database,
  the non-negative index of the entry is returned. Otherwise any
  zero address or port will be filled in, and the index returned.
  On error, -1 is returned.
*/
int
db_add(component_db_t * db, component_entry_t * entry)
{
  int retval = -1;

  if (0 > db_find(db, entry)) {
    /* not in our database */
    ulapi_mutex_take(db->mutex);
    if (db->index >= db->size) {
      if (0 == db->size) {
	db->size = 1;
	db->entries = my_malloc(db->size * sizeof(*entry));
      } else {
	db->size *= 2;
	db->entries = my_realloc(db->entries, db->size * sizeof(*entry));
	smsg_print_debug(SMSG_DEBUG_DB, "Grew db to %d entries\n", db->size);
      }
      if (NULL == db->entries) {
	db->size = 0;
	db->index = 0;
	retval = -1;
	smsg_print_debug(SMSG_DEBUG_DB, "Can't grow database\n");
      } else {
	if (0 == entry->address) entry->address = ulapi_get_host_address();
	if (0 == entry->port) entry->port = db->port++;
	retval = db->index;
	db->entries[db->index++] = *entry;
	smsg_print_debug(SMSG_DEBUG_DB, "Added to db with component %d %d %d %d %s %d %d\n", 
		(int) entry->component_id,
		(int) entry->instance_id,
		(int) entry->node_id,
		(int) entry->subsystem_id,
		ulapi_address_to_hostname(entry->address),
		(int) entry->port,
		(int) entry->fd);
      }
    } else {
      if (0 == entry->address) entry->address = ulapi_get_host_address();
      if (0 == entry->port) entry->port = db->port++;
      retval = db->index;
      db->entries[db->index++] = *entry;
      smsg_print_debug(SMSG_DEBUG_DB, "Added to db with component %d %d %d %d %s %d %d\n", 
	      (int) entry->component_id,
	      (int) entry->instance_id,
	      (int) entry->node_id,
	      (int) entry->subsystem_id,
	      ulapi_address_to_hostname(entry->address),
	      (int) entry->port,
	      (int) entry->fd);
    }
    ulapi_mutex_give(db->mutex);
  } else {
      smsg_print_debug(SMSG_DEBUG_DB, "Already have db entry for component %d %d %d %d %s %d %d\n", 
	      (int) entry->component_id,
	      (int) entry->instance_id,
	      (int) entry->node_id,
	      (int) entry->subsystem_id,
	      ulapi_address_to_hostname(entry->address),
	      (int) entry->port,
	      (int) entry->fd);
  }

  return retval;
}

/*
  Given 'fd', add this entry to the database. If it's already in the
  database, the non-negative index of the entry is returned. Otherwise
  any zero address or port will be filled in, and the index returned.
  On error, -1 is returned.
*/
int
db_add_fd(component_db_t * db, int fd, component_entry_t * entry)
{
  int retval = -1;

  if (0 > db_find_fd(db, fd, entry)) {
    /* not in our database */
    ulapi_mutex_take(db->mutex);
    if (db->index >= db->size) {
      if (0 == db->size) {
	db->size = 1;
	db->entries = my_malloc(db->size * sizeof(*entry));
      } else {
	db->size *= 2;
	db->entries = my_realloc(db->entries, db->size * sizeof(*entry));
	smsg_print_debug(SMSG_DEBUG_DB, "Grew db to %d entries\n", db->size);
      }
      if (NULL == db->entries) {
	db->size = 0;
	db->index = 0;
	retval = -1;
	smsg_print_debug(SMSG_DEBUG_DB, "Can't grow database\n");
      } else {
	if (0 == entry->address) entry->address = ulapi_get_host_address();
	if (0 == entry->port) entry->port = db->port++;
	retval = db->index;
	db->entries[db->index++] = *entry;
	smsg_print_debug(SMSG_DEBUG_DB, "Added to db with component %d %d %d %d %s %d %d\n", 
		(int) entry->component_id,
		(int) entry->instance_id,
		(int) entry->node_id,
		(int) entry->subsystem_id,
		ulapi_address_to_hostname(entry->address),
		(int) entry->port,
		(int) entry->fd);
      }
    } else {
      if (0 == entry->address) entry->address = ulapi_get_host_address();
      if (0 == entry->port) entry->port = db->port++;
      retval = db->index;
      db->entries[db->index++] = *entry;
      smsg_print_debug(SMSG_DEBUG_DB, "Added to db with component %d %d %d %d %s %d %d\n", 
	      (int) entry->component_id,
	      (int) entry->instance_id,
	      (int) entry->node_id,
	      (int) entry->subsystem_id,
	      ulapi_address_to_hostname(entry->address),
	      (int) entry->port,
	      (int) entry->fd);
    }
    ulapi_mutex_give(db->mutex);
  } else {
      smsg_print_debug(SMSG_DEBUG_DB, "Already have db entry for component %d %d %d %d %s %d %d\n", 
	      (int) entry->component_id,
	      (int) entry->instance_id,
	      (int) entry->node_id,
	      (int) entry->subsystem_id,
	      ulapi_address_to_hostname(entry->address),
	      (int) entry->port,
	      (int) entry->fd);
  }

  return retval;
}

/*
  Pass an index, and the entry will be filled in and the index returned.
  If there is no entry at that index, the entry is left alone and a 
  negative value is returned.
*/
int
db_lookup(component_db_t * db, int index, component_entry_t * entry)
{
  int retval = -1;

  ulapi_mutex_take(db->mutex);

  if (index < 0 || index >= db->index) {
    retval = -1;
  } else {
    *entry = db->entries[index];
    retval = index;
  }

  ulapi_mutex_give(db->mutex);

  return retval;
}

/*
  Returns index of last entry, -1 if empty
*/
int
db_last(component_db_t * db)
{
  int index;

  ulapi_mutex_take(db->mutex);

  index = db->index - 1;

  ulapi_mutex_give(db->mutex);

  return index;
}

int smsg_register_component(int fd, /* if >= 0, the proxy fd */
			    smsg_byte component_id,
			    smsg_byte instance_id,
			    smsg_byte node_id,
			    smsg_byte subsystem_id,
			    smsg_addr * host_addr,
			    smsg_port * component_port)
{
  int proxy;
  int port = SMSG_PORT;
  char host[] = "127.0.0.1";

  /* reading, decoding and unpacking smsg messages */
  enum {READ_SIZE = 80};	/* how big a block to read */
  char readbuf[READ_SIZE]; /* into here */
  int readlen;			/* how many chars were read */
  serdes_decode_state state;	/* decoder */
  smsg_byte smsg_inbuf[SMSG_INBUFSIZE];	/* decoded and packed smsg message */
  int smsg_inbuflen;		/* how big smsg_inbuf was decoded to be */

  /* packing, encoding and writing smsg messages */
  smsg_byte smsg_outbuf[SMSG_MAX_MESSAGE_SIZE];	/* packed smsg message */
  int smsg_outbuflen;		/* how big smsg_outbuf was packed to be */
  char writebuf[SMSG_WRITEBUFSIZE];	/* encoded message */
  int writebuflen;		/* how big writebuf was encoded to be */

  /* messages we'll send and receive */
  smsg_request_dynreg_t request_dynreg;
  smsg_reply_dynreg_t reply_dynreg;

  proxy = (fd >= 0 ? 1 : 0);

#define RETURN(r) \
  if (! proxy && 0 <= fd) ulapi_socket_close(fd); \
  return (r)

  /* initialize the decoder */
  if (0 != serdes_decode_state_init(&state, readbuf, (char *) smsg_inbuf, READ_SIZE, SMSG_INBUFSIZE)) {
    RETURN(-1);
  }

  /* open connection to node manager */
  if (! proxy) {
    fd = ulapi_socket_get_client_id(port, host);
    if (0 > fd) {
      RETURN(-1);
    }
  }

  /* send a request to register this component and instance */
  request_dynreg.identifier = SMSG_CODE_REQUEST_DYNREG;
  request_dynreg.sequence_number = 1;
  request_dynreg.component_id = component_id;
  request_dynreg.instance_id = instance_id;
  request_dynreg.node_id = node_id;
  request_dynreg.subsystem_id = subsystem_id;
  smsg_outbuflen = smsg_request_dynreg_to_message(&request_dynreg, smsg_outbuf);
  writebuflen = serdes_encode((char *) smsg_outbuf, smsg_outbuflen, writebuf, SMSG_WRITEBUFSIZE);
  ulapi_socket_write(fd, writebuf, writebuflen);

  /* receive a reply */
  for (;;) {
    readlen = ulapi_socket_read(fd, readbuf, READ_SIZE);
    if (0 == readlen) break;	/* end of file */
    if (0 > readlen) {
      RETURN(-1);
    }
    /* try to form a full message */
    for (;;) {
      smsg_inbuflen = serdes_decode(readbuf, &readlen, (char *) smsg_inbuf, &state);
      if (0 == smsg_inbuflen) break; /* not a full message yet */
      if (0 > smsg_inbuflen) {	/* decoding error */
	RETURN(-1);
      }
      /* else we got a full message */
      if (SMSG_CODE_REPLY_DYNREG == smsg_message_identifier(smsg_inbuf)) {
	smsg_message_to_reply_dynreg(smsg_inbuf, &reply_dynreg);
	*host_addr = reply_dynreg.address;
	*component_port = reply_dynreg.port;
	RETURN(0);
      }
      /* else go something else */
      RETURN(-1);
    }
  }

  RETURN(-1);
#undef RETURN
}

int smsg_find_component(int fd, /* if >= 0, the proxy fd */
			smsg_byte component_id, /* what you are */
			smsg_byte instance_id, /* what instance */
			smsg_byte node_id, /* what node */
			smsg_byte subsystem_id, /* what subsystem */
			smsg_addr * host_addr, /* filled in with host */
			smsg_port * component_port) /* filled in with port */
{
  int proxy;
  int port = SMSG_PORT;
  char host[] = "127.0.0.1";

  /* reading, decoding and unpacking smsg messages */
  enum {READ_SIZE = 80};	/* how big a block to read */
  char readbuf[READ_SIZE]; /* into here */
  int readlen;			/* how many chars were read */
  serdes_decode_state state;	/* decoder */
  smsg_byte smsg_inbuf[SMSG_INBUFSIZE];	/* decoded and packed smsg message */
  int smsg_inbuflen;		/* how big smsg_inbuf was decoded to be */

  /* packing, encoding and writing smsg messages */
  smsg_byte smsg_outbuf[SMSG_MAX_MESSAGE_SIZE];	/* packed smsg message */
  int smsg_outbuflen;		/* how big smsg_outbuf was packed to be */
  char writebuf[SMSG_WRITEBUFSIZE];	/* encoded message */
  int writebuflen;		/* how big writebuf was encoded to be */

  /* messages we'll send and receive */
  smsg_query_dynreg_t query_dynreg;
  smsg_report_dynreg_t report_dynreg;

  proxy = (fd >= 0 ? 1 : 0);

#define RETURN(r) \
  if (! proxy && 0 <= fd) ulapi_socket_close(fd); \
  return (r)

  /* initialize the decoder */
  if (0 != serdes_decode_state_init(&state, readbuf, (char *) smsg_inbuf, READ_SIZE, SMSG_INBUFSIZE)) {
    RETURN(-1);
  }

  /* open connection to node manager */
  if (! proxy) {
    fd = ulapi_socket_get_client_id(port, host);
    if (0 > fd) {
      RETURN(-1);
    }
  }

  /* send a query to find this component */
  query_dynreg.identifier = SMSG_CODE_QUERY_DYNREG;
  query_dynreg.sequence_number = 1;
  query_dynreg.component_id = component_id;
  query_dynreg.instance_id = instance_id;
  query_dynreg.node_id = node_id;
  query_dynreg.subsystem_id = subsystem_id;
  smsg_outbuflen = smsg_query_dynreg_to_message(&query_dynreg, smsg_outbuf);
  writebuflen = serdes_encode((char *) smsg_outbuf, smsg_outbuflen, writebuf, SMSG_WRITEBUFSIZE);
  ulapi_socket_write(fd, writebuf, writebuflen);

  /* loop to receive message pieces and build a full message */
  for (;;) {
    readlen = ulapi_socket_read(fd, readbuf, READ_SIZE);
    if (0 == readlen) break;	/* end of file */
    if (0 > readlen) {
      RETURN(-1);
    }
    /* try to form a full message */
    for (;;) {
      smsg_inbuflen = serdes_decode(readbuf, &readlen, (char *) smsg_inbuf, &state);
      if (0 == smsg_inbuflen) break; /* not a full message yet */
      if (0 > smsg_inbuflen) {	/* decoding error */
	RETURN(-1);
      }
      /* else we got a full message */
      if (SMSG_CODE_REPORT_DYNREG == smsg_message_identifier(smsg_inbuf)) {
	smsg_message_to_report_dynreg(smsg_inbuf, &report_dynreg);
	if (0 == report_dynreg.address || 0 == report_dynreg.port) {
	  RETURN(-1);
	} else {
	  *host_addr = report_dynreg.address;
	  *component_port = report_dynreg.port;
	  RETURN(0);
	}
      }
      /* else got something else */
      RETURN(-1);
    }
  }

  RETURN(-1);
#undef RETURN
}

void
smsg_message_handler_thread(void *args)
{
  smsg_message_handler_t handler;
  int fd;
  void *handler_args;
  int retval;

  /* reading, decoding and unpacking smsg messages */
  enum {READ_SIZE = 80};	/* how big a block to read */
  char readbuf[READ_SIZE]; /* into here */
  int readlen;			/* how many chars were read */
  serdes_decode_state state;	/* decoder */
  smsg_byte smsg_inbuf[SMSG_INBUFSIZE]; /* decoded and packed smsg message */
  int smsg_inbuflen;		/* how big smsg_inbuf was decoded to be */

  handler = ((smsg_message_handler_thread_args_t *) args)->handler;
  fd = ((smsg_message_handler_thread_args_t *) args)->fd;
  handler_args = ((smsg_message_handler_thread_args_t *) args)->handler_args;
  /* this frees the memory allocated by smsg_start_message_handler */
  my_free(args);

#define PEXIT(r)	\
  smsg_print_debug(SMSG_DEBUG_MSG, "Stopping message handler on fd %d with return %d\n", (int) fd, (int) r); \
  if (fd >= 0) ulapi_socket_close(fd); \
  ulapi_task_exit(0); \
  return

  /* initialize the decoder */
  retval = serdes_decode_state_init(&state, readbuf, (char *) smsg_inbuf, READ_SIZE, SMSG_INBUFSIZE);
  if (0 != retval) {
    PEXIT(NULL);
  }

  for (;;) {
    /* read from fd */
    readlen = ulapi_socket_read(fd, readbuf, READ_SIZE);
    if (0 == readlen) break;	/* end of file */
    if (0 > readlen) break;	/* read error */

    for (;;) {
      smsg_inbuflen = serdes_decode(readbuf, &readlen, (char *) smsg_inbuf, &state);
      if (0 == smsg_inbuflen) break;
      if (0 > smsg_inbuflen) {
	PEXIT(NULL);
      }

      /* handle message */
      if (0 != handler(smsg_inbuf, fd, handler_args)) {
	PEXIT(NULL);
      }
    } /* for (;;) to build message */
  } /* for (;;) to read fd */

  PEXIT(NULL);
#undef PEXIT
}

int
smsg_start_message_handler(smsg_message_handler_t handler, int fd, void * handler_args, void **thread_ptr)
{
  void *thread;
  smsg_message_handler_thread_args_t * args;
  ulapi_result retval;

  /* these are freed just after reference in the smsg_message_handler_thread */
  args = my_malloc(sizeof(smsg_message_handler_thread_args_t));
  args->handler = handler;
  args->fd = fd;
  args->handler_args = handler_args;

  thread = ulapi_task_new();
  if (NULL == thread) {
    return -1;
  }

  retval = ulapi_task_start(thread,
			    smsg_message_handler_thread,
			    args,
			    ulapi_prio_highest(),
			    1);

  if (ULAPI_OK != retval) {
    if (NULL != handler_args) {
      my_free(handler_args);
    }
  } else {
    if (NULL != thread_ptr) {
      *thread_ptr = thread;
    }
  }

  return retval;
}

static smsg_byte smsg_node_id = 1;
static smsg_byte smsg_subsystem_id = 1;

smsg_byte
smsg_set_node_id(smsg_byte id)
{
  return smsg_node_id = id;
}

smsg_byte
smsg_get_node_id(void)
{
  return smsg_node_id;
}

smsg_byte
smsg_set_subsystem_id(smsg_byte id)
{
  return smsg_subsystem_id = id;
}

smsg_byte
smsg_get_subsystem_id(void)
{
  return smsg_subsystem_id;
}
