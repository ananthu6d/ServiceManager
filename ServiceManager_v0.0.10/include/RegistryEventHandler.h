/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2020 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* RegistryEventHandler.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dRegistryEventHandler_H)
#define __6dRegistryEventHandler_H

/**
 * Header Includes
 */
#include "AppConfigParams.h"
#include "AppHeaders.h"
#include "UDPEventMonitor.h"
#include "ServiceManagerInterface.pb.h"
#include "InstanceInfo.h"

/************************************************************************
 * Class : RegistryEventHandler
 * Purpose : to perform reg/dereg releated operations 
 ************************************************************************/

class CRegistryEventHandler : public IEventListener
{
	private:
		/**
		 * Private Variables
		 */

		long	mesl_HandlerId;
		SServiceManagerEvent* pmeS_ServiceManagerEventReq;
		SServiceManagerEvent meS_ServiceManagerEventResp;
		char 	pmesc_ClientIp[65];
		long 	mesl_ClientPort;

		char	pmesc_StatusDesc[100];
		int 	mesi_StatusCode;

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

		//method to initialise the class object
		void    mefn_initialize();

		//Sending response to Client	
		void 	mefn_sendResponse();

		//Set InstanceInfo
		bool 	mefn_setInstanceInfo(const ResourceRegistrationReq&,CInstanceInfo*);

		//Query builder
		void 	mefn_UpdateQueryBuilder(char*,const int&,const int&,const int&,const int&);

		//process
		bool 	mefn_processNetworkEvent();

		bool 	mefn_processSocketReconnect(const SocketReconnectReq&);
		void 	mefn_processRequestTimeout(CInstanceInfo*);
		void 	mefn_processEnquiryTimeout(CInstanceInfo*);

		//Registration
		bool 	mefn_processRegisterInstance(const ResourceRegistrationReq&);
		bool 	mefn_processRegistrationResource(const ResourceRegistrationReq&);
		bool 	mefn_processReregistrationResource(const ResourceRegistrationReq&,const CInstanceInfo*);
		bool 	mefn_updateResources(const ResourceRegistrationReq&,const CInstanceInfo*);

		//Deregistration
		bool 	mefn_processDeRegisterServiceResource(const ResourceDeRegistrationReq&);
		bool 	mefn_processDeRegisterInstance(const ResourceDeRegistrationReq&);

		//Enquiry Response
		void 	mefn_processEnquiryResponse(const EnquireStatusResp&);


		//Serilaize	
		void 	mefn_serilizeResponse(const ResourceRegistrationReq&,int,char*);
		void 	mefn_serilizeResponse(const ResourceDeRegistrationReq&,int,char*,int);
		void    mefn_serilizeResponse(const SocketReconnectReq&,int,char*);
		void 	mefn_serilizeResponse(int,char*);

		void 	mefn_serilizeRequest(int,string,int);

		//DB Insertions
		void 	mefn_InsertInstanceInfoToDB(const CInstanceInfo*);

	protected:
		/**
		 * Protected Member Functions
		 */

	public:
		/**
		 * Public Member Functions
		 */
		//Constructor
		CRegistryEventHandler();


		//Destructor
		~CRegistryEventHandler();

		//method to print object
		void    mcfn_printObject();
		//call back method for reporting events
		void    mcfn_onNotifyEvent(CGenericEvent&,long);
		//method to subscribe for event
		void    mcfn_subscribeEvent(IAppCallBack *,CEventCriteria *);
		//method to unsubscribe event
		void    mcfn_unsubscribeEvent(IAppCallBack *,CEventCriteria *);
		//method to get sync key
		long    mcfn_getSynchronizationKey(CGenericEvent &){return mesl_HandlerId;}

		void	mcfn_setHandlerId(long slL_HandlerId) {mesl_HandlerId = slL_HandlerId;}
};


#endif
