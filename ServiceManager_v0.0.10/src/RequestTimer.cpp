/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technolog					*
 *  (c) 2008 6d Technologies		                               	*
 *  All Rights Reserved				                       	*
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
#include "RequestTimer.h"
#include "Exception.h"
#include "AppHeaders.h"
#include "UDPEventMonitor.h"

/************************************************************************
* Class     : CRequestTimer                                             
* Method    : Constructor                                               
* Purpose   : Creates Object and initializes its variables              
* Arguments : Nil                                                       
* Returns   : Nil                                                       
*************************************************************************/

CRequestTimer::CRequestTimer()
{
	mesi_TimerDur 			= 0;
	meb_IsRequestTimerStarted 	= false;
	pmeC_TimerManager 		= CTimerManager::mcfn_getInstance();

        if(!pmeC_TimerManager)
                throw CException(100,"Could not get TimerManager");

	pmeI_EventListener=nullptr;
}
 // end of Constructor

/************************************************************************
 * Class     : CRequestTimer                                            
 * Method    : Destructor                                                
 * Purpose   : releases its variables and destroys the object            
 * Arguments : Nil                                                       
 * Returns   : Nil                                                       
 *************************************************************************/

CRequestTimer::~CRequestTimer()
{
	//CLOGENTRYEXIT(CG_EventLog,0);

	meC_RequestTimerTicks.mcfn_stopTimer();
	pmeC_TimerManager->mcfn_unregister(&meC_RequestTimerTicks);


} // end of Destructor

/************************************************************************
 * Class     : CRequestTimer
 * Method    : mcfn_getSynchronizationKey
 * Purpose   : return SynKey
 * Arguments : Nil
 * Returns   : SynKey
 *************************************************************************/

long CRequestTimer::mcfn_getSynchronizationKey()
{
	//CLOGENTRYEXIT(CG_EventLog,0);
	
	CGenericEvent CL_GenericEvent;
	return pmeI_EventListener->mcfn_getSynchronizationKey(CL_GenericEvent);
	//return (long)this;
}

/************************************************************************
 * Class     : CRequestTimer
 * Method    : mcfn_printObject
 * Purpose   : print function
 * Arguments : Nil
 * Returns   : Nil
 *************************************************************************/

void CRequestTimer::mcfn_printObject()
{
	//CLOGENTRYEXIT(CG_EventLog,0);
}

/************************************************************************
 * Class     : CRequestTimer
 * Method    : mcfn_startRequestTimer
 * Purpose   : start Request Timer
 * Arguments : IEventListener*,CInstanceInfo*,siL_MilliSec
 * Returns   : Nil
 *************************************************************************/

void CRequestTimer::mcfn_startRequestTimer(IEventListener* pIL_EventListener,CInstanceInfo* pCL_InstanceInfo,int siL_MilliSec)
{
	pmeI_EventListener	= pIL_EventListener;
	pmeC_InstanceInfo	= pCL_InstanceInfo;
	EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID,"RequestTimer","startRequestTimer",this,"","Timer Duration: %dms",siL_MilliSec);
	
	meC_RequestTimerTicks.mcfn_setTimer(0,siL_MilliSec);
	pmeC_TimerManager->mcfn_register(&meC_RequestTimerTicks,this);
	mesi_TimerDur 			= siL_MilliSec;
	meb_IsRequestTimerStarted 	= true; 
}

/************************************************************************
 * Class     : CRequestTimer
 * Method    : mcfn_onNotifyEvent
 * Purpose   : Dispatch Timer to Monitor
 * Arguments : CGenericEvent&,slL_ThreadId
 * Returns   : Nil
 *************************************************************************/

void CRequestTimer::mcfn_onNotifyEvent(CGenericEvent & CL_GenericEvent,long slL_ThreadId)
{

	EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID,"RequestTimer","TimerEvent",this,"",\
			"Timer Elapsed: %dms", mesi_TimerDur);

	CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pmeI_EventListener,EVT_REQUEST_TIMEOUT,ENQUIRY_TYPE,pmeC_InstanceInfo);
		
	meb_IsRequestTimerStarted = false;	
}
