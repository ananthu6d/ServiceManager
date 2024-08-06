/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2020 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* InstanceHandler.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dInstanceHandler_H)
#define __6dInstanceHandler_H

/**
 * Header Includes
 */
#include "AppConfigParams.h"
#include "AppHeaders.h"
#include "UDPEventMonitor.h"
#include "ServiceManagerInterface.pb.h"
//#include "InstanceInfo.h"
#include "ServiceInfo.h"
#include "EnquiryTimer.h"
#include "RequestTimer.h"
/************************************************************************
 * Class : InstanceHandler
 * Purpose : to perform reg/dereg releated operations 
 ************************************************************************/

class CInstanceHandler : public IEventListener
{
	private:
		/**
		 * Private Variables
		 */

		//RegistryHandler
		long	mesl_HandlerId;
		SServiceManagerEvent* pmeS_ServiceManagerEventReq;
		SServiceManagerEvent meS_ServiceManagerEventResp;
		char 	pmesc_ClientIp[65];
		long 	mesl_ClientPort;
	
		char	pmesc_StatusDesc[100];
		int 	mesi_StatusCode;

		//InstanceInfo
		string  meC_SignalingIp;
                char    pmesc_InstanceName[30];
                char    pmesc_RegistrationTime[20];
                char    pmesc_LastHeartBeatTime[20];
                char    pmesc_ClientIP[64];

                char    mesc_Status;

                int     mesi_TotalResourceCount;
                int     mesi_InboundResourceCount;
                int     mesi_OutboundResourceCount;
                int     mesi_SignalingPort;
                int     mesi_InstanceId;
                int     mesi_HeartBeatMissCount;

		int     mesi_IBDBusyCount;
                int     mesi_OBDBusyCount;

		CRequestTimer* pmeC_RequestTimer;
                CEnquiryTimer* pmeC_EnquiryTimer;

		map	<string,CServiceInfo*> meC_ServiceInfoMap;

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
		void 	mefn_setInstanceInfo(const ResourceRegistrationReq&);

		//Query builder
		void 	mefn_UpdateQueryBuilder(char*,const int&,const int&,const int&,const int&);
		void 	mefn_updateRegisterDataBase(const ResourceRegistrationReq&);

		//process
		bool 	mefn_processNetworkEvent();

		void 	mefn_processRequestTimeout();
		void 	mefn_processEnquiryTimeout();
		void    mefn_processForceEnquiry();

		//Registration
		bool 	mefn_processRegisterInstance(const ResourceRegistrationReq&);
		bool 	mefn_processRegistrationResource(const ResourceRegistrationReq&);
		bool 	mefn_processReregistrationResource(const ResourceRegistrationReq&);
		bool 	mefn_updateResources(const ResourceRegistrationReq&);

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
		void 	mefn_InsertInstanceInfoToDB();


		//For ServiceInfo
		bool mefn_activateService(const string&);
		bool mefn_deActivateService(const string&);
		void mefn_deActivateInstance();
		void mefn_activateInstance();

		string mefn_getServiceLists(const ResourceRegistrationReq&);
		string mefn_getServiceLists(const ResourceDeRegistrationReq&);
		string mefn_getServiceLists(const EnquireStatusResp&);

	protected:
		/**
		 * Protected Member Functions
		 */

	public:
		/**
		 * Public Member Functions
		 */
		//Constructor
		CInstanceHandler();


		//Destructor
		~CInstanceHandler();

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

		void	mcfn_setSynKey(long slL_HandlerId) {mesl_HandlerId = slL_HandlerId;}

		// Setter Functions
		void    mcfn_setSignalingIP(char* pscL_IP)                      { meC_SignalingIp = pscL_IP;                    }
                void    mcfn_setLastHeartBeatTime(const char* pscL_Time)        { strcpy(pmesc_LastHeartBeatTime, pscL_Time);   }
                void    mcfn_setRegistrationTime(const char* pscL_Time)         { strcpy(pmesc_RegistrationTime, pscL_Time);    }
                void    mcfn_setTotalResourceCount(const int& siL_Count)        { mesi_TotalResourceCount = siL_Count;          }
                void    mcfn_setInboundResourceCount(const int& siL_Count)      { mesi_InboundResourceCount = siL_Count;        }
                void    mcfn_setOutboundResourceCount(const int& siL_Count)     { mesi_OutboundResourceCount = siL_Count;       }
                void    mcfn_setSignalingPort(const int& siL_Port)              { mesi_SignalingPort=siL_Port;                  }
                void    mcfn_setInstanceId(const int& siL_Id)                   { mesi_InstanceId = siL_Id;                     }
                void    mcfn_setHeartBeatMissCount(const int& siL_Count)        { mesi_HeartBeatMissCount = siL_Count;          }
                void    mcfn_setStatus(const char scL_Status)                   { mesc_Status=scL_Status;                       }
                void    mcfn_setInstanceName(const char* pscL_Name)             { strcpy(pmesc_InstanceName, pscL_Name);        }
                void    mcfn_setClientIP(const char* pscL_IP)                   { strcpy(pmesc_ClientIP, pscL_IP);              }
                void    mcfn_setClientPort(const long& slL_Port)                { mesl_ClientPort= slL_Port;                    }
		void    mcfn_setIBDBusyCount(const int& siL_Count)      	{ mesi_IBDBusyCount = siL_Count;        	}
                void    mcfn_setOBDBusyCount(const int& siL_Count)      	{ mesi_OBDBusyCount = siL_Count;        	}


                // Getter Functions
                string  mcfn_getSignalingIP()                   { return meC_SignalingIp;               }
                int     mcfn_getTotalResourceCount()            { return mesi_TotalResourceCount;       }
                int     mcfn_getInboundResourceCount()          { return mesi_InboundResourceCount;     }
                int     mcfn_getOutboundResourceCount()         { return mesi_OutboundResourceCount;    }
                int     mcfn_getSignalingPort()                 { return mesi_SignalingPort;            }
                int     mcfn_getInstanceId()                    { return mesi_InstanceId;               }
                int     mcfn_getHeartBeatMissCount()            { return mesi_HeartBeatMissCount;       }
                char    mcfn_getStatus()                        { return mesc_Status;                   }
                char*   mcfn_getInstanceName()                  { return pmesc_InstanceName;            }
                char*   mcfn_getRegistrationTime()              { return pmesc_RegistrationTime;        }
                char*   mcfn_getLastHeartBeatTime()             { return pmesc_LastHeartBeatTime;       }
                char*   mcfn_getClientIP()                      { return pmesc_ClientIP;                }
                long    mcfn_getClientPort()                    { return mesl_ClientPort;               }
		int     mcfn_getIBDBusyCount()          	{ return mesi_IBDBusyCount;     	}
                int     mcfn_getOBDBusyCount()          	{ return mesi_OBDBusyCount;     	}
		const   map<string,CServiceInfo*>& mcfn_getServiceMap() { return meC_ServiceInfoMap;    }

};


#endif
