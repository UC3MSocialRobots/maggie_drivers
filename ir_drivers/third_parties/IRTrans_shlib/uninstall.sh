#!/bin/bash

LOCALINCLUDE=/usr/local/include/irtrans
LOCALLIB=/usr/local/lib/

###
echo "Removing headers..."
rm -fr $LOCALINCLUDE

###
echo "Removing libs..."
rm -f $LOCALLIB/libIR*
