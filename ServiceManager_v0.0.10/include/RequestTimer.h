/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.				*
 *  (c) 2008 6d Technologies		                              	*
 *  All Rights Reserved				                       	*
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


#if !defined(_RequestTimer_H)
#define _RequestTimer_H


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
 * Class :CRequestTimer
 * Purpose : Manage Request/Network Timer
 ************************************************************************/

class CRequestTimer : public ITimer
{
	private:
		CTimerManager*	pmeC_TimerManager;

		//TimerTicks for Enquring
		CTimerTicks 	meC_RequestTimerTicks;

		int 	mesi_TimerDur;

		IEventListener* pmeI_EventListener;

		CInstanceInfo* 	pmeC_InstanceInfo;

		bool meb_IsRequestTimerStarted;
	public:

		/**
		 * Constructor
		 */

		CRequestTimer();
		/**
		 * Destructort
		 */

		~CRequestTimer();


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

		void 	mcfn_startRequestTimer(IEventListener*,CInstanceInfo*,int siL_MilliSec);

		void 	mcfn_stopRequestTimer();

		void 	mcfn_printObject();

	  	void    mcfn_onNotifyEvent(CGenericEvent &,long);

		bool 	mcfn_IsRequestTimerStarted() {	return meb_IsRequestTimerStarted;	}
}; // end of the class CRequestTimer

inline
void CRequestTimer::mcfn_stopRequestTimer()
{
	meC_RequestTimerTicks.mcfn_stopTimer();
	pmeC_TimerManager->mcfn_unregister(&meC_RequestTimerTicks);
}


#endif // end of _RequestTimer_H

