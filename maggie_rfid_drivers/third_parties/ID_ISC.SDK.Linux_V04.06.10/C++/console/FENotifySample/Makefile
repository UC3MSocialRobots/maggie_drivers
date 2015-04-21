# Makefile
# Copyright 2006	FEIG ELECTRONIC GmbH, All Rights reserved.
#					Lange Strasse 4
#					D-35781 Weilburg
#					Federal Republic of Germany
#					phone	: +49 6471 31090
#					fax		: +49 6471 310999
#					e-mail	: obid-support@feig.de
#					internet: http://www.feig.de
#

# Macros
DEBUG = #-g
INCLUDE_FECOM = ../../../fecom-lib/include/
INCLUDE_FETCP = ../../../fetcp-lib/include/
INCLUDE_FEUSB = ../../../feusb-lib/include/
INCLUDE_FEISC = ../../../feisc-lib/include/
INCLUDE_FEDM  = ../../../fedm-classlib/src/

INCLUDE_DIR     = -I"${INCLUDE_FECOM}" -I"${INCLUDE_FETCP}" -I"${INCLUDE_FEUSB}" -I"${INCLUDE_FEISC}" -I"${INCLUDE_FEDM}"
INCLUDE_FILES   = #${INCLUDE_FEISC}feisc.h

# linker call
FENotifySample : FENotifySample.o
	g++ ${DEBUG} -o FENotifySample FENotifySample.o -lFedmIscCore -lfeisc -lc

# compiler calls
FENotifySample.o : ${INCLUDE_FILES} FENotifySample.cpp
	g++ -c -Wall ${DEBUG} ${INCLUDE_DIR} FENotifySample.cpp




