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
      smsg_print_debug(SMSG_DEBUG_MSG, "Bad message for type: %s\n", smsg_id_to_string(identifier));
    }
    printf("%d %f\n", (int) report_test.count, (double) report_test.time);
    break;

  default:
    smsg_print_debug(SMSG_DEBUG_MSG, "Unknown message type: %s\n", smsg_id_to_string(identifier));
    break;
  } /* switch (identifier) */

  return 0;
}

/* 
   Usage: querytest <options>, which are:
   -h                : print help
   -d <debug mask>   : set debug printing level
   -c <component id> : set the component id, default 1
   -i <instance id>  : set the instance id, default 1
   -n <node id>      : set the node id, default 1
   -s <subsystem id> : set the subsystem id, default 1
*/

static void print_help(void)
{
  printf("Usage: querytest <options>, which are:\n");
  printf("-h                : print help\n");
  printf("-d <debug mask>   : set debug printing level\n");
  printf("-c <component id> : set the component id, default 1\n");
  printf("-i <instance id>  : set the instance id, default 1\n");
  printf("-n <node id>      : set the node id, default 1\n");
  printf("-s <subsystem id> : set the subsystem id, default 1\n");

  return;
}

int main(int argc, char *argv[])
{
  int option;
  int debug_mask = 0;
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
    option = ulapi_getopt(argc, argv, ":c:i:n:s:d:h");
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

    case 'h':
      print_help();
      return 0;
      break;

    case 'd':
      if (1 != sscanf(optarg, "%i", &debug_mask)) {
	fprintf(stderr, "bad value for -d: %s\n", optarg);
	return 1;
      }
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
  smsg_set_debug_mask(debug_mask);

  for (;; ulapi_sleep(1)) {
    if (0 == smsg_find_component(-1, component_id, instance_id, node_id, subsystem_id, &address, &port)) {
      break;
    }
  }
  smsg_print_debug(SMSG_DEBUG_CFG, "Found component on %s port %d\n", ulapi_address_to_hostname(address), (int) port);

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
      smsg_print_debug(SMSG_DEBUG_CFG, "Reporter disconnected\n");
      break;
    }
  }

  return 0;
}

