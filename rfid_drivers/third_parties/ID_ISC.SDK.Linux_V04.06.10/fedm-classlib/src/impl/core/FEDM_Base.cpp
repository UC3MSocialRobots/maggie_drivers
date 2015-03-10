/*-------------------------------------------------------
|                                                       |
|                      FEDM_Base.cpp                    |
|                                                       |
---------------------------------------------------------

Copyright  2000-2012	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	13.05.2000
Version       		:	04.03.00 / 21.08.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	base class for FEDM class library
						collects all access functions for data memories


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#include <stdio.h>
#include <string.h>
#include "FEDM_Base.h"

// the following codeline is for Linux
// please define _FEDM_LINUX with compiler option
#ifdef _FEDM_LINUX
#if !defined(_FEDM_SUPPORT_SLINK)
	#include <dlfcn.h>
#endif
#endif

#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif


#if defined(_FEDM_WINDOWS) && defined(_MSC_VER)
	#if _MSC_VER >= 1600
	    #if defined(_DEBUG)
			#define FECOM_LIB_NAME	"fecom.dll"
			#define FEUSB_LIB_NAME	"feusb.dll"
			#define FETCP_LIB_NAME	"fetcp.dll"
		#else
			#define FECOM_LIB_NAME	"fecom.dll"
			#define FEUSB_LIB_NAME	"feusb.dll"
			#define FETCP_LIB_NAME	"fetcp.dll"
		#endif
	#else
		#define FECOM_LIB_NAME	"fecom.dll"
		#define FEUSB_LIB_NAME	"feusb.dll"
		#define FETCP_LIB_NAME	"fetcp.dll"
	#endif
#endif

//#####################################################################################
// Construction/Destruction
//#####################################################################################

int FEDM_Base::m_iLanguage = FEDM_LANG_ENGLISH;


FEDM_Base::FEDM_Base()
{
	m_iLastError = 0;
#ifdef _FEDM_WINDOWS
	m_iWinVer = 0;
#endif

#ifdef _FEDM_COM_SUPPORT
	m_pFeComLib		= NULL;		// pointer or handle to fecom
	m_pFeComPtrList	= NULL;		// pointer to structure with function pointers of FECOM
	m_pFecomLibName	= NULL;		// pointer to alternative fecom library name
#endif

#ifdef _FEDM_USB_SUPPORT
	m_pFeUsbLib		= NULL;		// pointer or handle to feusb
	m_pFeUsbPtrList	= NULL;		// pointer to structure with function pointers of FEUSB
	m_pFeusbLibName	= NULL;		// pointer to alternative feusb library name
#endif

#ifdef _FEDM_TCP_SUPPORT
	m_pFeTcpLib		= NULL;		// pointer or handle to fetcp
	m_pFeTcpPtrList	= NULL;		// pointer to structure with function pointers of FETCP
	m_pFetcpLibName	= NULL;		// pointer to alternative fetcp library name
#endif

}

FEDM_Base::~FEDM_Base()
{
#if !defined(_FEDM_SUPPORT_SLINK)

#if defined(_FEDM_COM_SUPPORT) && !defined(_FEDM_SUPPORT_SLINK)
	if(m_pFeComLib != NULL)
#ifdef _FEDM_WINDOWS
		FreeLibrary((HINSTANCE)m_pFeComLib);
#endif // _FEDM_WINDOWS
#ifdef _FEDM_LINUX
		dlclose(m_pFeComLib);
#endif // _FEDM_LINUX
#endif // #if defined(_FEDM_COM_SUPPORT) && !defined(_FEDM_SUPPORT_SLINK)


#if defined(_FEDM_USB_SUPPORT) && !defined(_FEDM_SUPPORT_SLINK)
	if(m_pFeUsbLib != NULL)
#ifdef _FEDM_WINDOWS
		FreeLibrary((HINSTANCE)m_pFeUsbLib);
#endif // _FEDM_WINDOWS
#ifdef _FEDM_LINUX
		dlclose(m_pFeUsbLib);
#endif // _FEDM_LINUX
#endif // #if defined(_FEDM_USB_SUPPORT)&& !defined(_FEDM_SUPPORT_SLINK)


#if defined(_FEDM_TCP_SUPPORT) && !defined(_FEDM_SUPPORT_SLINK)
	if(m_pFeTcpLib != NULL)
#ifdef _FEDM_WINDOWS
		FreeLibrary((HINSTANCE)m_pFeTcpLib);
#endif // _FEDM_TCP_SUPPORT
#ifdef _FEDM_LINUX
		dlclose(m_pFeTcpLib);
#endif // _FEDM_LINUX
#endif // #if defined(_FEDM_TCP_SUPPORT)&& !defined(_FEDM_SUPPORT_SLINK)

#endif // #if !defined(_FEDM_SUPPORT_SLINK)

#if defined(_FEDM_COM_SUPPORT) && !defined(_FEDM_SUPPORT_SLINK)
	if(m_pFeComPtrList != NULL)
		delete (_FEDM_FECOM_PTR*)m_pFeComPtrList;
#endif

#if defined(_FEDM_USB_SUPPORT) && !defined(_FEDM_SUPPORT_SLINK)
	if(m_pFeUsbPtrList != NULL)
		delete (_FEDM_FEUSB_PTR*)m_pFeUsbPtrList;
#endif

#if defined(_FEDM_TCP_SUPPORT) && !defined(_FEDM_SUPPORT_SLINK)
	if(m_pFeTcpPtrList != NULL)
		delete (_FEDM_FETCP_PTR*)m_pFeTcpPtrList;
#endif
}

//#####################################################################################
// query functions
//#####################################################################################

char* FEDM_Base::GetLibVersion()
{
	return FEDM_VERSION;
}


/***************************************************************************
  Begin        		:	11.05.2001 / M. Hultsch
  Version       	:	01.03.00 / 11.05.2001 / M. Hultsch

  Function			:	set language setting

  Parameters		:	int iLanguage	-	7: german
											9: english

  Return value		:	FEDM_OK or error code (<0)

***************************************************************************/
int FEDM_Base::SetLanguage(int iLanguage)
{
	int iBack = FEDM_OK;
	
	switch(iLanguage)
	{
	case FEDM_LANG_GERMAN:
		m_iLanguage = FEDM_LANG_GERMAN;
		break;

	case FEDM_LANG_ENGLISH:
		m_iLanguage = FEDM_LANG_ENGLISH;
		break;

	default:
		iBack = FEDM_ERROR_UNKNOWN_LANGUAGE;
	}

	return iBack;
}

int FEDM_Base::GetLanguage()
{
	return m_iLanguage;
}

void FEDM_Base::SetLastError(int iErrorCode)
{
	m_iLastError = iErrorCode;
}


/***************************************************************************
  Begin        		:	22.11.2000 / M. Hultsch
  Version       	:	01.03.00 / 11.05.2001 / M. Hultsch

  Function			:	return an error text for FEDM-error code

  Parameters		:	char* sTxt		-	pointer to text buffer
						int iErrCode	-	error code

  Return value		:	status byte (>=0) or error code (<0)

***************************************************************************/
int FEDM_Base::GetErrorText(char* sText, int iErrCode)
{
	int iBack = FEDM_OK;
	FEDM_CHK3(sText);

	switch(m_iLanguage)
	{
	case FEDM_LANG_GERMAN:
		GetErrorText(FEDM_LANG_GERMAN, sText, iErrCode);
		break;

	case FEDM_LANG_ENGLISH:
		GetErrorText(FEDM_LANG_ENGLISH, sText, iErrCode);
		break;

	default:
		iBack = FEDM_ERROR_UNKNOWN_LANGUAGE;
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	11.05.2001 / M. Hultsch
  Version       	:	04.03.00 / 21.08.2012 / M. Hultsch

  Function			:	return an error text for FEDM-error code

  Parameters		:	int iLanguage	-	language identifier
						char* sTxt		-	pointer to text buffer
						int iErrCode	-	error code

  Return value		:	status byte (>=0) or error code (<0)

***************************************************************************/
int FEDM_Base::GetErrorText(int iLanguage, char* sText, int iErrCode)
{
	int iLang = FEDM_LANG_ENGLISH;

	// use default setting, when iLanguage == 0
	if(iLanguage == 0)
		iLang = m_iLanguage;
	else if(iLanguage == FEDM_LANG_GERMAN)
		iLang = FEDM_LANG_GERMAN;
	else if(iLanguage == FEDM_LANG_ENGLISH)
		iLang = FEDM_LANG_ENGLISH;


	if(iLang == FEDM_LANG_GERMAN)
	{
		switch(iErrCode)
		{
		case FEDM_OK:
			strcpy(sText, "OK");
			break;
		case FEDM_ERROR_BLOCK_SIZE:
			strcpy(sText, "Fehler in Modul FEDM: Blockgröße ist falsch");
			break;
		case FEDM_ERROR_BIT_BOUNDARY:
			strcpy(sText, "Fehler in Modul FEDM: Bitgrenze überschritten");
			break;
		case FEDM_ERROR_BYTE_BOUNDARY:
			strcpy(sText, "Fehler in Modul FEDM: Bytegrenze überschritten");
			break;
		case FEDM_ERROR_ARRAY_BOUNDARY:
			strcpy(sText, "Fehler in Modul FEDM: Arraygrenze überschritten");
			break;
		case FEDM_ERROR_BUFFER_LENGTH:
			strcpy(sText, "Fehler in Modul FEDM: Puffergrenze überschritten");
			break;
		case FEDM_ERROR_PARAMETER:
			strcpy(sText, "Fehler in Modul FEDM: Unbekannter Parameter oder Parameter außerhalb zulässigem Bereich");
			break;
		case FEDM_ERROR_STRING_LENGTH:
			strcpy(sText, "Fehler in Modul FEDM: Zeichenkette ist zu lang");
			break;
		case FEDM_ERROR_ODD_STRING_LENGTH:
			strcpy(sText, "Fehler in Modul FEDM: Eine Zeichenkette mit gerader Anzahl Zeichen wird erwartet");
			break;
		case FEDM_ERROR_NO_DATA:
			strcpy(sText, "Fehler in Modul FEDM: Keine Protokolldaten empfangen");
			break;
		case FEDM_ERROR_NO_READER_HANDLE:
			strcpy(sText, "Fehler in Modul FEDM: Kein Handle eines Leserobjektes gefunden");
			break;
		case FEDM_ERROR_NO_PORT_HANDLE:
			strcpy(sText, "Fehler in Modul FEDM: Kein Handle einer seriellen Schnittstelle gefunden");
			break;
		case FEDM_ERROR_UNKNOWN_CONTROL_BYTE:
			strcpy(sText, "Fehler in Modul FEDM: Unbekanntes Steuerbyte");
			break;
		case FEDM_ERROR_UNKNOWN_MEM_ID:
			strcpy(sText, "Fehler in Modul FEDM: Unbekannte Speicherkonstante");
			break;
		case FEDM_ERROR_UNKNOWN_POLL_MODE:
			strcpy(sText, "Fehler in Modul FEDM: Unbekannter Poll-Modus");
			break;
		case FEDM_ERROR_NO_TABLE_DATA:
			strcpy(sText, "Fehler in Modul FEDM: Keine Tabellendaten vorhanden");
			break;
		case FEDM_ERROR_UNKNOWN_ERROR_CODE:
			strcpy(sText, "Fehler in Modul FEDM: Unbekannter Fehlercode");
			break;
		case FEDM_ERROR_UNKNOWN_COMMAND:
			strcpy(sText, "Fehler in Modul FEDM: Unbekanntes ISO-Host-Kommando");
			break;
		case FEDM_ERROR_UNSUPPORTED:
			strcpy(sText, "Fehler in Modul FEDM: Keine Unterstützung für diesen Parameter oder diese Funktion");
			break;
		case FEDM_ERROR_NO_MORE_MEM:
			strcpy(sText, "Fehler in Modul FEDM: Kein Systemspeicher mehr verfügbar");
			break;
		case FEDM_ERROR_NO_READER_FOUND:
			strcpy(sText, "Fehler in Modul FEDM: Kein Leser gefunden");
			break;
		case FEDM_ERROR_NULL_POINTER:
			strcpy(sText, "Fehler in Modul FEDM: NULL-Zeiger übergeben");
			break;
		case FEDM_ERROR_UNKNOWN_READER_TYPE:
			strcpy(sText, "Fehler in Modul FEDM: Unbekannter Lesertyp");
			break;
		case FEDM_ERROR_UNSUPPORTED_READER_TYPE:
			strcpy(sText, "Fehler in Modul FEDM: Funktion kann für diesen Lesertyp nicht verwendet werden");
			break;
		case FEDM_ERROR_UNKNOWN_TABLE_ID:
			strcpy(sText, "Fehler in Modul FEDM: Unbekannte Tabellenkonstante");
			break;
		case FEDM_ERROR_UNKNOWN_LANGUAGE:
			strcpy(sText, "Fehler in Modul FEDM: Unbekannte Sprachenkonstante");
			break;
		case FEDM_ERROR_NO_TABLE_SIZE:
			strcpy(sText, "Fehler in Modul FEDM: Tabelle hat Größe 0");
			break;
		case FEDM_ERROR_SENDBUFFER_OVERFLOW:
			strcpy(sText, "Fehler in Modul FEDM: Sendepuffer ist voll");
			break;
		case FEDM_ERROR_VERIFY:
			strcpy(sText, "Fehler in Modul FEDM: Daten sind ungleich");
			break;
		case FEDM_ERROR_OPEN_FILE:
			strcpy(sText, "Fehler in Modul FEDM: Fehler beim Öffnen der Datei");
			break;
		case FEDM_ERROR_SAVE_FILE:
			strcpy(sText, "Fehler in Modul FEDM: Fehler beim Speichern der Datei");
			break;
		case FEDM_ERROR_UNKNOWN_TRANSPONDER_TYPE:
			strcpy(sText, "Fehler in Modul FEDM: unbekannter Transpondertyp");
			break;
		case FEDM_ERROR_READ_FILE:
			strcpy(sText, "Fehler in Modul FEDM: Fehler beim Lesen der Datei");
			break;
		case FEDM_ERROR_WRITE_FILE:
			strcpy(sText, "Fehler in Modul FEDM: Fehler beim Schreiben in Datei");
			break;
		case FEDM_ERROR_UNKNOWN_EPC_TYPE:
			strcpy(sText, "Fehler in Modul FEDM: unbekannter EPC-Typ");
			break;
		case FEDM_ERROR_UNSUPPORTED_PORT_DRIVER:
			strcpy(sText, "Fehler in Modul FEDM: Funktion unterstützt nicht den aktiven Kommunikations-Treiber");
			break;
		case FEDM_ERROR_UNKNOWN_ADDRESS_MODE:
			strcpy(sText, "Fehler in Modul FEDM: unbekannter Adressmodus");
			break;
		case FEDM_ERROR_ALREADY_CONNECTED:
			strcpy(sText, "Fehler in Modul FEDM: Leserobjekt ist bereits mit einem Port verbunden");
			break;
		case FEDM_ERROR_NOT_CONNECTED:
			strcpy(sText, "Fehler in Modul FEDM: Leserobjekt ist nicht mit einem Port verbunden");
			break;
		case FEDM_ERROR_NO_MODULE_HANDLE:
			strcpy(sText, "Fehler in Modul FEDM: Kein Handle eines Lesermoduls gefunden");
			break;
		case FEDM_ERROR_EMPTY_MODULE_LIST:
			strcpy(sText, "Fehler in Modul FEDM: Die Modulliste ist leer");
			break;
		case FEDM_ERROR_MODULE_NOT_FOUND:
			strcpy(sText, "Fehler in Modul FEDM: Modul nicht in Modulliste gefunden");
			break;
		case FEDM_ERROR_DIFFERENT_OBJECTS:
			strcpy(sText, "Fehler in Modul FEDM: Laufzeitobjekte sind nicht identisch");
			break;
		case FEDM_ERROR_NOT_AN_EPC:
			strcpy(sText, "Fehler in Modul FEDM: Seriennummer des Transponders ist keine EPC");
			break;
		case FEDM_ERROR_OLD_LIB_VERSION:
			strcpy(sText, "Fehler in Modul FEDM: Alte Bibliotheksversion. Funktionsaufruf nicht möglich.");
			break;
		case FEDM_ERROR_WRONG_READER_TYPE:
			strcpy(sText, "Fehler in Modul FEDM: Ungültiger Lesertyp.");
			break;
		case FEDM_ERROR_CRC:
			strcpy(sText, "Fehler in Modul FEDM: Checksummen-Fehler beim Verifizieren der Datei.");
			break;
		case FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ:
			strcpy(sText, "Fehler in Modul FEDM: Die CFG-Block muss vor dem Schreiben zuerst gelesen werden.");
			break;
		case FEDM_ERROR_UNSUPPORTED_CONTROLLER_TYPE:
			strcpy(sText, "Fehler in Modul FEDM: Controller-Typ wird in Funktion nicht unterstützt");
			break;
		case FEDM_ERROR_VERSION_CONFLICT:
			strcpy(sText, "Fehler in Modul FEDM: Versionskonflikt mit einer oder mehrerer abhängiger Bibliotheken");
			break;
		case FEDM_ERROR_UNSUPPORTED_NAMESPACE:
			strcpy(sText, "Fehler in Modul FEDM: Parameter wird vom angeschlossenen Leser nicht unterstützt");
			break;
		case FEDM_ERROR_TASK_STILL_RUNNING:
			strcpy(sText, "Fehler in Modul FEDM: Asynchroner Task ist noch am laufen");
			break;
		case FEDM_ERROR_TAG_HANDLER_NOT_IDENTIFIED:
			strcpy(sText, "Fehler in Modul FEDM: TagHandler-Typ konnte nicht identifiziert werden");
			break;
		case FEDM_ERROR_UNVALID_IDD_LENGTH:
			strcpy(sText, "Fehler in Modul FEDM: Längenangabe für IDD ist außerhalb des zulässigen Bereichs");
			break;
		case FEDM_ERROR_UNVALID_IDD_FORMAT:
			strcpy(sText, "Fehler in Modul FEDM: Formateinstellung für IDD ist falsch");
			break;
		case FEDM_ERROR_UNKNOWN_TAG_HANDLER_TYPE:
			strcpy(sText, "Fehler in Modul FEDM: Unbekannter TagHandler-Typ");
			break;
		case FEDM_ERROR_UNSUPPORTED_TRANSPONDER_TYPE:
			strcpy(sText, "Fehler in Modul FEDM: Transpondertyp wird für die gewählte Aktion nicht unterstützt");
			break;
		case FEDM_ERROR_CONNECTED_WITH_OTHER_MODULE:
			strcpy(sText, "Fehler in Modul FEDM: Ein anderes Leserobjekt ist bereits mit diesem Port verbunden");
			break;
		case FEDM_ERROR_INVENTORY_NO_TID_IN_UID:
			strcpy(sText, "Fehler in Modul FEDM: Inventory mit Rückgabe von UID = EPC + TID, aber TID fehlt");
			break;
		
		case FEDM_XML_ERROR_NO_XML_FILE:
			strcpy(sText, "Fehler in Modul FEDM_XML: Datei ist kein XML-Dokument");
			break;
		case FEDM_XML_ERROR_NO_OBID_TAG:
			strcpy(sText, "Fehler in Modul FEDM_XML: Datei enthält kein Element 'OBID'");
			break;
		case FEDM_XML_ERROR_NO_CHILD_TAG:
			strcpy(sText, "Fehler in Modul FEDM_XML: Kein Sub-Element vorhanden");
			break;
		case FEDM_XML_ERROR_TAG_NOT_FOUND:
			strcpy(sText, "Fehler in Modul FEDM_XML: Element nicht in Dokument");
			break;
		case FEDM_XML_ERROR_DOC_NOT_WELL_FORMED:
			strcpy(sText, "Fehler in Modul FEDM_XML: XML-Dokument nicht wohlgeformt");
			break;
		case FEDM_XML_ERROR_NO_TAG_VALUE:
			strcpy(sText, "Fehler in Modul FEDM_XML: Kein Element-Inhalt vorhanden");
			break;
		case FEDM_XML_ERROR_NO_TAG_ATTRIBUTE:
			strcpy(sText, "Fehler in Modul FEDM_XML: Kein Attribute vorhanden");
			break;
		case FEDM_XML_ERROR_DOC_FILE_VERSION:
			strcpy(sText, "Fehler in Modul FEDM_XML: Ungültige Dokument-Version");
			break;
		case FEDM_XML_ERROR_DOC_FILE_FAMILY:
			strcpy(sText, "Fehler in Modul FEDM_XML: Dokument für andere Leserfamilie");
			break;
		case FEDM_XML_ERROR_WRONG_CONTROLLER_TYPE:
			strcpy(sText, "Fehler in Modul FEDM_XML: Ungültiger Controller-Typ");
			break;
		case FEDM_XML_ERROR_WRONG_MEM_BANK_TYPE:
			strcpy(sText, "Fehler in Modul FEDM_XML: Ungültiger Speicherbank-Typ");
			break;
		default:
			sprintf(sText, "Fehler in Modul FEDM: Unbekannter Fehlercode (%d | 0x%08X)", iErrCode, iErrCode);
			return FEDM_ERROR_UNKNOWN_ERROR_CODE;
		}
	}
	else if(iLang == FEDM_LANG_ENGLISH)
	{
		switch(iErrCode)
		{
		case FEDM_OK:
			strcpy(sText, "OK");
			break;
		case FEDM_ERROR_BLOCK_SIZE:
			strcpy(sText, "Error in Module FEDM: Block size in the access constant is incorrect");
			break;
		case FEDM_ERROR_BIT_BOUNDARY:
			strcpy(sText, "Error in Module FEDM: Bit boundary in the access constant is incorrect");
			break;
		case FEDM_ERROR_BYTE_BOUNDARY:
			strcpy(sText, "Error in Module FEDM: Byte boundary in the access constant is incorrect");
			break;
		case FEDM_ERROR_ARRAY_BOUNDARY:
			strcpy(sText, "Error in Module FEDM: Array boundary of a data container was exceeded");
			break;
		case FEDM_ERROR_BUFFER_LENGTH:
			strcpy(sText, "Error in Module FEDM: Length of the data buffer is insufficient");
			break;
		case FEDM_ERROR_PARAMETER:
			strcpy(sText, "Error in Module FEDM: Unknown transfer parameter or parameter value is out of valid range");
			break;
		case FEDM_ERROR_STRING_LENGTH:
			strcpy(sText, "Error in Module FEDM: Transferred string is too long");
			break;
		case FEDM_ERROR_ODD_STRING_LENGTH:
			strcpy(sText, "Error in Module FEDM: Transferred string contains an odd number of characters. Even number of characters expected");
			break;
		case FEDM_ERROR_NO_DATA:
			strcpy(sText, "Error in Module FEDM: No data in the protocol");
			break;
		case FEDM_ERROR_NO_READER_HANDLE:
			strcpy(sText, "Error in Module FEDM: No reader handle set");
			break;
		case FEDM_ERROR_NO_PORT_HANDLE:
			strcpy(sText, "Error in Module FEDM: No port handle set");
			break;
		case FEDM_ERROR_UNKNOWN_CONTROL_BYTE:
			strcpy(sText, "Error in Module FEDM: Unknown control byte");
			break;
		case FEDM_ERROR_UNKNOWN_MEM_ID:
			strcpy(sText, "Error in Module FEDM: Unknown memory ID");
			break;
		case FEDM_ERROR_UNKNOWN_POLL_MODE:
			strcpy(sText, "Error in Module FEDM: Unknown poll mode");
			break;
		case FEDM_ERROR_NO_TABLE_DATA:
			strcpy(sText, "Error in Module FEDM: No data in a table");
			break;
		case FEDM_ERROR_UNKNOWN_ERROR_CODE:
			strcpy(sText, "Error in Module FEDM: Unknown error code");
			break;
		case FEDM_ERROR_UNKNOWN_COMMAND:
			strcpy(sText, "Error in Module FEDM: Unknown ISO-Host command");
			break;
		case FEDM_ERROR_UNSUPPORTED:
			strcpy(sText, "Error in Module FEDM: No support for this parameter or function");
			break;
		case FEDM_ERROR_NO_MORE_MEM:
			strcpy(sText, "Error in Module FEDM: No more program memory available");
			break;
		case FEDM_ERROR_NO_READER_FOUND:
			strcpy(sText, "Error in Module FEDM: No reader found");
			break;
		case FEDM_ERROR_NULL_POINTER:
			strcpy(sText, "Error in Module FEDM: The transferred pointer is NULL");
			break;
		case FEDM_ERROR_UNKNOWN_READER_TYPE:
			strcpy(sText, "Error in Module FEDM: Unknown reader type");
			break;
		case FEDM_ERROR_UNSUPPORTED_READER_TYPE:
			strcpy(sText, "Error in Module FEDM: The Function doesn't support this reader type");
			break;
		case FEDM_ERROR_UNKNOWN_TABLE_ID:
			strcpy(sText, "Error in Module FEDM: Unknown table constant");
			break;
		case FEDM_ERROR_UNKNOWN_LANGUAGE:
			strcpy(sText, "Error in Module FEDM: Unknown language constant");
			break;
		case FEDM_ERROR_NO_TABLE_SIZE:
			strcpy(sText, "Error in Module FEDM: The table has the size 0");
			break;
		case FEDM_ERROR_SENDBUFFER_OVERFLOW:
			strcpy(sText, "Error in Module FEDM: The Sendbuffer is full");
			break;
		case FEDM_ERROR_VERIFY:
			strcpy(sText, "Error in Module FEDM: Data are not equal");
			break;
		case FEDM_ERROR_OPEN_FILE:
			strcpy(sText, "Error in Module FEDM: File open error");
			break;
		case FEDM_ERROR_SAVE_FILE:
			strcpy(sText, "Error in Module FEDM: File save error");
			break;
		case FEDM_ERROR_UNKNOWN_TRANSPONDER_TYPE:
			strcpy(sText, "Error in Module FEDM: Unknown transponder type");
			break;
		case FEDM_ERROR_READ_FILE:
			strcpy(sText, "Error in Module FEDM: File read error");
			break;
		case FEDM_ERROR_WRITE_FILE:
			strcpy(sText, "Error in Module FEDM: File write error");
			break;
		case FEDM_ERROR_UNKNOWN_EPC_TYPE:
			strcpy(sText, "Error in Module FEDM: Unknown EPC type");
			break;
		case FEDM_ERROR_UNSUPPORTED_PORT_DRIVER:
			strcpy(sText, "Error in Module FEDM: Function does not support the active communication driver");
			break;
		case FEDM_ERROR_UNKNOWN_ADDRESS_MODE:
			strcpy(sText, "Error in Module FEDM: Unknown address mode");
			break;
		case FEDM_ERROR_ALREADY_CONNECTED:
			strcpy(sText, "Error in Module FEDM: Reader object is already connected with a communication port");
			break;
		case FEDM_ERROR_NOT_CONNECTED:
			strcpy(sText, "Error in Module FEDM: Reader object is not connected with a communication port");
			break;
		case FEDM_ERROR_NO_MODULE_HANDLE:
			strcpy(sText, "Error in Module FEDM: No module handle found");
			break;
		case FEDM_ERROR_EMPTY_MODULE_LIST:
			strcpy(sText, "Error in Module FEDM: The module list is empty");
			break;
		case FEDM_ERROR_MODULE_NOT_FOUND:
			strcpy(sText, "Error in Module FEDM: Module not found in module list");
			break;
		case FEDM_ERROR_DIFFERENT_OBJECTS:
			strcpy(sText, "Error in Module FEDM: Runtime objects are different");
			break;
		case FEDM_ERROR_NOT_AN_EPC:
			strcpy(sText, "Error in Module FEDM: IDD of transponder is not an EPC");
			break;
		case FEDM_ERROR_OLD_LIB_VERSION:
			strcpy(sText, "Error in Module FEDM: Old library version. Function call not possible.");
			break;
		case FEDM_ERROR_WRONG_READER_TYPE:
			strcpy(sText, "Error in Module FEDM: Wrong reader type");
			break;
		case FEDM_ERROR_CRC:
			strcpy(sText, "Error in Module FEDM: Verification of file content terminates with checksum error");
			break;
		case FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ:
			strcpy(sText, "Error in Module FEDM: The CFG-Block must be first read before write");
			break;
		case FEDM_ERROR_UNSUPPORTED_CONTROLLER_TYPE:
			strcpy(sText, "Error in Module FEDM: Function does not support this controller type");
			break;
		case FEDM_ERROR_VERSION_CONFLICT:
			strcpy(sText, "Error in Module FEDM: Version conflict with one or more dependent libraries");
			break;
		case FEDM_ERROR_UNSUPPORTED_NAMESPACE:
			strcpy(sText, "Error in Module FEDM: Connected reader does not support this parameter");
			break;
		case FEDM_ERROR_TASK_STILL_RUNNING:
			strcpy(sText, "Error in Module FEDM: Asynchronous task is still running");
			break;
		case FEDM_ERROR_TAG_HANDLER_NOT_IDENTIFIED:
			strcpy(sText, "Error in Module FEDM: TagHandler type could not be identified");
			break;
		case FEDM_ERROR_UNVALID_IDD_LENGTH:
			strcpy(sText, "Error in Module FEDM: Value of IDD-Length is out of range");
			break;
		case FEDM_ERROR_UNVALID_IDD_FORMAT:
			strcpy(sText, "Error in Module FEDM: Value of IDD-Format is out of range");
			break;
		case FEDM_ERROR_UNKNOWN_TAG_HANDLER_TYPE:
			strcpy(sText, "Error in Module FEDM: Unknown TagHandler type");
			break;
		case FEDM_ERROR_UNSUPPORTED_TRANSPONDER_TYPE:
			strcpy(sText, "Error in Module FEDM: Transponder type is not supported by the selected task");
			break;
		case FEDM_ERROR_CONNECTED_WITH_OTHER_MODULE:
			strcpy(sText, "Error in Module FEDM: Another Reader object is already connected with this communication port");
			break;
		case FEDM_ERROR_INVENTORY_NO_TID_IN_UID:
			strcpy(sText, "Error in Module FEDM: Inventory with return of UID = EPC + TID, but TID is missing");
			break;
		
		case FEDM_XML_ERROR_NO_XML_FILE:
			strcpy(sText, "Error in Module FEDM_XML: File is not a XML document");
			break;
		case FEDM_XML_ERROR_NO_OBID_TAG:
			strcpy(sText, "Error in Module FEDM_XML: File contains no element 'OBID'");
			break;
		case FEDM_XML_ERROR_NO_CHILD_TAG:
			strcpy(sText, "Error in Module FEDM_XML: No sub-element found");
			break;
		case FEDM_XML_ERROR_TAG_NOT_FOUND:
			strcpy(sText, "Error in Module FEDM_XML: Element not in the document");
			break;
		case FEDM_XML_ERROR_DOC_NOT_WELL_FORMED:
			strcpy(sText, "Error in Module FEDM_XML: XML document not well-formed");
			break;
		case FEDM_XML_ERROR_NO_TAG_VALUE:
			strcpy(sText, "Error in Module FEDM_XML: No content of element found");
			break;
		case FEDM_XML_ERROR_NO_TAG_ATTRIBUTE:
			strcpy(sText, "Error in Module FEDM_XML: No attribute found");
			break;
		case FEDM_XML_ERROR_DOC_FILE_VERSION:
			strcpy(sText, "Error in Module FEDM_XML: Unvalid document version");
			break;
		case FEDM_XML_ERROR_DOC_FILE_FAMILY:
			strcpy(sText, "Error in Module FEDM_XML: The Document is for another reader family");
			break;
		case FEDM_XML_ERROR_DOC_FILE_TYPE:
			strcpy(sText, "Error in Module FEDM_XML: The document is not a valid OBID file type");
			break;
		case FEDM_XML_ERROR_WRONG_CONTROLLER_TYPE:
			strcpy(sText, "Error in Module FEDM_XML: Wrong controller type");
			break;
		case FEDM_XML_ERROR_WRONG_MEM_BANK_TYPE:
			strcpy(sText, "Error in Module FEDM_XML: Wrong memory bank type");
			break;
		default:
			sprintf(sText, "Error in Module FEDM: unknown error code (%d | 0x%08X)", iErrCode, iErrCode);
			return FEDM_ERROR_UNKNOWN_ERROR_CODE;
		}
	}
	else
	{
		return FEDM_ERROR_UNKNOWN_LANGUAGE;
	}

	return FEDM_OK;
}




//#####################################################################################
// basic access functions with ID
//#####################################################################################

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	get an boolean data from memory

  Parameters	:	const char* ID		-	data identifier
					bool* Data			-	pointer to data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(const char* ID, bool* Data, FEDM_BYTE_ARRAY& Array, int BlockSize)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int i;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index, BitNr;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( ByteCnt != 1 )
		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( BitCnt != 1 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;
	
	BitNr = 1;
	for( i=0; i<BitPos;i++ ) BitNr *= 2;

	if( Array[Index] & BitNr )	*Data = true;	// set bit
	else						*Data = false;	// delete bit

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	get an BOOL data from memory

  Parameters	:	const char* ID		-	data identifier
					BOOL* Data			-	pointer to data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_Base::GetData(const char* ID, BOOL* Data, FEDM_BYTE_ARRAY& Array, int BlockSize)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index, BitNr;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( ByteCnt != 1 )
		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( BitCnt != 1 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;
	
	BitNr = 1;
	for( int i=0; i<BitPos;i++ ) BitNr *= 2;

	if( Array[Index] & BitNr )	*Data = TRUE;	// set bit
	else						*Data = FALSE;	// delete bit

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	get an unsigned char data from memory
					access of bits is possible

  Parameters	:	const char* ID		-	data identifier
					unsigned char* Data			-	pointer to data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(const char* ID, unsigned char* Data, FEDM_BYTE_ARRAY& Array, int BlockSize)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( ByteCnt != 1 )
		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( BitCnt + BitPos > 8 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	*Data = Array[Index];

	// bit shifting
	if( BitCnt > 0 )
	{
		*Data <<= (8 - BitPos - BitCnt);
		*Data >>= (8 - BitPos - BitCnt);
		*Data >>= BitPos;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	get an array of unsigned char data from memory
					- access of bits is not possible
					- the requested number of byte can be 1..ByteCnt,
					  where ByteCnt is decoded from ID

  Parameters	:	const char* ID		-	data identifier
					unsigned char* Data	-	pointer to data array
					int Cnt				-	number of requested byte
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(const char* ID, unsigned char* Data, int Cnt, FEDM_BYTE_ARRAY& Array, int BlockSize)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int i;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	int adr = 0;

	if(Cnt == 0)
		return FEDM_OK;	// nothing to do
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;

	
	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	// security check
	if( (Cnt < 0) || (BitCnt > 0) )
		return FEDM_ERROR_PARAMETER;

	// security check
	if( Cnt > ByteCnt )
		return FEDM_ERROR_PARAMETER;

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( Cnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// copy and shift data into data array
	adr = Index;
	for(i=0; i<Cnt; i++)
	{
		*(Data+i) = Array[adr];
		if( (MemID == FEDM_RFC_EEDATA_MEM ||
			 MemID == FEDM_RFC_RAMDATA_MEM ||
			 MemID == FEDM_ACC_EEDATA_MEM ||
			 MemID == FEDM_ACC_RAMDATA_MEM ) &&
			(adr % BlockSize) == BlockSize-3 )
			adr += 2;

		adr++;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	get an unsigned int data from memory
					access of bits is possible

  Parameters	:	const char* ID		-	data identifier
					unsigned int* Data	-	pointer to data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(const char* ID, unsigned int* Data, FEDM_BYTE_ARRAY& Array, int BlockSize)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int i;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( ByteCnt > 4 )
		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( (BitPos + BitCnt)> 32 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	*Data = 0;

	for( i=0; i< ByteCnt; i++ )
	{
		// change Motorola format to Intel format
		*Data += ( Array[Index+ByteCnt-1-i] << (8*i) );
	}

	// bit shifting
	if( BitCnt > 0 )
	{
		*Data <<= (32 - BitPos - BitCnt);
		*Data >>= (32 - BitPos - BitCnt);
		*Data >>= BitPos;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	get an __int64 data from memory
					access of bits is possible

  Parameters	:	const char* ID		-	data identifier

					__int64* Data		-	pointer to data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(const char* ID, __int64* Data, FEDM_BYTE_ARRAY& Array, int BlockSize)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int i;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	int adr = 0;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( ByteCnt > 8 )
		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( (BitPos + BitCnt) > 64 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	*Data = 0;

	adr = Index+ByteCnt-1;
	for( i=0; i< ByteCnt; i++ )
	{
		// change Motorola format to Intel format
		*Data += ( (__int64)Array[adr] << (8*i) );
		if( (MemID == FEDM_RFC_EEDATA_MEM ||
			 MemID == FEDM_RFC_RAMDATA_MEM ||
			 MemID == FEDM_ACC_EEDATA_MEM ||
			 MemID == FEDM_ACC_RAMDATA_MEM ) &&
			(adr % BlockSize) == 0 )
			adr -= 2;

		adr--;
	}

	// bit shifting
	if( BitCnt > 0 )
	{
		// error compensation !?
		// ein Fehler tritt hier auf, wenn nach dem Linksschieben im MSB eine 1 steht
		// dann werden beim Rechtsschieben nur noch 1er nachgeschoben
		// >> also nur max 63 x Linksschieben und das MSB manuell auf 0 setzen
		// >> mgliche Erklärung: das Sign-Flag wird als Bitquelle benutzt
		if( (64 - BitPos - BitCnt) > 0 )
		{
			*Data <<= (64 - BitPos - BitCnt - 1);
#ifdef __GNUC__
			*Data &= 0x7FFFFFFFFFFFFFFFLL;
#else
			*Data &= 0x7FFFFFFFFFFFFFFF;
#endif
			*Data >>= (64 - BitPos - BitCnt - 1);
		}
		*Data >>= BitPos;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 29.07.2008 / M. Hultsch

  Function		:	get an CString data from memory
					access of bits is possible
					every byte is changed into two ascii char
					example: 0x23 -> "23"

  Parameters	:	const char* ID		-	data identifier
					CString& Data		-	reference to data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_Base::GetData(const char* ID, CString& Data, FEDM_BYTE_ARRAY& Array, int BlockSize)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(&Data);
	FEDM_CHK3(&Array);

	int i;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	int adr = 0;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check (removed since V3.00.00)
/*	switch(MemID)
	{
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
		if( BlockSize != 16 )
			return FEDM_ERROR_BYTE_BOUNDARY;
		break;
	case FEDM_TMPDATA_MEM:
	case FEDM_ACC_EEDATA_MEM:
	case FEDM_ACC_RAMDATA_MEM:
		if( BlockSize != 32 )
			return FEDM_ERROR_BYTE_BOUNDARY;
		break;
	}*/

	// security check (removed since V3.00.00)
//	if( ByteCnt > BlockSize )
//		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( (BitPos + BitCnt) > BlockSize*8 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( BitPos > 7 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;
	
	char buffer[5];
	unsigned char ucTmp;
	unsigned char* ucData = new unsigned char[ByteCnt];
	memset( &ucData[0], '\0', ByteCnt);

	Data = "";

	if(BitCnt > 0)
	{
		for(i=0; i<ByteCnt-1; i++)
		{
			ucData[i] = (Array[Index+i]>>BitPos) | (Array[Index+i+1]<<(8-BitPos));
		}
		ucData[ByteCnt-1] = Array[Index+ByteCnt-1]>>BitPos;

		// unmask unused bits in MSB
		ucData[0] <<= (8-(BitCnt%8));		
		ucData[0] >>= (8-(BitCnt%8));		
	}
	else
	{
		adr = Index;
		for(i=0; i<ByteCnt; i++)
		{
			ucData[i] = Array[adr];
			if( (MemID == FEDM_RFC_EEDATA_MEM ||
				 MemID == FEDM_RFC_RAMDATA_MEM ||
				 MemID == FEDM_ACC_EEDATA_MEM ||
				 MemID == FEDM_ACC_RAMDATA_MEM ) &&
				(adr % BlockSize) == BlockSize-3 )
				adr += 2;

			adr++;
		}
	}


	// hex -> ascii
	for( i=0; i< ByteCnt; i++ )
	{
		ucTmp = ucData[i];

		buffer[0] = '\0';
		sprintf( buffer, "%02X", ucTmp );
		Data += buffer;
	}

	delete [] ucData;

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 29.07.2008 / M. Hultsch

  Function		:	get an STL-string data from memory
					access of bits is possible
					every byte is changed into two ascii char
					example: 0x23 -> "23"

  Parameters	:	const char* ID		-	data identifier
					string& Data		-	reference to data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(const char* ID, string& Data, FEDM_BYTE_ARRAY& Array, int BlockSize)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(&Data);
	FEDM_CHK3(&Array);

	int i;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	int adr = 0;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check (removed since V3.00.00)
/*	switch(MemID)
	{
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
		if( BlockSize > 16 )
			return FEDM_ERROR_BYTE_BOUNDARY;
		break;
	case FEDM_TMPDATA_MEM:
		if( BlockSize > 32 )
			return FEDM_ERROR_BYTE_BOUNDARY;
		break;
	}*/

	// security check (removed since V3.00.00)
//	if( ByteCnt > BlockSize )
//		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( (BitPos + BitCnt) > BlockSize*8 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( BitPos > 7 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;
	
	char buffer[5];
	unsigned char ucTmp;
	unsigned char* ucData = new unsigned char[ByteCnt];
	memset( &ucData[0], '\0', ByteCnt);

	Data = "";

	if(BitCnt > 0)
	{
		for(i=0; i<ByteCnt-1; i++)
		{
			ucData[i] = (Array[Index+i]>>BitPos) | (Array[Index+i+1]<<(8-BitPos));
		}
		ucData[ByteCnt-1] = Array[Index+ByteCnt-1]>>BitPos;

		// unmask unused bits in MSB
		ucData[0] <<= (8-(BitCnt%8));		
		ucData[0] >>= (8-(BitCnt%8));		
	}
	else
	{
		adr = Index;
		for(i=0; i<ByteCnt; i++)
		{
			ucData[i] = Array[adr];
			if( (MemID == FEDM_RFC_EEDATA_MEM ||
				 MemID == FEDM_RFC_RAMDATA_MEM ||
				 MemID == FEDM_ACC_EEDATA_MEM ||
				 MemID == FEDM_ACC_RAMDATA_MEM ) &&
				(adr % BlockSize) == BlockSize-3 )
				adr += 2;

			adr++;
		}
	}

	// hex -> ascii
	for( i=0; i< ByteCnt; i++ )
	{
		ucTmp = ucData[i];

		buffer[0] = '\0';
		sprintf( buffer, "%02X", ucTmp );
		Data += buffer;
	}

	delete [] ucData;

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 29.07.2008 / M. Hultsch

  Function		:	get an strinf of char data from memory
					- access of bits is possible
					- the requested number of byte can be 1..ByteCnt,
					  where ByteCnt is decoded from ID
					- every byte is changed into two ascii char
					- example: 0x23 -> "23"

  Parameters	:	const char* ID		-	data identifier
					char* Data			-	pointer to data
					int DataLen			-	number of requested byte
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(const char* ID, char* Data, int DataLen, FEDM_BYTE_ARRAY& Array, int BlockSize)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int i, Len;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	int adr = 0;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( DataLen%2 )
		return FEDM_ERROR_ODD_STRING_LENGTH;

	// security check
	if( DataLen > 2*ByteCnt)
		return FEDM_ERROR_BUFFER_LENGTH;

	// security check (removed since V3.00.00)
//	if( ByteCnt > BlockSize )
//		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( (BitPos + BitCnt) > BlockSize*8 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( BitPos > 7 )
		return FEDM_ERROR_BIT_BOUNDARY;

	Len = (DataLen>>1); // fast divide by 2
	
	// security check
	if( Len > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;
	
	char buffer[5];
	unsigned char ucTmp;
	unsigned char* ucData = new unsigned char[ByteCnt];
	memset( &ucData[0], '\0', ByteCnt);

	memset( &Data[0], '\0', DataLen);

	if(BitCnt > 0)
	{
		for(i=0; i<Len-1; i++)
		{
			ucData[i] = (Array[Index+i]>>BitPos) | (Array[Index+i+1]<<(8-BitPos));
		}
		ucData[Len-1] = Array[Index+Len-1]>>BitPos;

		// unmask unused bits in MSB
		ucData[0] <<= (8-(BitCnt%8));		
		ucData[0] >>= (8-(BitCnt%8));		
	}
	else
	{
		adr = Index;
		for(i=0; i<Len; i++)
		{
			ucData[i] = Array[adr];
			if( (MemID == FEDM_RFC_EEDATA_MEM ||
				 MemID == FEDM_RFC_RAMDATA_MEM ||
				 MemID == FEDM_ACC_EEDATA_MEM ||
				 MemID == FEDM_ACC_RAMDATA_MEM ) &&
				(adr % BlockSize) == BlockSize-3 )
				adr += 2;

			adr++;
		}
	}

	// hex -> ascii
	for(i=0; i<Len; i++)
	{
		ucTmp = ucData[i];

		buffer[0] = '\0';
		sprintf( buffer, "%02X", ucTmp );
		Data[2*i]	= buffer[0];
		Data[2*i+1] = buffer[1];
	}

	delete [] ucData;

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	set an boolean data in memory

  Parameters	:	const char* ID		-	data identifier
					bool Data			-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::SetData( const char* ID, bool Data, FEDM_BYTE_ARRAY& Array, int BlockSize )
{
	FEDM_CHK3(ID);
	FEDM_CHK3(&Array);


	int  i;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int  Index, BitNr;
	bool OldData = false;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( ByteCnt != 1 )
		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( BitCnt != 1 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// get old parameter
	GetData( ID, &OldData, Array, BlockSize );

	BitNr = 1;
	for( i=0; i<BitPos;i++ ) BitNr *= 2;

	if( Data )	Array[Index] |= BitNr;	// set bit in Data
	else		Array[Index] &= ~BitNr;	// delete bit in Data

	if( OldData != Data )
		return FEDM_MODIFIED;	// Data has modified memory

	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	set an BOOL data in memory

  Parameters	:	const char* ID		-	data identifier
					BOOL Data			-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_Base::SetData( const char* ID, BOOL Data, FEDM_BYTE_ARRAY& Array, int BlockSize )
{
	return SetData( ID, Data ? true : false, Array, BlockSize );
}
#endif

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	set an unsigned char data in memory
					access of bits is possible

  Parameters	:	const char* ID		-	data identifier
					unsigned char Data	-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::SetData( const char* ID, unsigned char Data, FEDM_BYTE_ARRAY& Array, int BlockSize )
{
	FEDM_CHK3(ID);
	FEDM_CHK3(&Array);

	unsigned char ucMask = 0;
	int i, ii;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index, BitNr;
	unsigned char OldData = 0x00;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;

	
	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( BitPos + BitCnt > 8 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// get old parameter
	GetData( ID, &OldData, Array, BlockSize );
	
	// set bits in memory
	if( BitCnt > 0 )
	{
		BitNr = 1;
		for(i=0; i<BitPos;i++) BitNr *= 2;

		// delete old bits
		for (ii = 0; ii < BitCnt; ii++)
		{
			ucMask |= BitNr;
			Array[Index] &= ~BitNr;
			BitNr *= 2;
		}

		Data <<= BitPos;
		Array[Index] |= (Data & ucMask);
		OldData <<= BitPos;		// shift OldData, otherwise we get an verification error
	}
	// set bytes in memory
	else
	{
		ucMask = 0xFF;
		Array[Index] = Data;
	}

	if( OldData != (Data & ucMask) )
		return FEDM_MODIFIED;	// Data has modified memory

	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	set an array of unsigned char data in memory
					- set of bits is not possible
					- the number of byte in Cnt can be 1..ByteCnt,
					  where ByteCnt is decoded from ID

  Parameters	:	const char* ID		-	data identifier
					unsigned char* Data	-	pointer to data array
					int Cnt				-	number of byte in Data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::SetData( const char* ID, unsigned char* Data, int Cnt, FEDM_BYTE_ARRAY& Array, int BlockSize )
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int i;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	int adr = 0;
	unsigned char* OldData;

	if( Cnt == 0 ) // nothing to do
		return FEDM_OK;

	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;

	// security check
	if( Cnt < 0 )
		return FEDM_ERROR_PARAMETER;

	
	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	// security check
	if( BitCnt > 0 )
		return FEDM_ERROR_PARAMETER;

	// security check
	if(Cnt > ByteCnt)
		return FEDM_ERROR_PARAMETER;

	Index = BlockAdr * BlockSize + BytePos;
	
	// security check
	if( Cnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	OldData = new unsigned char[Cnt];
	if(OldData == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	// get old parameter
	GetData( ID, OldData, Cnt, Array, BlockSize );

	// set bytes
	adr = Index;
	for( i=0; i<Cnt; i++)
	{
		Array[adr] = *(Data+i);
		if( (MemID == FEDM_RFC_EEDATA_MEM ||
			 MemID == FEDM_RFC_RAMDATA_MEM ||
			 MemID == FEDM_ACC_EEDATA_MEM ||
			 MemID == FEDM_ACC_RAMDATA_MEM ) &&
			(adr % BlockSize) == BlockSize-3 )
			adr += 2;

		adr++;
	}

	if( memcmp(OldData, Data, Cnt) != 0 )
	{
		delete[] OldData;
		return FEDM_MODIFIED;	// Data has modified memory
	}

	delete[] OldData;
	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	set an unsigned int data in memory

  Parameters	:	const char* ID		-	data identifier
					unsigned int Data			-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::SetData( const char* ID, unsigned int Data, FEDM_BYTE_ARRAY& Array, int BlockSize )
{
	FEDM_CHK3(ID);
	FEDM_CHK3(&Array);

	unsigned char ucMask[4];
	int i;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index, BitNr;
	unsigned int uiMask = 0;
	unsigned int OldData = 0;

	memset(ucMask, 0, 4);
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( ByteCnt > 4 )
		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( BitPos + BitCnt > 32 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( BitCnt > ByteCnt*8 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// get old parameter
	GetData( ID, &OldData, Array, BlockSize );

	// set bits in memory
	if( BitCnt > 0 )
	{
		BitNr = 1;
		BitNr <<= BitPos;

		// build mask
		for ( i = 0; i < BitCnt; i++ )
		{
			uiMask |= BitNr;
			BitNr <<= 1;
		}

		ucMask[3] = (unsigned char) (uiMask & 0x000000FF);
		ucMask[2] = (unsigned char)((uiMask & 0x0000FF00) >>  8);
		ucMask[1] = (unsigned char)((uiMask & 0x00FF0000) >> 16);
		ucMask[0] = (unsigned char)((uiMask & 0xFF000000) >> 24);

		// delete data in array
		for ( i = 0; i < ByteCnt; i++ )
		{
			Array[Index+ByteCnt-1-i] &= ~ucMask[3-i];
		}

		Data <<= BitPos;
		uiMask >>= BitPos;
		Index = BlockAdr * BlockSize + BytePos;
		for( i=0; i< ByteCnt; i++ )
		{
			// set data in Motorola format
			Array[Index+ByteCnt-1-i] |= (*( ((unsigned char*)&Data) + i ) & ucMask[3-i]);
		}
		OldData <<= BitPos; // shift OldData, otherwise we get an verification error
	}
	// set bytes in memory
	else
	{
		for( i=0; i< ByteCnt; i++ )
		{
			// set data in Motorola format
			Array[Index+ByteCnt-1-i] = *( ((unsigned char*)&Data) + i );
		}

		uiMask = 0xFFFFFFFF;
	}

	if( OldData != (Data & uiMask) )
		return FEDM_MODIFIED;	// Data has modified memory
	
	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	set an __int64 data in memory
					access of bits is possible

  Parameters	:	const char* ID		-	data identifier
					__int64 Data		-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::SetData( const char* ID, __int64 Data, FEDM_BYTE_ARRAY& Array, int BlockSize )
{
	FEDM_CHK3(ID);
	FEDM_CHK3(&Array);

	unsigned char ucMask[8];
	int i;
	int idx;
	int adr = 0;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index, BitNr;
	__int64 i64Mask = 0;
	__int64 OldData = 0;

	memset(ucMask, 0, 8);
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;


	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( ByteCnt > 8 )
		return FEDM_ERROR_BYTE_BOUNDARY;
	
	// security check
	if( BitPos + BitCnt > 64 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( BitCnt > ByteCnt*8 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( ByteCnt > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// get old parameter
	GetData( ID, &OldData, Array, BlockSize );

	// set bits in memory
	if( BitCnt > 0 )
	{
		BitNr = 1;
		for( i=0; i<BitPos;i++ ) BitNr *= 2;

		// delete or set bits, build mask
		idx = ByteCnt - 1;
		for ( i = 0; i < BitCnt; i++ )
		{
			ucMask[idx] |= BitNr;
			Array[Index+ByteCnt-1] &= ~BitNr;
			BitNr *= 2;
			// test of byte boundary
			if( (i > 0) && (!((i+BitPos+1)%8)) )
			{
				BitNr = 1;
				Index--;
				idx--;
			}
		}

		Data <<= BitPos;
		Index = BlockAdr * BlockSize + BytePos;
		adr = Index+ByteCnt-1;
		for( i=0; i< ByteCnt; i++ )
		{
			// set data in Motorola format
			Array[adr] |= *( ((unsigned char*)&Data) + i );
			if( (MemID == FEDM_RFC_EEDATA_MEM ||
				 MemID == FEDM_RFC_RAMDATA_MEM ||
				 MemID == FEDM_ACC_EEDATA_MEM ||
				 MemID == FEDM_ACC_RAMDATA_MEM ) &&
				(adr % BlockSize) == 0 )
				adr -= 2;

			adr--;
		}
		OldData <<= BitPos; // shift OldData, otherwise we get an verification error

		i64Mask |=  (__int64)ucMask[0];
		i64Mask |= ((__int64)ucMask[1] <<  8);
		i64Mask |= ((__int64)ucMask[2] << 16);
		i64Mask |= ((__int64)ucMask[3] << 24);
		i64Mask |= ((__int64)ucMask[4] << 32);
		i64Mask |= ((__int64)ucMask[5] << 40);
		i64Mask |= ((__int64)ucMask[6] << 48);
		i64Mask |= ((__int64)ucMask[7] << 56);
	}
	// set bytes in memory
	else
	{
		adr = Index+ByteCnt-1;
		for( i=0; i< ByteCnt; i++ )
		{
			// set data in Motorola format
			Array[adr] = *( ((unsigned char*)&Data) + i );
			if( (MemID == FEDM_RFC_EEDATA_MEM ||
				 MemID == FEDM_RFC_RAMDATA_MEM ||
				 MemID == FEDM_ACC_EEDATA_MEM ||
				 MemID == FEDM_ACC_RAMDATA_MEM ) &&
				(adr % BlockSize) == 0 )
				adr -= 2;

			adr--;
		}

#ifdef _FEDM_WINDOWS
	#if _MSC_VER <= 1200
		i64Mask = 0xFFFFFFFFFFFFFFFF;
	#else
		#ifdef _FEDM_EVC4
		i64Mask = 0xFFFFFFFFFFFFFFFF;
		#else
		i64Mask = 0xFFFFFFFFFFFFFFFFLL;
		#endif
	#endif
#endif
#ifdef _FEDM_LINUX
		i64Mask = 0xFFFFFFFFFFFFFFFFLL;
#endif
	}

	if( OldData != (Data & i64Mask) )
		return FEDM_MODIFIED;	// Data has modified memory
		
	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	set an CString data in memory
					- access of bits is not possible
					- the number of 'byte' in Data can be 1..ByteCnt,
					  where ByteCnt is decoded from ID
					- every two chars are changed into one byte
					- example: "23" -> 0x23

  Parameters	:	const char* ID		-	data identifier
					CString Data		-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_Base::SetData( const char* ID, CString Data, FEDM_BYTE_ARRAY& Array, int BlockSize )
{
	FEDM_CHK3(ID);
	FEDM_CHK3(&Array);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	int adr = 0;
	int iLen = 0;
	CString OldData = "";
	char	cTmp[5];
	unsigned int uiTmp;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;
	

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
	iLen = Data.GetLength();
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
	iLen = Data.Length();
#endif

	if(iLen==0 || iLen>2*ByteCnt)
		return FEDM_ERROR_STRING_LENGTH;

	// security check
	if( iLen%2 )
		return FEDM_ERROR_ODD_STRING_LENGTH;
	
	// security check
	if( BitCnt > 0 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( iLen > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// get old parameter
	GetData( ID, OldData, Array, BlockSize );
	
	int i;
	adr = Index;
#ifdef __BORLANDC__	// Borland C++ Builder: AnsiStrings index starts with 1
	for( i=1; i<(iLen+1); i+=2 )
#else
	for( i=0; i<iLen; i+=2 )
#endif
	{
		cTmp[0] = Data[i];
		cTmp[1] = Data[i+1];
		cTmp[2] = '\0';
		sscanf( (const char*)cTmp, "%02x", &uiTmp );
		Array[adr] = (unsigned char)uiTmp;
		if( (MemID == FEDM_RFC_EEDATA_MEM ||
			 MemID == FEDM_RFC_RAMDATA_MEM ||
			 MemID == FEDM_ACC_EEDATA_MEM ||
			 MemID == FEDM_ACC_RAMDATA_MEM ) &&
			(adr % BlockSize) == BlockSize-3 )
			adr += 2;

		adr++;
	}
	
	if( OldData != Data )
		return FEDM_MODIFIED;	// Data has modified memory

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	set an STL-string data in memory
					- access of bits is not possible
					- the number of 'byte' in Data can be 1..ByteCnt,
					  where ByteCnt is decoded from ID
					- every two chars are changed into one byte
					- example: "23" -> 0x23

  Parameters	:	const char* ID		-	data identifier
					string Data			-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::SetData( const char* ID, string Data, FEDM_BYTE_ARRAY& Array, int BlockSize )
{
	FEDM_CHK3(ID);
	FEDM_CHK3(&Array);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	int adr = 0;
	string OldData = "";
	char	cTmp[5];
	unsigned int uiTmp;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;
	

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( (int)Data.length()==0 || (int)Data.length()>2*ByteCnt )
		return FEDM_ERROR_STRING_LENGTH;


	// security check
	if( Data.length()%2 )
		return FEDM_ERROR_ODD_STRING_LENGTH;
	
	// security check
	if( BitCnt > 0 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( (int)Data.length() > ((int)Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// get old parameter
	GetData( ID, OldData, Array, BlockSize );
	
	int i;
	adr = Index;
	for( i=0; i<(int)Data.length(); i+=2 )
	{
		cTmp[0] = Data[i];
		cTmp[1] = Data[i+1];
		cTmp[2] = '\0';
		sscanf( (const char*)cTmp, "%02x", &uiTmp );
		Array[adr] = (unsigned char)uiTmp;
		if( (MemID == FEDM_RFC_EEDATA_MEM ||
			 MemID == FEDM_RFC_RAMDATA_MEM ||
			 MemID == FEDM_ACC_EEDATA_MEM ||
			 MemID == FEDM_ACC_RAMDATA_MEM ) &&
			(adr % BlockSize) == BlockSize-3 )
			adr += 2;

		adr++;
	}
	
	if( OldData != Data )
		return FEDM_MODIFIED;	// Data has modified memory

	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	set an array of char data in memory
					- access of bits is not possible
					- the number of 'byte' in Data can be 1..ByteCnt,
					  where ByteCnt is decoded from ID
					- every two chars are changed into one byte
					- example: "23" -> 0x23

  Parameters	:	const char* ID		-	data identifier
					char* Data			-	pointer to data
					int DataLen			-	number of chars in Data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::SetData( const char* ID, char* Data, int DataLen, FEDM_BYTE_ARRAY& Array, int BlockSize )
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int Index;
	int adr = 0;
	char*	OldData;
	char	cTmp[5];
	unsigned int uiTmp;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;
	

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	Index = BlockAdr * BlockSize + BytePos;

	// security check
	if( DataLen > 2*ByteCnt)
		return FEDM_ERROR_STRING_LENGTH;

	// security check
	if( DataLen%2 )
		return FEDM_ERROR_ODD_STRING_LENGTH;
	
	// security check
	if( BitCnt > 0 )
		return FEDM_ERROR_BIT_BOUNDARY;
	
	// security check
	if( DataLen > (int)(Array.capacity() - Index) )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	OldData = new char[DataLen+1];
	if(OldData == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	// get old parameter
	GetData( ID, OldData, DataLen, Array, BlockSize );
	OldData[DataLen] = '\0';
	
	int i;
	adr = Index;
	for( i=0; i<DataLen; i+=2 )
	{
		cTmp[0] = Data[i];
		cTmp[1] = Data[i+1];
		cTmp[2] = '\0';
		sscanf( (const char*)cTmp, "%02x", &uiTmp );
		Array[adr] = (unsigned char)uiTmp;
		if( (MemID == FEDM_RFC_EEDATA_MEM ||
			 MemID == FEDM_RFC_RAMDATA_MEM ||
			 MemID == FEDM_ACC_EEDATA_MEM ||
			 MemID == FEDM_ACC_RAMDATA_MEM ) &&
			(adr % BlockSize) == BlockSize-3 )
			adr += 2;

		adr++;
	}
	
	if( strcmp(OldData, Data) != 0 )
	{
		delete[] OldData;
		return FEDM_MODIFIED;	// Data has modified memory
	}

	delete[] OldData;
	return FEDM_OK;
}






//#####################################################################################
// basic access functions with direct memory address
//#####################################################################################

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	01.05.00 / 19.07.2002 / M. Hultsch

  Function		:	get an unsigned char data from memory
					access of bits is not possible

  Parameters	:	int Adr				-	memory address
					unsigned char* Data			-	pointer to data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(int Adr, unsigned char* Data, FEDM_BYTE_ARRAY& Array)
{
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	// security check
	if( Adr<0 || Adr>(int)Array.capacity() )
		return FEDM_ERROR_ARRAY_BOUNDARY;	

	*Data = Array[Adr];

	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	01.05.00 / 19.07.2002 / M. Hultsch

  Function		:	get an array of unsigned char data from memory
					access of bits is not possible

  Parameters	:	int Adr				-	memory address
					unsigned char* Data			-	pointer to data
					int Cnt				-	size of array
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(int Adr, unsigned char* Data, int Cnt, FEDM_BYTE_ARRAY& Array)
{
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int i;

	// security check
	if( Cnt <= 0 )
		return FEDM_ERROR_PARAMETER;
	
	// security check
	if( Adr<0 || (Adr+Cnt)>(int)Array.capacity() )
		return FEDM_ERROR_ARRAY_BOUNDARY;	

	for( i=0; i<Cnt; i++)
		*(Data+i) = Array[Adr+i];


	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	01.05.00 / 19.07.2002 / M. Hultsch

  Function		:	get an unsigned int data from memory
					access of bits is not possible

  Parameters	:	int Adr				-	memory address
					unsigned int* Data			-	pointer to data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(int Adr, unsigned int* Data, FEDM_BYTE_ARRAY& Array)
{
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int i;
	*Data = 0;

	// security check
	if( Adr<0 || (Adr+4)>(int)Array.capacity() )
		return FEDM_ERROR_ARRAY_BOUNDARY;	

	for( i=0; i<3; i++ )
		// change Motorola format into Intel format
		*Data += ( Array[Adr+3-i] << (8*i) );

	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	01.05.00 / 19.07.2002 / M. Hultsch

  Function		:	get an __int64 data from memory
					access of bits is not possible

  Parameters	:	int Adr				-	memory address
					__int64* Data		-	pointer to data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::GetData(int Adr, __int64* Data, FEDM_BYTE_ARRAY& Array)
{
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int i;
	*Data = 0;

	// security check
	if( Adr<0 || (Adr+8)>(int)Array.capacity() )
		return FEDM_ERROR_ARRAY_BOUNDARY;	

	for( i=0; i<8; i++ )
		// change Motorola format into Intel format
		*Data += ( (__int64)Array[Adr+7-i] << (8*i) );

	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 29.07.2008 / M. Hultsch

  Function		:	get an CString data from memory
					access of bits is not possible
					every byte is changed into two ascii char
					example: 0x23 -> "23"

  Parameters	:	int Adr				-	memory address
					CString& Data		-	reference to data
					int Cnt				-	count of bytes
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_Base::GetData(int Adr, CString& Data, int Cnt, FEDM_BYTE_ARRAY& Array)
{
	FEDM_CHK3(&Data);
	FEDM_CHK3(&Array);

	int i;
	char buffer[5];
	unsigned char ucTmp;

	// security check
	if( Cnt <= 0 )
		return FEDM_ERROR_PARAMETER;
	
	// security check
	if( Adr<0 || (Adr+Cnt)>(int)Array.capacity() )
		return FEDM_ERROR_ARRAY_BOUNDARY;	

	unsigned char* ucData = new unsigned char[Cnt];
	memset( &ucData[0], '\0', Cnt);
	Data = "";

	for(i=0; i<Cnt; i++)
		ucData[i] = Array[Adr+i];

	// hex -> ascii
	for(i=0; i<Cnt; i++)
	{
		ucTmp = ucData[i];

		buffer[0] = '\0';
		sprintf( buffer, "%02X", ucTmp );
		Data += buffer;
	}

	delete [] ucData;

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	01.05.00 / 19.07.2002 / M. Hultsch

  Function		:	set an unsigned char data in memory
					access of bits is not possible

  Parameters	:	int Adr				-	memory address
					unsigned char Data			-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::SetData( int Adr, unsigned char Data, FEDM_BYTE_ARRAY& Array )
{
	FEDM_CHK3(&Array);

	unsigned char OldData = 0x00;

	// security check
	if( Adr<0 || Adr>(int)Array.capacity() )
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// get old parameter
	GetData( Adr, &OldData, Array );
	
	Array[Adr] = Data;

	if( OldData != Data )
		return FEDM_MODIFIED;	// Data has modified memory

	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	01.05.00 / 19.07.2002 / M. Hultsch

  Function		:	set an array of unsigned char data in memory
					access of bits is not possible

  Parameters	:	int Adr				-	memory address
					unsigned char Data			-	data
					int Cnt				-	size of array
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::SetData( int Adr, unsigned char* Data, int Cnt, FEDM_BYTE_ARRAY& Array )
{
	FEDM_CHK3(Data);
	FEDM_CHK3(&Array);

	int i;

	// security check
	if( Cnt <= 0 )
		return FEDM_ERROR_PARAMETER;
	
	// security check
	if( Adr<0 || (Adr+Cnt)>(int)Array.capacity() )
		return FEDM_ERROR_ARRAY_BOUNDARY;	

	unsigned char* OldData;

	OldData = new unsigned char[Cnt];
	if(OldData == NULL)
		return FEDM_ERROR_NO_MORE_MEM;


	// get old parameter
	GetData( Adr, OldData, Cnt, Array );
	
	for(i=0; i<Cnt; i++)
		Array[Adr+i] = *(Data+i);

	if( *OldData != *Data )
	{
		delete[] OldData;
		return FEDM_MODIFIED;	// Data has modified memory
	}

	delete[] OldData;
	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	01.05.00 / 19.07.2002 / M. Hultsch

  Function		:	set an unsigned int data in memory
					access of bits is not possible

  Parameters	:	int Adr				-	memory address
					unsigned int Data			-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/

int FEDM_Base::SetData( int Adr, unsigned int Data, FEDM_BYTE_ARRAY& Array )
{
	FEDM_CHK3(&Array);

	int i;
	unsigned int OldData = 0;

	// security check
	if( Adr<0 || (Adr+4)>(int)Array.capacity() )
		return FEDM_ERROR_ARRAY_BOUNDARY;	

	// get old parameter
	GetData( Adr, &OldData, Array );
	
	// Byteweise schreiben
	for( i=0; i<4; i++ )
	{
		// change Intel format into Motorola format
		Array[Adr+3-i] = *( ((unsigned char*)&Data) + i );
	}

	if( OldData != Data )
		return FEDM_MODIFIED;	// Data has modified memory
	
	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	01.05.00 / 19.07.2002 / M. Hultsch

  Function		:	set an __int64 data in memory
					access of bits is not possible

  Parameters	:	int Adr				-	memory address
					__int64 Data		-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
int FEDM_Base::SetData( int Adr, __int64 Data, FEDM_BYTE_ARRAY& Array )
{
	FEDM_CHK3(&Array);

	int i;
	__int64 OldData = 0;

	// security check
	if( Adr<0 || (Adr+8)>(int)Array.capacity() )
		return FEDM_ERROR_ARRAY_BOUNDARY;	

	// get old parameter
	GetData( Adr, &OldData, Array );

	for( i=0; i<8; i++ )
	{
		// change Intel format into Motorola format
		Array[Adr+7-i] = *( ((unsigned char*)&Data) + i );
	}

	if( OldData != Data )
		return FEDM_MODIFIED;	// Data has modified memory
	
	return FEDM_OK;
}

/***************************************************************************
  Begin			:	18.05.2000 / M. Hultsch
  Version       :	01.05.00 / 19.07.2002 / M. Hultsch

  Function		:	set an CString data in memory
					access of bits is not possible
					every two chars are changed into one byte
					example: "23" -> 0x23

  Parameters	:	int Adr				-	memory address
					CString Data		-	data
					FEDM_BYTE_ARRAY*	-	reference to the memory
					int BlockSize		-	size of a block (memory is block-organized)

  Return Value	:	int					-	error code

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_Base::SetData( int Adr, CString Data, FEDM_BYTE_ARRAY& Array )
{
	FEDM_CHK3(&Array);

	int		i;
	CString OldData = "";
	char	cTmp[5];
	unsigned int uiTmp;

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
	int iLen = Data.GetLength();
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
	int iLen = Data.Length();
#endif
#if (! (defined(_FEDM_MFC_SUPPORT) || defined(__BORLANDC__)) )
	int iLen = 0;
#endif

	// security check
	if( iLen%2 )
		return FEDM_ERROR_ODD_STRING_LENGTH;

	// security check
	if( iLen == 0 )
		return FEDM_ERROR_STRING_LENGTH;

	// security check
	if( Adr<0 || Adr+iLen>(int)Array.capacity() )
		return FEDM_ERROR_ARRAY_BOUNDARY;	

	// get old parameter
	GetData( Adr, OldData, iLen/2, Array );

	int j=0;
#ifdef __BORLANDC__	// Borland C++ Builder: AnsiStrings index starts with 1
	for( i=1; i<(iLen+1); i+=2 )
#else
	for( i=0; i<iLen; i+=2 )
#endif
	{
		cTmp[0] = Data[i];
		cTmp[1] = Data[i+1];
		cTmp[2] = '\0';
		sscanf( (const char*)cTmp, "%02x", &uiTmp );
		Array[Adr + j++] = (unsigned char)uiTmp;
	}

	if( OldData != Data)
		return FEDM_MODIFIED;	// Data has modified memory

	return FEDM_OK;
}
#endif


#ifdef _FEDM_COM_SUPPORT
void FEDM_Base::SetFecomLibName(char* pszLibName)
{
	m_pFecomLibName = pszLibName;
}
#endif

#ifdef _FEDM_USB_SUPPORT
void FEDM_Base::SetFeusbLibName(char* pszLibName)
{
	m_pFeusbLibName = pszLibName;
}
#endif

#ifdef _FEDM_TCP_SUPPORT
void FEDM_Base::SetFetcpLibName(char* pszLibName)
{
	m_pFetcpLibName = pszLibName;
}
#endif



/*void* FEDM_Base::LoadLib(const char* pszLibName)
{
	void* pLib = NULL;

#if defined(_FEDM_WINDOWS)
	pLib = (void*)LoadLibrary(pszLibName);
#elif defined(_WIN32_WCE)
	pLib = (void*)LoadLibrary(_T(pszLibName));
#elif defined(_FEDM_LINUX)
	pLib = dlopen(pszLibName, RTLD_NOW | RTLD_GLOBAL);
#endif

	return pLib;	
}


void FEDM_Base::CloseLib(void* pLib)
{
#if defined(_FEDM_WINDOWS)
	if(pLib != NULL)
		FreeLibrary((HINSTANCE)pLib);
#endif

#if defined(_FEDM_LINUX)
	if(pLib != NULL)
		dlclose(pLib);
#endif
}*/

/***************************************************************************
  Begin			:	29.07.2003 / M. Hultsch
  Version       :	03.00.12 / 13.08.2009 / M. Hultsch

  Function		:	dynamical load of fecom library

  Parameters	:	-

  Return Value	:	void* pLib	- pointer or handle to library

***************************************************************************/
#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
void* FEDM_Base::LoadFeCom()
{
	if(m_pFeComLib != NULL)
		return m_pFeComLib;

	void* pLib = NULL;

#ifdef _FEDM_WINDOWS
	GetOS();

	if(	m_pFecomLibName != NULL)
	{
	#ifdef _WIN32_WCE
		wchar_t wsName[256];
		swprintf(wsName, L"%S", m_pFecomLibName);
		pLib = (void*)LoadLibrary(wsName);
	#else
		pLib = (void*)LoadLibrary(m_pFecomLibName);
	#endif
	}
	else
	{
		if(m_iWinVer == FEDM_OS_WCE)
			pLib = (void*)LoadLibrary(_T("fecomce.dll"));
		else
			pLib = (void*)LoadLibrary(_T("fecom.dll"));
	}
#endif
#ifdef _FEDM_LINUX
#if defined(__APPLE__)
	pLib = dlopen("libfecom.dylib", RTLD_NOW | RTLD_GLOBAL);
#else
	pLib = dlopen("libfecom.so", RTLD_NOW | RTLD_GLOBAL);
#endif
#endif

	if(pLib != NULL)
		m_pFeComLib = pLib;

	return pLib;	
}
#endif // #if !defined(_FEDM_SUPPORT_SLINK)
#endif




/***************************************************************************
  Begin			:	25.04.2003 / M. Hultsch
  Version       :	03.00.12 / 13.08.2009 / M. Hultsch

  Function		:	dynamical load of feusb library

  Parameters	:	-

  Return Value	:	void* pLib	- pointer or handle to library

***************************************************************************/
#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
void* FEDM_Base::LoadFeUsb()
{
	if(m_pFeUsbLib != NULL)
		return m_pFeUsbLib;

	void* pLib = NULL;

#ifdef _FEDM_WINDOWS
	GetOS();
	
	if(	m_pFeusbLibName != NULL)
	{
	#ifdef _WIN32_WCE
		wchar_t wsName[256];
		swprintf(wsName, L"%s", m_pFeusbLibName);
		pLib = (void*)LoadLibrary(wsName);
	#else
		pLib = (void*)LoadLibrary(m_pFeusbLibName);
	#endif
	}
	else
	{
		if(m_iWinVer == FEDM_OS_WCE)
			pLib = (void*)LoadLibrary(_T("feusbce.dll"));
		else
			pLib = (void*)LoadLibrary(_T("feusb.dll"));
	}
#endif
#ifdef _FEDM_LINUX
#if defined(__APPLE__)
	pLib = dlopen("libfeusb.dylib", RTLD_NOW | RTLD_GLOBAL);
#else
	pLib = dlopen("libfeusb.so", RTLD_NOW | RTLD_GLOBAL);
#endif
#endif

	if(pLib != NULL)
		m_pFeUsbLib = pLib;

	return pLib;	
}
#endif // #if !defined(_FEDM_SUPPORT_SLINK)
#endif



/***************************************************************************
  Begin			:	22.05.2003 / M. Hultsch
  Version       :	03.00.12 / 13.08.2009 / M. Hultsch

  Function		:	dynamical load of fetcp library

  Parameters	:	-

  Return Value	:	void* pLib	- pointer or handle to library
***************************************************************************/
#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
void* FEDM_Base::LoadFeTcp()
{
	if(m_pFeTcpLib != NULL)
		return m_pFeTcpLib;

	void* pLib = NULL;

#ifdef _FEDM_WINDOWS
	GetOS();
	
	if(	m_pFetcpLibName != NULL)
	{
	#ifdef _WIN32_WCE
		wchar_t wsName[256];
		swprintf(wsName, L"%s", m_pFetcpLibName);
		pLib = (void*)LoadLibrary(wsName);
	#else
		pLib = (void*)LoadLibrary(m_pFetcpLibName);
	#endif
	}
	else
	{
		if(m_iWinVer == FEDM_OS_WCE)
		{
			pLib = (void*)LoadLibrary(_T("fetcpce.dll"));
		}
		else
		{
			pLib = (void*)LoadLibrary(_T("fetcp.dll"));
		}
	}
#endif
#ifdef _FEDM_LINUX
#if defined(__APPLE__)
	pLib = dlopen("libfetcp.dylib", RTLD_NOW | RTLD_GLOBAL);
#else
	pLib = dlopen("libfetcp.so", RTLD_NOW | RTLD_GLOBAL);
#endif
#endif

	if(pLib != NULL)
		m_pFeTcpLib = pLib;

	return pLib;	
}
#endif // #if !defined(_FEDM_SUPPORT_SLINK)
#endif


/***************************************************************************
  Begin			:	22.01.2008 / M. Hultsch
  Version       :	03.00.00 / 03.03.2008 / M. Hultsch

  Function		:	dynamical load of function pointer

  Parameters	:	void* pLib				- pointer or handle to library
					const char* szFctName	- name of the function

  Return Value	:	pointer to function or NULL
***************************************************************************/
#if !defined(_FEDM_SUPPORT_SLINK)
void* FEDM_Base::GetFunctionPtr(void* pLib, const char* szFctName)
{
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
}
#endif

/***************************************************************************
  Begin			:	29.07.2003 / M. Hultsch
  Version       :	02.04.04 / 22.08.2006 / M. Hultsch

  Function		:	get a pointer to a function from fecom library

  Parameters	:	constant with function-id (see fecom.h)

  Return Value	:	void* pFct	- pointer to a function or NULL

***************************************************************************/
#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
void* FEDM_Base::GetFeComFunction(unsigned int uiFct)
{
	void* pLib = NULL;

	if(uiFct >= 1000 && uiFct < 1100)
	{
		pLib = LoadFeCom();
		if(pLib == NULL)
			return NULL;
	}
	else
	{
		return NULL;
	}

	if(m_pFeComPtrList == NULL)
	{
		m_pFeComPtrList = (void*)new _FEDM_FECOM_PTR;
		if(m_pFeComPtrList == NULL)
			return NULL;
	}


	switch(uiFct)
	{
	// function pointers of fecom
	case FECOM_GET_DLL_VERSION:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetDLLVersion != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetDLLVersion;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetDLLVersion = (LPFN_FECOM_GET_DLL_VERSION)GetFunctionPtr(pLib, "FECOM_GetDLLVersion");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetDLLVersion != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetDLLVersion;
		}
		return NULL;
	
	case FECOM_GET_ERROR_TEXT:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetErrorText != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetErrorText;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetErrorText = (LPFN_FECOM_GET_ERROR_TEXT)GetFunctionPtr(pLib, "FECOM_GetErrorText");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetErrorText != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetErrorText;
		}
		return NULL;
	
	case FECOM_GET_LAST_ERROR:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetLastError != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetLastError;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetLastError = (LPFN_FECOM_GET_LAST_ERROR)GetFunctionPtr(pLib, "FECOM_GetLastError");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetLastError != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetLastError;
		}
		return NULL;
	
	case FECOM_DETECT_PORT:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDetectPort != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDetectPort;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDetectPort = (LPFN_FECOM_DETECT_PORT)GetFunctionPtr(pLib, "FECOM_DetectPort");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDetectPort != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDetectPort;
		}
		return NULL;

	case FECOM_ADD_EVENT_HANDLER:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnAddEventHandler != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnAddEventHandler;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnAddEventHandler = (LPFN_FECOM_ADD_EVENT_HANDLER)GetFunctionPtr(pLib, "FECOM_AddEventHandler");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnAddEventHandler != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnAddEventHandler;
		}
		return NULL;
	
	case FECOM_DEL_EVENT_HANDLER:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDelEventHandler != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDelEventHandler;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDelEventHandler = (LPFN_FECOM_DEL_EVENT_HANDLER)GetFunctionPtr(pLib, "FECOM_DelEventHandler");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDelEventHandler != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDelEventHandler;
		}
		return NULL;
	
	case FECOM_OPEN_PORT:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnOpenPort != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnOpenPort;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnOpenPort = (LPFN_FECOM_OPEN_PORT)GetFunctionPtr(pLib, "FECOM_OpenPort");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnOpenPort != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnOpenPort;
		}
		return NULL;
	
	case FECOM_CLOSE_PORT:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnClosePort != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnClosePort;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnClosePort = (LPFN_FECOM_CLOSE_PORT)GetFunctionPtr(pLib, "FECOM_ClosePort");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnClosePort != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnClosePort;
		}
		return NULL;
	
	case FECOM_GET_PORT_LIST:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortList != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortList;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortList = (LPFN_FECOM_GET_PORT_LIST)GetFunctionPtr(pLib, "FECOM_GetPortList");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortList != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortList;
		}
		return NULL;
	
	case FECOM_GET_PORT_PARA:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortPara != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortPara;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortPara = (LPFN_FECOM_GET_PORT_PARA)GetFunctionPtr(pLib, "FECOM_GetPortPara");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortPara != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortPara;
		}
		return NULL;
	
	case FECOM_SET_PORT_PARA:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnSetPortPara != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnSetPortPara;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnSetPortPara = (LPFN_FECOM_SET_PORT_PARA)GetFunctionPtr(pLib, "FECOM_SetPortPara");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnSetPortPara != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnSetPortPara;
		}
		return NULL;
	
	case FECOM_DO_PORT_CMD:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDoPortCmd != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDoPortCmd;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDoPortCmd = (LPFN_FECOM_DO_PORT_CMD)GetFunctionPtr(pLib, "FECOM_DoPortCmd");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDoPortCmd != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnDoPortCmd;
		}
		return NULL;
	
	case FECOM_GET_PORT_HND:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortHnd != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortHnd;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortHnd = (LPFN_FECOM_GET_PORT_HND)GetFunctionPtr(pLib, "FECOM_GetPortHnd");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortHnd != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnGetPortHnd;
		}
		return NULL;
	
	case FECOM_TRANSCEIVE:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnTransceive != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnTransceive;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnTransceive = (LPFN_FECOM_TRANSCEIVE)GetFunctionPtr(pLib, "FECOM_Transceive");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnTransceive != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnTransceive;
		}
		return NULL;
	
	case FECOM_TRANSMIT:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnTransmit != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnTransmit;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnTransmit = (LPFN_FECOM_TRANSMIT)GetFunctionPtr(pLib, "FECOM_Transmit");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnTransmit != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnTransmit;
		}
		return NULL;
	
	case FECOM_RECEIVE:
		if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnReceive != NULL)
		{
			return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnReceive;
		}
		else
		{
			((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnReceive = (LPFN_FECOM_RECEIVE)GetFunctionPtr(pLib, "FECOM_Receive");
			if(((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnReceive != NULL)
				return (void*)((_FEDM_FECOM_PTR*)m_pFeComPtrList)->lpfnReceive;
		}
		return NULL;
	}

	return NULL;
}
#endif // #if !defined(_FEDM_SUPPORT_SLINK)
#endif // #ifdef _FEDM_COM_SUPPORT


/***************************************************************************
  Begin			:	25.04.2003 / M. Hultsch
  Version       :	03.00.00 / 24.04.2008 / M. Hultsch

  Function		:	get a pointer to a function from feusb library

  Parameters	:	constant with function-id (see feusb.h)

  Return Value	:	void* pFct	- pointer to a function or NULL

***************************************************************************/
#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
void* FEDM_Base::GetFeUsbFunction(unsigned int uiFct)
{
	void* pLib = NULL;

	if(uiFct >= 1100 && uiFct < 1200)
	{
		pLib = LoadFeUsb();
		if(pLib == NULL)
			return NULL;
	}
	else
	{
		return NULL;
	}

	if(m_pFeUsbPtrList == NULL)
	{
		m_pFeUsbPtrList = (void*)new _FEDM_FEUSB_PTR;
		if(m_pFeUsbPtrList == NULL)
			return NULL;
	}


	switch(uiFct)
	{
	// function pointers of feusb
	case FEUSB_GET_DLL_VERSION:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDLLVersion != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDLLVersion;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDLLVersion = (LPFN_FEUSB_GET_DLL_VERSION)GetFunctionPtr(pLib, "FEUSB_GetDLLVersion");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDLLVersion != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDLLVersion;
		}
		return NULL;
	
	case FEUSB_GET_DRV_VERSION:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDrvVersion != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDrvVersion;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDrvVersion = (LPFN_FEUSB_GET_DRV_VERSION)GetFunctionPtr(pLib, "FEUSB_GetDrvVersion");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDrvVersion != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDrvVersion;
		}
		return NULL;
	
	case FEUSB_GET_ERROR_TEXT:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetErrorText != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetErrorText;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetErrorText = (LPFN_FEUSB_GET_ERROR_TEXT)GetFunctionPtr(pLib, "FEUSB_GetErrorText");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetErrorText != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetErrorText;
		}
		return NULL;
	
	case FEUSB_GET_LAST_ERROR:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetLastError != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetLastError;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetLastError = (LPFN_FEUSB_GET_LAST_ERROR)GetFunctionPtr(pLib, "FEUSB_GetLastError");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetLastError != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetLastError;
		}
		return NULL;
	
	case FEUSB_SCAN:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnScan != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnScan;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnScan = (LPFN_FEUSB_SCAN)GetFunctionPtr(pLib, "FEUSB_Scan");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnScan != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnScan;
		}
		return NULL;
	
	case FEUSB_SCAN_AND_OPEN:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnScanAndOpen != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnScanAndOpen;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnScanAndOpen = (LPFN_FEUSB_SCAN_AND_OPEN)GetFunctionPtr(pLib, "FEUSB_ScanAndOpen");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnScanAndOpen != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnScanAndOpen;
		}
		return NULL;
	
	case FEUSB_GET_SCAN_LIST_PARA:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetScanListPara != NULL) 
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetScanListPara;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetScanListPara = (LPFN_FEUSB_GET_SCAN_LIST_PARA)GetFunctionPtr(pLib, "FEUSB_GetScanListPara");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetScanListPara != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetScanListPara;
		}
		return NULL;
	
	case FEUSB_GET_SCAN_LIST_SIZE:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetScanListSize != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetScanListSize;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetScanListSize = (LPFN_FEUSB_GET_SCAN_LIST_SIZE)GetFunctionPtr(pLib, "FEUSB_GetScanListSize");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetScanListSize != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetScanListSize;
		}
		return NULL;
	
	case FEUSB_CLEAR_SCAN_LIST:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnClearScanList != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnClearScanList;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnClearScanList = (LPFN_FEUSB_CLEAR_SCAN_LIST)GetFunctionPtr(pLib, "FEUSB_ClearScanList");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnClearScanList != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnClearScanList;
		}
		return NULL;
	
	case FEUSB_ADD_EVENT_HANDLER:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnAddEventHandler != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnAddEventHandler;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnAddEventHandler = (LPFN_FEUSB_ADD_EVENT_HANDLER)GetFunctionPtr(pLib, "FEUSB_AddEventHandler");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnAddEventHandler != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnAddEventHandler;
		}
		return NULL;
	
	case FEUSB_DEL_EVENT_HANDLER:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnDelEventHandler != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnDelEventHandler;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnDelEventHandler = (LPFN_FEUSB_DEL_EVENT_HANDLER)GetFunctionPtr(pLib, "FEUSB_DelEventHandler");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnDelEventHandler != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnDelEventHandler;
		}
		return NULL;
	
	case FEUSB_OPEN_DEVICE:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnOpenDevice != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnOpenDevice;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnOpenDevice = (LPFN_FEUSB_OPEN_DEVICE)GetFunctionPtr(pLib, "FEUSB_OpenDevice");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnOpenDevice != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnOpenDevice;
		}
		return NULL;
	
	case FEUSB_CLOSE_DEVICE:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnCloseDevice != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnCloseDevice;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnCloseDevice = (LPFN_FEUSB_CLOSE_DEVICE)GetFunctionPtr(pLib, "FEUSB_CloseDevice");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnCloseDevice != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnCloseDevice;
		}
		return NULL;
	
	case FEUSB_IS_DEVICE_PRESENT:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnIsDevicePresent != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnIsDevicePresent;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnIsDevicePresent = (LPFN_FEUSB_IS_DEVICE_PRESENT)GetFunctionPtr(pLib, "FEUSB_IsDevicePresent");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnIsDevicePresent != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnIsDevicePresent;
		}
		return NULL;
	
	case FEUSB_GET_DEVICE_LIST:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDeviceList != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDeviceList;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDeviceList = (LPFN_FEUSB_GET_DEVICE_LIST)GetFunctionPtr(pLib, "FEUSB_GetDeviceList");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDeviceList != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDeviceList;
		}
		return NULL;
	
	case FEUSB_GET_DEVICE_PARA:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDevicePara != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDevicePara;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDevicePara = (LPFN_FEUSB_GET_DEVICE_PARA)GetFunctionPtr(pLib, "FEUSB_GetDevicePara");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDevicePara != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDevicePara;
		}
		return NULL;
	
	case FEUSB_SET_DEVICE_PARA:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnSetDevicePara != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnSetDevicePara;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnSetDevicePara = (LPFN_FEUSB_SET_DEVICE_PARA)GetFunctionPtr(pLib, "FEUSB_SetDevicePara");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnSetDevicePara != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnSetDevicePara;
		}
		return NULL;
	
	case FEUSB_GET_DEVICE_HND:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDeviceHnd != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDeviceHnd;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDeviceHnd = (LPFN_FEUSB_GET_DEVICE_HND)GetFunctionPtr(pLib, "FEUSB_GetDeviceHnd");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDeviceHnd != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnGetDeviceHnd;
		}
		return NULL;

	case FEUSB_TRANSCEIVE:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnTransceive != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnTransceive;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnTransceive = (LPFN_FEUSB_TRANSCEIVE)GetFunctionPtr(pLib, "FEUSB_Transceive");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnTransceive != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnTransceive;
		}
		return NULL;

	case FEUSB_TRANSMIT:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnTransmit != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnTransmit;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnTransmit = (LPFN_FEUSB_TRANSMIT)GetFunctionPtr(pLib, "FEUSB_Transmit");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnTransmit != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnTransmit;
		}
		return NULL;

	case FEUSB_RECEIVE:
		if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnReceive != NULL)
		{
			return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnReceive;
		}
		else
		{
			((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnReceive = (LPFN_FEUSB_RECEIVE)GetFunctionPtr(pLib, "FEUSB_Receive");
			if(((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnReceive != NULL)
				return (void*)((_FEDM_FEUSB_PTR*)m_pFeUsbPtrList)->lpfnReceive;
		}
		return NULL;
	}

	return NULL;
}
#endif // #if !defined(_FEDM_SUPPORT_SLINK)
#endif // #ifdef _FEDM_USB_SUPPORT




/***************************************************************************
  Begin			:	22.05.2003 / M. Hultsch
  Version       :	02.04.04 / 22.08.2006 / M. Hultsch

  Function		:	get a pointer to a function from fetcp library

  Parameters	:	constant with function-id (see fetcp.h)

  Return Value	:	void* pFct	- pointer to a function or NULL

***************************************************************************/
#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
void* FEDM_Base::GetFeTcpFunction(unsigned int uiFct)
{
	void* pLib = NULL;

	if(uiFct >= 1200 && uiFct < 1300)
	{
		pLib = LoadFeTcp();
		if(pLib == NULL)
			return NULL;
	}
	else
	{
		return NULL;
	}

	if(m_pFeTcpPtrList == NULL)
	{
		m_pFeTcpPtrList = (void*)new _FEDM_FETCP_PTR;
	}


	switch(uiFct)
	{
	// function pointers of fetcp
	case FETCP_GET_DLL_VERSION:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetDLLVersion != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetDLLVersion;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetDLLVersion = (LPFN_FETCP_GET_DLL_VERSION)GetFunctionPtr(pLib, "FETCP_GetDLLVersion");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetDLLVersion != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetDLLVersion;
		}
		return NULL;
	
	case FETCP_GET_ERROR_TEXT:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetErrorText != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetErrorText;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetErrorText = (LPFN_FETCP_GET_ERROR_TEXT)GetFunctionPtr(pLib, "FETCP_GetErrorText");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetErrorText != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetErrorText;
		}
		return NULL;
	
	case FETCP_GET_LAST_ERROR:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetLastError != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetLastError;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetLastError = (LPFN_FETCP_GET_LAST_ERROR)GetFunctionPtr(pLib, "FETCP_GetLastError");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetLastError != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetLastError;
		}
		return NULL;
	
	case FETCP_ADD_EVENT_HANDLER:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnAddEventHandler != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnAddEventHandler;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnAddEventHandler = (LPFN_FETCP_ADD_EVENT_HANDLER)GetFunctionPtr(pLib, "FETCP_AddEventHandler");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnAddEventHandler != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnAddEventHandler;
		}
		return NULL;
	
	case FETCP_DEL_EVENT_HANDLER:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDelEventHandler != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDelEventHandler;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDelEventHandler = (LPFN_FETCP_DEL_EVENT_HANDLER)GetFunctionPtr(pLib, "FETCP_DelEventHandler");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDelEventHandler != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDelEventHandler;
		}
		return NULL;
	
	case FETCP_DETECT:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDetect != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDetect;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDetect = (LPFN_FETCP_CONNECT)GetFunctionPtr(pLib, "FETCP_Detect");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDetect != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDetect;
		}
		return NULL;
	
	case FETCP_CONNECT:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnConnect != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnConnect;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnConnect = (LPFN_FETCP_CONNECT)GetFunctionPtr(pLib, "FETCP_Connect");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnConnect != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnConnect;
		}
		return NULL;
	
	case FETCP_DISCONNECT:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDisConnect != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDisConnect;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDisConnect = (LPFN_FETCP_DISCONNECT)GetFunctionPtr(pLib, "FETCP_DisConnect");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDisConnect != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnDisConnect;
		}
		return NULL;
	
	case FETCP_GET_SOCKET_LIST:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketList != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketList;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketList = (LPFN_FETCP_GET_SOCKET_LIST)GetFunctionPtr(pLib, "FETCP_GetSocketList");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketList != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketList;
		}
		return NULL;
	
	case FETCP_GET_SOCKET_PARA:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketPara != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketPara;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketPara = (LPFN_FETCP_GET_SOCKET_PARA)GetFunctionPtr(pLib, "FETCP_GetSocketPara");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketPara != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketPara;
		}
		return NULL;
	
	case FETCP_SET_SOCKET_PARA:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnSetSocketPara != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnSetSocketPara;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnSetSocketPara = (LPFN_FETCP_SET_SOCKET_PARA)GetFunctionPtr(pLib, "FETCP_SetSocketPara");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnSetSocketPara != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnSetSocketPara;
		}
		return NULL;
	
	case FETCP_GET_SOCKET_HND:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketHnd != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketHnd;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketHnd = (LPFN_FETCP_GET_SOCKET_HND)GetFunctionPtr(pLib, "FETCP_GetSocketHnd");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketHnd != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketHnd;
		}
		return NULL;
	
	case FETCP_GET_SOCKET_STATE:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketState != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketState;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketState = (LPFN_FETCP_GET_SOCKET_STATE)GetFunctionPtr(pLib, "FETCP_GetSocketState");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketState != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnGetSocketState;
		}
		return NULL;

	case FETCP_TRANSCEIVE:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnTransceive != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnTransceive;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnTransceive = (LPFN_FETCP_TRANSCEIVE)GetFunctionPtr(pLib, "FETCP_Transceive");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnTransceive != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnTransceive;
		}
		return NULL;

	case FETCP_TRANSMIT:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnTransmit != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnTransmit;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnTransmit = (LPFN_FETCP_TRANSMIT)GetFunctionPtr(pLib, "FETCP_Transmit");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnTransmit != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnTransmit;
		}
		return NULL;

	case FETCP_RECEIVE:
		if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnReceive != NULL)
		{
			return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnReceive;
		}
		else
		{
			((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnReceive = (LPFN_FETCP_RECEIVE)GetFunctionPtr(pLib, "FETCP_Receive");
			if(((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnReceive != NULL)
				return (void*)((_FEDM_FETCP_PTR*)m_pFeTcpPtrList)->lpfnReceive;
		}
		return NULL;
	}

	return NULL;
}
#endif // #if !defined(_FEDM_SUPPORT_SLINK)
#endif // #ifdef _FEDM_TCP_SUPPORT

/***************************************************************************
  Beginn        	:	14.12.2000 / M. Hultsch
  Version       	:	02.00.00 / 14.12.2000 / M. Hultsch
						
  Beschreibung  	:	Funktion ermittelt die Betriebssystemversion
						
  Übergabe			:	-
						
  Rückgabe			:	-
						
***************************************************************************/
#ifdef _FEDM_WINDOWS
void FEDM_Base::GetOS()
{
	OSVERSIONINFO osInfo;

	m_iWinVer = 0;

	osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&osInfo);

	if(osInfo.dwPlatformId == 1) // VER_PLATFORM_WIN32_WINDOWS
	{
		if(osInfo.dwMinorVersion < 10)
			m_iWinVer = FEDM_OS_W95;
		else
		{
			if(osInfo.dwMinorVersion < 90)
				m_iWinVer = FEDM_OS_W98;
			else
				m_iWinVer = FEDM_OS_WME;
		}
	}
	else if(osInfo.dwPlatformId == 2) // VER_PLATFORM_WIN32_NT
	{
		if(osInfo.dwMajorVersion >= 5)
			m_iWinVer = FEDM_OS_W2K;
		else
			m_iWinVer = FEDM_OS_WNT4;
	}
	else if(osInfo.dwPlatformId == 3) // VER_PLATFORM_WIN32_CE
	{
		m_iWinVer = FEDM_OS_WCE;
	}
}
#endif
