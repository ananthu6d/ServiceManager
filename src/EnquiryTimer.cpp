/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.				*		    
 *  (c) 2008 6d Technologies		                                *
 *  All Rights Reserved				                        *
 *                                                                      *
 ************************************************************************
 *
 *  MediaTimer.cpp
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -------------------------------------------------------
 *   1 23Sep08   RKM       Created
 */


/**
* Header Includes
*/
#include "EnquiryTimer.h"
#include "Exception.h"
#include "AppHeaders.h"
#include "UDPEventMonitor.h"

/************************************************************************
* Class     : CEnquiryTimer                                             
* Method    : Constructor                                               
* Purpose   : Creates Object and initializes its variables              
* Arguments : Nil                                                       
* Returns   : Nil                                                       
*************************************************************************/

CEnquiryTimer::CEnquiryTimer()
{
	mesi_TimerDur = 0;
	pmeC_TimerManager = CTimerManager::mcfn_getInstance();

        if(!pmeC_TimerManager)
                throw CException(100,"Could not get TimerManager");

	pmeI_EventListener=nullptr;
}


/************************************************************************
 * Class     : CEnquiryTimer                                            
 * Method    : Destructor                                                
 * Purpose   : releases its variables and destroys the object            
 * Arguments : Nil                                                       
 * Returns   : Nil                                                       
 *************************************************************************/

CEnquiryTimer::~CEnquiryTimer()
{
	//CLOGENTRYEXIT(CG_EventLog,0);

	meC_EnquiryTimerTicks.mcfn_stopTimer();
	pmeC_TimerManager->mcfn_unregister(&meC_EnquiryTimerTicks);


} // end of Destructor

/************************************************************************
 * Class     : CEnquiryTimer
 * Method    : mcfn_getSynchronizationKey
 * Purpose   : return the synkey
 * Arguments : Nil
 * Returns   : SynKey
 *************************************************************************/

long CEnquiryTimer::mcfn_getSynchronizationKey()
{
	//CLOGENTRYEXIT(CG_EventLog,0);
	
	CGenericEvent CL_GenericEvent;
	return pmeI_EventListener->mcfn_getSynchronizationKey(CL_GenericEvent);
	//return (long)this;
}

/************************************************************************
 * Class     : CEnquiryTimer
 * Method    : mcfn_printObject
 * Purpose   : print function
 * Arguments : Nil
 * Returns   : Nil
 *************************************************************************/

void CEnquiryTimer::mcfn_printObject()
{
	//CLOGENTRYEXIT(CG_EventLog,0);
}

/************************************************************************
 * Class     : CEnquiryTimer
 * Method    : mcfn_startEnquiryTimer
 * Purpose   : start Enquiry Timer
 * Arguments : IEventListener* pIL_EventListener,CInstanceInfo* pCL_InstanceInfo,int siL_MilliSec
 * Returns   : Nil
 *************************************************************************/

void CEnquiryTimer::mcfn_startEnquiryTimer(IEventListener* pIL_EventListener,CInstanceInfo* pCL_InstanceInfo,int siL_MilliSec)
{
	//CLOGENTRYEXIT(CG_EventLog,0);
	pmeI_EventListener=pIL_EventListener;
	pmeC_InstanceInfo=pCL_InstanceInfo;
	EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID,"EnquiryTimer","startEnquiryTimer",this,"","Timer Duration: %dms",siL_MilliSec);
	
	meC_EnquiryTimerTicks.mcfn_setTimer(0,siL_MilliSec);
	pmeC_TimerManager->mcfn_register(&meC_EnquiryTimerTicks,this);
	mesi_TimerDur = siL_MilliSec;
}

/************************************************************************
 * Class     : CEnquiryTimer
 * Method    : mcfn_onNotifyEvent
 * Purpose   : Dispatch Timeout to monitor
 * Arguments : CGenericEvent&, slL_ThreadId
 * Returns   : Nil
 *************************************************************************/

void CEnquiryTimer::mcfn_onNotifyEvent(CGenericEvent & CL_GenericEvent,long slL_ThreadId)
{

	EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID,"EnquiryTimer","TimerEvent",this,"",\
			"Timer Elapsed: %dms", mesi_TimerDur);

	CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pmeI_EventListener,EVT_ENQUIRY_TIMEOUT,ENQUIRY_TYPE,pmeC_InstanceInfo);
		
}
