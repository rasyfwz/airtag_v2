# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/rasyafawwaz/.espressif/v5.4.2/esp-idf/components/bootloader/subproject")
  file(MAKE_DIRECTORY "/home/rasyafawwaz/.espressif/v5.4.2/esp-idf/components/bootloader/subproject")
endif()
file(MAKE_DIRECTORY
  "/home/rasyafawwaz/esp/airtag_v2/i2c_basic/build/bootloader"
  "/home/rasyafawwaz/esp/airtag_v2/i2c_basic/build/bootloader-prefix"
  "/home/rasyafawwaz/esp/airtag_v2/i2c_basic/build/bootloader-prefix/tmp"
  "/home/rasyafawwaz/esp/airtag_v2/i2c_basic/build/bootloader-prefix/src/bootloader-stamp"
  "/home/rasyafawwaz/esp/airtag_v2/i2c_basic/build/bootloader-prefix/src"
  "/home/rasyafawwaz/esp/airtag_v2/i2c_basic/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/rasyafawwaz/esp/airtag_v2/i2c_basic/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/rasyafawwaz/esp/airtag_v2/i2c_basic/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
