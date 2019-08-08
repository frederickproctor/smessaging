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

/*
  The report test message time is a float, and since FLT_EPSILON is 1.2e7
  and too small to hold the units part of the time in seconds since the
  epoch, we save our start time here and subtract it off as the baseline.
*/

#include <stdio.h>		/* fprintf, stderr */
#include <stdlib.h>		/* atoi */
#include <stdarg.h>		/* va_list,start,end */
#include <string.h>		/* strncpy, strerror */
#include <errno.h>		/* errno */
#include <ulapi.h>
#include "smsg.h"

static ulapi_real start_time;

static int client_message_handler(smsg_byte *smsg_inbuf, int fd, void *handler_args)
{
  static smsg_uint count = 0;
  smsg_byte identifier;
  smsg_query_test_t query_test;
  smsg_report_test_t report_test;
  int smsg_outbuflen;
  smsg_byte smsg_outbuf[SMSG_MAX_MESSAGE_SIZE];
  char writebuf[serdes_encode_size(sizeof(smsg_outbuf))];
  int writebuflen;

  identifier = smsg_message_identifier(smsg_inbuf);

  /* handle message */
  switch (identifier) {
  case SMSG_CODE_QUERY_TEST:
    /* echo the input sequence number back */
    /* FIXME -- need some Smessaging rules for this */
    smsg_message_to_query_test(smsg_inbuf, &query_test);
    report_test.sequence_number = query_test.sequence_number;
    report_test.count = count++;
    report_test.time = (smsg_float) (ulapi_time() - start_time);
    smsg_outbuflen = smsg_report_test_to_message(&report_test, smsg_outbuf);
    writebuflen = serdes_encode((char *) smsg_outbuf, smsg_outbuflen, writebuf, sizeof(writebuf));
    ulapi_socket_write(fd, writebuf, writebuflen);
    break;

  default:
    smsg_print_debug(SMSG_DEBUG_MSG, "Unknown message type: %d\n", (int) identifier);
    break;
  } /* switch (identifier) */

  return 0;
}

/* 
   Usage: reporttest
   [-c <component id>]
   [-i <instance id>] 
   [-n <node id>]
   [-s <subsystem id>]
*/

int main(int argc, char *argv[])
{
  int option;
  smsg_addr address;
  smsg_port port;
  ulapi_integer myserver_id;
  ulapi_integer connection_id;
  smsg_byte component_id = 1;
  smsg_byte instance_id = 1;
  smsg_byte node_id = 1;
  smsg_byte subsystem_id = 1;

  for (opterr = 0;;) {
    option = ulapi_getopt(argc, argv, ":c:i:n:s:");
    if (option == -1)
      break;

    switch (option) {
    case 'c':
      component_id = atoi(optarg);
      break;

    case 'i':
      instance_id = atoi(optarg);
      break;

    case 'n':
      node_id = atoi(optarg);
      break;

    case 's':
      subsystem_id = atoi(optarg);
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

  smsg_set_debug_name("Reporttest");
  smsg_set_debug_mask(SMSG_DEBUG_ALL);

  if (0 != smsg_register_component(-1, component_id, instance_id, node_id, subsystem_id, &address, &port)) {
    fprintf(stderr, "Can't register component\n");
    return 1;
  }
  smsg_print_debug(SMSG_DEBUG_CFG, "Registered on %s port %d\n", ulapi_address_to_hostname(address), (int) port);

  /* now run a server */
  myserver_id = ulapi_socket_get_server_id(port);
  if (myserver_id < 0) {
    smsg_print_debug(SMSG_DEBUG_CFG, "Can't get socket fd for port %d\n", (int) port);
    return 1;
  }

  start_time = ulapi_time();

  /* and wait for connections */
  for (;;) {
    smsg_print_debug(SMSG_DEBUG_CFG, "Waiting for client connection...\n");
    connection_id = ulapi_socket_get_connection_id(myserver_id);
    if (0 != smsg_start_message_handler(client_message_handler, connection_id, NULL, NULL)) {
      smsg_print_debug(SMSG_DEBUG_CFG, "Can't spawn server thread\n");
      break;
    }
  }
  
  return 0;
}

