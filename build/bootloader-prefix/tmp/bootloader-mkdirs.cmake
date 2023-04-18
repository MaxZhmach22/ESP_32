# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/Espressif/frameworks/esp-idf-v5.0.1/components/bootloader/subproject"
  "E:/Work/ESP32/WIFI_STA_UDP_CLIENT_TX/build/bootloader"
  "E:/Work/ESP32/WIFI_STA_UDP_CLIENT_TX/build/bootloader-prefix"
  "E:/Work/ESP32/WIFI_STA_UDP_CLIENT_TX/build/bootloader-prefix/tmp"
  "E:/Work/ESP32/WIFI_STA_UDP_CLIENT_TX/build/bootloader-prefix/src/bootloader-stamp"
  "E:/Work/ESP32/WIFI_STA_UDP_CLIENT_TX/build/bootloader-prefix/src"
  "E:/Work/ESP32/WIFI_STA_UDP_CLIENT_TX/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/Work/ESP32/WIFI_STA_UDP_CLIENT_TX/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/Work/ESP32/WIFI_STA_UDP_CLIENT_TX/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
