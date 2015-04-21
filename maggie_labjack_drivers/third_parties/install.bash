#!/bin/bash

#   User's guide: http://labjack.com/support/u12/users-guide
#
#   Based on installation guide from:
#   http://labjack.com/support/linux-and-mac-os-x-drivers
#
#   U12 Driver in C language for Linux
#   http://labjack.com/support/u12/ljacklm

# install dependencies
echo "INSTALLING DEPENDENCIES..."
echo

PACKAGES="build-essential
          libusb-1.0-0-dev
          git-core"

sudo apt-get install $PACKAGES

echo "FINISH!"
echo

# get source code and install exodriver
echo "INSTALLING EXODRIVER..."
echo

cd /tmp
git clone git://github.com/labjack/exodriver.git
cd exodriver/

# solve bug
sed -i "193s/usermod.*/usermod -a -G \$GROUP \$USER/" install.sh
sudo bash install.sh

echo "FINISH!"
echo

# get source code and install labjack driver (C lang)
echo "INSTALLING LJACKLM DRIVER..."
echo

cd /tmp
wget http://labjack.com/sites/default/files/2013/10/ljacklm.zip

unzip ljacklm.zip
cd ljacklm/libljacklm/
make
sudo make install

echo "FINISH!"
echo

# get source code and install labjack driver (python lang)
echo "INSTALLING LABJACKPYTHON DRIVER..."
echo

cd /tmp
git clone git://github.com/labjack/LabJackPython.git
cd LabJackPython/
sudo python setup.py install

echo "FINISH!"
echo
