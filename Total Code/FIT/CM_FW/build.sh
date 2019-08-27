#!/bin/sh

if [ -d build ]
then
    echo "start building"
else
    echo "mkdir output dir"
    mkdir build
    echo "start building"
fi

rm -rf ./build/*
cd build
export CFLAGS=" -m32 -mcpu=8548 -mabi=spe -mspe -mfloat-gprs=double --sysroot=/opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/ppce500v2-fsl-linux-gnuspe"
export CXXFLAGS=" -m32 -mcpu=8548 -mabi=spe -mspe -mfloat-gprs=double --sysroot=/opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/ppce500v2-fsl-linux-gnuspe"

cmake -DCMAKE_TOOLCHAIN_FILE=../p1020_toolchain.cmake ..
make VERBOSE=0 
/opt/fsl-networking/QorIQ-SDK-V1.7/sysroots/i686-fslsdk-linux/usr/bin/powerpc-fsl-linux-gnuspe/powerpc-fsl-linux-gnuspe-objdump -D RTS > RTSdump
cksum RTS | awk '{print $1}' >RTSCRC

if [ $# -eq 1 ]
then
    if [ $1 = "qemu" ]
    then
        qemu-ppc -cpu e500v2_v22 RTS
    fi
fi
