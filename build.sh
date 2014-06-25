#! /bin/sh

if [ $1 ='1' ] then

ehco "Make for the X86"

make clean -f Makefile-x86
make all -f Makefiel-x86

else 

echo "Make for the arm"

make clean -f Makefile-arm
make all -f Makefile-arm


fi
