/*-------------------------------------------------------
|                                                       |
|                  InterfaceConfig.cpp                  |
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
Begin        		:	15.12.2006

Version       		:	01.00.00 / 11.01.2007 / Benjamin Stadin

Operation Systems	:	Linux
*/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../../../../fecom-lib/include/fecom.h"

#include "InterfaceConfig.h"

using namespace std; 

//------------------------------------------------------------------------------
// Name: InterfaceConfig()
// Desc: constructor
//------------------------------------------------------------------------------
InterfaceConfig::InterfaceConfig()
{
	m_iError = 0;
	m_ifSettings.init();
}

//------------------------------------------------------------------------------
// Name: ~InterfaceConfig()
// Desc: destructor
//------------------------------------------------------------------------------
InterfaceConfig::~InterfaceConfig()
{
}

//------------------------------------------------------------------------------
// Name: init()
// Desc: sets default values to the config settings
//------------------------------------------------------------------------------
void InterfaceConfig::init()
{
	m_ifSettings.init();
}

//------------------------------------------------------------------------------
// Name: checkIfIPAddress(char *ip, int len)
// Desc: returns 0 if a given IP address is a valid ipv4 address, otherwise -1
//------------------------------------------------------------------------------
int InterfaceConfig::checkIfIPAddress(char *ip, int len)
{
	// Check if only digits
	for (int i=0; i<len; i++)
	{
		if (ip[i] == '.')
			continue;
		if (!isdigit(ip[i]))
			return -1;
	}

	in_addr a;	
	int t = inet_aton(ip, &a);
	if (t == 0)
		return -1;
	
	return 0;
}

//------------------------------------------------------------------------------
// Name: writeConfigToFile(char* fileName)
// Desc: writes the configuration to a file (default is feconfig.dat)
//------------------------------------------------------------------------------
int InterfaceConfig::writeConfigToFile(char* fileName)
{
	map<string, string> values;
	string var;
	char value[32] = {0};
	int ret = 0;
	
	sprintf(value, "%d", m_ifSettings.iCommMode);
	values["CommMode"] = value;
	
	values["Host"] = m_ifSettings.cHost;
	
	sprintf(value, "%d", m_ifSettings.iIpPort);
	values["IpPort"] = value; 
	
	sprintf(value, "%d", m_ifSettings.iComPortNumber);
	values["ComPortNumber"] = value;
	
	sprintf(value, "%d", m_ifSettings.iBaud);
	values["Baud"] = value;
	
	sprintf(value, "%d", m_ifSettings.iBusAddress);
	values["BusAddress"] = value;
	
	values["Frame"] =  m_ifSettings.cFrame;
	
	sprintf(value, "%d", m_ifSettings.iTimeout);
	values["Timeout"] = value;

	values["TxTimeControl"] = m_ifSettings.bTxTimeControl == true ? "1" : "0";
	
	sprintf(value, "%d", m_ifSettings.iTxDelayTime);
	values["TxDelayTime"] = value;
	
	sprintf(value, "%d", m_ifSettings.iCharTimeoutMpy);
	values["CharTimeoutMpy"] = value;
	
	values["Rts"] = m_ifSettings.bRts == true ? "1" : "0";
	values["Dtr"] = m_ifSettings.bDtr == true ? "1" : "0";
	
	ofstream cfgFile;
	cfgFile.open (fileName);
	if (cfgFile.is_open())
	{
		map<string, string>::const_iterator iter;
		for (iter=values.begin(); iter != values.end(); ++iter)
		{
			char configVar[32];
			char configValue[32];
			
			strcpy(configVar, iter->first.data());
			strcpy(configValue, iter->second.data());
			
			cfgFile << configVar << " " << configValue << endl;
		}	
		cfgFile.close();
	}
	else
		ret = -1;
	
	return ret;
}

//------------------------------------------------------------------------------
// Name: readConfigFromFile(char* fileName)
// Desc: reads a configuration from file
//------------------------------------------------------------------------------
int InterfaceConfig::readConfigFromFile(char* fileName)
{
	char configVar[64] = {0};
	char configValue[64] = {0};
	int ret = 0;
	
	m_ifSettings.init();
	
	ifstream cfgFile;
	cfgFile.open (fileName);
	if (cfgFile.is_open())
	{
		int wordCount = 0;
		while (cfgFile.good())
		{			
			// read word / value pair
			if (wordCount % 2 == 0)
			{
				wordCount++;
				cfgFile >> configVar;
				continue;
			}
			else
				cfgFile >> configValue;
				
			wordCount++;
		
			if (strcmp(configVar, "CommMode") == 0)
			{
				int mode;
				mode = atoi(configValue);
				if (mode == 0 || mode == 1 || mode == 2)
					m_ifSettings.iCommMode = mode;
			}
			else if (strcmp(configVar, "Host") == 0)
			{
				if (checkIfIPAddress(configValue, strlen(configValue)) == 0)
					strcpy(m_ifSettings.cHost, configValue);
			}
			else if (strcmp(configVar, "IpPort") == 0)
			{
				int port;
				port = atoi(configValue);
				if (port >= 0)
					m_ifSettings.iIpPort = port;
			}
			else if (strcmp(configVar, "ComPortNumber") == 0)
			{
				int comPort;
				comPort = atoi(configValue);
				if (comPort >= 1 && comPort <= 256)
					m_ifSettings.iComPortNumber = comPort;
			}
			else if (strcmp(configVar, "Baud") == 0)
			{
				int baud;
				baud = atoi(configValue);
				if (baud > 0)
					m_ifSettings.iBaud = baud;
			}
			else if (strcmp(configVar, "BusAddress") == 0)
			{
				int busAddress;
				busAddress = atoi(configValue);
				if (busAddress > 0 && busAddress < 256)
					m_ifSettings.iBusAddress = busAddress;
			}			
			else if (strcmp(configVar, "Frame") == 0)
			{
				strcpy(m_ifSettings.cFrame, configValue);
			}
			else if (strcmp(configVar, "Timeout") == 0)
			{
				int timeout;
				timeout = atoi(configValue);
				if (timeout >= 0)
					m_ifSettings.iTimeout = timeout;
			}
			else if (strcmp(configVar, "TxTimeControl") == 0)
			{
				int timeCtrl;
				timeCtrl = atoi(configValue);
				if (timeCtrl == 1)
					m_ifSettings.bTxTimeControl = true;
				else
					m_ifSettings.bTxTimeControl = false;
			}
			else if (strcmp(configVar, "CharTimeoutMpy") == 0)
			{
				int timeoutMpy;
				timeoutMpy = atoi(configValue);
				if (timeoutMpy >= 0)
					m_ifSettings.iCharTimeoutMpy = timeoutMpy;
			}
			else if (strcmp(configVar, "Rts") == 0)
			{
				int rts;
				rts = atoi(configValue);
				if (rts == 1)
					m_ifSettings.bRts = true;
				else
					m_ifSettings.bRts = false;
			}
			else if (strcmp(configVar, "Dtr") == 0)
			{
				int dtr;
				dtr = atoi(configValue);
				if (dtr == 1)
					m_ifSettings.bDtr = true;
				else
					m_ifSettings.bDtr = false;
			}
		}
		cfgFile.close();
	}
	else
		ret = -1; // Not fatal, file doesn't exist yet
	
	return ret;
}

//------------------------------------------------------------------------------
// Name: getComPorts()
// Desc: writes all available com ports to a given array of int pointers
//------------------------------------------------------------------------------
int InterfaceConfig::getComPorts(int* comPorts) 
{
	int count = 0;
	
	for (int port = 1; port <= 256; port++)
	{
		if (0 == FECOM_DetectPort(port))
		{
			comPorts[count] = port;
			count++;
		}
	}

	return count;
}

//------------------------------------------------------------------------------
// Name: setComPortConfiguration(int portHandle) 
// Desc: sets the com port configuration from the config settings
//------------------------------------------------------------------------------
int InterfaceConfig::setComPortConfiguration(int portHandle) 
{
	char str[256];

	sprintf(str, "%d", m_ifSettings.iBaud);
	m_iError = FECOM_SetPortPara(portHandle, "Baud", str);
	if (m_iError < 0) 
		return m_iError;
	
	m_iError = FECOM_SetPortPara(portHandle, "Frame", m_ifSettings.cFrame);
	if (m_iError < 0) 
		return m_iError;

	sprintf(str, "%d", m_ifSettings.iTimeout);
	m_iError = FECOM_SetPortPara(portHandle, "Timeout", str);
	if (m_iError < 0) 
		return m_iError;
	
	if (m_ifSettings.bTxTimeControl) {
		m_iError = FECOM_SetPortPara(portHandle, "TxTimeControl", "1");
	}
	else {
		m_iError = FECOM_SetPortPara(portHandle, "TxTimeControl", "0");
	}
	if (m_iError < 0) 
		return m_iError;

	if (m_ifSettings.bDtr) {
		m_iError = FECOM_DoPortCmd(portHandle, "SetDTR", "ON");
	}
	else {
		m_iError = FECOM_DoPortCmd(portHandle, "SetDTR", "OFF");
	}
	if (m_iError < 0) 
		return m_iError;

	if (m_ifSettings.bRts) {
		m_iError = FECOM_DoPortCmd(portHandle, "SetRTS", "ON");
	}
	else {
		m_iError = FECOM_DoPortCmd(portHandle, "SetRTS", "OFF");
	}
	if (m_iError < 0) 
		return m_iError;

	sprintf(str, "%d", m_ifSettings.iTxDelayTime);
	m_iError = FECOM_SetPortPara(portHandle, "TxDelayTime", str);
	if (m_iError < 0) 
		return m_iError;

	sprintf(str, "%d", m_ifSettings.iCharTimeoutMpy);
	m_iError = FECOM_SetPortPara(portHandle, "CharTimeoutMpy", str);
	if (m_iError < 0) 
		return m_iError;

	return 0;
}

//------------------------------------------------------------------------------
// Name: getComPortConfiguration(int portHandle) 
// Desc: gets the port configuration and writes it to the config settings
//------------------------------------------------------------------------------
int InterfaceConfig::getComPortConfiguration(int portHandle) 
{
	char str[256];

	m_iError = FECOM_GetPortPara(portHandle, "PortNr", str);
	if (m_iError < 0) 
		return m_iError;
	m_ifSettings.iComPortNumber = atoi(str);

	m_iError = FECOM_GetPortPara(portHandle, "Baud", str);
	if (m_iError < 0) 
		return m_iError;
	m_ifSettings.iBaud = atoi(str);

	m_iError = FECOM_GetPortPara(portHandle, "Frame", str);
	if (m_iError < 0) 
		return m_iError;
	strcpy(m_ifSettings.cFrame, str);
	
	m_iError = FECOM_GetPortPara(portHandle, "Timeout", str);
	if (m_iError < 0) 
		return m_iError;
	m_ifSettings.iTimeout = atoi(str);

	m_iError = FECOM_GetPortPara(portHandle, "TxTimeControl", str);
	if (m_iError < 0) 
		return m_iError;
	if (strcmp(str, "1") == 0) {
		m_ifSettings.bTxTimeControl = true;
	}
	else {
		m_ifSettings.bTxTimeControl = false;
	}

	m_iError = FECOM_GetPortPara(portHandle, "GetDTR", str);
	if (m_iError < 0) 
		return m_iError;
	if (strcmp(str,"ON")==0) {
		m_ifSettings.bDtr = true;
	}
	else {
		m_ifSettings.bDtr = false;
	}

	m_iError = FECOM_GetPortPara(portHandle, "GetRTS", str);
	if (m_iError < 0) 
		return m_iError;
	if (strcmp(str,"ON")==0) {
		m_ifSettings.bRts = true;
	}
	else {
		m_ifSettings.bRts = false;
	}

	m_iError = FECOM_GetPortPara(portHandle, "TxDelayTime", str);
	if (m_iError < 0) 
		return m_iError;
	m_ifSettings.iTxDelayTime = atoi(str);

	m_iError = FECOM_GetPortPara(portHandle, "CharTimeoutMpy", str);
	if (m_iError < 0) 
		return m_iError;
	m_ifSettings.iCharTimeoutMpy = atoi(str);

	return 0;
}

//------------------------------------------------------------------------------
// Name: getLastError() 
// Desc: get last error
//------------------------------------------------------------------------------
int InterfaceConfig::getLastError() 
{
	return m_iError;
}

//------------------------------------------------------------------------------
// Name: setCommMode(int iCommMode) 
// Desc: saves the communication mode (com, usb, tcp)
//------------------------------------------------------------------------------
void InterfaceConfig::setCommMode(int iCommMode) 
{
	m_ifSettings.iCommMode = iCommMode;
}

//------------------------------------------------------------------------------
// Name: getCommMode() 
// Desc: gets the communication mode that was used te last time
//------------------------------------------------------------------------------
int InterfaceConfig::getCommMode() 
{
	return m_ifSettings.iCommMode;
}

//------------------------------------------------------------------------------
// Name: setHost(char* host) 
// Desc: writes the host name to the config settings
//------------------------------------------------------------------------------
void InterfaceConfig::setHost(char* host) 
{
	if (checkIfIPAddress(host, strlen(host)) == 0)
	{
		memset(m_ifSettings.cHost, 0, sizeof(m_ifSettings.cHost));
		strcpy(m_ifSettings.cHost, host);
	}
}

//------------------------------------------------------------------------------
// Name: getHost() 
// Desc: returns the host name from the settings
//------------------------------------------------------------------------------
char* InterfaceConfig::getHost() 
{
	return m_ifSettings.cHost;
}

//------------------------------------------------------------------------------
// Name: setIPPort(int ipPort) 
// Desc: sets the IP port
//------------------------------------------------------------------------------
void InterfaceConfig::setIPPort(int ipPort) 
{
	if (ipPort > 65536 || ipPort < 0)
		return;
	m_ifSettings.iIpPort = ipPort;
}

//------------------------------------------------------------------------------
// Name: getIPPort() 
// Desc: gets the IP port
//------------------------------------------------------------------------------
int InterfaceConfig::getIPPort() 
{
	return m_ifSettings.iIpPort;
}

//------------------------------------------------------------------------------
// Name: setComPortNumber(int comPortNumber) 
// Desc: sets the com port number
//------------------------------------------------------------------------------
void InterfaceConfig::setComPortNumber(int comPortNumber) 
{
	m_ifSettings.iComPortNumber = comPortNumber;
}

//------------------------------------------------------------------------------
// Name: getComPortNumber() 
// Desc: gets the com port number that was used the last time
//------------------------------------------------------------------------------
int InterfaceConfig::getComPortNumber() 
{
	return m_ifSettings.iComPortNumber;
}
	
//------------------------------------------------------------------------------
// Name: setBaud(int baud) 
// Desc: saves the baud rate to the config settings
//------------------------------------------------------------------------------
void InterfaceConfig::setBaud(int baud) 
{
	m_ifSettings.iBaud = baud;
}
	
//------------------------------------------------------------------------------
// Name: getBaud() 
// Desc: gets the baud rate from the settings
//------------------------------------------------------------------------------
int InterfaceConfig::getBaud() 
{
	return m_ifSettings.iBaud;
}

//------------------------------------------------------------------------------
// Name: setBusAddress(int address) 
// Desc: saves the bus address to the config settings
//------------------------------------------------------------------------------
void InterfaceConfig::setBusAddress(int address)
{
	m_ifSettings.iBusAddress = address;
}

//------------------------------------------------------------------------------
// Name: getBusAddress() 
// Desc: gets the bus address from the settings
//------------------------------------------------------------------------------
int InterfaceConfig::getBusAddress() 
{
	return m_ifSettings.iBusAddress;
}
	
//------------------------------------------------------------------------------
// Name: setFrame(char* frame) 
// Desc: sets the serial port frame to the settings
//------------------------------------------------------------------------------
void InterfaceConfig::setFrame(char* frame) 
{
	if (strlen(frame) > 0)
	{
		memset(m_ifSettings.cFrame, 0, sizeof(m_ifSettings.cFrame));
		strcpy(m_ifSettings.cFrame, frame);
	}
}

//------------------------------------------------------------------------------
// Name: getFrame() 
// Desc: gets the serial port frame from the settings
//------------------------------------------------------------------------------
char* InterfaceConfig::getFrame() 
{
	return (char*)m_ifSettings.cFrame;
}
	
//------------------------------------------------------------------------------
// Name: setTimeout(int timeout) 
// Desc: sets the timeout
//------------------------------------------------------------------------------
void InterfaceConfig::setTimeout(int timeout) 
{
	m_ifSettings.iTimeout = timeout;
}
	
//------------------------------------------------------------------------------
// Name: getTimeout() 
// Desc: gets the timeout
//------------------------------------------------------------------------------
int InterfaceConfig::getTimeout() 
{
	return m_ifSettings.iTimeout;
}
	
//------------------------------------------------------------------------------
// Name: setTxTimeControl(bool txTimeControl) 
// Desc: sets the tx timecontrol for the com port
//------------------------------------------------------------------------------
void InterfaceConfig::setTxTimeControl(bool txTimeControl) 
{
	m_ifSettings.bTxTimeControl = txTimeControl;
}
	
//------------------------------------------------------------------------------
// Name: getTxTimeControl() 
// Desc: gets the tx timecontrol for the com port
//------------------------------------------------------------------------------
bool InterfaceConfig::getTxTimeControl() 
{
	return m_ifSettings.bTxTimeControl;
}
	
//------------------------------------------------------------------------------
// Name: setTxDelayTime(int txDelayTime) 
// Desc: sets the tx delay time for the com port
//------------------------------------------------------------------------------
void InterfaceConfig::setTxDelayTime(int txDelayTime) 
{
	m_ifSettings.iTxDelayTime = txDelayTime;
}
	
//------------------------------------------------------------------------------
// Name: getTxDelayTime() 
// Desc: sets the tx delay time for the com port
//------------------------------------------------------------------------------
int InterfaceConfig::getTxDelayTime() 
{
	return m_ifSettings.iTxDelayTime;
}
	
//------------------------------------------------------------------------------
// Name: setCharTimeoutMpy(int charTimeoutMpy) 
// Desc: sets the char timeout multiplier for the com port
//------------------------------------------------------------------------------
void InterfaceConfig::setCharTimeoutMpy(int charTimeoutMpy) 
{
	m_ifSettings.iCharTimeoutMpy = charTimeoutMpy;
}
	
//------------------------------------------------------------------------------
// Name: getCharTimeoutMpy() 
// Desc: gets the char timeout multiplier for the com port
//------------------------------------------------------------------------------
int InterfaceConfig::getCharTimeoutMpy() 
{
	return m_ifSettings.iCharTimeoutMpy;
}

//------------------------------------------------------------------------------
// Name: setRTS(bool rts) 
// Desc: sets Rts mode of com port on/off
//------------------------------------------------------------------------------
void InterfaceConfig::setRTS(bool rts) 
{
	m_ifSettings.bRts = rts;
}
	
//------------------------------------------------------------------------------
// Name: getRTS() 
// Desc: sets Rts mode of com port on/off
//------------------------------------------------------------------------------
bool InterfaceConfig::getRTS() 
{
	return m_ifSettings.bRts;
}

//------------------------------------------------------------------------------
// Name: setDTR(bool dtr) 
// Desc: sets the dtr mode of the com port on/off
//------------------------------------------------------------------------------
void InterfaceConfig::setDTR(bool dtr) 
{
	m_ifSettings.bDtr = dtr;
}
	
//------------------------------------------------------------------------------
// Name: getDTR() 
// Desc: gets the dtr mode of the com port on/off
//------------------------------------------------------------------------------
bool InterfaceConfig::getDTR() 
{
	return m_ifSettings.bDtr;
}
