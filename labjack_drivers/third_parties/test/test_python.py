#!/usr/bin/env python
# -*- coding: utf-8 -*-

#   test_labjack.py
#
#   Copyright 2014 Raul Perula-Martinez <raul.perula@uc3m.es>
#
#   Based on the example code from:
#       http://labjack.com/support/u12/examples/python
#
#   More examples:
#       http://labjack.com/support/u12/users-guide/5

import u12

d = u12.U12()
print d.eAnalogIn(0)

print "\nsimilar output: {'overVoltage': 0, 'idnum': 0, 'voltage': 1.4599609375}"
