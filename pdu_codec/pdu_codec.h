
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#define SIZE_OF_MSG 136			//Corresponds exactly to AT+CMGS=140 (max allowed SMS size at 7bit alphabet pdu encoding)

char** encode_to_pdu(size_t* n_msgs, char* to, char* message, int messagelen, int alphabet, int flash_sms, int report, char* mode, int validity, int replace_msg, int system_msg, int to_type, char *smsc);
void free_pdu(size_t* n_msgs, char** pdu);
