#########################################################################################
#                                                                                       #
# CAG RTS project example for code structure using CMake                                #
#                                                                                       #
# CAG Copyright (c) 2015                                                                #
#                                                                                       #
#########################################################################################

#########################################################################################
# General Settings
#########################################################################################


PROJECT(RTS)                       #projcet name
CMAKE_MINIMUM_REQUIRED(VERSION 2.8) #cmake version

# Actiating c, requires a modern compiler
SET(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -static -O0 -Wall -g -ggdb")  #set debug mode c flags
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -static -O3 -Wall ")         #set release mode c flags
SET(CMAKE_BUILD_TYPE "Release")     #set Debug or Release

#######################################
# The home-made library
#######################################
# set variables for home made library

SET(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}) #modify this to yours

#########################################################################################
# Executable
#########################################################################################

# add the src directory to CMake build, the binary dependent settings occurs in the
# src folder CMakeLists.txt

ADD_SUBDIRECTORY(Sys)
ADD_SUBDIRECTORY(InterComm)
ADD_SUBDIRECTORY(SOE)
ADD_SUBDIRECTORY(AMS)
ADD_SUBDIRECTORY(CLIENT)
ADD_SUBDIRECTORY(CS1131)
ADD_SUBDIRECTORY(OPC)
ADD_SUBDIRECTORY(P2P)
ADD_SUBDIRECTORY(MODBUS)
ADD_SUBDIRECTORY(GPS)
ADD_SUBDIRECTORY(NTP)
ADD_SUBDIRECTORY(ExterComm)
ADD_SUBDIRECTORY(CommonLib)
ADD_SUBDIRECTORY(Task)
ADD_SUBDIRECTORY(LED)
ADD_SUBDIRECTORY(Config)
ADD_SUBDIRECTORY(Version)
ADD_SUBDIRECTORY(Monitor)

INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR} ) 

AUX_SOURCE_DIRECTORY(Task/src DIR_SRCS)    #assigin current dir path to var DIR_SRCS

ADD_EXECUTABLE(RTS ${DIR_SRCS} ) #means source file in DIR_SRCS will be built into a executable file `RTS`


TARGET_LINK_LIBRARIES(RTS INTERCOMM) # add subdir source

TARGET_LINK_LIBRARIES(RTS CS1131) # add subdir source

TARGET_LINK_LIBRARIES(RTS SOE) # add subdir source

TARGET_LINK_LIBRARIES(RTS AMS) # add subdir source

TARGET_LINK_LIBRARIES(RTS CLIENT) # add subdir source

TARGET_LINK_LIBRARIES(RTS OPC) # add subdir source

TARGET_LINK_LIBRARIES(RTS P2P) # add subdir source

TARGET_LINK_LIBRARIES(RTS MODBUS) # add subdir source

TARGET_LINK_LIBRARIES(RTS GPS) # add subdir source

TARGET_LINK_LIBRARIES(RTS NTP) # add subdir source

TARGET_LINK_LIBRARIES(RTS EXTERCOMM) # add subdir source

TARGET_LINK_LIBRARIES(RTS SYS) # add subdir source

TARGET_LINK_LIBRARIES(RTS TASK) # add subdir source

TARGET_LINK_LIBRARIES(RTS COMMONLIB) # add subdir source

TARGET_LINK_LIBRARIES(RTS LED) # add subdir source

TARGET_LINK_LIBRARIES(RTS CONFIG) # add subdir source

TARGET_LINK_LIBRARIES(RTS VERSION) # add subdir source

TARGET_LINK_LIBRARIES(RTS MONITOR) # add subdir source


