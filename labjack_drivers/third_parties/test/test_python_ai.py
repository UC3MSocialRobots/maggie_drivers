#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#       test_python_ai.py
#
#       Copyright 2014 Raul Perula-Martinez <raul.perula@uc3m.es>
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#
#       Based on the example code from:
#       http://labjack.com/support/u12/examples/python
#
#       More examples:
#       http://labjack.com/support/u12/users-guide/5

import u12

d = u12.U12()

while True:
    print d.eAnalogIn(channel=0, gain=0)
