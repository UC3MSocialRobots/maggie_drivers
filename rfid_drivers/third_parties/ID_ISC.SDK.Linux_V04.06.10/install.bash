#!/bin/bash

LOCALINCLUDE=/usr/local/include/rfid
LOCALLIB=/usr/local/lib/

### copy udev rules
echo "Copying udev rules..."
cp feusb-lib/etc/udev/rules.d/41-feig.rules /etc/udev/rules.d/

### copy headers
echo "Copying headers..."

mkdir $LOCALINCLUDE --parents

cp fecom-lib/include/*.h $LOCALINCLUDE

cp fedm-classlib/src/*.h $LOCALINCLUDE
mkdir $LOCALINCLUDE/impl/core/ --parents && cp fedm-classlib/src/impl/core/*.h $LOCALINCLUDE/impl/core/
mkdir $LOCALINCLUDE/impl/core/i_scan/ && cp fedm-classlib/src/impl/core/i_scan/*.h $LOCALINCLUDE/impl/core/i_scan/
mkdir $LOCALINCLUDE/impl/core/i_scan/function_unit/ && cp fedm-classlib/src/impl/core/i_scan/function_unit/*.h $LOCALINCLUDE/impl/core/i_scan/function_unit/
mkdir $LOCALINCLUDE/impl/core/i_scan/peripheral_devices/ && cp fedm-classlib/src/impl/core/i_scan/peripheral_devices/*.h $LOCALINCLUDE/impl/core/i_scan/peripheral_devices/
mkdir $LOCALINCLUDE/impl/core/i_scan/tag_handler/ && cp fedm-classlib/src/impl/core/i_scan/tag_handler/*.h $LOCALINCLUDE/impl/core/i_scan/tag_handler/
mkdir $LOCALINCLUDE/impl/core/i_scan/utility/ && cp fedm-classlib/src/impl/core/i_scan/utility/*.h $LOCALINCLUDE/impl/core/i_scan/utility/
mkdir $LOCALINCLUDE/impl/core/xml_parser/ && cp fedm-classlib/src/impl/core/xml_parser/*.h $LOCALINCLUDE/impl/core/xml_parser/
mkdir $LOCALINCLUDE/impl/myaxxess/ && cp fedm-classlib/src/impl/myaxxess/*.h $LOCALINCLUDE/impl/myaxxess/

cp fefu-lib/include/*.h $LOCALINCLUDE
cp feisc-lib/include/*.h $LOCALINCLUDE
cp fescr-lib/include/*.h $LOCALINCLUDE
cp fetcl-lib/include/*.h $LOCALINCLUDE
cp fetcp-lib/include/*.h $LOCALINCLUDE
cp feusb-lib/include/*.h $LOCALINCLUDE

### copy libs
echo "Copying libs..."

# get machine architecture
MACHINE_TYPE=`uname -m`
if [ ${MACHINE_TYPE} == "x86_64" ]; then
  LIBPATH=x64
else
  LIBPATH=x86
fi

cp fecom-lib/bin/$LIBPATH/libfecom.so.3.1.0 $LOCALLIB
cp fedm-classlib/bin/core/$LIBPATH/libFedmIscCore.so.4.6.10 $LOCALLIB
# only 32b
if [ ${LIBPATH} == "x86" ]; then
    cp fedm-classlib/bin/myaxxess/$LIBPATH/libFedmIscMyAxxess.so.4.6.10 $LOCALLIB
fi
cp fefu-lib/bin/$LIBPATH/libfefu.so.2.0.0 $LOCALLIB
cp feisc-lib/bin/$LIBPATH/libfeisc.so.7.3.1 $LOCALLIB
cp fescr-lib/bin/$LIBPATH/libfescr.so.1.2.1 $LOCALLIB
cp fetcl-lib/bin/$LIBPATH/libfetcl.so.2.2.1 $LOCALLIB
cp fetcp-lib/bin/$LIBPATH/libfetcp.so.2.2.0 $LOCALLIB
cp feusb-lib/bin/$LIBPATH/libfeusb.so.4.2.1 $LOCALLIB

# create symbolic links
echo "Creating symbolic links..."
cd $LOCALLIB

ln -sf libfecom.so.3.1.0 libfecom.so.3
ln -sf libfecom.so.3 libfecom.so

ln -sf libFedmIscCore.so.4.6.10 libFedmIscCore.so.4
ln -sf libFedmIscCore.so.4 libFedmIscCore.so

# only 32b
if [ ${LIBPATH} == "x86" ]; then
    ln -sf libFedmIscMyAxxess.so.4.6.10 libFedmIscMyAxxess.so.4
    ln -sf libFedmIscMyAxxess.so.4 libFedmIscMyAxxess.so
fi

ln -sf libfefu.so.2.0.0 libfefu.so.2
ln -sf libfefu.so.2 libfefu.so

ln -sf libfeisc.so.7.3.1 libfeisc.so.7
ln -sf libfeisc.so.7 libfeisc.so

ln -sf libfescr.so.1.2.1 libfescr.so.1
ln -sf libfescr.so.1 libfescr.so

ln -sf libfetcl.so.2.2.1 libfetcl.so.2
ln -sf libfetcl.so.2 libfetcl.so

ln -sf libfetcp.so.2.2.0 libfetcp.so.2
ln -sf libfetcp.so.2 libfetcp.so

ln -sf libfeusb.so.4.2.1 libfeusb.so.4
ln -sf libfeusb.so.4 libfeusb.so

# config links
ldconfig
