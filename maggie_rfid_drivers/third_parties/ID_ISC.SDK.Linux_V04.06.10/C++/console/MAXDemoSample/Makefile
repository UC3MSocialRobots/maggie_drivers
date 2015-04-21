# Makefile
# Copyright 2009	FEIG ELECTRONIC GmbH, All Rights reserved.
#			Lange Strasse 4
#			D-35781 Weilburg
#			Federal Republic of Germany
#			phone	: +49 6471 31090
#			fax		: +49 6471 310999
#			e-mail	: obid-support@feig.de
#			internet: http://www.feig.de
#
# Version 00.01.00 / 03.09.2009

# Operation Systems	:	Linux(x86)

#DEFINES
TARGET 		= max_demo_sample
CFLAGS 		= -O0 -g -Wall
PATH_FEISC	= ../../../feisc-lib/include/
PATH_FEDM	= ../../../fedm-classlib/src/
PATH_FETCP	= ../../../fetcp-lib/include/
PATH_FECOM	= ../../../fecom-lib/include/
PATH_FEUSB	= ../../../feusb-lib/include/
PATH_FEMYAXXESS	= ../../../fedm-classlib/src/impl/myaxxess/
INCL 		= -I${PATH_FEISC} -I${PATH_FEMYAXXESS} -I${PATH_FETCP} -I${PATH_FEDM} -I${PATH_FECOM} -I${PATH_FEUSB}
LDFLAGS 	= 
OBJFILES 	= $(TARGET).o

# Define the target directories.
TARGETDIR	=
#Define the SRC directories
SRC_DIR		=
#define the LIB directory
LIB_DIR		=
#define the BIN directory
BIN_DIR		=

LDFLAGS		= -lFedmIscCore -lfeisc -lfetcp -lFedmIscMyAxxess 

# Include the Path to the OBJ Files
OBJ 		= $(OBJFILES:%.o=$(TARGETDIR)%.o)


CXX = g++


.PHONY: $(OBJ)

#### Make all: Create directory and Compile/Link ####
all: clean $(TARGETDIR) $(OBJFILES)
	$(CXX) ${CFLAGS} ${OBJ} -o $(TARGETDIR)$(TARGET) ${LDFLAGS}
	@echo "----------------------------- ${TARGET} created -----------------------------"
	@echo ""


###### Compiler - Generate Object Files ###########
#%.o: $(SRC_DIR)%.cpp
#	$(CXX) ${CFLAGS} ${INCL} -c $< -o $(TARGETDIR)$@
max_demo_sample.o: max_demo_sample.cpp
	$(CXX) ${CFLAGS} ${INCL} -c $< -o $@


#### Clean target deletes all generated files ####
clean:
	rm -f $(TARGETDIR)/${TARGET} 
	rm -f $(TARGETDIR)/*.o
	rm -f $(TARGETDIR)/*~
	rm -f *~
#	rm -f -r $(TARGETDIR)

getName:
	@echo $(TARGET)
 
