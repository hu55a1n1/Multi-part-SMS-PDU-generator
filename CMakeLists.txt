cmake_minimum_required(VERSION 3.3)
project(Multi_part_SMS_PDU_generator)
enable_language(C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g -Wall -pedantic")
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build)

add_definitions(-DNUMBER_OF_MODEMS=1 -DNOSTATS)

set(SMSTOOLS3_SOURCE_FILES
        smstools3/alarm.c
        smstools3/blacklist.c
        smstools3/cfgfile.c
        smstools3/charset.c
        smstools3/encode.c
        smstools3/extras.c
        smstools3/locking.c
        smstools3/logging.c
        smstools3/modeminit.c
        smstools3/pdu.c
        smstools3/smsd.c
        smstools3/smsd_cfg.c
        smstools3/stats.c
        smstools3/whitelist.c
        smstools3/smspdu.c
        )

add_library(SMSTOOLS3 STATIC ${SMSTOOLS3_SOURCE_FILES})


add_executable(sample1 sample1.c)
target_link_libraries(sample1 SMSTOOLS3)
