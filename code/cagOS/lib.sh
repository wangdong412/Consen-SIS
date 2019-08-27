
rm ../*.a

make clean
make BUILDCORE=CORE0 VB=0 NUM=0
find . -name "*.o" | xargs -i cp {} os_lib
cd os_lib
ar rcs cagOsCORE0-0.a *.o
cp cagOsCORE0-0.a ../../
cd ..


make clean
make BUILDCORE=CORE1 VB=0 NUM=0
find . -name "*.o" | xargs -i cp {} os_lib
cd os_lib
ar rcs cagOsCORE1-0.a *.o
cp cagOsCORE1-0.a ../../
cd ..

make clean
make BUILDCORE=CORE0 VB=0 NUM=1
find . -name "*.o" | xargs -i cp {} os_lib
cd os_lib
ar rcs cagOsCORE0-1.a *.o
cp cagOsCORE0-1.a ../../
cd ..


make clean
make BUILDCORE=CORE1 VB=0 NUM=1
find . -name "*.o" | xargs -i cp {} os_lib
cd os_lib
ar rcs cagOsCORE1-1.a *.o
cp cagOsCORE1-1.a ../../
cd ..

make clean
make BUILDCORE=CORE0 VB=0 NUM=2
find . -name "*.o" | xargs -i cp {} os_lib
cd os_lib
ar rcs cagOsCORE0-2.a *.o
cp cagOsCORE0-2.a ../../
cd ..


make clean
make BUILDCORE=CORE1 VB=0 NUM=2
find . -name "*.o" | xargs -i cp {} os_lib
cd os_lib
ar rcs cagOsCORE1-2.a *.o
cp cagOsCORE1-2.a ../../
cd ..
