#!/bin/bash

#   User's guide: http://labjack.com/support/u12/users-guide
#
#   Based on installation guide from:
#   http://labjack.com/support/linux-and-mac-os-x-drivers
#
#   U12 Driver in C language for Linux
#   http://labjack.com/support/u12/ljacklm

### get source code and install exodriver
echo "INSTALLING EXODRIVER..."
echo

cd /tmp
rm -fr exodriver
git clone git://github.com/labjack/exodriver.git
cd exodriver/

# solve bug
sed -i "193s/usermod.*/usermod -a -G \$GROUP \$USER/" install.sh
sudo bash install.sh

# config links
ldconfig

echo "FINISH!"
echo

### get source code and install labjack driver (C lang)
echo "INSTALLING LJACKLM DRIVER..."
echo

cd /tmp
rm -fr ljacklm*
wget http://labjack.com/sites/default/files/2013/10/ljacklm.zip

unzip ljacklm.zip
cd ljacklm/libljacklm/
make
sudo make install

# config links
ldconfig

echo "FINISH!"
echo

### get source code and install labjack driver (python lang)
#echo "INSTALLING LABJACKPYTHON DRIVER..."
#echo
#
#cd /tmp
#rm -fr LabJackPython*
#git clone git://github.com/labjack/LabJackPython.git
#cd LabJackPython/
#sudo python setup.py install
#
##config links
#ldconfig
#
#echo "FINISH!"
#echo
