/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2020 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* ConsumerHandler.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dConsumerHandler_H)
#define __6dConsumerHandler_H

/**
 * Header Includes
 */
#include "AppConfigParams.h"
#include "UDPEventMonitor.h"

/************************************************************************
 * Class : ConsumerHandler
 * Purpose : Handler which handles fetch and release 
 ************************************************************************/

class CConsumerHandler : public IEventListener
{
	private:
		/**
		 * Private Variables
		 */
		long mesl_SynKey;

	protected:
		/**
		 * Protected Variables
		 */

	public:
		/**
		 * Public Variables
		 */

	private:
		/**
		 * Private Member Functions
		 */

		void 	mefn_processFetchResource(const FetchInstanceReq&,char*,char*,long);
		void	mefn_processReleaseResource(const ReleaseResourceReq&,char*,char*,long);
		void	mefn_processConsumerRegistration(const ResourceConsumerRegistrationReq&,char*,char*,long);
		//Serialize
		bool 	mefn_serilizeResponseAndSend(const int&,char*,const int&,char*,char*,long,char*,long);
		bool 	mefn_serilizeResponseAndSend(const int&,char*,int,char*,char*,long);
		bool    mefn_serilizeResponseAndSend(const int&,int,char*,char*,long);
		bool 	mefn_serilizeResponseAndSend(int,char*,char*,char*,long);
	
		//method to initialise the class object
		void    mefn_initialize();


	protected:
		/**
		 * Protected Member Functions
		 */

	public:
		/**
		 * Public Member Functions
		 */
		//Constructor
		CConsumerHandler();


		//Destructor
		~CConsumerHandler();

		//method to print object
		void    mcfn_printObject();
		//call back method for reporting events
		void    mcfn_onNotifyEvent(CGenericEvent&,long);
		//method to subscribe for event
		void    mcfn_subscribeEvent(IAppCallBack *,CEventCriteria *);
		//method to unsubscribe event
		void    mcfn_unsubscribeEvent(IAppCallBack *,CEventCriteria *);
		//method to get sync key
		long    mcfn_getSynchronizationKey(CGenericEvent &)	{ return mesl_SynKey;	  }
		//Method to create Singleton Reference Object
		void 	mcfn_setSynKey(const long& slL_SynKey) 		{ mesl_SynKey=slL_SynKey; }
	
		

};


#endif
