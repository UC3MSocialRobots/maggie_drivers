/*-------------------------------------------------------
|                                                       |
|                  FEDM_Logger.h	                    |
|                                                       |
---------------------------------------------------------

Copyright © 2012		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	13.02.2012

Version       		:	04.00.10 / 13.02.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	logger wrapper class for support for external logmanager (FELOGMNG.dll), when #define _FELOGMNG_SUPPORT is set


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/



#if !defined(_FEDM_LOGGER_H_INCLUDED_)
#define _FEDM_LOGGER_H_INCLUDED_

#ifdef _FELOGMNG_SUPPORT
	#include <strstream>
	#include "felogmng.h"
#endif


#if defined(_FELOGMNG_LINUX)
	#include <dlfcn.h>
#endif
	

#ifdef _FELOGMNG_SUPPORT

	// for info, warning, error, data (protocol, apdu)
	// a - logflag
	// b - message
	#if defined(_FELOGMNG_WINDOWS) || defined(_FELOGMNG_WINDOWS_CE)
	#define FEDM_TRACE(logger, a, b) \
	{ \
		if(logger != NULL && logger->m_lpfnAddLogMessage != NULL) \
		{ \
			LARGE_INTEGER	logTime; \
			::QueryPerformanceCounter(&logTime); \
			std::ostrstream oss; \
			oss << b << std::ends; \
			((LPFN_FELOGMNG_ADD_LOG_MESSAGE)logger->m_lpfnAddLogMessage)(logger->m_pLogger, (unsigned long long)logTime.QuadPart, a, oss.str()); \
			oss.freeze(false); \
		} \
	}
	#endif

	#if defined(_FELOGMNG_LINUX)
	#define FEDM_TRACE(logger, a, b) \
	{ \
		if(logger != NULL && logger->m_lpfnAddLogMessage != NULL) \
		{ \
			std::ostrstream oss; \
			oss << b << std::ends; \
			((LPFN_FELOGMNG_ADD_LOG_MESSAGE)logger->m_lpfnAddLogMessage)(logger->m_pLogger, (unsigned long long)clock(), a, oss.str()); \
			oss.freeze(false); \
		} \
	}
	#endif

	// for function call with optional call parameters
	// a - logflag for function call
	// b - message for function call
	// c - logflag for call parameters
	// d - message for call parameters
	#if defined(_FELOGMNG_WINDOWS) || defined(_FELOGMNG_WINDOWS_CE)
	#define FEDM_TRACE_FNC(logger, a, b, c, d) \
	{ \
		if(logger != NULL && logger->m_lpfnAddLogMessageFnc != NULL) \
		{ \
			LARGE_INTEGER	logTime; \
			::QueryPerformanceCounter(&logTime); \
			std::ostrstream oss1, oss2; \
			oss1 << b << std::ends; \
			oss2 << d << std::ends; \
			((LPFN_FELOGMNG_ADD_LOG_MESSAGE_FNC)logger->m_lpfnAddLogMessageFnc)(logger->m_pLogger, (unsigned long long)logTime.QuadPart, a, oss1.str(), c, oss2.str()); \
			oss1.freeze(false); \
			oss2.freeze(false); \
		} \
	}
	#endif

	#if defined(_FELOGMNG_LINUX)
	#define FEDM_TRACE_FNC(logger, a, b, c, d) \
	{ \
		if(logger != NULL && logger->m_lpfnAddLogMessageFnc != NULL) \
		{ \
			std::ostrstream oss1, oss2; \
			oss1 << b << std::ends; \
			oss2 << d << std::ends; \
			((LPFN_FELOGMNG_ADD_LOG_MESSAGE_FNC)logger->m_lpfnAddLogMessageFnc)(logger->m_pLogger, (unsigned long long)clock(), a, oss1.str(), c, oss2.str()); \
			oss1.freeze(false); \
			oss2.freeze(false); \
		} \
	}
	#endif

	#define FEDM_SETPROP1(logger, a, b) \
	{ \
		if(logger != NULL && logger->m_lpfnSetLoggerProp1 != NULL) \
		{ \
			((LPFN_FELOGMNG_SET_LOGGER_PROP1)logger->m_lpfnSetLoggerProp1)(logger->m_pLogger, a, b); \
		} \
	}

	#define FEDM_SETPROP2(logger, a, b) \
	{ \
		if(logger != NULL && logger->m_lpfnSetLoggerProp2 != NULL) \
		{ \
			((LPFN_FELOGMNG_SET_LOGGER_PROP2)logger->m_lpfnSetLoggerProp2)(logger->m_pLogger, a, b); \
		} \
	}

#else

	#define FEDM_TRACE(logger, a, b) {}
	#define FEDM_TRACE_FNC(logger, a, b, c, d) {}
	#define FEDM_SETPROP1(logger, a, b) {}
	#define FEDM_SETPROP2(logger, a, b) {}

#endif




class /*_FEDM_ISC_CORE_EXT_CLASS*/ FEDM_Logger
{
public:
	FEDM_Logger(unsigned __int64 ui64SourceLayer, unsigned int uiInstallCounter, std::string sVersion)
	{
		m_pLogger = NULL;
		m_pLib = NULL;
		m_lpfnCreateLogger = NULL;
		m_lpfnDeleteLogger = NULL;
		m_lpfnSetLoggerProp1 = NULL;
		m_lpfnSetLoggerProp2 = NULL;
		m_lpfnAddLogMessage = NULL;
		m_lpfnAddLogMessageFnc = NULL;

#ifdef _FELOGMNG_SUPPORT
	#if defined(_FEDM_WINDOWS)
		m_pLib = LoadLib("felogmng.dll");
	#elif defined(_FEDM_LINUX) || defined(_MAC_OSX)
		#if defined(_MAC_OSX)
		m_pLib = LoadLib("libfelogmng.dylib");
		#else
		m_pLib = LoadLib("libfelogmng.so");
		#endif
	#endif

		if(m_pLib != NULL)
		{
			// get all function pointers
			m_lpfnCreateLogger		= GetFunctionPtr(m_pLib, "FELOGMNG_CreateLogger");
			m_lpfnDeleteLogger		= GetFunctionPtr(m_pLib, "FELOGMNG_DeleteLogger");
			m_lpfnSetLoggerProp1	= GetFunctionPtr(m_pLib, "FELOGMNG_SetLoggerProp1");
			m_lpfnSetLoggerProp2	= GetFunctionPtr(m_pLib, "FELOGMNG_SetLoggerProp2");
			m_lpfnAddLogMessage		= GetFunctionPtr(m_pLib, "FELOGMNG_AddLogMessage");
			m_lpfnAddLogMessageFnc	= GetFunctionPtr(m_pLib, "FELOGMNG_AddLogMessageFnc");
	
			if(m_lpfnCreateLogger != NULL)
			{
				int iBack = ((LPFN_FELOGMNG_CREATE_LOGGER)m_lpfnCreateLogger)(ui64SourceLayer, FELOGMNG_SRC_OBJ, uiInstallCounter, 0, 0, 0, "", &m_pLogger);
				if(iBack == 0)
				{
					if(m_pLogger != NULL && uiInstallCounter == 1)
					{
						FEDM_TRACE(this, FELOGMNG_DBG_INFO, sVersion);
					}
				}
			}
		}
#endif
	}

	~FEDM_Logger()
	{
#ifdef _FELOGMNG_SUPPORT
		if(m_pLogger != NULL && m_lpfnDeleteLogger != NULL)
		{
			((LPFN_FELOGMNG_DELETE_LOGGER)m_lpfnDeleteLogger)(m_pLogger);
			m_pLogger = NULL;
		}
#endif
	}

	void* LoadLib(const char* pszLibName)
	{
		void* pLib = NULL;

#ifdef _FELOGMNG_SUPPORT
	#if defined(_FEDM_WINDOWS)
		pLib = (void*)LoadLibrary(pszLibName);
	#elif defined(_WIN32_WCE)
		pLib = (void*)LoadLibrary(_T(pszLibName));
	#elif defined(_FEDM_LINUX)
		pLib = dlopen(pszLibName, RTLD_NOW | RTLD_GLOBAL);
	#endif
#endif

		return pLib;	
	}

	void  CloseLib(void* pLib)
	{
#ifdef _FELOGMNG_SUPPORT
	#if defined(_FEDM_WINDOWS)
		if(pLib != NULL)
			FreeLibrary((HINSTANCE)pLib);
	#endif

	#if defined(_FEDM_LINUX)
		if(pLib != NULL)
			dlclose(pLib);
	#endif
#endif
	}

	void* GetFunctionPtr(void* pLib, const char* szFctName)
	{
#ifdef _FELOGMNG_SUPPORT
	#ifdef _FEDM_WINDOWS
		#ifdef _WIN32_WCE
			#ifdef _FEDM_EVC4
			FARPROC pAdr = NULL;
			wchar_t* pwc      = (wchar_t *)malloc( sizeof( wchar_t )*sizeof(szFctName));
			mbstowcs( pwc, szFctName, sizeof(szFctName) );
			pAdr =  GetProcAddress((HINSTANCE)pLib, pwc);
			free(pwc);
			return pAdr;
			#else
			return GetProcAddress((HINSTANCE)pLib, CA2W(szFctName));
			#endif
		#else
			return GetProcAddress((HINSTANCE)pLib, szFctName);
		#endif
	#endif
	#ifdef _FEDM_LINUX
		return dlsym(pLib, szFctName);
	#endif
#else
		return NULL;
#endif
	}

public:
	void* m_pLogger;
	void* m_pLib;
	void* m_lpfnCreateLogger;
	void* m_lpfnDeleteLogger;
	void* m_lpfnSetLoggerProp1;
	void* m_lpfnSetLoggerProp2;
	void* m_lpfnAddLogMessage;
	void* m_lpfnAddLogMessageFnc;

};

#endif // !defined(_FEDM_LOGGER_H_INCLUDED_)
