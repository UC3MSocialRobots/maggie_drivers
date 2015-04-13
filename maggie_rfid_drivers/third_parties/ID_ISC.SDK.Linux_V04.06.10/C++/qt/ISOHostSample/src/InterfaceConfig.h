#ifndef INTERFACECONFIG_H
#define INTERFACECONFIG_H

const int COM_DEVICE = 0;
const int USB_DEVICE = 1;
const int TCP_DEVICE = 2;

typedef struct _InterfaceSettings
{
	// Interface settings
	int	iCommMode;
	char	cHost[32];
	int	iIpPort;
	int	iComPortNumber;
	int	iBaud;
	int	iBusAddress;
	char	cFrame[8];
	int	iTimeout;
	bool	bTxTimeControl;
	int	iTxDelayTime;
	int	iCharTimeoutMpy;
	bool	bRts;
	bool	bDtr;
	
	// Default values
	void init()
	{
		iCommMode = 0;
		memset(cHost, 0, sizeof(cHost));
		strcpy(cHost, "0.0.0.0");
		iIpPort = 0;
		iComPortNumber = 1;
		iBaud = 38400;
		iBusAddress = 255;
		memset(cFrame, 0, sizeof(cFrame));
		strcpy (cFrame, "8E1");
		iTimeout = 2000;
		bTxTimeControl = true;
		iTxDelayTime = 5;
		iCharTimeoutMpy = 1;
		bRts = false;
		bDtr = false;
	}
	
} InterfaceSettings;

class InterfaceConfig
{	
  public:
	InterfaceConfig();
	~InterfaceConfig();
	
	void init();
	
	int writeConfigToFile(char* fileName = "feconfig.dat");
	int readConfigFromFile(char* fileName = "feconfig.dat");
	
	// operations for serial port
	int getComPorts(int* iComPorts);
	int setComPortConfiguration(int portHandle);
	int getComPortConfiguration(int portHandle);
	int getLastError();

	void setCommMode(int device);
	int  getCommMode();
	
	void setHost(char* host);
	char *getHost();
	
	void setIPPort(int ipPort = 10001);
	int  getIPPort();
	
	void setComPortNumber(int comPortNumber);
	int  getComPortNumber();
	
	void setBaud(int baud);
	int  getBaud(); 
	
	void setBusAddress(int address);
	int  getBusAddress();
	
	void setFrame(char* frame);
	char *getFrame();
	
	void setTimeout(int timeout);
	int  getTimeout();
	
	void setTxTimeControl(bool txTimeControl);
	bool getTxTimeControl();
	
	void setTxDelayTime(int txDelayTime);
	int  getTxDelayTime();
	
	void setCharTimeoutMpy(int charTimeoutMpy);
	int  getCharTimeoutMpy();
	
	void setRTS(bool rts);
	bool getRTS();
	
	void setDTR(bool dtr);
	bool getDTR();
  private:
  	int checkIfIPAddress(char *ip, int len);
	InterfaceSettings m_ifSettings;
	int  m_iError;
};

#endif
