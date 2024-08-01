/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.				*
 *  (c) 2008 6d Technologies		                                *
 *  All Rights Reserved				                   	*	
 *                                                                      *
 ************************************************************************
 *
 *  Timer.h
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -------------------------------------------------------
 *   1 25May24   Ananthu       Created
 */


#if !defined(_EnquiryTimer_H)
#define _EnquiryTimer_H


/**
* Header Includes
*/

#include "FrwkInterfaces.h"
#include "TimerManager.h"
#include "TimerTicks.h"
#include "EventCriteria.h"

class CInstanceInfo;

using namespace _6d::_Utils;
using namespace _6d::_Common;
using namespace _6d;
using namespace _6d::_Framework;

/************************************************************************
 * Class : CEnquiryTimer
 * Purpose :to send a timer for enquiry 
 ************************************************************************/

class CEnquiryTimer : public ITimer
{
	private:
		CTimerManager*	pmeC_TimerManager;

		//TimerTicks for Enquring
		CTimerTicks 	meC_EnquiryTimerTicks;

		int 		mesi_TimerDur;

		IEventListener* pmeI_EventListener;

		CInstanceInfo* 	pmeC_InstanceInfo;
	public:

		/**
		 * Constructor
		 */

		CEnquiryTimer();
		/**
		 * Destructort
		 */

		~CEnquiryTimer();


		/**
		 * This virtual method should be implemented by the sub-classes
		 * to define the Synchronization Key 
		 * DispatchManager dispatches event sequencially associated with 
		 * that key
		 * @param1 Event Information required to get the synchronization
		 * key
		 */

		long 	mcfn_getSynchronizationKey();

		void 	mcfn_onTimerExpired(CTimerTicks *){};

		void 	mcfn_startEnquiryTimer(IEventListener*,CInstanceInfo*,int siL_MilliSec);

		void 	mcfn_stopEnquiryTimer();

		void 	mcfn_printObject();

	  	void    mcfn_onNotifyEvent(CGenericEvent &,long);
}; // end of the class CEnquiryTimer

inline
void CEnquiryTimer::mcfn_stopEnquiryTimer()
{
	meC_EnquiryTimerTicks.mcfn_stopTimer();
	pmeC_TimerManager->mcfn_unregister(&meC_EnquiryTimerTicks);
}


#endif // end of _EnquiryTimer_H

