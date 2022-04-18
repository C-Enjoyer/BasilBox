/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.5 */

#ifndef PB_MESSAGES_PB_H_INCLUDED
#define PB_MESSAGES_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _BasilMessage_MessageType { 
    BasilMessage_MessageType_noMessageType = 0, 
    BasilMessage_MessageType_infoMessageType = 1 
} BasilMessage_MessageType;

/* Struct definitions */
typedef struct _InfoMessage { 
    bool has_pingId;
    int32_t pingId; 
} InfoMessage;

typedef struct _BasilMessage { 
    pb_size_t which_message;
    union {
        InfoMessage infoMessage;
    } message; 
} BasilMessage;


/* Helper constants for enums */
#define _BasilMessage_MessageType_MIN BasilMessage_MessageType_noMessageType
#define _BasilMessage_MessageType_MAX BasilMessage_MessageType_infoMessageType
#define _BasilMessage_MessageType_ARRAYSIZE ((BasilMessage_MessageType)(BasilMessage_MessageType_infoMessageType+1))


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define InfoMessage_init_default                 {false, 0}
#define BasilMessage_init_default                {0, {InfoMessage_init_default}}
#define InfoMessage_init_zero                    {false, 0}
#define BasilMessage_init_zero                   {0, {InfoMessage_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define InfoMessage_pingId_tag                   1
#define BasilMessage_infoMessage_tag             1

/* Struct field encoding specification for nanopb */
#define InfoMessage_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, INT32,    pingId,            1)
#define InfoMessage_CALLBACK NULL
#define InfoMessage_DEFAULT NULL

#define BasilMessage_FIELDLIST(X, a) \
X(a, STATIC,   ONEOF,    MESSAGE,  (message,infoMessage,message.infoMessage),   1)
#define BasilMessage_CALLBACK NULL
#define BasilMessage_DEFAULT NULL
#define BasilMessage_message_infoMessage_MSGTYPE InfoMessage

extern const pb_msgdesc_t InfoMessage_msg;
extern const pb_msgdesc_t BasilMessage_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define InfoMessage_fields &InfoMessage_msg
#define BasilMessage_fields &BasilMessage_msg

/* Maximum encoded size of messages (where known) */
#define BasilMessage_size                        13
#define InfoMessage_size                         11

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
