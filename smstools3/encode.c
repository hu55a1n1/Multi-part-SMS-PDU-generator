/*
gcc encode.c pdu.c charset.c logging.c extras.c locking.c stats.c cfgfile.c alarm.c modeminit.c whitelist.c blacklist.c smsd_cfg.c smsd.c -o pdu -D NUMBER_OF_MODEMS=1 -D NOSTATS
gcc encode.c codec.o -o testing
*/


#include <string.h>
#include <stdio.h>
#include "pdu.h"

int main(void) {
    // smsd.c:  make_pdu(to,text,textlen,alphabet,flash,report,with_udh,udh_data,DEVICE.mode,pdu,validity, replace_msg, system_msg, to_type);

    // char* message = "www.knivesandengraving.com";
    char *message = "viltsik";
    char *to = "3725215471";
    int alphabet = 1;
    int flash = 0;
    int with_udh = 0;
    char *udh_data = "";
    char *mode = "new";
    int report = 0;
    char pdu[1024] = {0};
    int validity = 167; // 1 day
    int system_msg = 0;
    int replace_msg = 0;
    int to_type = 1;
    char *smsc = "";

    make_pdu(to, message, strlen(message), alphabet, flash, report, with_udh, udh_data, mode, pdu, validity,
             replace_msg, system_msg, to_type, smsc);

    printf("\nPDU: %s\n\n", pdu);

    char *mode1 = "new";
    int alphabet1[3] = {0};
    char sendr[25] = {0};
    char date[25] = {0};
    char recvd_time[25] = {0};
    char message1[1024] = {0};
    char smsc1[25] = {0};
    int with_udh1[3] = {0};
    char a_udh_data[3] = {0};
    char a_udh_type[3] = {0};
    int is_statusreport;
    int is_unsupported_pdu;
    char from_toa[3] = {0};
    int report1[2] = {0};
    int replace[2] = {0};
    char warning_headers[20] = {0};
    int flash1[2] = {0};
    int bin_udh = 0;
    char *received_msg = "07916998051030F2040B916998775706F80000410192518220611254791E447F83E4E530194447A7E72E10";


    splitpdu(received_msg, mode1, alphabet1, sendr, date, recvd_time, message1, \
               smsc1, with_udh1, a_udh_data, a_udh_type, &is_statusreport, \
               &is_unsupported_pdu, from_toa, report1, replace, warning_headers, \
               flash1, bin_udh);
    printf("received message = %s from sender = %s at time = %s\n", message1, sendr, recvd_time);

    splitpdu(pdu, mode1, alphabet1, sendr, date, recvd_time, message1, \
               smsc1, with_udh1, a_udh_data, a_udh_type, &is_statusreport, \
               &is_unsupported_pdu, from_toa, report1, replace, warning_headers, \
               flash1, bin_udh);
    if (!is_unsupported_pdu) {
        printf("there is a msg\n");
    }
    else {
        printf("there's a problem with that pdu. It isn't supported!\n");
    }

    return 0;
}
