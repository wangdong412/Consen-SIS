#compile cfgs
SET(CMAKE_SYSTEM_NAME "Linux")

#SET(CMAKE_C_COMPILER /opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/x86_64-fslsdk-linux/usr/bin/powerpc-fsl-linux-gnuspe/powerpc-fsl-linux-gnuspe-gcc)
SET(CMAKE_C_COMPILER /opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/i686-fslsdk-linux/usr/bin/powerpc-fsl-linux-gnuspe/powerpc-fsl-linux-gnuspe-gcc)

#SET(CMAKE_SYSROOT "/opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/ppce500v2-fsl-linux-gnuspe")

SET(CMAKE_FIND_ROOT_PATH /opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/ppce500v2-fsl-linux-gnuspe/)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
