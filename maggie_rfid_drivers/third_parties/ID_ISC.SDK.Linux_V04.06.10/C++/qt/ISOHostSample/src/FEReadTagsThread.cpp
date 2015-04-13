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
	int iStatus;
	bool bMore;
	
	m_iscReader->SetBusAddress(0xFF);

	while(true) 
	{
		//Cmd Inventory
		m_iscReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x01);
		m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0x00);
		m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE_MORE, false);
		//Execute RF_Reset
		iBack = m_iscReader->SendProtocol(0x69);
		if ((iBack != 0) && (iBack != 0x84))
		{
			// wait for ui to update
			m_mutex.lock();
			emit onError(iBack);
			m_condition.wait(&m_mutex);
			m_mutex.unlock();
		}
		
		msleep(50);
		
		if (iBack == 0)
		{
			bMore = true;
			m_iscReader->ResetTable(FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_TRTYPE
									|FEDM_ISC_DATA_SNR
									|FEDM_ISC_DATA_RxDB
									|FEDM_ISC_DATA_TxDB
									|FEDM_ISC_DATA_RxCB
									|FEDM_ISC_DATA_TxCB
									|FEDM_ISC_DATA_SEC_STATUS);
			while(bMore)
			{
				//Execute inventory command
				iStatus = m_iscReader->SendProtocol(0xB0);

				//more data
				if (iStatus == 0x94)
				{
					m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE_MORE, true);
					bMore = true;
				}
				else
				{
					m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE_MORE, false);
					bMore = false;
					if( iStatus == 0x00 || iStatus == 0x83 || iStatus == 0x84 
					    || iStatus ==0x90 || iStatus == 0x93 
					    || iStatus == 0x94 || iStatus == 0x95 )
					{
						m_mutex.lock();
						emit onNewTag(iStatus);
						m_condition.wait(&m_mutex);
						m_mutex.unlock();
					}
					else if (iStatus == 0x01)
					{
						m_mutex.lock();
						emit onNoTag(iStatus);	
						m_condition.wait(&m_mutex);
						m_mutex.unlock();										
					}
					else
					{
						m_mutex.lock();
						emit onError(iStatus);
						m_condition.wait(&m_mutex);
						m_mutex.unlock();
					}
				}
					
			}
			
		}	
		msleep(50);

		// pause signal
		m_mutex.lock();
		if (m_bWait)
			m_condition.wait(&m_mutex);
		m_bWait = false;
		m_mutex.unlock();
		
		if(m_bFinish)
			return;
		
	}
}

