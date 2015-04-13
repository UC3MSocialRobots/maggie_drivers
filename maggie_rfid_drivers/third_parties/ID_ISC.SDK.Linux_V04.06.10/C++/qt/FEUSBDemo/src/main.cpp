#include <QtGui>
#include "FEUSBDemoDlg.h"

#define ORG_DOMAIN "feig.de"
#define APP_NAME   "FEUSBDemo"

int main(int argc, char * argv[])
{  
    // Initialize main dialog window
    Q_INIT_RESOURCE(res);
     // Create QT application object
    QApplication app(argc, argv);
    // add app information
    app.setOrganizationDomain(ORG_DOMAIN);
    app.setApplicationName(APP_NAME);
    
    FEUSBDemoDlg win;
    win.setWindowIcon(QPixmap(":/images/obid.png"));    
    win.show();
    // Start app loop
    return app.exec();
}

