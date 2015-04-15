#!/bin/bash

clear

echo "INSTALLING rfid DRIVER..."
echo
cd `rospack find maggie_rfid_drivers`/third_parties/ID_ISC.SDK.Linux_V04.06.10
sudo bash install.bash
echo "FINISH!"
echo
