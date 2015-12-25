cmake_minimum_required(VERSION 3.3)
project(Multi_part_SMS_PDU_generator)
enable_language(C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=c99")

add_definitions(-DNUMBER_OF_MODEMS=1 -DNOSTATS)

set(SMSTOOLS3_SOURCE_FILES
        pdu_codec/smstools3/alarm.c
        pdu_codec/smstools3/blacklist.c
        pdu_codec/smstools3/cfgfile.c
        pdu_codec/smstools3/charset.c
        pdu_codec/smstools3/encode.c
        pdu_codec/smstools3/extras.c
        pdu_codec/smstools3/locking.c
        pdu_codec/smstools3/logging.c
        #    pdu_codec/smstools3/main2.c
        pdu_codec/smstools3/modeminit.c
        #    pdu_codec/smstools3/msg.c
        pdu_codec/smstools3/pdu.c
        pdu_codec/smstools3/smsd.c
        pdu_codec/smstools3/smsd_cfg.c
        pdu_codec/smstools3/stats.c
        pdu_codec/smstools3/whitelist.c
)

set(PDU_SOURCE_FILES
        pdu_codec/pdu_codec.c
        pdu_codec/pdu_test.c
)

add_library(SMSTOOLS3 ${SMSTOOLS3_SOURCE_FILES})
add_executable(Multi_part_SMS_PDU_generator ${PDU_SOURCE_FILES})

target_link_libraries(Multi_part_SMS_PDU_generator SMSTOOLS3)
