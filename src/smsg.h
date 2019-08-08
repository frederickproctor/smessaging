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
  A Smessage consists of a one-byte identifier, a one-byte sequence
  number and optionally some number of bytes containing parameters
  associated with the identifier.

  Identifiers 0 through 31 inclusive are reserved by Smessaging for
  its purposes, such as registering components or establishing
  connections. Identifiers 32 through 255 inclusive can be used
  by Smessaging-based applications. If an application needs more than
  these 224 message identifiers, it can use the parameter bytes as
  identifier extensions to support an arbitrarily large number of
  message types.

  Following the identifier is a single byte indicating the sequence
  number of the message. This limits the sequence number to the range
  0 to 255 inclusive. Since the sequence number is used for
  handshaking, strictly speaking it need only be a single bit. The
  larger range was chosen to make message diagnostics easier.

  Following the sequence number are bytes containing the parameters
  associated with the identifier, if any.
*/

#ifndef SMSG_H
#define SMSG_H

#include <stddef.h>		/* sizeof */
#include <limits.h>		/* SHRT,INT_MAX */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "serdes.h"		/* serdes_encode,decode_size */

#ifdef __cplusplus
extern "C" {
#if 0
}
#endif
#endif

/* identifiers reserved by Smessaging */
enum {
  SMSG_CODE_REQUEST_DYNREG = 1,
  SMSG_CODE_REPLY_DYNREG = 2,
  SMSG_CODE_QUERY_DYNREG = 3,
  SMSG_CODE_REPORT_DYNREG = 4,
  SMSG_CODE_QUERY_ALLREG = 5,
  SMSG_CODE_REPORT_ALLREG = 6,
  SMSG_CODE_OPEN_CLIENT_CONNECTION = 7,
  SMSG_CODE_RETURN_CLIENT_CONNECTION = 8,
  SMSG_CODE_CLOSE_CLIENT_CONNECTION = 9,
  SMSG_CODE_OPEN_SERVER_CONNECTION = 10,
  SMSG_CODE_RETURN_SERVER_CONNECTION = 11,
  SMSG_CODE_CLOSE_SERVER_CONNECTION = 12,
  SMSG_CODE_QUERY_TEST = 13,
  SMSG_CODE_REPORT_TEST = 14
};

/*
  This is unsigned so that identifiers can be set and compared to
  numbers in the range of 128 to 255 without compiler warnings.
*/
typedef unsigned char smsg_byte;

typedef unsigned int smsg_uint;
typedef float smsg_float;

typedef unsigned int smsg_addr;	/* e.g., 192.168.0.1 */
typedef unsigned int smsg_port;	/* e.g., 11601 */

/* Returns the identifier. */
#define smsg_message_identifier(msg) (*((smsg_byte *) msg))

/*
  Request dynamic registration of this component.

  [1] [seq] [component id] [instance id] [node id] [subsystem id]
*/
typedef struct {
  smsg_byte identifier;		/* 1 */
  smsg_byte sequence_number;
  smsg_byte component_id;
  smsg_byte instance_id;
  smsg_byte node_id;
  smsg_byte subsystem_id;
} smsg_request_dynreg_t;

extern int smsg_message_to_request_dynreg(smsg_byte *msg, smsg_request_dynreg_t *smsg_msg);
extern int smsg_request_dynreg_to_message(smsg_request_dynreg_t *smsg_msg, smsg_byte *msg);

typedef struct {
  smsg_byte identifier;
  smsg_byte sequence_number;
  smsg_byte component_id;
  smsg_byte instance_id;
  smsg_byte node_id;
  smsg_byte subsystem_id;
  smsg_addr address;		/* this is your host network address */
  smsg_port port;		/* this is your port */
} smsg_reply_dynreg_t;

extern int smsg_message_to_reply_dynreg(smsg_byte *msg, smsg_reply_dynreg_t *smsg_msg);
extern int smsg_reply_dynreg_to_message(smsg_reply_dynreg_t *smsg_msg, smsg_byte *msg);

typedef struct {
  smsg_byte identifier;
  smsg_byte sequence_number;
  smsg_byte component_id;
  smsg_byte instance_id;
  smsg_byte node_id;
  smsg_byte subsystem_id;
} smsg_query_dynreg_t;

extern int smsg_message_to_query_dynreg(smsg_byte *msg, smsg_query_dynreg_t *smsg_msg);
extern int smsg_query_dynreg_to_message(smsg_query_dynreg_t *smsg_msg, smsg_byte *msg);

typedef struct {
  smsg_byte identifier;
  smsg_byte sequence_number;
  smsg_byte component_id;
  smsg_byte instance_id;
  smsg_byte node_id;
  smsg_byte subsystem_id;
  smsg_addr address;		/* and is on this host network address */
  smsg_port port;			/* and this port */
} smsg_report_dynreg_t;

extern int smsg_message_to_report_dynreg(smsg_byte *msg, smsg_report_dynreg_t *smsg_msg);
extern int smsg_report_dynreg_to_message(smsg_report_dynreg_t *smsg_msg, smsg_byte *msg);

typedef struct {
  smsg_byte identifier;
  smsg_byte sequence_number;
} smsg_query_allreg_t;

extern int smsg_message_to_query_allreg(smsg_byte *msg, smsg_query_allreg_t *smsg_msg);
extern int smsg_query_allreg_to_message(smsg_query_allreg_t *smsg_msg, smsg_byte *msg);

typedef struct {
  smsg_byte identifier;
  smsg_byte sequence_number;
  smsg_byte component_id;
  smsg_byte instance_id;
  smsg_byte node_id;
  smsg_byte subsystem_id;
  smsg_addr address;		/* and is on this host network address */
  smsg_port port;		/* and this port */
} smsg_report_allreg_t;

extern int smsg_message_to_report_allreg(smsg_byte *msg, smsg_report_allreg_t *smsg_msg);
extern int smsg_report_allreg_to_message(smsg_report_allreg_t *smsg_msg, smsg_byte *msg);

/* message equivalent to opening a socket connection to a server as a client */
typedef struct {
  /* the destination ids will be filled in by the sender, and put
     into a table by the proxy for association with the connection id */
  smsg_byte identifier;
  smsg_byte sequence_number;
  smsg_addr address;
  smsg_port port;
} smsg_open_client_connection_t;

extern int smsg_message_to_open_client_connection(smsg_byte *msg, smsg_open_client_connection_t *smsg_msg);
extern int smsg_open_client_connection_to_message(smsg_open_client_connection_t *smsg_msg, smsg_byte *msg);

/* message equivalent of returning a file descriptor to a client */
typedef struct {
  smsg_byte identifier;
  smsg_byte sequence_number;
  smsg_byte connection;		/* the "file descriptor" */
} smsg_return_client_connection_t;

extern int smsg_message_to_return_client_connection(smsg_byte *msg, smsg_return_client_connection_t *smsg_msg);
extern int smsg_return_client_connection_to_message(smsg_return_client_connection_t *smsg_msg, smsg_byte *msg);

/* message equivalent to opening a socket connection as a server to await clients */
typedef struct {
  smsg_byte identifier;
  smsg_byte sequence_number;
  smsg_addr address;
  smsg_port port;
} smsg_open_server_connection_t;

extern int smsg_message_to_open_server_connection(smsg_byte *msg, smsg_open_server_connection_t *smsg_msg);
extern int smsg_open_server_connection_to_message(smsg_open_server_connection_t *smsg_msg, smsg_byte *msg);

/* message equivalent of returning a file descriptor to a server */
typedef struct {
  smsg_byte identifier;
  smsg_byte sequence_number;
  smsg_byte connection;		/* the "file descriptor" */
} smsg_return_server_connection_t;

extern int smsg_message_to_return_server_connection(smsg_byte *msg, smsg_return_server_connection_t *smsg_msg);
extern int smsg_return_server_connection_to_message(smsg_return_server_connection_t *smsg_msg, smsg_byte *msg);

/* a request for the test message */
typedef struct {
  smsg_byte identifier;
  smsg_byte sequence_number;
} smsg_query_test_t;

extern int smsg_message_to_query_test(smsg_byte *msg, smsg_query_test_t *smsg_msg);
extern int smsg_query_test_to_message(smsg_query_test_t *smsg_msg, smsg_byte *msg);

/* a test message that returns a count and a time in seconds */
typedef struct {
  smsg_byte identifier;
  smsg_byte sequence_number;
  smsg_uint count;
  smsg_float time;
} smsg_report_test_t;

extern int smsg_message_to_report_test(smsg_byte *msg, smsg_report_test_t *smsg_msg);
extern int smsg_report_test_to_message(smsg_report_test_t *smsg_msg, smsg_byte *msg);

#define SMSG_HOST "localhost"
enum {SMSG_PORT = 3794};
enum {SMSG_PORT_BASE = 11601};
enum {SMSG_PORT_RANGE = 11719 - SMSG_PORT_BASE};
enum {SMSG_PORT_HOWMANY = SMSG_PORT_RANGE + 1};
/* available ports are SMSG_PORT_BASE through 
   SMSG_PORT_BASE + SMSG_PORT_RANGE, inclusive */

#define RANGE(lo,hi) (((double) (hi)) - ((double) (lo)))

#define FRACT(val,lo,hi) ((((double) (val)) - ((double) (lo))) / (((double) (hi)) - ((double) (lo))))

#define scale_smsg_int(val,lo,hi) ((smsg_int) (RANGE(INT_MIN,INT_MAX) * FRACT(val,lo,hi) + INT_MIN))
#define unscale_smsg_int(val,lo,hi) ((double) (RANGE(lo,hi) * FRACT(val,INT_MIN,INT_MAX) + lo))

#define scale_smsg_uint(val,hi) ((smsg_uint) (UINT_MAX * FRACT(val,0,hi)))
#define unscale_smsg_uint(val,hi) ((double) ((hi) * FRACT(val,0,UINT_MAX)))

#define scale_smsg_short(val,lo,hi) ((smsg_short) (RANGE(SHRT_MIN,SHRT_MAX) * FRACT(val,lo,hi) + SHRT_MIN))
#define unscale_smsg_short(val,lo,hi) ((double) (RANGE(lo,hi) * FRACT(val,SHRT_MIN,SHRT_MAX) + lo))

#define scale_smsg_ushort(val,hi) ((smsg_ushort) (USHRT_MAX * FRACT(val,0,hi)))
#define unscale_smsg_ushort(val,hi) ((double) (hi * FRACT(val,0,USHRT_MAX)))

#define scale_smsg_byte(val,hi) ((smsg_byte) (UCHAR_MAX * FRACT(val,0,hi)))
#define unscale_smsg_byte(val,hi) ((double) (hi * FRACT(val,0,UCHAR_MAX)))

/* component database used by node manager and proxy */

typedef struct {
  /* key */
  smsg_byte component_id;
  smsg_byte instance_id;
  smsg_byte node_id;
  smsg_byte subsystem_id;

  /* value */
  smsg_addr address;
  smsg_port port;
  int fd;			/* what port the proxy is using, if any */
} component_entry_t;

typedef struct {
  void *mutex;
  component_entry_t *entries;
  int size;
  int index;
  smsg_port port;
} component_db_t;

extern int
db_init(component_db_t *db);

extern int
db_free(component_db_t *db);

/*
  Fill in entry.component,instance,node,subsystem_id and pass pointer.
  If it's in the DB, the rest will be filled in and the non-negative
  index of the entry is returned.  Otherwise, a negative value is
  returned and entry is left alone.
*/
extern int
db_find(component_db_t *db, component_entry_t *entry);

/*
  Given 'fd', look up the entry at that fd.  If it's in the DB,
  'entry' will be filled in and the non-negative index of the entry is
  returned.  Otherwise, a negative value is returned and entry is left
  alone.
*/
extern int
db_find_fd(component_db_t *db, int fd, component_entry_t *entry);

/*
  Add this entry to the database. If it's already in the database,
  the non-negative index of the entry is returned. Otherwise any
  zero address or port will be filled in, and the index returned.
  On error, -1 is returned.
*/
extern int
db_add(component_db_t *db, component_entry_t *entry);

/*
  Given 'fd', add this entry to the database. If it's already in the
  database, the non-negative index of the entry is returned. Otherwise
  any zero address or port will be filled in, and the index returned.
  On error, -1 is returned.
*/
extern int
db_add_fd(component_db_t *db, int fd, component_entry_t *entry);

/*
  Pass an index, and the entry will be filled in and the index returned.
  If there is no entry at that index, the entry is left alone and a 
  negative value is returned.
*/
extern int
db_lookup(component_db_t *db, int index, component_entry_t *entry);

/*
  Returns index of last entry, -1 if empty
*/
extern int
db_last(component_db_t *db);

/*!
  \defgroup Dynamic Registration

  A System is composed of two or more Components that interoperate
  using SMSG messages. Communication between components can take place
  peer-to-peer using any particular medium (e.g., RS-232 serial
  communication), or via IP-based networking. If IP-based networking
  is used, then each IP addressable host is a Node that runs exactly
  one Node Manager serving the well-known SMSG TCP/IP socket port
  3794 and broadcasting on the associated UDP port.

  Components can be grouped into Subsystems that may span any Nodes in
  the System. Each node is a member of exactly one Subsystem. Each
  Component has a Component ID, an Instance ID, a Node ID and a
  Subsystem ID.

  The Component ID is a well-known positive number that identifies the
  type of Component, e.g., a global waypoint driver.

  The Instance ID is an arbitrary but unique positive number that
  distinguishes between mutiple instances of one type of Component on
  a Node.

  The Node ID is an arbitrary but unique positive number that
  distinguishes between multiple Nodes in a System, e.g., the least
  significant byte of the Node's host IP address could serve as the
  Node ID. 

  The Subsystem ID is an arbitrary but unique positive number that
  serves to associate Components and Nodes in a System, e.g., all the
  Components on a single physical vehicle.

  In a Networked Software Environment where there is at least one Node
  and associated Node Manager, each Node Manager serves the well-known
  SMSG TCP/IP port and waits for requests from components for dynamic
  registration. Components connect as clients to this port, and send
  requests with their Component ID and Instance ID. The Instance ID
  may be zero, in which case one is uniquely assigned by the Node
  Manager.

  The Node Manager assigns a unique TCP/IP port to this client, and
  replies with the original Component ID, the Instance ID (which may
  have been assigned), the Node ID and Subsystem ID, the Node's IP
  address and the assigned port. The Node Manager records this
  information in the Node Database for subsequent responses to queries
  by other Components.

  The client Component then serves its port, waiting for SMSG messages
  and responding accordingly. Typically this takes place in a
  concurrent thread of execution, along with the execution of the
  Component's normal operations. 

  Components that need to interoperate with other Components query their
  Node Manager for the address and port of the target Component. If
  the target Component has already registered, its address and port
  are reported. Otherwise, 0 values for the address and port are
  reported. Typically the requesting Component delays and tries again
  repeatedly for some period, then fails. 

  Once a Component has received the address and port of a target
  Component, it connects to that target as a client and exchanges SMSG
  messages. Typically this takes place in a concurrent thread of
  execution, along with the execution of the Component's normal
  operation. 

  Different Node Managers exchange their Node Databases, so that each
  knows the addresses and ports of all other Components in the System,
  not only their own. This is done via UDP broadcasts on the
  well-known SMSG port. 

  When a Component registers, its Node Manager broadcasts the
  registration information. All Node Managers listen for broadcast
  messages, and update their Node Databases with newly registered
  Components. Each Node Database will contain some entries for
  Components on its Node, and possibly many other entries for
  Components on all the other Nodes. 

  If a Node Manager cannot find an entry for a queried Component, it
  broadcasts a request for this component. The Node Manager whose Node
  Database contains the requested entry will respond. All other Node
  Managers will update their Node Databases. If the update takes place
  after the requesting Component has timed out, the Component will
  fail. Otherwise, the requesting Component will eventually receive a
  report with the address and port of its target.

  The Node Manager can reply to queries about Components on its Node
  with the local address (127.0.0.1) to limit physical network
  traffic. When reporting to a broadcast request from other Node
  Managers, the network address should be reported. 

  Some components may not execute in the Networked Software
  Environment, for example, a global position reporting component that
  communicates using a serial RS-232 link. If the component is
  connected to a host that is executing in a Networked Software
  Environment, the host Node Manager can act as a Proxy on the
  Component's behalf. The Node Manager must be configured to listen to
  the non-network ports, e.g., through a list of serial port devices
  provided to the Node Manager at startup or through a configuration
  file.

  To act as a Proxy, the Node Manager will see a request from the
  Component to register. As before, the Node Manager will assign the
  host address and unique port for this Component. The Component can
  ignore this, other than to take it as successful registration, since
  the Component cannot serve this port itself. The Proxy serves the
  port, and relays all SMSG messages between clients and the Component
  via the non-network link.

  If the non-networked Component wants to interoperate with another
  Component, it makes a query to the Node Manager via the Proxy. The
  Proxy looks at each message, extracting those appropriate for the
  Node Manager, and replies accordinly. The reply to the Component
  will contain the network address and port of the target
  Component. In a networked environment, the Component would open a
  client connection to the target Component. Here, the Component sends
  a Connect message, which is picked up by the Proxy which opens a
  client connection and sends a Connected response with an identifier
  equivalent to a file descriptor. The Component then encapsulates a
  SMSG message in a Send message with that file descriptor, which is
  extracted by the Proxy and sent via the client connection. Messages
  that come from the target Component are encapsulated in a Received
  message with that file descriptor.
*/

/*
  source ids are used for requests, replies and open server connections.
  destination ids are used for queries, reports and open client connections.
*/

extern void *fwdcpy(void *dest, const void *src, size_t n);
extern void *revcpy(void *dest, const void *src, size_t n);

/* copies to little endian byte array */
#ifdef WORDS_BIGENDIAN
#define little_endian_copy revcpy
#else
#define little_endian_copy fwdcpy
#endif

/* "Type to Bytes" */
#define T_TO_B(tptr,mptr) little_endian_copy((mptr), (tptr), sizeof(*(tptr))); (mptr) += sizeof(*(tptr))

/* "Type from Bytes" */
#define T_FR_B(tptr,mptr) little_endian_copy((tptr), (mptr), sizeof(*(tptr))); (mptr) += sizeof(*(tptr))

/* what a message handler should look like */
typedef int (*smsg_message_handler_t)(smsg_byte *smsg_inbuf, int fd, void *handler_args);

/* args to the message handler thread */
typedef struct {
  smsg_message_handler_t handler; /* the caller's handler */
  int fd;			/* the file descriptor to read */
  void *handler_args;			/* any args to the handler */
} smsg_message_handler_thread_args_t;

/* the thread to be spawned */
extern void smsg_message_handler_thread(void *args);

typedef void *(*start_routine)(void *);

extern int
smsg_start_message_handler(smsg_message_handler_t handler, int fd, void *args, void **thread_ptr);

/* convenience function for sending a message and returning the first response */
extern int smsg_send_and_recv(int proxy_fd, smsg_byte *smsg_msgout, smsg_byte *smsg_outbuf, smsg_byte *writebuf, int writebufsize, smsg_byte *readbuf, int readbufsize, serdes_decode_state *state, smsg_byte *smsg_inbuf, int smsg_inbufsize, smsg_byte *smsg_msgin);

/* called by nonse to get proxy to open a socket */
extern int
smsg_get_proxy_socket_fd(int fd, int port);

/* called by proxy client to set up connection via proxy */
extern int
smsg_get_proxy_server_fd(int fd, char *hostname, smsg_port port);

/* get a broadcast port */
extern int
smsg_get_broadcast_fd(void);

/* broadcast a query */
extern int
smsg_send_broadcast(smsg_byte *smsg_msgout);

/* called by client to register itself with node manager */
extern int
smsg_register_component(int fd, /* if >= 0, the proxy fd */
			smsg_byte component_id, /* what you are */
			smsg_byte instance_id, /* what instance */
			smsg_byte node_id, /* what node */
			smsg_byte subsystem_id, /* what subsystem */
			smsg_addr *host_addr, /* filled in with host */
			smsg_port *component_port); /* filled in with port */

/* called by client to find a component via the node manager */
extern int
smsg_find_component(int fd,	/* if >= 0, the proxy fd */
		    smsg_byte component_id, /* what you are */
		    smsg_byte instance_id, /* what instance */
		    smsg_byte node_id, /* what node */
		    smsg_byte subsystem_id, /* what subsystem */
		    smsg_addr *host_addr, /* filled in with host */
		    smsg_port *component_port); /* filled in with port */

/* this union of all our messages will give us the max message size */
typedef union {
  smsg_request_dynreg_t request_dynreg;
  smsg_reply_dynreg_t reply_dynreg;
  smsg_query_dynreg_t query_dynreg;
  smsg_report_dynreg_t report_dynreg;
  smsg_query_allreg_t query_allreg;
  smsg_report_allreg_t report_allreg;
  smsg_open_client_connection_t open_client_connection;
  smsg_return_client_connection_t return_client_connection;
  smsg_open_server_connection_t open_server_connection;
  smsg_return_server_connection_t return_server_connection;
} smsg_all_message_t;

/* the max size of the packed bytes for our messages, which will be
   smaller than the max structure size due to packing */
#define SMSG_MAX_MESSAGE_SIZE sizeof(smsg_all_message_t)

/* how much space a decoded message will take */
#define SMSG_INBUFSIZE serdes_decode_size(SMSG_MAX_MESSAGE_SIZE)

/* how much space an encoded message will take */
#define SMSG_WRITEBUFSIZE serdes_encode_size(SMSG_MAX_MESSAGE_SIZE)

/* fills in the network address of the calling host */
extern smsg_addr
smsg_get_host_address(void);

extern smsg_byte
smsg_set_node_id(smsg_byte id);

extern smsg_byte
smsg_get_node_id(void);

extern smsg_byte
smsg_set_subsystem_id(smsg_byte id);

extern smsg_byte
smsg_get_subsystem_id(void);

enum {
  SMSG_DEBUG_CFG = 0x1,
  SMSG_DEBUG_MSG = 0x2,
  SMSG_DEBUG_ERR = 0x4,
  SMSG_DEBUG_REG = 0x8,
  SMSG_DEBUG_BCAST = 0x10,
  SMSG_DEBUG_MEM = 0x20,
  SMSG_DEBUG_DB = 0x40,
  SMSG_DEBUG_ALL = ~0
};

extern unsigned int smsg_debug_mask;
extern char *smsg_debug_name;

extern void
smsg_set_debug_mask(unsigned int mask);

extern void
smsg_set_debug_name(char *name);

extern void
smsg_print_debug(unsigned int mask, const char *fmt, ...);

#ifdef __cplusplus
#if 0
{
#endif
} /* really matches extern "C" above */
#endif

#endif	/* SMSG_H */
