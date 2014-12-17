/*
gcc -c pdu_codec.c
ld -r pdu_codec.o PDU-master/codec.o -o pdu.o
gcc pdu_test.c pdu.o -o pdu_test
*/


#include "pdu_codec.h"

#define MSG_NUM_LIMIT   9     //*DON'T INCREASE THAT ANY FURTHER, AS 10 is 2 digits and will cause problem with snprintf below while adding UDH header.

static int ref = 0;

void free_pdu(size_t* n_msgs, char** pdu){
  int k;
  for(k = 0; k < (*n_msgs); k++){
    //printf("%s\n", pdu[k]);
    free(pdu[k]);
  }
  free(pdu);
  return;
}

char** encode_to_pdu(size_t* n_msgs, char* to, char* message, int messagelen, int alphabet, int flash_sms, int report, char* mode, int validity, int replace_msg, int system_msg, int to_type, char *smsc){   // 140 chars = 306 pdu!!!

  int i = 0;
  int j = 0;
  int numberOfMsgs = 0;
  int charsInLastMsg = (int)(strlen(message)%SIZE_OF_MSG);
  char m[MSG_NUM_LIMIT][1024] = {0};

  int with_udh = 0;
  char udh_data[18] = {0};

  srand(time(NULL));
  ref = rand() % 90 + 10;

  //ref = (ref + 1) % 9;

  if((int)strlen(message) > SIZE_OF_MSG*MSG_NUM_LIMIT){
    return NULL;
  }

  if((int)(strlen(message)%SIZE_OF_MSG)){
    numberOfMsgs = (int)(strlen(message)/SIZE_OF_MSG) + 1;
  }
  else{
    numberOfMsgs = (int)(strlen(message)/SIZE_OF_MSG);
  }
  *n_msgs = numberOfMsgs;
  //printf("message length = %d, we will have %d messages, and last msg will have %d characters\n", (int)strlen(message), numberOfMsgs, charsInLastMsg);


  char **pdu = malloc(numberOfMsgs * sizeof(char*));
  for (j =0 ; j < numberOfMsgs; j++){
    pdu[j] = malloc(512 * sizeof(char));
  }

  if(numberOfMsgs == 1){    // Don't add UDH.
    with_udh = 0;
    make_pdu(to, message, strlen(message), alphabet, flash_sms, report, with_udh, udh_data, mode, pdu[0], validity, replace_msg, system_msg, to_type, smsc);
    //printf("single message pdu = %s\n", pdu[0]);
    return pdu;
  }
  else{
    with_udh = 1;
    for(i=0;i<numberOfMsgs;i++){
      if(i == numberOfMsgs - 1){

        memcpy(m[i], message + (SIZE_OF_MSG*i), charsInLastMsg);
        m[i][charsInLastMsg] = '\0';
      }
      else{
        memcpy(m[i], message + (SIZE_OF_MSG*i), SIZE_OF_MSG);
        m[i][SIZE_OF_MSG] = '\0';
        
      }
      bzero(udh_data, 18);

      snprintf(udh_data, 18, "05 00 03 %d 0%d 0%d", ref, numberOfMsgs, i+1);
      printf("UDH Header = %s\n", udh_data);
      make_pdu(to, m[i], strlen(m[i]), alphabet, flash_sms, report, with_udh, udh_data, mode, pdu[i], validity, replace_msg, system_msg, to_type, smsc);

      //printf("size to send: %d\n", (int)((strlen(pdu[i])-2)/2));
      //printf("m%d = %s\n", i, m[i]);
      //printf("pdu%d = %s\n", i, pdu[i]);
    }
  }

  return pdu;
}