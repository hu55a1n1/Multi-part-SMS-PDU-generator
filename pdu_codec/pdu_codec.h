/*  
This file is part of Multi-part-SMS-PDU-generator.
Copyright (C) 2015 Shoaib Ahmed.

Multi-part-SMS-PDU-generator is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Multi-part-SMS-PDU-generator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Multi-part-SMS-PDU-generator.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#define SIZE_OF_MSG 136			//Corresponds exactly to AT+CMGS=140 (max allowed SMS size at 7bit alphabet pdu encoding)

char** encode_to_pdu(size_t* n_msgs, char* to, char* message, int messagelen, int alphabet, int flash_sms, int report, char* mode, int validity, int replace_msg, int system_msg, int to_type, char *smsc);
void free_pdu(size_t* n_msgs, char** pdu);
