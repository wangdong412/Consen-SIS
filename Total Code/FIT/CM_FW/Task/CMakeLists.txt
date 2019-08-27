#########################################################################################
# Library sources
#########################################################################################

INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR} ) 

# Set the source directory and header dirctory
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# group source files for the projects categories, here only one is used
FILE(GLOB LIBRARY_SOURCE ${SOURCE_DIR}/*.c)
FILE(GLOB LIBRARY_HEADER ${HEADER_DIR}/*.h)

# create main group
SOURCE_GROUP(Library FILES ${LIBRARY_SOURCE} ${LIBRARY_HEADER})

# Add the sources and headers to one large file list
SET(EXTEND_SOURCE ${EXTEND_SOURCE} ${LIBRARY_SOURCE})
SET(EXTEND_HEADER ${EXTEND_HEADER} ${LIBRARY_HEADER})

#########################################################################################
# Library
#########################################################################################

# create executable by adding all source and header files
ADD_LIBRARY(TASK ${EXTEND_SOURCE} ${EXTEND_HEADER})
