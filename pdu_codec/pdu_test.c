/*
gcc -c pdu.c charset.c logging.c extras.c locking.c stats.c cfgfile.c alarm.c modeminit.c whitelist.c blacklist.c smsd_cfg.c smsd.c -D NUMBER_OF_MODEMS=1 -D NOSTATS -g
ld -r pdu.o charset.o logging.o extras.o locking.o stats.o cfgfile.o alarm.o modeminit.o whitelist.o blacklist.o smsd_cfg.o smsd.o -o codec.o
gcc -c pdu_codec.c
ld -r pdu_codec.o codec.o -o pdu.o
gcc pdu_test.c pdu.o -o pdu_test

For sending arabic, use alphabet=2 and unicode characters for every two bytes as \x06\x33\x06\x44\x06\x4a\x06\x45 for سليم. https://www.branah.com/unicode-converter 
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

  char* message = "1, 2, 3, 4, 5, 6, 7, 8, 9 and 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 1, 2, 3, 4, 5, 6, 7, 8, 9 and 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100.";
  //char* message = "1, 2, 3, 4, 5, 6, 7, 8, 9 and 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, !!";
  //char* message1 = "ئهجگچج";  //Should be 07916998051030F2040B916998775706F80008410192810153610C06260647062C06AF0686062C
                              //in encoding 2 0011000891797765800008A70CD8A6D987D8ACDAAFDA86D8AC
  //char* message1 = "\x06\xA3";
  char *message1 = "\x06\x33\x06\x44\x06\x4a\x06\x45"; 
  //const wchar_t *name = L"r\u00e9sum\u00e9";
  //setlocale(LC_CTYPE, "en_US.UTF-8");
  //wprintf(L"name is %ls\n", name);
  char* to = "97775608";
  int alphabet = 0;
  int flash = 0;
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
  
  char **pdu = encode_to_pdu(&msgs, to, message, strlen(message), alphabet, flash, report, mode, validity, replace_msg, system_msg, to_type, smsc);

  for(k=0;k<msgs;k++){
    printf("pdu%d = %s\n", (int)k, pdu[k]);

  }
    if(pdu){
        free_pdu(&msgs, pdu);
    }

  char **pdu1 = encode_to_pdu(&msgs, to, message1, strlen(message1), alphabet, flash, report, mode, validity, replace_msg, system_msg, to_type, smsc);
    
  for(k=0;k<msgs;k++){
    printf("pdu1%d = %s\n", (int)k, pdu1[k]);

  }
    if(pdu1){
        free_pdu(&msgs, pdu1);
    }
//  free_pdu(&msgs, pdu1);
    /*

  char* mode1 = "new";
  int alphabet1;
  char sendr[25] = {};
  char date[25] = {};
  char recvd_time[25] = {};
  char message2[1024] = {};
  char smsc1[25] = {};
  int with_udh1[3] = {};
  char a_udh_data[3] = {};
  char a_udh_type[3] = {};
  int is_statusreport;
  int is_unsupported_pdu;
  char from_toa[3] = {};
  int report1[2] = {};
  int replace[2] = {};
  char warning_headers[20] = {};
  int flash1[2] = {};
  int bin_udh = 0; 
  char* received_msg = "07916998051030F7040B916998775706F80000411121019465610641791B348F03";

  splitpdu(received_msg, mode1, &alphabet1, sendr, date, recvd_time, message2,\
               smsc1, with_udh1, a_udh_data, a_udh_type, &is_statusreport,\
               &is_unsupported_pdu, from_toa, report1, replace, warning_headers,\
               flash1, bin_udh);
  printf("received message = %s from sender = %s at time = %s\n", message2, sendr, recvd_time);

  splitpdu(pdu, mode1, &alphabet1, sendr, date, recvd_time, message2,\
               smsc1, with_udh1, a_udh_data, a_udh_type, &is_statusreport,\
               &is_unsupported_pdu, from_toa, report1, replace, warning_headers,\
               flash1, bin_udh);
  if(!is_unsupported_pdu){
    printf("Valid pdu. Now access output. See pdu.h for details!\n");
  }
  else{
    printf("There's a problem with that pdu. It isn't supported!\n");
  }
     */
  
  return(0);
}
