/*
gcc -c pdu.c charset.c logging.c extras.c locking.c stats.c cfgfile.c alarm.c modeminit.c whitelist.c blacklist.c smsd_cfg.c smsd.c -D NUMBER_OF_MODEMS=1 -D NOSTATS -g
ld -r pdu.o charset.o logging.o extras.o locking.o stats.o cfgfile.o alarm.o modeminit.o whitelist.o blacklist.o smsd_cfg.o smsd.o -o codec.o
gcc -c pdu_codec.c
ld -r pdu_codec.o PDU-master/codec.o -o pdu.o
gcc pdu_test.c pdu.o -o pdu_test
*/


#include "pdu_codec.h"

int main()
{
  int cport_nr=16,        /* /dev/ttyUSB0 for testing */
      bdrate=38400;       /* 38400 baud */

  unsigned char buf[4096] = {0};
  char* str;
  int t1, k;

  char s[30] = {0};

  char* message = "1, 2, 3, 4, 5, 6, 7, 8, 9 and 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100.";
  //char* message = "1, 2, 3, 4, 5, 6, 7, 8, 9 and 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, !!";
  char* message1 = "1, 2, 3, 4, 5, 6, 7, 8, 9!!";

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
  free_pdu(&msgs, pdu);

  char **pdu1 = encode_to_pdu(&msgs, to, message1, strlen(message1), alphabet, flash, report, mode, validity, replace_msg, system_msg, to_type, smsc);
  for(k=0;k<msgs;k++){
    printf("pdu1%d = %s\n", (int)k, pdu[k]);

  }
  free_pdu(&msgs, pdu1);


  char* mode1 = "new";
  int alphabet1[3] = {};
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
  char* received_msg = "07916998051030F2040B916998775706F80000410192518220611254791E447F83E4E530194447A7E72E10";

  splitpdu(received_msg, mode1, alphabet1, sendr, date, recvd_time, message2,\
               smsc1, with_udh1, a_udh_data, a_udh_type, &is_statusreport,\
               &is_unsupported_pdu, from_toa, report1, replace, warning_headers,\
               flash1, bin_udh);
  printf("received message = %s from sender = %s at time = %s\n", message1, sendr, recvd_time);

  splitpdu(pdu, mode1, alphabet1, sendr, date, recvd_time, message2,\
               smsc1, with_udh1, a_udh_data, a_udh_type, &is_statusreport,\
               &is_unsupported_pdu, from_toa, report1, replace, warning_headers,\
               flash1, bin_udh);
  if(!is_unsupported_pdu){
    printf("Valid pdu. Now access output. See pdu.h for details!\n");
  }
  else{
    printf("There's a problem with that pdu. It isn't supported!\n");
  }
  return(0);
}
