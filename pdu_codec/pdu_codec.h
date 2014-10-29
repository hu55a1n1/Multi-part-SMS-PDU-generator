
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define SIZE_OF_MSG 140

char** encode_to_pdu(size_t* n_msgs, char* to, char* message, int messagelen, int alphabet, int flash_sms, int report, char* mode, int validity, int replace_msg, int system_msg, int to_type, char *smsc);
void free_pdu(size_t* n_msgs, char** pdu);
