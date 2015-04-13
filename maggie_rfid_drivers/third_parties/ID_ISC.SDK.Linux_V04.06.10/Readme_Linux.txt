Copyright © 2007-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
			Lange Strasse 4
			D-35781 Weilburg
			Federal Republic of Germany
			phone    : +49 6471 31090
			fax      : +49 6471 310999
			e-mail   : obid-support@feig.de
			Internet : http://www.feig.de

OBID and OBID i-scan are registereds trademark of FEIG ELECTRONIC GmbH


===============================
	ID ISC.SDK.Linux

	    V4.06.10

	   2014-12-11
===============================


1. What is new (main changes)
- Support for new Reader type ID ISC.CPR60
- Linux: Bugfix for Notification-Mode
- Updated Reader Configuration namespaces in ReaderConfig


NOTE: To learn more about OBID i-scan Readers use ISOStart V9.8.2 (or higher) and for OBID classic-pro Readers use CPRStart V9.7.1 (or higher)


2. Installation
Copy all directories into your local path. Please hold the dirctory structure as the makefiles are matched to this.
To install the library files, follow the installation instructions of the manuals. Use the batch file install_x86.sh or install_x64.sh to copy all shared libraries to /usr/lib resp. usr/lib64 to create symbolic links.
Cyphered protocol transmission depends on the installation of openSSL V0.9.8
When communication with an USB Reader is recommended, please have a look into the manual of FEUSB for proper installation.


3. C++ Sample projects for console
- FENotifySample demonstrates the Notification-Mode for Readers supporting the Notification-Mode
- MAXDemoSample demonstrates elementary operations with the MAX-Reader MAX50
- myAxxess_NotifySample demonstrates elementary callback operations with the MAX-Reader MAX50


4. C++ Sample projects with QT4 (run qmake to create a makefile)
- FEUSBDemo demonstrates the use of libfeusb.so
- ISOHostSample demonstrates the ISO-Host-Commands with all readers
- BRMDemo demonstrates the Buffered Read Mode for Readers supporting the Buffered-Read-Mode
- NotifySample demonstrates the notification mode for Readers supporting the Notification-Mode


5. More samples can be found in the free available ISC.SDK.Win
