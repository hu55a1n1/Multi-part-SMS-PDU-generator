Multi-part-SMS-PDU-generator
============================

A simplified C library for generating PDU encoded multi-part multilingual SMS. This library uses the versatile SMS Server Tools for encoding/decoding. 

User provided string is split into 140 char chunks and then encoded into pdu format. 

This library can be used to encode multilingual messages (including arabic and persian) to PDU and decode a PDU into its component data. 
To know more about PDU encoding, you may find the material in the 'docs' directory useful. The SMS-PDU converter [here](http://www.diafaan.com/sms-tutorials/gsm-modem-tutorial/online-sms-pdu-decoder/) may also come in handy.


Build:
======

This project depends on CMake.
If CMake isn't already installed on your system, you can do so by building it from [source](https://cmake.org/download) (which is the preferred method).

Then run the following from the project's root directory:

```
mkdir -p build
cd build
cmake ..
make
```

This should have built `smstools3` as a static library and `sample1.c` as well in the build directory.

To rebuild, delete the build directory and run above commands again.


Usage:
======

The API is pretty straight forward. Check the sample provided. 

And, don't forget to call `free_pdu()` after you're done with the pdu.

For sending arabic (or other languages), use `alphabet=2` and unicode for every two bytes as `\x06\x33\x06\x44\x06\x4a\x06\x45` for سليم.

[Here's](https://www.branah.com/unicode-converter) a good online converter.


Support:
========

If you have any queries, please ask on stackoverflow.com using the tag `multipart-pdu-gen-c`.


Credits:
========

This project essentially provides a linkable API to an earlier modified version of [SMS Server Tools 3](smstools3.kekekasvi.com/) code. So all credit to Stefan Frings (original author of SMS Server Tools), Keijo "Keke" Kasvi (current maintainer) and other contributors.
