#!/bin/bash

# get machine architecture
MACHINE_TYPE=`uname -m`
if [ ${MACHINE_TYPE} == "x86_64" ]; then
  LIBPATH=./x64
else
  LIBPATH=./x86
fi

LOCALINCLUDE=/usr/local/include/irtrans
LOCALLIB=/usr/local/lib/

### copy headers
echo "Copying headers..."
mkdir $LOCALINCLUDE --parents
cp $LIBPATH/*.h $LOCALINCLUDE

### copy libs
echo "Copying libs..."
cp $LIBPATH/*.so $LOCALLIB

# config links
ldconfig
