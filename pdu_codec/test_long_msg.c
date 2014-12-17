/*
gcc -c pdu.c charset.c logging.c extras.c locking.c stats.c cfgfile.c alarm.c modeminit.c whitelist.c blacklist.c smsd_cfg.c smsd.c -D NUMBER_OF_MODEMS=1 -D NOSTATS -g
ld -r pdu.o charset.o logging.o extras.o locking.o stats.o cfgfile.o alarm.o modeminit.o whitelist.o blacklist.o smsd_cfg.o smsd.o -o codec.o
gcc -c pdu_codec.c
ld -r pdu_codec.o PDU-master/codec.o -o pdu.o
gcc test_long_msg.c ../rs232.c ../timer.c pdu.o -o test_long_msg -lrt

For sending arabic, use alphabet=2 and unicode characters for every two bytes as \x06\x33\x06\x44\x06\x4a\x06\x45 for سليم. https://www.branah.com/unicode-converter 
*/

#include "../rs232.h"
#include "../timer.h"
#include "pdu_codec.h"
#include <locale.h>
#include <wchar.h>

#define FALSE 0
#define TRUE 1
#define GSM_SERIAL_PORT   22    //ttyAMA0


bool parseFind(const char *const theString, const char *objectOfDesire) {
    if (!theString) return 0;                       // If we get a NULL pointer bail
    return strstr(theString, objectOfDesire);
}

int readSerialPort(int serial_port, char *buffer) {
    int i;
    int n = RS232_PollComport(serial_port, buffer, 4095); // Poll serial port

    if (n > 0) {
        buffer[n] = '\0';   /* always put a "null" at the end of a string! */
        for (i = 0; i < n; i++) {
            if (buffer[i] < 32)  /* replace unreadable control-codes by dots */
            {
                //buffer[i] = '.';
            }
        }
        //printf("%s\n", (char *)buffer);
    }
    return n;
}


bool init_sim(int serial_port, int baud_rate) {
    char buf[4096] = {};
    bool sent = FALSE;
    int t1;

    if (RS232_OpenComport(serial_port, baud_rate))        // Returns 1 for error!
    {
        //if(debug){ printf("Can not open comport\n");}
        sent = FALSE;
    }
    char *msg = "AT+CMGF=0\r";
    //if(debug) { printf("Initializing sim900 module: %s\n", msg);}
    int wrote_bytes = RS232_SendBuf(serial_port, msg, strlen(msg));

    if (wrote_bytes == -1) {
        printf("Error in write: failed to send\n");
        sent = FALSE;
    }

    if (timeout_init()) {
        fprintf(stderr, "timeout_init(): %s.\n", strerror(errno));
        sent = FALSE;
    }

    t1 = timeout_set(3);    // Should be outside the while loop.
    while (1)                        // poll if ready
    {

        if (readSerialPort(GSM_SERIAL_PORT, buf)) {
            if (parseFind(buf, "OK")) {      // if ready to receive msg content
                printf("Initialised modem successfully\n");
                sent = TRUE;
                break;
            }
        }
        if (timeout_passed(t1)) {
            //printf("Failed to initialise modem\n");
            sent = FALSE;
            break;
        }
        usleep(100000);
        //bzero(buf, 4096);
    }

    timeout_unset(t1);
    if (timeout_done()) {
        fprintf(stderr, "timeout_done(): %s.\n", strerror(errno));
    }

    return sent;
}

bool sendMsgToNumber(const char* number, char* message){
    char s[30] = {0};
    int t1;
    bool sent = FALSE;
    char buf[4096] = {};
    //char numSansCC[10] = {};
    sleep(3);
    if (timeout_init()) {
        fprintf(stderr, "timeout_init(): %s.\n", strerror(errno));
        return FALSE;
    }

    //printf("size to send: %d\n", (int)((strlen(message)-2)/2));

    snprintf(s, 29, "AT+CMGS=%d\r", (int)((strlen(message)-2)/2));    // send msg PDU mode
    RS232_SendBuf(GSM_SERIAL_PORT, s, strlen(s));

    t1 = timeout_set(3);    // Should be outside the while loop.
    while(1)                        // poll if ready
    {

        if(readSerialPort(GSM_SERIAL_PORT, buf)) {
            if(parseFind(buf, ">")){      // if ready to receive msg content
                printf("Modem reply: %s\n", buf);
                printf("\nmsg started\n");
                RS232_SendBuf(GSM_SERIAL_PORT, message, strlen(message));
                RS232_SendBuf(GSM_SERIAL_PORT, "\x1a", 2);
                sent = TRUE;
                break;
            }
        }
        if(timeout_passed(t1))
        {
            RS232_SendBuf(GSM_SERIAL_PORT, "\x1b", 2);
            sent = FALSE;
            break;
        }
        usleep(100000);
        //bzero(buf, 4096);
    }

    timeout_unset(t1);
    if (timeout_done()) {
        fprintf(stderr, "timeout_done(): %s.\n", strerror(errno));
    }
    return sent;
}

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
