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
#include "pdu_codec.h"
#include <locale.h>
#include <wchar.h>

int main()
{
  int cport_nr=22,        /* /dev/ttyAMA0 for testing */
      bdrate=38400;       /* 38400 baud */

  unsigned char buf[4096] = {0};
  char* str;
  int t1, k;


  char s[30] = {0};

  char* message = "1, 2, 3, 4, 5, 6, 7, 8, 9 and 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,123490913092380918230"; //140
  //char* message = "1, 2, 3, 4, 5, 6, 7, 8, 9 and 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, "; //90
  //char* message = "(rest)\nYou have been successfully added as Admin.\n\nDetails:\nUsername: 96897775608\nPassword: 1235\nControl panel number: 96897775606\nDefault language: Arabic\nCountry code: 968\n\nwww.astrolabetech.com/s720";
  //char* message1 = "ئهجگچج";  //Should be 07916998051030F2040B916998775706F80008410192810153610C06260647062C06AF0686062C
                              //in encoding 2 0011000891797765800008A70CD8A6D987D8ACDAAFDA86D8AC
  //char* message1 = "\x06\xA3";
  //char *message1 = "\x06\x33\x06\x44\x06\x4a\x06\x45"; 
  //const wchar_t *name = L"r\u00e9sum\u00e9";
  //setlocale(LC_CTYPE, "en_US.UTF-8");
  //wprintf(L"name is %ls\n", name);
  char* to = "96897775608";
  int alphabet = 0;
  int flash = 1;
  int with_udh = 0;
  char* udh_data = "";
  char* mode = "new";
  int report = 0;
  int validity = 167; // 1 day
  int system_msg = 0;
  int replace_msg = 0;
  int to_type = 1;
  char* smsc = "";
  size_t msgs;


  if(!init_sim(GSM_SERIAL_PORT, 38400)){
        printf("failed to init sim module\n");
    }
  char **pdu = encode_to_pdu(&msgs, to, message, strlen(message), alphabet, flash, report, mode, validity, replace_msg, system_msg, to_type, smsc);
/*
  for(k=0;k<msgs;k++){
    printf("pdu%d = %s\n", (int)k, pdu[k]);

  }
  */
  if(pdu){
    for(k=0;k<msgs;k++){
            printf("pdu%d = %s\n", (int)k, pdu[k]);
            if(sendMsgToNumber(to, pdu[k])){
                printf("msg sent\n");
            }
            else{
                printf("msg not sent\n");
            }
            if(msgs > 1){
                sleep(2);       //Necessary if more than one msg
            }
        }
      free_pdu(&msgs, pdu);
  }
/*
  char **pdu1 = encode_to_pdu(&msgs, to, message1, strlen(message1), alphabet, flash, report, mode, validity, replace_msg, system_msg, to_type, smsc);
    
  for(k=0;k<msgs;k++){
    printf("pdu1%d = %s\n", (int)k, pdu1[k]);

  }
  if(pdu1){
      free_pdu(&msgs, pdu1);
  }
*/
  return(0);
}
