# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/vrpn/src"
  "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/vrpn/build"
  "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn_install"
  "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/tmp"
  "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp"
  "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src"
  "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/sofia/Documents/E-Rocket-All-Stuff/E-Rocket-flight-software/build/vrpn_vendor/vrpn-prefix/src/vrpn-stamp${cfgdir}") # cfgdir has leading slash
endif()
