cmake_minimum_required(VERSION 3.15)
project(cpp C)

set(CMAKE_C_STANDARD 11)

include_directories(.
        /usr/lib/jvm/java-1.8.0-openjdk-1.8.0.242.b08-0.el7_7.x86_64/include
        /usr/lib/jvm/java-1.8.0-openjdk-1.8.0.242.b08-0.el7_7.x86_64/include/linux)

add_executable(cpp
        edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper.c
        edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper.h
        main.c
        oligotm.c
        oligotm.h
        thal.c
        thal.h
        thal_parameters.c
        thal_parameters.h)

target_link_libraries(cpp m)