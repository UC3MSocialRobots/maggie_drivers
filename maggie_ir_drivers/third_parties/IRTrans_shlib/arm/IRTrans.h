#ifdef WIN32

#include <winsock2.h>
#include <windows.h>

#endif

#ifdef LINUX

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#define closesocket close

#endif


#include <stdio.h>

#include "remote.h"
#include "network.h"
#include "errcode.h"


#ifdef WIN32

#ifdef IRTRANSDLL_EXPORTS
#define IRTRANSDLL_API __declspec(dllexport)
#else
#ifdef  __cplusplus
#define IRTRANSDLL_API extern "C" __declspec(dllimport)
#else
#define IRTRANSDLL_API __declspec(dllimport)
#endif
#endif

#else

#define IRTRANSDLL_API

#endif


IRTRANSDLL_API int ConnectIRTransServer (char host[],SOCKET *sock);
IRTRANSDLL_API void DisconnectIRTransServer (SOCKET serv);
IRTRANSDLL_API NETWORKSTATUS *LearnIRCode (SOCKET serv, char rem[],char com[],unsigned short timeout);
IRTRANSDLL_API NETWORKSTATUS *LearnRepeatIRCode (SOCKET serv, char rem[],char com[],unsigned short timeout);
IRTRANSDLL_API NETWORKSTATUS *SendRemoteCommand (SOCKET serv, char rem[],char com[],int mask,int LEDSel,int bus);
IRTRANSDLL_API NETWORKRECV *ReceiveIR (SOCKET serv);
IRTRANSDLL_API NETWORKSTATUS *ReloadIRDatabase (SOCKET serv);
IRTRANSDLL_API int GetDeviceStatus (SOCKET serv, void *result);
IRTRANSDLL_API int GetDeviceStatusExN (SOCKET serv, int offset,void *result);
IRTRANSDLL_API NETWORKSTATUS *SendRemoteCommandEx (SOCKET serv, char rem[],char com[],word mask,byte LEDSel,byte bus);
IRTRANSDLL_API NETWORKSTATUS *SendCCFCommand (SOCKET serv, char ccfstr[],byte repeat,word mask,byte LEDSel,byte bus);
IRTRANSDLL_API NETWORKSTATUS *SendCCFCommandLong (SOCKET serv, char ccfstr[],byte repeat,word mask,byte LEDSel,byte bus);
IRTRANSDLL_API int GetRemotes (SOCKET serv,int16_t offset,REMOTEBUFFER *rem);
IRTRANSDLL_API int GetCommands (SOCKET serv,char rem[],int16_t offset,COMMANDBUFFER *com);


IRTRANSDLL_API NETWORKSTATUS *SetTimerEntry (SOCKET serv,byte num,byte mode,byte year,byte month,byte day,byte weekday,byte hour,byte min,
											 byte ledsel,word targetmask,byte remote[],byte command[],byte bus);

/**********************************************************
Set Timer Parameter:

serv:		IRTrans Server conncection
num:		Timer Number (0-3)
mode:		Timer Mode
				TIMER_OFF
				TIMER_ONCE
				TIMER_REPEAT
year:		Year
month:		Month
day:		Day
Weekday:	Weekday
				WD_SUNDAY					
				WD_MONDAY	
				WD_TUESDAY	
				WD_WEDNESDAY
				WD_THURSDAY	
				WD_FRIDAY	
				WD_SATURDAY	
hour:		Hour
min:		Minute
ledsel:		LED Select	
				INTERNAL_LEDS
				EXTERNAL_LEDS
				SEND_ACTION (Send Action to IP Clients instead of sending IR Codes
targetmask:	Targetmask for IRTrans serial bus (0 if unused)
remote:		Timer Remotecontrol
command:	Timer Remotecommand
bus:		Bus number of the IRTrans device (0-254), 0 for the first device

For all unused Time parameters (year,month,day,hour,minute,weekday) use TIMER_UNUSED

***********************************************************/

#define WD_SUNDAY		0
#define WD_MONDAY		1
#define WD_TUESDAY		2
#define WD_WEDNESDAY	3
#define WD_THURSDAY		4
#define WD_FRIDAY		5
#define WD_SATURDAY		6

#define	TIMER_UNUSED	0xff
#define SEND_ACTION		128
#define TIMER_OFF		1
#define TIMER_ONCE		2
#define TIMER_REPEAT	4
