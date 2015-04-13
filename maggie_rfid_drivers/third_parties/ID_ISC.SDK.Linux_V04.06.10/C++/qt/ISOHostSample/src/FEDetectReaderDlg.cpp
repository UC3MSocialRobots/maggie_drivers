/*-------------------------------------------------------
|                                                       |
|                  FEDetectReaderDlg.cpp                |
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

Version       		:	01.00.00 / 11.01.2007 / Benjamin Stadin

Operation Systems	:	Linux
*/

#include "FEDetectReaderDlg.h"
#include "InterfaceConfig.h"

#include "../../../../feusb-lib/include/feusb.h"

#include <stdio.h>


//------------------------------------------------------------------------------
// Name: FEDetectReaderDlg(QWidget *parent)
// Desc: constructor
//------------------------------------------------------------------------------
FEDetectReaderDlg::FEDetectReaderDlg(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	
	// read configuration. The configuration is stored in file feconfig.dat
	config.readConfigFromFile();
      
	connect(ui.detectPushButton, SIGNAL(clicked()), this, SLOT(onDetectButton()));
	connect(ui.morePushButton, SIGNAL(clicked()), this, SLOT(onMoreButton()));
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onOkButton()));

	// list available com ports
      	QStringList comPorts;
      	int count = 0;
	int iPorts[256];
	count = config.getComPorts(iPorts);	
	comPorts += "-";
	for (int i = 0; i < count; i++) {
		char cPortNr[30] = {0};
		sprintf(cPortNr, "%i", iPorts[i]);
		comPorts += cPortNr;
	}  
	
	m_bReaderDetected = false;
	m_reader = NULL;
    
	ui.comPortComboBox->addItems(comPorts);
	
	// set values from config to ui
	if (config.getCommMode() == COM_DEVICE)
		ui.comPortRadio->setChecked(true);
	else if (config.getCommMode() == TCP_DEVICE)
		ui.tcpRadio->setChecked(true);
	else if (config.getCommMode() == USB_DEVICE)
		ui.usbRadio->setChecked(true);
		
	char sHost[32] = {0};
	strcpy(sHost, config.getHost());
	ui.ipAddressLineEdit->setText(sHost);
	
	int iIpPort = config.getIPPort();
	char sIpPort[16] = {0};
	sprintf(sIpPort, "%d", iIpPort);
	ui.ipPortLineEdit->setText(sIpPort);
	
	char sComPort[8] = {0};
	sprintf(sComPort, "%d", config.getComPortNumber());
	for (int i=0; i<ui.comPortComboBox->count(); i++)
	{
		if (ui.comPortComboBox->itemText(i) == sComPort)
		{
			ui.comPortComboBox->setCurrentIndex(i);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: ~FEDetectReaderDlg()
// Desc: destructor
//------------------------------------------------------------------------------
FEDetectReaderDlg::~FEDetectReaderDlg()
{
	//
}

//------------------------------------------------------------------------------
// Name: setReader(FEDM_ISCReaderModule *reader) 
// Desc: sets a pointer to a reader (set by main application to keep the detect class independent
// from the main application)
//------------------------------------------------------------------------------
void FEDetectReaderDlg::setReader(FEDM_ISCReaderModule *reader) 
{
	this->m_reader = reader;
}

//------------------------------------------------------------------------------
// Name: getReader() 
// Desc: convenience function to check if a reader has already been set
//------------------------------------------------------------------------------
FEDM_ISCReader *FEDetectReaderDlg::getReader() 
{
	return m_reader;
}

//------------------------------------------------------------------------------
// Name: onDetectButton() 
// Desc: detect a reader when detect button is clicked
//------------------------------------------------------------------------------
void FEDetectReaderDlg::onDetectButton() 
{
	int tmpCharTimeoutMpy = 8;
	int status;
	int baud;
	int error = 0;
	int iBack;
	char sErrorText[256];
	QString sMsg;
	char* sPort;

	m_bReaderDetected = false;

	ui.readerListWidget->clear();
	
	if (m_reader == NULL)
	{
		QMessageBox::critical(this, tr("Error"), "No reader set", QMessageBox::Abort);	
		return;
	}

	// close the previously used communication port
	if(m_reader->GetPortHnd() > 0) 
	{
		m_reader->DisConnect();
	}
	
	// When switching between readers the current protocol might be wrong, so set standard protocol
	// and switch to advanced later if supported
	m_reader->SetProtocolFrameSupport(FEDM_PRT_FRAME_STANDARD);

	// now open the new selected port and try to detect the reader
	if (ui.comPortRadio->isChecked())
	{
		config.setCommMode(COM_DEVICE);
		// Set CharTimeoutMultiplier temporary to 8 to be sure to detect slow usb-serial devices
		tmpCharTimeoutMpy = config.getCharTimeoutMpy();
		config.setCharTimeoutMpy(8);

		if (ui.comPortComboBox->currentText() == "-")
			return;
		
		sPort = qstrdup(ui.comPortComboBox->currentText().toLatin1());
		int iComPort;
		sscanf(sPort, "%d", &iComPort); 

		iBack = m_reader->ConnectCOMM(iComPort);
		
		delete [] sPort;
		
		if (iBack == FEDM_OK) 
		{
			char val[256] = {0};

			config.setComPortConfiguration(m_reader->GetPortHnd());
			
			/* 
				NOTE: The Baud rate setting on the serial port settings form is overwritten 
				by FindBaudRate(). 
				On some Linux systems the baud rate detection doesn't work properly. 
				If FindBaudRate() doesn't work for you, comment out the block below. The 
				effective baud rate will then be the value you set on the serial port settings form.
			*/
			
			// BLOCK BEGIN
			m_reader->FindBaudRate(); 
			status = m_reader->GetPortPara("Baud", val);
			if (status == 0) 
			{
				baud = atoi(val);
				config.setBaud(baud);
			}

			status = m_reader->GetPortPara("Frame", val);
			if (status == 0) 
			{
				char frm[8] = {0};
				strcpy(frm, val);
				config.setFrame(frm);
			}
			// BLOCK END
			
		}
		else 
		{
			return;
		}

	}
	else if (ui.usbRadio->isChecked())
	{	
		config.setCommMode(USB_DEVICE);
		FEUSB_Scan(FEUSB_SCAN_ALL, NULL);
		if (FEUSB_GetScanListSize() > 1) 
		{
			QMessageBox::critical(this, tr("Error"),
					"More than one USB-Reader detected.\nPlease connect only 1 USB-Reader!",
					QMessageBox::Abort);
			return;
		}

		if (FEUSB_GetScanListSize() == 1) 
		{
			iBack = m_reader->ConnectUSB(0);
			if (iBack != FEDM_OK) 
			{
				m_reader->GetErrorText(sErrorText, iBack);
				QMessageBox::critical(this, tr("Error"), sErrorText,
								QMessageBox::Abort);
				return;
			}
		}
		else
		{
			return;
		}

	}
	else if (ui.tcpRadio->isChecked())
	{	
		char* sHost;
		char* sIpPort;
		
		config.setCommMode(TCP_DEVICE);
		
		sHost = qstrdup(ui.ipAddressLineEdit->text().toLatin1());
		sIpPort = qstrdup(ui.ipPortLineEdit->text().toLatin1());
		int iIpPort = atoi(sIpPort);
		
		int iBack = m_reader->ConnectTCP(sHost, iIpPort);
		
		delete [] sHost;
		delete [] sIpPort;
		
		if(iBack != FEDM_OK)
		{
			m_reader->GetErrorText(sErrorText, iBack);
			//m_reader->SetPortHnd(0);
			sMsg.sprintf("%d: %s", iBack, sErrorText);
			QMessageBox::critical(this, tr("QMessageBox::critical()"), sMsg, QMessageBox::Abort);
		}
	}
		
	// try to get the readers type info
	if (m_reader->GetPortHnd() > 0)
	{
		error = m_reader->SendProtocol(0x65);
		if (error == FEDM_OK)
		{			
			QString sReaderName = m_reader->GetReaderName();
			ui.readerListWidget->addItem(sReaderName);
			switch(m_reader->GetReaderType())
			{
				case FEDM_ISC_TYPE_ISCLRU1000:
				case FEDM_ISC_TYPE_ISCLRU2000:
					m_reader->SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
					break;
				default:
					m_reader->SetProtocolFrameSupport(FEDM_PRT_FRAME_STANDARD);
					break;
			}
			m_bReaderDetected = true;
		}
		else
		{
			m_reader->GetErrorText(sErrorText, error);
			QMessageBox::critical(this, tr("Error"), sErrorText, QMessageBox::Abort);
		}
	}

	// A char timeout multiplier of 8 is used to detect all devices. 
	// Set it back to config value again
	if (config.getCommMode() == COM_DEVICE && m_reader->GetPortHnd() > 0)
	{
		config.setCharTimeoutMpy(tmpCharTimeoutMpy);
		config.setComPortConfiguration(m_reader->GetPortHnd());
	}
}

//------------------------------------------------------------------------------
// Name: isReaderDetected() 
// Desc: returns true if a reader was detected by the detect function
//------------------------------------------------------------------------------
bool FEDetectReaderDlg::isReaderDetected()
{
	return m_bReaderDetected;
}

//------------------------------------------------------------------------------
// Name: FEDetectReaderDlg() 
// Desc: further configuration parameters for serial port
//------------------------------------------------------------------------------
void FEDetectReaderDlg::onMoreButton() 
{
	// set com port settings in extended settings dialog (more button)
	char cTimeout[16] = {0};
	sprintf(cTimeout, "%d", config.getTimeout());
	char cDelayTime[16] = {0};
	sprintf(cDelayTime, "%d", config.getTxDelayTime());
	char cCharTimeout[16] = {0};
	sprintf(cCharTimeout, "%d", config.getCharTimeoutMpy());
	serialPortSettingsDlg.ui.timeoutLineEdit->setText(cTimeout);
	serialPortSettingsDlg.ui.delayTimeLineEdit->setText(cDelayTime);
	serialPortSettingsDlg.ui.charTimeoutLineEdit->setText(cCharTimeout);
	serialPortSettingsDlg.ui.txTimeCheckBox->setChecked(config.getTxTimeControl());
	serialPortSettingsDlg.ui.rtsCheckBox->setChecked(config.getRTS());
	serialPortSettingsDlg.ui.dtrCheckBox->setChecked(config.getDTR());
	// set baud rate combo bx to selected 
	char cBaud[16] = {0};
	bool found = false;
	sprintf(cBaud, "%d", config.getBaud());
	for (int i=0; i<serialPortSettingsDlg.ui.baudRateComboBox->count(); i++)
	{
		if (ui.comPortComboBox->itemText(i) == cBaud)
		{
			serialPortSettingsDlg.ui.baudRateComboBox->setCurrentIndex(i);
			found = true;
			break;
		}
	}
	if (!found)
	{
		serialPortSettingsDlg.ui.baudRateComboBox->setCurrentIndex(0);
		serialPortSettingsDlg.ui.baudRateComboBox->setItemText(0, cBaud);
	}

	// show serial port settings window
	if (serialPortSettingsDlg.exec() != QDialog::Accepted)
		return;
		
	char* sTimeout = qstrdup(serialPortSettingsDlg.ui.timeoutLineEdit->text().toLatin1());
	char* sTxDelayTime = qstrdup(serialPortSettingsDlg.ui.delayTimeLineEdit->text().toLatin1());
	char* sCharTimeout = qstrdup(serialPortSettingsDlg.ui.charTimeoutLineEdit->text().toLatin1());
	int iIndex = serialPortSettingsDlg.ui.baudRateComboBox->currentIndex();
	char* sBaud = qstrdup(serialPortSettingsDlg.ui.baudRateComboBox->itemText(iIndex).toLatin1());
	bool bTxTimeCtl = serialPortSettingsDlg.ui.txTimeCheckBox->isChecked();
	bool bRts = serialPortSettingsDlg.ui.rtsCheckBox->isChecked();
	bool bDtr = serialPortSettingsDlg.ui.dtrCheckBox->isChecked();
	
	config.setTimeout(atoi(sTimeout));
	config.setTxDelayTime(atoi(sTxDelayTime));
	config.setCharTimeoutMpy(atoi(sCharTimeout));
	config.setBaud(atoi(sBaud));
	config.setTxTimeControl(bTxTimeCtl);
	config.setRTS(bRts);
	config.setDTR(bDtr);
	
	if (m_reader->GetPortHnd() > 0)
		config.setComPortConfiguration(m_reader->GetPortHnd());
	
	delete [] sTimeout;
	delete [] sTxDelayTime;
	delete [] sCharTimeout;
	delete [] sBaud;
}

//------------------------------------------------------------------------------
// Name: onOkButton() 
// Desc: saves the configuration when ok button is clicked
//------------------------------------------------------------------------------
void FEDetectReaderDlg::onOkButton() 
{
	if (ui.comPortRadio->isChecked())
		config.setCommMode(COM_DEVICE);
	else if (ui.usbRadio->isChecked())
		config.setCommMode(USB_DEVICE);
	else if (ui.tcpRadio->isChecked())
		config.setCommMode(TCP_DEVICE);
		
	char* tmp = qstrdup(ui.ipAddressLineEdit->text().toLatin1());
	config.setHost(tmp);
	delete [] tmp;
	
	tmp = qstrdup(ui.ipPortLineEdit->text().toLatin1());
	int ipPort = atoi(tmp);
	config.setIPPort(ipPort);
	delete [] tmp;
	
	QString qsComPort = ui.comPortComboBox->currentText();
	if (qsComPort != "-")
	{
		tmp = qstrdup(qsComPort.toLatin1());
		int iComPort = atoi(tmp);
		config.setComPortNumber(iComPort);
		delete [] tmp;
	}
	
	config.writeConfigToFile();
}
