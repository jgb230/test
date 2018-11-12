#!/bin/bash

if [ "$1" = "" ]; then
para_make="-j4"
else
para_make=$1
fi

currDir=`pwd`

options+=" -DCMAKE_BUILD_TYPE=DEBUG" #set to DEBUG or RELEASE

mkdir -p build
cd build

cmake  $options ../

#cmake ../
make $para_make

cp *.so ../lib/
#cp mem/*.so ../lib/
#cp thread/*.so ../lib/