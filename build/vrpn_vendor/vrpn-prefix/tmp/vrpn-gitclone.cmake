# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp/vrpn-gitclone-lastrun.txt" AND EXISTS "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp/vrpn-gitinfo.txt" AND
  "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp/vrpn-gitclone-lastrun.txt" IS_NEWER_THAN "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp/vrpn-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp/vrpn-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/vrpn/src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/vrpn/src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"
            clone --no-checkout --depth 1 --no-single-branch --config "advice.detachedHead=false" "https://github.com/vrpn/vrpn" "src"
    WORKING_DIRECTORY "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/vrpn"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/vrpn/vrpn'")
endif()

execute_process(
  COMMAND "/usr/bin/git"
          checkout "v07.35" --
  WORKING_DIRECTORY "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/vrpn/src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'v07.35'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/vrpn/src"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/vrpn/src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp/vrpn-gitinfo.txt" "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp/vrpn-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp/vrpn-gitclone-lastrun.txt'")
endif()
