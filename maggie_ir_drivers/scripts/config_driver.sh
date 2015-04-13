#!/bin/bash

# giving superpowers to irserver
cd `rospack find ir_drivers`/third_parties/irserver/
sudo chown root:root irserver
sudo chown root:root irserver64
sudo chmod u+s irserver
sudo chmod u+s irserver64
