/*-------------------------------------------------------
|                                                       |
|                  main.cpp                   |
|                                                       |
---------------------------------------------------------

Copyright  2007		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : info@feig.de
						Internet : http://www.feig.de
					
Author         		:	Benjamin Stadin
Begin        		:	11.12.2006

Version       		:	01.00.00 / 01.12.2007 / Benjamin Stadin

Operation Systems	:	Linux
*/

#include <QtGui>
#include "ISOHostSampleDlg.h"

#define ORG_DOMAIN "feig.de"
#define APP_NAME   "ISOHostSample"

int main(int argc, char * argv[])
{  
    // Initialize main dialog window
    Q_INIT_RESOURCE(res);
     // Create QT application object
    QApplication app(argc, argv);
    // add app information
    app.setOrganizationDomain(ORG_DOMAIN);
    app.setApplicationName(APP_NAME);
    
    ISOHostSampleDlg win;
    win.setWindowIcon(QPixmap(":/images/obid.png"));
    //layout.addWidget(&t);
    
    win.show();
    // The resize event will set the table and hex data widget to their correct size
    // Size must be different than defined in the ui (annoying...)
    win.resize(866, 580);
    // Start app loop
    return app.exec();
}

