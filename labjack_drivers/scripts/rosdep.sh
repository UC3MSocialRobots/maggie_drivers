#!/bin/bash

#   User's guide: http://labjack.com/support/u12/users-guide
#
#   Based on installation guide from:
#   http://labjack.com/support/linux-and-mac-os-x-drivers
#
#   U12 Driver in C language for Linux
#   http://labjack.com/support/u12/ljacklm

# External, ROS and system package dependencies

# With this snippet the user now can use sh/bash or any sh like interpreter to run a bash script
if [ -z "$BASH_VERSION" ]
then
    exec bash "$0" "$@"
fi

clear

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

cd `rospack find labjack_drivers`/third_parties/
git clone git://github.com/labjack/exodriver.git
cd exodriver/

echo "WARNING! MODIFY FIRST THE install.sh FILE (LINE 193)"
echo "'usermod -a -G \$GROUP \$USER'"
echo "PRESS ENTER TO CONTINUE"
echo
read

sudo sh install.sh

echo "FINISH!"
echo

# get source code and install labjack driver (C)
echo "INSTALLING LJACKLM DRIVER..."
echo

cd `rospack find labjack_drivers`/third_parties/
wget http://labjack.com/sites/default/files/2013/10/ljacklm.zip

unzip ljacklm.zip
cd ljacklm/libljacklm/
make
sudo make install

echo "FINISH!"
echo

# get source code and install labjack driver (python)
echo "INSTALLING LABJACKPYTHON DRIVER..."
echo

cd `rospack find labjack_drivers`/third_parties/
git clone git://github.com/labjack/LabJackPython.git
cd LabJackPython/
sudo python setup.py install

echo "FINISH!"
echo
