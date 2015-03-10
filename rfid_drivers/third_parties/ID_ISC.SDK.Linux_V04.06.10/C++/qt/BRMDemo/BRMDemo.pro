TEMPLATE = app
CONFIG += qt warn_on
CONFIG += qdbus
INCLUDEPATH += ../../../fecom-lib/include/ ../../../fetcp-lib/include/ ../../../feusb-lib/include/ ../../../fedm-classlib/src/
LIBS += -L/usr/lib -lFedmIscCore -lfeisc -lfecom -lfetcp -lfeusb

HEADERS     += src/FEUITable.h src/BRMDemoDlg.h src/FEDetectReaderDlg.h src/InterfaceConfig.h src/FEReadTagsThread.h src/FESerialPortSettingsDlg.h
RESOURCES   += res.qrc 
SOURCES     += src/main.cpp src/BRMDemoDlg.cpp src/FEUITable.cpp src/FEDetectReaderDlg.cpp src/InterfaceConfig.cpp src/FEReadTagsThread.cpp src/FESerialPortSettingsDlg.cpp
FORMS       += ui/FEDetectReaderDlg.ui ui/BRMDemoDlg.ui ui/FESerialPortSettingsDlg.ui

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro images
sources.path = ISOHostSample
