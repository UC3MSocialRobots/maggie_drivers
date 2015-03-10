/*-------------------------------------------------------
|                                                       |
|                  FEReadTagsThread.cpp                 |
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

Version       		:	01.00.00 / 13.12.2006 / Benjamin Stadin

Operation Systems	:	Linux
*/

#include "FEReadTagsThread.h"
#include "../../../../fedm-classlib/src/FedmIscCore.h"
#include "../../../../feisc-lib/include/feisc.h"

//------------------------------------------------------------------------------
// Name: readTags()
// Desc: starts a thread or triggers a signal to wake the thread up again
//------------------------------------------------------------------------------
void FEReadTagsThread::readTags()
{
    QMutexLocker locker(&m_mutex);

    if (!isRunning()) 
    {
    	m_bWait = false;
        start(LowPriority);
    }
    else 
    {
        m_condition.wakeOne();
    }
}

//------------------------------------------------------------------------------
// Name: FEReadTagsThread()
// Desc: constructor
//------------------------------------------------------------------------------
FEReadTagsThread::FEReadTagsThread(FEDM_ISCReader* reader, QObject *parent)
    : QThread(parent)
{
    m_bFinish = false;
    m_iscReader = reader;
    m_bWait = m_bWait = true;;
}

//------------------------------------------------------------------------------
// Name: ~FEReadTagsThread()
// Desc: destructor
//------------------------------------------------------------------------------
FEReadTagsThread::~FEReadTagsThread()
{
    m_mutex.lock();
    m_bWait = false;
    m_bFinish = true;
    m_condition.wakeOne();
    m_mutex.unlock();
	
    wait();
}

//------------------------------------------------------------------------------
// Name: pauseThread()
// Desc: sets a signal to pause the loop in run()
//------------------------------------------------------------------------------
void FEReadTagsThread::pauseThread()
{
	m_mutex.lock();
	m_bWait = true;
	m_mutex.unlock();
}

//------------------------------------------------------------------------------
// Name: running()
// Desc: returns false if the loop in run() is waiting for a signal to continue,
// 	 or if the thread isnt started yet
//------------------------------------------------------------------------------
bool FEReadTagsThread::running()
{
	return (!m_bWait && isRunning());
}

//------------------------------------------------------------------------------
// Name: run()
// Desc: thread function to read tags. Triggers signals (emit xy) that can be catched
//	 by the main application
//------------------------------------------------------------------------------
void FEReadTagsThread::run()
{
	int iBack;
	int ret = 1;
	
	iBack = m_iscReader->SendProtocol(0x65);
	if (iBack < 0)
	{
		m_mutex.lock();
		emit onError(ret);
		m_condition.wait(&m_mutex);
		m_mutex.unlock();
	}

	while(true) 
	{
		if (m_bFinish)
			return;
	
		switch(unsigned char t = m_iscReader->GetReaderType())
		{
		case FEDM_ISC_TYPE_ISCLR200:
			m_iscReader->SetData(FEDM_ISCLR_TMP_BRM_SETS, (unsigned char)255);			
			ret = m_iscReader->SendProtocol(0x21);
			if (ret < 0)
			{
				m_mutex.lock();
				emit onError(ret);			
				m_condition.wait(&m_mutex);
				m_mutex.unlock();
			}
			break;
		default:
			m_iscReader->SetData(FEDM_ISC_TMP_ADV_BRM_SETS, (UINT)255);
			ret = m_iscReader->SendProtocol(0x22);
			if (ret < 0)
			{
				m_mutex.lock();
				emit onError(ret);
				m_condition.wait(&m_mutex);
				m_mutex.unlock();
			}
			break;
		}
		
		if (m_iscReader->GetTableLength(FEDM_ISC_BRM_TABLE) > 0)
		{
			m_mutex.lock();
			emit onNewData(ret);
			m_condition.wait(&m_mutex);
			m_mutex.unlock();
	
			// clear data buffer
			m_iscReader->SendProtocol(0x32); 
		}

		// pause
		m_mutex.lock();
		if (m_bWait)
			m_condition.wait(&m_mutex);
		m_bWait = false;
		m_mutex.unlock();
		
		msleep(50);
		
	}
}

