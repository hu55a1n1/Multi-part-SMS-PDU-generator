/**************************************************

file: main.c
purpose: simple demo that receives characters from
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

**************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "rs232.h"
#include "timer.h"
#include "pdu_codec/pdu.h"


char* parsedData;
int fd, res;
char incomingNo[15];


const char* const parseData(const char* const theString,const char* start, const char* end){
    if (!theString) return 0;                       // If we get a NULL pointer bail        
    size_t startSize = strlen(start);               // get size of string 
    char* startP = strstr (theString,start);        // looks for string gives pointer including look
    if(!startP) return 0;                           // If we didn't find begining of string *MEM LEAK IF TAKEN OUT*
    startP+=startSize;                              // offset (gets rid of delim)
    char* endP = strstr ((startP),end);             // starts at startP looks for END string
    if(!endP) return 0;                             // We didn't find end                   *MEM LEAK IF TAKEN OUT*
    free(parsedData);                               // if it has been allocated, Free the memory
    parsedData = NULL;
    parsedData = (char*) malloc((size_t)(sizeof(char)*(endP-startP)+1));      // get memory 
    if (parsedData == NULL) {
        return 0;
    }                                               // if we couldn't get mem

    int dataPos=0;
    while ( startP != endP ){                       // grab between starP and endP
        parsedData[dataPos++]= *startP++;
    }
    parsedData[dataPos]= '\0';                      // NULL to make a proper string
    return parsedData;                                      // gives back what it can. parsData has class scope.
}


bool parseFind(const char* const theString ,const char* objectOfDesire){
    if (!theString) return 0;                       // If we get a NULL pointer bail        
    return strstr(theString,objectOfDesire);
}


int readSerialPort(int comport_number, unsigned char *buffer)
{
  int i;
  int n = RS232_PollComport(comport_number, buffer, 4095); // Poll serial port

  if(n > 0)
  {
    buffer[n] = 0;   /* always put a "null" at the end of a string! */

    for(i=0; i < n; i++)
    {
      if(buffer[i] < 32)  /* replace unreadable control-codes by dots */
      {
        buffer[i] = '.';
      }
    }

    printf("%s\n", (char *)buffer);
  }
  return n;
}

int main()
{
  int cport_nr=16,        /* /dev/ttyUSB0 for testing */
      bdrate=38400;       /* 38400 baud */

  unsigned char buf[4096] = {0};
  char* str;
  int t1;

  if(RS232_OpenComport(cport_nr, bdrate))
  {
    printf("Can not open comport\n");

    return(0);
  }
  if (timeout_init()) {
    fprintf(stderr, "timeout_init(): %s.\n", strerror(errno));
    return 1;
  }
  
  while(1)
  {
    if(readSerialPort(cport_nr, buf)) {
    	if(parseData(buf, "+CLIP: \"", "\",")){  // If there's an incoming call
        str = "ATH\r";
        RS232_SendBuf(cport_nr, str, strlen(str));   // cancel the call
        printf("\nNumber was %s\n\n", parsedData);
        strncpy(incomingNo, parsedData, 15);    // copy number for use later
        sleep(3);                     // very important delay for gsm module
        char s[30] = {0};
        /*
        snprintf(s, 29, "AT+CMGS=\"%s\"\r", incomingNo);    // send msg
        RS232_SendBuf(cport_nr, s, strlen(s));

        while(1)                        // poll if ready
        {
          t1 = timeout_set(3);
          if(readSerialPort(cport_nr, buf)) {
            if(parseFind(buf, ">")){      // if ready to receive msg content
            	printf("\nmsg started\n");
            	str = "Test msg";
            	RS232_SendBuf(cport_nr, str, strlen(str));
             	RS232_SendBuf(cport_nr, "\x1a", 2);
        	    break;
            }
          }
          if(timeout_passed(t1))
          {
            RS232_SendBuf(cport_nr, "\x1b", 2);
            break;
          }
          usleep(100000);
        }
        */

        RS232_SendBuf(cport_nr, "20", 2);

        while(1)                        // poll if ready
        {
          t1 = timeout_set(3);
          if(readSerialPort(cport_nr, buf)) {
            if(parseFind(buf, ">")){      // if ready to receive msg content
              printf("\nmsg started\n");
              str = "0011000A9173521245170004A70776696C7473696B";
              RS232_SendBuf(cport_nr, str, strlen(str));
              RS232_SendBuf(cport_nr, "\x1a", 2);
              break;
            }
          }
          if(timeout_passed(t1))
          {
            RS232_SendBuf(cport_nr, "\x1b", 2);
            break;
          }
          usleep(100000);
        }
      }
    }
    usleep(100000);  /* sleep for 100 milliSeconds */
  }
  return(0);
}

