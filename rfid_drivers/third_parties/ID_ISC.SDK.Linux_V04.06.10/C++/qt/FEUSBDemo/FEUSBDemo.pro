TEMPLATE = app
CONFIG += qt warn_on
unix:contains(QT_CONFIG, qdbus):CONFIG += qdbus
INCLUDEPATH += ../../../fecom-lib/include/ ../../../fetcp-lib/include/ ../../../feusb-lib/include/ ../../../fedm-classlib/src/
#CONFIG += debug
LIBS += -L/usr/lib -lFedmIscCore -lfeusb

HEADERS     += src/FEUSBDemoDlg.h src/ScanAndOpenDlg.h src/GetScanListParaDlg.h src/DeviceIDDlg.h src/DeviceHandleDlg.h src/GetDeviceParaDlg.h src/SetDeviceParaDlg.h src/TransceiveDlg.h
RESOURCES   += res.qrc
SOURCES     += src/main.cpp src/FEUSBDemoDlg.cpp src/ScanAndOpenDlg.cpp src/GetScanListParaDlg.cpp src/DeviceIDDlg.cpp src/DeviceHandleDlg.cpp src/GetDeviceParaDlg.cpp src/SetDeviceParaDlg.cpp src/TransceiveDlg.cpp
FORMS       += ui/FEUSBDemoDlg.ui ui/ScanAndOpenDlg.ui ui/GetScanListParaDlg.ui ui/DeviceIDDlg.ui ui/DeviceHandleDlg.ui ui/GetDeviceParaDlg.ui ui/SetDeviceParaDlg.ui ui/TransceiveDlg.ui

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

target.path = build/FEUSBDemoDlg
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro images
sources.path = FEUSBDemoDlg
INSTALLS += target sources
