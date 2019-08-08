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

#include <stdio.h>		/* fprintf, stderr */
#include <stdlib.h>		/* atoi */
#include <stdarg.h>		/* va_list,start,end */
#include <string.h>		/* strncpy, strerror */
#include <errno.h>		/* errno */
#include <getopt.h>
#include <ulapi.h>
#include "smsg.h"

static int client_message_handler(smsg_byte *smsg_inbuf, int fd, void *handler_args)
{
  smsg_byte identifier;
  smsg_report_test_t report_test;

  identifier = smsg_message_identifier(smsg_inbuf);

  /* handle message */
  switch (identifier) {
  case SMSG_CODE_REPORT_TEST:
    if (0 != smsg_message_to_report_test(smsg_inbuf, &report_test)) {
      dprintf(SMSG_DEBUG_MSG, "Bad message for type: %d\n", (int) identifier);
    }
    printf("%d %f\n", (int) report_test.count, (double) report_test.time);
    break;

  default:
    dprintf(SMSG_DEBUG_MSG, "Unknown message type: %d\n", (int) identifier);
    break;
  } /* switch (identifier) */

  return 0;
}

/* 
   Usage: querytest 
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
  ulapi_integer myclient_id;
  smsg_byte component_id = 1;
  smsg_byte instance_id = 1;
  smsg_byte node_id = 1;
  smsg_byte subsystem_id = 1;
  smsg_query_test_t query_test;
  int smsg_outbuflen;
  smsg_byte smsg_outbuf[SMSG_MAX_MESSAGE_SIZE];
  char writebuf[serdes_encode_size(sizeof(smsg_outbuf))];
  int writebuflen;

  for (opterr = 0;;) {
    option = getopt(argc, argv, ":c:i:n:s:");
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

  smsg_set_debug_name("Querytest");
  smsg_set_debug_mask(SMSG_DEBUG_ALL);

  for (;; ulapi_sleep(1)) {
    if (0 == smsg_find_component(-1, component_id, instance_id, node_id, subsystem_id, &address, &port)) {
      break;
    }
  }
  dprintf(SMSG_DEBUG_CFG, "Found component on %s port %d\n", ulapi_address_to_hostname(address), (int) port);

  /* now connect as a client */
  /* FIXME -- have integer address, need char[] host */
  myclient_id = ulapi_socket_get_client_id(port, ulapi_address_to_hostname(address));
  if (myclient_id < 0) {
    smsg_print_debug(SMSG_DEBUG_CFG, "Can't get socket fd for port %d\n", (int) port);
    return 1;
  }
  
  /* and set up the message handler */
  if (0 != smsg_start_message_handler(client_message_handler, myclient_id, NULL, NULL)) {
    smsg_print_debug(SMSG_DEBUG_CFG, "Can't spawn client thread\n");
    return 1;
  }

  for (query_test.sequence_number = 1; ; ulapi_sleep(1)) {
    query_test.sequence_number++;
    smsg_outbuflen = smsg_query_test_to_message(&query_test, smsg_outbuf);
    writebuflen = serdes_encode((char *) smsg_outbuf, smsg_outbuflen, writebuf, sizeof(writebuf));
    if (0 > ulapi_socket_write(myclient_id, writebuf, writebuflen)) {
      dprintf(SMSG_DEBUG_CFG, "Reporter disconnected\n");
      break;
    }
  }

  return 0;
}

