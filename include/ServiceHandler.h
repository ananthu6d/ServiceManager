/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2020 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* ServiceHandler.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dServiceHandler_H)
#define __6dServiceHandler_H

/**
 * Header Includes
 */
#include "AppConfigParams.h"
#include "UDPEventMonitor.h"
#include "ServiceResource.h"

/************************************************************************
 * Class : ServiceHandler
 * Purpose : to handler Service related opertions 
 ************************************************************************/

class CServiceHandler : public IEventListener
{
	private:
		/**
		 * Private Variables
		 */
		char  	pmesc_ServiceName[20];
		char 	mesc_Status;

		string 	meC_ServiceType;

		int 	mesi_ServiceId;
		int 	mesi_LicenseCap;
		int 	mesi_AllotedCap; //NotUsed
		int 	mesi_TotalServiceBusyCount;

		long 	mesl_SynKey;

		float 	mef_LicenseCapPercentage;

		std::map<string,CServiceResource*>meC_ResourceMap;
		std::map<string,CServiceResource*>::iterator meC_CurrentResource;
		std::mutex meC_ResourceMutex;


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

		//Process
		void 	mefn_processAddResource(CServiceResource*);
		void 	mefn_processLoadResource(CServiceResource*);

		void 	mefn_processRemoveResource(string);

		void 	mefn_processDeactivateService(string);
		void 	mefn_processActivateService(string);
		
		void 	mefn_processResetBusyCount(string);

		void 	mefn_updateTotalChannelCount();


	protected:
		/**
		 * Protected Member Functions
		 */

	public:
		/**
		 * Public Member Functions
		 */
		//Constructor
		CServiceHandler();


		//Destructor
		~CServiceHandler();

		//method to print object
		void    mcfn_printObject();
		//call back method for reporting events
		void    mcfn_onNotifyEvent(CGenericEvent&,long);
		//method to subscribe for event
		void    mcfn_subscribeEvent(IAppCallBack *,CEventCriteria *);
		//method to unsubscribe event
		void    mcfn_unsubscribeEvent(IAppCallBack *,CEventCriteria *);

		long 	mcfn_getSynchronizationKey(CGenericEvent &)	{ return mesl_SynKey; }

		void 	mcfn_setSynKey(long slL_Key) 			{ mesl_SynKey=slL_Key; }

		
		//Getter and Setter Functions
		void 	mcfn_setServiceName(const char* pscL_ServiceName)	{ strcpy(pmesc_ServiceName,pscL_ServiceName); 	}
		void 	mcfn_setServiceId(const int& siL_ServiceId) 		{ mesi_ServiceId = siL_ServiceId; 		}
		void 	mcfn_setServiceType(const string& CL_ServiceType) 	{ meC_ServiceType = CL_ServiceType; 		}
		void 	mcfn_setLicenseCap(const int& siL_LicenseCap) 		{ mesi_LicenseCap = siL_LicenseCap;		}
		void 	mcfn_setAllotedCap(const int& siL_AllotedCap) 		{ mesi_AllotedCap = siL_AllotedCap;		}
		void 	mcfn_setTotalServiceBusyCount(const int& siL_BusyCount)	{ mesi_TotalServiceBusyCount = siL_BusyCount;	}
		void 	mcfn_setStatus(const char& scL_status) 			{ mesc_Status = scL_status;   			}
		void 	mcfn_setLicenseCapPercentage(const float& fL_CapPer) 	{ mef_LicenseCapPercentage =  fL_CapPer; 	}

		char* 	mcfn_getServiceName() 				{ return pmesc_ServiceName; 		}
		int 	mcfn_getServiceId() 				{ return mesi_ServiceId;  		}
		string 	mcfn_getServiceType() 				{ return meC_ServiceType;		}
		int 	mcfn_getLicenseCap() 				{ return mesi_LicenseCap; 		}
		char 	mcfn_getStatus() 				{ return mesc_Status;     		}
		int 	mcfn_getAllotedCap() 				{ return mesi_AllotedCap; 		}
		int 	mcfn_getTotalServiceBusyCount() 		{ return mesi_TotalServiceBusyCount;	}
		float 	mcfn_getLicenseCapPercentage() 			{ return mef_LicenseCapPercentage;	}

		bool 	mcfn_fetchResource(char* pscL_SignalingIp,long& slL_SignalingPort,int& siL_InstanceId,int& siL_ErrorCode);
		bool 	mcfn_releaseResource(string CL_SignalingIpPort,int& siL_ErrorCode);

		bool 	mcfn_setResourceBusyCount(const string&,const int&);
};

#endif
