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

#include <stdio.h>		/* f,printf(), stderr, perror() */
#include <stddef.h>		/* sizeof() */
#include <stdarg.h>		/* va_list,ap,end */
#include <stdlib.h>		/* atoi */
#include <string.h>		/* strcmp */
#include <ulapi.h>
#include <getopt.h>
#include "serdes.h"
#include "smsg.h"

/* the component database */
static component_db_t db;

typedef struct {
  int fd;
  void *mutex;
} shared_fd_t;

static int nodemgr_broadcast_handler(smsg_byte *smsg_inbuf, int broadcastee_fd, void *handler_args)
{
  shared_fd_t shared_fd;
  smsg_byte identifier;
  component_entry_t component;
  int index, last;
  smsg_query_allreg_t query_allreg;
  smsg_report_allreg_t report_allreg;
  int smsg_outbuflen;
  smsg_byte smsg_outbuf[SMSG_MAX_MESSAGE_SIZE];
  char writebuf[serdes_encode_size(sizeof(smsg_outbuf))];
  int writebuflen;

  shared_fd = *((shared_fd_t *) handler_args);
  identifier = smsg_message_identifier(smsg_inbuf);

  dprintf(SMSG_DEBUG_MSG, "Got broadcast message %d\n", (int) identifier);

  /* handle message */
  switch (identifier) {
  case SMSG_CODE_QUERY_ALLREG:
    /* we got a query, so report all we have */
    smsg_message_to_query_allreg(smsg_inbuf, &query_allreg);
    /* no parameters except sequence_number, which we're ignoring */
    for (index = 0, last = db_last(&db); index <= last; index++) {
      if (db_lookup(&db, index, &component) == index) {
	report_allreg.component_id = component.component_id;
	report_allreg.instance_id = component.instance_id;
	report_allreg.node_id = component.node_id;
	report_allreg.subsystem_id = component.subsystem_id;
	report_allreg.address = component.address;
	report_allreg.port = component.port;
	report_allreg.sequence_number = 1;
	smsg_outbuflen = smsg_report_allreg_to_message(&report_allreg, smsg_outbuf);
	writebuflen = serdes_encode((char *) smsg_outbuf, smsg_outbuflen, writebuf, sizeof(writebuf));
	ulapi_mutex_take(shared_fd.mutex);
	ulapi_socket_write(shared_fd.fd, writebuf, writebuflen);
	ulapi_mutex_give(shared_fd.mutex);
	dprintf(SMSG_DEBUG_BCAST, "Broadcasting component %d %d %d %d %s %d\n", 
		(int) component.component_id,
		(int) component.instance_id,
		(int) component.node_id,
		(int) component.subsystem_id,
		ulapi_address_to_hostname(component.address),
		(int) component.port);
      }
    }
    break;

  case SMSG_CODE_REPORT_ALLREG:
    /* we got some news on a component from another node manager */
    smsg_message_to_report_allreg(smsg_inbuf, &report_allreg);
    component.component_id = report_allreg.component_id;
    component.instance_id = report_allreg.instance_id;
    component.node_id = report_allreg.node_id;
    component.subsystem_id = report_allreg.subsystem_id;
    component.address = report_allreg.address;
    component.port = report_allreg.port;
    component.fd = -1;
    /* ignore component.fd */
    if (0 > db_find(&db, &component)) {
      dprintf(SMSG_DEBUG_BCAST, "News on component %d %d %d %d %s %d\n", 
	      (int) component.component_id,
	      (int) component.instance_id,
	      (int) component.node_id,
	      (int) component.subsystem_id,
	      ulapi_address_to_hostname(component.address),
	      (int) component.port);
      if (0 > db_add(&db, &component)) {
	smsg_print_debug(SMSG_DEBUG_BCAST, "Can't update db with broadcast entry\n");
      }
    } else {
      dprintf(SMSG_DEBUG_BCAST, "This one is my component\n");
    }
    break;

  default:
    smsg_print_debug(SMSG_DEBUG_MSG, "Unknown message type: %d\n", (int) identifier);
    break;
  } /* switch (identifier) */

  return 0;
}

static int nodemgr_message_handler(smsg_byte *smsg_inbuf, int fd, void *handler_args)
{
  shared_fd_t shared_fd;
  smsg_byte identifier;
  component_entry_t component;
  int bad;
  smsg_request_dynreg_t request_dynreg;
  smsg_reply_dynreg_t reply_dynreg;
  smsg_query_dynreg_t query_dynreg;
  smsg_report_dynreg_t report_dynreg;
  smsg_query_allreg_t query_allreg;
  int smsg_outbuflen;
  smsg_byte smsg_outbuf[SMSG_MAX_MESSAGE_SIZE];
  char writebuf[serdes_encode_size(sizeof(smsg_outbuf))];
  int writebuflen;

  shared_fd = *((shared_fd_t *) handler_args);
  identifier = smsg_message_identifier(smsg_inbuf);

  dprintf(SMSG_DEBUG_MSG, "Got node message %d\n", (int) identifier);

  /* handle message */
  switch (identifier) {
  case SMSG_CODE_REQUEST_DYNREG:
    /* register this component and reply */
    smsg_message_to_request_dynreg(smsg_inbuf, &request_dynreg);
    component.component_id = request_dynreg.component_id;
    component.instance_id = request_dynreg.instance_id;
    if (request_dynreg.node_id != smsg_get_node_id()) {
      dprintf(SMSG_DEBUG_REG, "Component node id %d doesn't match node manager node id %d\n", (int) request_dynreg.node_id, (int) smsg_get_node_id());
    }
    if (request_dynreg.subsystem_id != smsg_get_subsystem_id()) {
      dprintf(SMSG_DEBUG_REG, "Component subsystem id %d doesn't match node manager subsystem id %d\n", (int) request_dynreg.subsystem_id, (int) smsg_get_subsystem_id());
    }
    component.node_id = request_dynreg.node_id;
    component.subsystem_id = request_dynreg.subsystem_id;
    component.address = ulapi_get_host_address();
    component.port = 0;		/* will be filled in */
    component.fd = -1;
    bad = 0;
    if (0 > db_find(&db, &component)) {
      bad = (0 > db_add(&db, &component)) ? 1 : 0;
    }
    reply_dynreg.component_id = component.component_id;
    reply_dynreg.instance_id = component.instance_id;
    reply_dynreg.node_id = component.node_id;
    reply_dynreg.subsystem_id = component.subsystem_id;
    reply_dynreg.address = component.address;
    reply_dynreg.port = component.port;
    if (bad) {
      /* reply that we can't register them due to db error */
      reply_dynreg.address = 0;
      reply_dynreg.port = 0;
    }
    dprintf(SMSG_DEBUG_REG, "Replying with %s port %d\n", ulapi_address_to_hostname(reply_dynreg.address), reply_dynreg.port);
    reply_dynreg.sequence_number = 1;
    smsg_outbuflen = smsg_reply_dynreg_to_message(&reply_dynreg, smsg_outbuf);
    writebuflen = serdes_encode((char *) smsg_outbuf, smsg_outbuflen, writebuf, sizeof(writebuf));
    ulapi_socket_write(fd, writebuf, writebuflen);
    break;

  case SMSG_CODE_REPLY_DYNREG:
  case SMSG_CODE_REPORT_DYNREG:
    /* we send these, shouldn't receive them */
    dprintf(SMSG_DEBUG_REG, "Unexpected message type: %d\n", (int) identifier);
    break;

  case SMSG_CODE_QUERY_DYNREG:
    /* look up this component and report */
    smsg_message_to_query_dynreg(smsg_inbuf, &query_dynreg);
    component.component_id = query_dynreg.component_id;
    component.instance_id = query_dynreg.instance_id;
    component.node_id = query_dynreg.node_id;
    component.subsystem_id = query_dynreg.subsystem_id;
    if (0 > db_find(&db, &component)) {
      /* can't find this component, so ask our other node manager
	 brothers to send us news */
      dprintf(SMSG_DEBUG_REG, "No record of component %d %d %d %d, broadcasting for news\n", (int) component.component_id, (int) component.instance_id, (int) component.node_id, (int) component.subsystem_id);

      query_allreg.sequence_number = 1;
      smsg_outbuflen = smsg_query_allreg_to_message(&query_allreg, smsg_outbuf);
      writebuflen = serdes_encode((char *) smsg_outbuf, smsg_outbuflen, writebuf, sizeof(writebuf));
      ulapi_mutex_take(shared_fd.mutex);
      ulapi_socket_write(shared_fd.fd, writebuf, writebuflen);
      ulapi_mutex_give(shared_fd.mutex);
      /* now fill in empty address and port for report message below */
      report_dynreg.address = 0;
      report_dynreg.port = 0;
    } else {
      /* we have this, so fill in the address and port */
      report_dynreg.address = component.address;
      report_dynreg.port = component.port;
    }
    /* now sent the report to the queryer */
    report_dynreg.sequence_number = 1;
    report_dynreg.component_id = component.component_id;
    report_dynreg.instance_id = component.instance_id;
    report_dynreg.node_id = component.node_id;
    report_dynreg.subsystem_id = component.subsystem_id;
    smsg_outbuflen = smsg_report_dynreg_to_message(&report_dynreg, smsg_outbuf);
    writebuflen = serdes_encode((char *) smsg_outbuf, smsg_outbuflen, writebuf, sizeof(writebuf));
    ulapi_socket_write(fd, writebuf, writebuflen);
    break;

  default:
    dprintf(SMSG_DEBUG_MSG, "Unknown message type: %d\n", (int) identifier);
    break;
  } /* switch (identifier) */

  return 0;
}

/*
  Usage: nodemgr
  [-n <node id>]
  [-s <subsystem id>]
*/

int main(int argc, char *argv[])
{
  int option;
  smsg_addr addr;
  int port = SMSG_PORT;
  int socket_fd;
  int broadcaster_fd;
  int broadcastee_fd;
  void *broadcaster_mutex;
  shared_fd_t shared_fd;
  int client_fd;

  smsg_set_debug_name("Nodemgr");
  smsg_set_debug_mask(SMSG_DEBUG_ALL);

  for (opterr = 0;;) {
    option = getopt(argc, argv, ":n:s:d:");
    if (option == -1)
      break;

    switch (option) {
    case 'n':
      smsg_set_node_id((smsg_byte) atoi(optarg));
      break;

    case 's':
      smsg_set_subsystem_id((smsg_byte) atoi(optarg));
      break;

    case 'd':
      if (! strcmp(optarg, "all"))
	smsg_set_debug_mask(SMSG_DEBUG_ALL);
      else
	smsg_set_debug_mask(strtoul(optarg, NULL, 0));
      break;

    case ':':
      fprintf(stderr, "Missing value for -%c\n", optopt);
      return 1;
      break;

    default:			/* '?' */
      fprintf (stderr, "Unrecognized option -%c\n", optopt);
      return 1;
      break;
    }
  }
  if (optind < argc) {
    fprintf(stderr, "Extra non-option characters: %s\n", argv[optind]);
    return 1;
  }

  addr = ulapi_get_host_address();
  if (0 == addr) {
    smsg_print_debug(SMSG_DEBUG_CFG, "Can't get host address\n");
    return 1;
  }
  dprintf(SMSG_DEBUG_CFG, "Host address is %s\n", ulapi_address_to_hostname(addr));

  socket_fd = ulapi_socket_get_server_id(port);
  if (socket_fd < 0) {
    smsg_print_debug(SMSG_DEBUG_CFG, "Can't get socket fd\n");
    return 1;
  }
  dprintf(SMSG_DEBUG_CFG, "Got socket fd %d\n", socket_fd);

  if (0 != db_init(&db)) {
    smsg_print_debug(SMSG_DEBUG_CFG, "Can't allocate database\n");
    return 1;
  }

  /* get the fd of the broadcaster port that will be written by
     both the client message handler when it can't find a requested
     component, and the broadcast message handler when it gets a
     request to broadcast out its components */
  broadcaster_fd = ulapi_socket_get_broadcaster_id(port);
  if (broadcaster_fd < 0) {
    smsg_print_debug(SMSG_DEBUG_CFG, "Can't get broadcaster fd\n");
    return 1;
  }
  dprintf(SMSG_DEBUG_CFG, "Got broadcaster fd %d\n", broadcaster_fd);
  broadcaster_mutex = ulapi_mutex_new(1);
  shared_fd.fd = broadcaster_fd;
  shared_fd.mutex = broadcaster_mutex;

  /* get the fd of the broadcastee port that will be read by the
     broadcastee thread awaiting requests from other node managers
     for our component database */
  broadcastee_fd = ulapi_socket_get_broadcastee_id(SMSG_PORT);
  if (broadcastee_fd < 0) {
    smsg_print_debug(SMSG_DEBUG_CFG, "Can't get broadcastee fd\n");
    return 1;
  }
  dprintf(SMSG_DEBUG_CFG, "Got broadcastee fd %d\n", broadcastee_fd);

  if (0 != smsg_start_message_handler(nodemgr_broadcast_handler, broadcastee_fd, &shared_fd, NULL)) {
    smsg_print_debug(SMSG_DEBUG_CFG, "Can't spawn broadcast thread\n");
    return 1;
  }
  dprintf(SMSG_DEBUG_CFG, "Spawning broadcast thread on fd %d\n", broadcastee_fd);

  for (;;) {
    dprintf(SMSG_DEBUG_CFG, "Waiting for client connection on fd %d...\n", socket_fd);
    client_fd = ulapi_socket_get_connection_id(socket_fd);
    if (client_fd < 0) {
      smsg_print_debug(SMSG_DEBUG_CFG, "Can't get client connection\n");
      return 1;
    }
    dprintf(SMSG_DEBUG_CFG, "Got a client connection on fd %d\n", client_fd);

    if (0 != smsg_start_message_handler(nodemgr_message_handler, client_fd, &shared_fd, NULL)) {
      smsg_print_debug(SMSG_DEBUG_CFG, "Can't spawn server thread\n");
      return 1;
    }
    dprintf(SMSG_DEBUG_CFG, "Spawning a server thread on client fd %d\n", client_fd);
  }

  return 0;
}
