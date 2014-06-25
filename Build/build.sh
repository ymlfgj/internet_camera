#! /bin/sh

if [ $1 = "1" ]; then

make clean -f Makefile-x86
make       -f Makefile-x86

echo "build for the x86 arch!"

elif [ $1 = "2" ]; then

make clean -f Makefile-arm
make       -f Makefile-arm

echo "build for the arm arch!"

fi
