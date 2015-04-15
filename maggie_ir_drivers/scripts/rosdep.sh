#!/bin/bash

# get source code and install exodriver
echo "INSTALLING DRIVERS..."
echo

cd `rospack find maggie_ir_drivers`/third_parties/IRTrans_shlib
sh install.sh

echo "FINISH!"
echo
