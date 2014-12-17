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

  int k;

  char* message = "1, 2, 3, 4, 5, 6, 7, 8, 9 and 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100. + 1, 2, 3, 4, 5, 6, 7, 8, 9 and 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100.\
                    1, 2, 3, 4, 5, 6, 7, 8, 9 and 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100.";

  char* to = "97775608";
  int alphabet = 0;
  int flash = 0;
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

  return(0);
}
