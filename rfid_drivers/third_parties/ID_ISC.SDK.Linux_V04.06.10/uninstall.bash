#!/bin/bash

LOCALINCLUDE=/usr/local/include/rfid
LOCALLIB=/usr/local/lib/

### remove udev rules
echo "Removing udev rules..."
rm -f /etc/udev/rules.d/41-feig.rules

### remove .h
echo "Removing headers..."
rm -fr $LOCALINCLUDE

### remove .so
echo "Removing libs..."
rm -f $LOCALLIB/libfecom.so*
rm -f $LOCALLIB/libFedmIscCore.so*
rm -f $LOCALLIB/libFedmIscMyAxxess.so*
rm -f $LOCALLIB/libfefu.so*
rm -f $LOCALLIB/libfeisc.so*
rm -f $LOCALLIB/libfescr.so*
rm -f $LOCALLIB/libfetcl.so*
rm -f $LOCALLIB/libfetcp.so*
rm -f $LOCALLIB/libfeusb.so*
