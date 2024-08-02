/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2020 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* ServiceResource.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dServiceResource_H)
#define __6dServiceResource_H

/**
 * Header Includes
 */

#include "AppHeaders.h"
#include "AppExterns.h"
#include "UDPEventMonitor.h"
#include "ResourceCache.h"
/************************************************************************
 * Class : ServiceResource
 * Purpose : to store resources
 ************************************************************************/

class CServiceResource 
{

	private:
		/**
                 * Private Membesrs
		 */

		string 	meC_SignalingIP;
		string  meC_ServiceType;
		
		long	mesl_SignalingPort;

		char    mesc_Status;

		int	mesi_ServiceId;
		int 	mesi_InstanceId;
		int 	mesi_TotalChannel; //Remove
		int 	mesi_BusyCount;

		//
		//BusyCount
		//

		//std::mutex meC_ServiceResourceMutex;
	public:
		/**
		 * Public Membesrs
		 */


	public:
		/**
                 * Public Functions
                 */

		//Constructor
		CServiceResource()
		{
			meC_SignalingIP		= "";
			meC_ServiceType		= "";
			mesi_InstanceId 	= 0x00;
			mesl_SignalingPort 	= 0x00;
			mesc_Status 		= 'A';
			mesi_ServiceId		= 0x00;
			mesi_TotalChannel	= 0x00;
			mesi_BusyCount		= 0x00;
		}
		//Destructor
		~CServiceResource()
		{

		}

		//TODO: getter setter functions

		bool 	mcfn_getNextResource(char* pscL_Ip,long& slL_Port, int& siL_InstanceId);

		bool 	mcfn_releaseResource();

		bool 	mcfn_deleteFromCache()
		{
			//TODO: remove instance_data from service map in external cache
		}

		bool 	mcfn_InsertIntoExternalCache()
		{
			return CResourceCache::mcfn_getInstance()->mcfn_insertBusyCount(meC_ServiceType+"_"+to_string(mesi_ServiceId),meC_SignalingIP+":"+to_string(mesl_SignalingPort));
		}

		void  	mcfn_deActivateResource()
		{
			if(mesc_Status == 'A')
				mesc_Status = 'D';

		}
		bool 	mcfn_activateResource()
		{
			if(mesc_Status == 'D')
			{
				mesc_Status	= 'A';
				mesi_BusyCount	= 0x00;
				return CResourceCache::mcfn_getInstance()->mcfn_resetBusyCount(meC_ServiceType+"_"+to_string(mesi_ServiceId),meC_SignalingIP+":"+to_string(mesl_SignalingPort));
			}
			return true;

		}

		bool 	mcfn_activateAndResetResource()
		{
			if(mesc_Status == 'D')
			{
				mesc_Status = 'A';
			}
			mesi_BusyCount  = 0x00;
			return CResourceCache::mcfn_getInstance()->mcfn_resetBusyCount(meC_ServiceType+"_"+to_string(mesi_ServiceId),meC_SignalingIP+":"+to_string(mesl_SignalingPort));	
		}

		bool 	mcfn_resetBusyCountInExternalCache()
		{
			mesi_BusyCount  = 0x00;
			return CResourceCache::mcfn_getInstance()->mcfn_resetBusyCount(meC_ServiceType+"_"+to_string(mesi_ServiceId),meC_SignalingIP+":"+to_string(mesl_SignalingPort));
		}

		void 	mcfn_setSignalingIP(char* pscL_IP)      	{ meC_SignalingIP=pscL_IP;	 	}
		void 	mcfn_setServiceType(const string& CL_Type)     	{ meC_ServiceType=CL_Type;	 	}
		void 	mcfn_setSignalingPort(const long& slL_Port)   	{ mesl_SignalingPort = slL_Port;       	}
		void	mcfn_setInstanceId(const int& siL_Id)          	{ mesi_InstanceId = siL_Id;           	}
		void 	mcfn_setStatus(const char scL_Status)       	{ mesc_Status=scL_Status;		}
		void 	mcfn_setServiceId(const int& siL_Id)         	{ mesi_ServiceId = siL_Id;             	}
		void 	mcfn_setTotalChannel(const int& siL_Count)      { mesi_TotalChannel = siL_Count;        }
		void 	mcfn_setBusyCount(const int& siL_Count)		{ mesi_BusyCount = siL_Count;		}

		string 	mcfn_getSignalingIP()                        	{ return meC_SignalingIP;             	}
		string 	mcfn_getServiceType()                        	{ return meC_ServiceType;             	}
		long 	mcfn_getSignalingPort()                        	{ return mesl_SignalingPort;           	}
		int 	mcfn_getInstanceId()                          	{ return mesi_InstanceId;              	}
		char 	mcfn_getStatus()                            	{ return mesc_Status;                  	}
		int 	mcfn_getServiceId()                        	{ return mesi_ServiceId;               	}
		int 	mcfn_getTotalChannel()                        	{ return mesi_TotalChannel;             }
		int     mcfn_getBusyCount()			         { return mesi_BusyCount;	        }
		void 	mcfn_incrementBusyCount()			{ mesi_BusyCount++;			}
		void 	mcfn_decrementBusyCount()			{ mesi_BusyCount--;			}
		void 	mcfn_resetBusyCount()				{ mesi_BusyCount = 0;			}


};

#endif
