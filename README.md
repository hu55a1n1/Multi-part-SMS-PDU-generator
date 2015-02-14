Multi-part-SMS-PDU-generator
============================

A simplified C library for generating PDU encoded multi-part multilingual SMS. This library uses the versatile SMS Server Tools for encoding/decoding. 

User provided string is split into 140 char chunks and then encoded into pdu format. 

This library can be used to encode multilingual messages (including arabic and persian) to PDU and decode a PDU into its component data. 
To know more about PDU encoding, you may find the material in the 'docs' directory useful. The SMS-PDU converter [here](http://www.diafaan.com/sms-tutorials/gsm-modem-tutorial/online-sms-pdu-decoder/) may also come in handy.


Compilation:
============

I apologize for the rather intricate compilation process. I will soon add a Makefile. 
For now, follow the steps below:

In the 'smstools3' directory:
-----------------------------
```
gcc -c pdu.c charset.c logging.c extras.c locking.c stats.c cfgfile.c alarm.c modeminit.c whitelist.c blacklist.c smsd_cfg.c smsd.c -D NUMBER_OF_MODEMS=1 -D NOSTATS -g
ld -r pdu.o charset.o logging.o extras.o locking.o stats.o cfgfile.o alarm.o modeminit.o whitelist.o blacklist.o smsd_cfg.o smsd.o -o codec.o
```

In the 'pdu_codec' directory:
-----------------------------
```
gcc -c pdu_codec.c
ld -r pdu_codec.o smstools3/codec.o -o pdu.o
```
To compile the sample:
----------------------
```
gcc sample.c pdu.o -o sample -lrt
```

Usage:
======

The API is pretty straight forward. Check the sample provided. 

And, don't forget to call free_pdu() after you're done with the pdu.

For sending arabic (or other languages), use alphabet=2 and unicode characters for every two bytes as \x06\x33\x06\x44\x06\x4a\x06\x45 for سليم. 

[Here's](https://www.branah.com/unicode-converter.) a good online converter.
