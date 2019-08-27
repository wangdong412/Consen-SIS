# Install script for directory: /vagrant/workspace/CM_FW

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/vagrant/workspace/CM_FW/build/Sys/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/InterComm/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/LOG/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/SOE/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/AMS/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/CLIENT/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/CS1131/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/OPC/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/P2P/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/MODBUS/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/GPS/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/NTP/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/ExterComm/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/CommonLib/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/Task/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/LED/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/Config/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/Version/cmake_install.cmake")
  INCLUDE("/vagrant/workspace/CM_FW/build/Monitor/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/vagrant/workspace/CM_FW/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/vagrant/workspace/CM_FW/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
