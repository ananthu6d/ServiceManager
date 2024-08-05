/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2020 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* InstanceInfo.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dInstanceInfo_H)
#define __6dInstanceInfo_H

/**
 * Header Includes
 */

#include "AppHeaders.h"
#include "InstanceServiceInfo.h"
#include "EnquiryTimer.h"
#include "RequestTimer.h"
/************************************************************************
 * Class : InstanceInfo
 * Purpose : used to store the details of Instance
 ************************************************************************/

class CInstanceInfo 
{

	private:
		/**
                 * Private Membesrs
		 */

		string 	meC_SignalingIp;
		char   	pmesc_InstanceName[30];
		char    pmesc_RegistrationTime[20];
		char    pmesc_LastHeartBeatTime[20];
		char 	pmesc_ClientIP[64];
		
		char    mesc_Status;

		int 	mesi_TotalResourceCount;
		int 	mesi_InboundResourceCount;
		int 	mesi_OutboundResourceCount;
		int 	mesi_SignalingPort;
		int 	mesi_InstanceId;
		int     mesi_HeartBeatMissCount;

		long 	mesl_ClientPort;

		int	mesi_IBDBusyCount; 
		int	mesi_OBDBusyCount; 

		map	<string,CInstanceServiceInfo*> meC_ServiceInfoMap;

		std::mutex meC_InstanceLock;
	public:
		/**
		 * Public Membesrs
		 */
		CRequestTimer* pmcC_RequestTimer;
		CEnquiryTimer* pmcC_EnquiryTimer;
		

	private:
		/**
                 * Private Functions
                 */
		void 	mefn_checkForForceEnquiry(int);
	public:
		/**
                 * Public Functions
		 */

		//Constructor
		CInstanceInfo();
		//Destructor
		~CInstanceInfo();

		void 	mcfn_setSignalingIP(char* pscL_IP)	          	{ meC_SignalingIp = pscL_IP; 			}
		void 	mcfn_setLastHeartBeatTime(const char* pscL_Time)  	{ strcpy(pmesc_LastHeartBeatTime, pscL_Time); 	}
		void 	mcfn_setRegistrationTime(const char* pscL_Time)  	{ strcpy(pmesc_RegistrationTime, pscL_Time); 	}
		void 	mcfn_setTotalResourceCount(const int& siL_Count)  	{ mesi_TotalResourceCount = siL_Count; 		}
		void 	mcfn_setInboundResourceCount(const int& siL_Count)	{ mesi_InboundResourceCount = siL_Count; 	}
		void 	mcfn_setOutboundResourceCount(const int& siL_Count) 	{ mesi_OutboundResourceCount = siL_Count; 	}
		void 	mcfn_setSignalingPort(const int& siL_Port)     		{ mesi_SignalingPort=siL_Port;		 	}
		void 	mcfn_setInstanceId(const int& siL_Id)           	{ mesi_InstanceId = siL_Id; 			}
		void 	mcfn_setHeartBeatMissCount(const int& siL_Count) 	{ mesi_HeartBeatMissCount = siL_Count; 		}
		void 	mcfn_setStatus(const char scL_Status)         		{ mesc_Status=scL_Status; 			}
		void 	mcfn_setInstanceName(const char* pscL_Name)      	{ strcpy(pmesc_InstanceName, pscL_Name); 	}
		void 	mcfn_setClientIP(const char* pscL_IP) 			{ strcpy(pmesc_ClientIP, pscL_IP);		}
		void 	mcfn_setClientPort(const long& slL_Port) 		{ mesl_ClientPort= slL_Port;			}


		// Getter Functions
		string	mcfn_getSignalingIP()			{ return meC_SignalingIp; 		}
		int	mcfn_getTotalResourceCount()            { return mesi_TotalResourceCount; 	}
		int	mcfn_getInboundResourceCount()          { return mesi_InboundResourceCount; 	}
		int	mcfn_getOutboundResourceCount()		{ return mesi_OutboundResourceCount; 	}
		int	mcfn_getSignalingPort()			{ return mesi_SignalingPort; 		}
		int	mcfn_getInstanceId()                    { return mesi_InstanceId; 		}
		int	mcfn_getHeartBeatMissCount()            { return mesi_HeartBeatMissCount; 	}
		char	mcfn_getStatus()                        { return mesc_Status; 			}
		char*	mcfn_getInstanceName()                  { return pmesc_InstanceName; 		}
		char*	mcfn_getRegistrationTime()             	{ return pmesc_RegistrationTime; 	}
		char*	mcfn_getLastHeartBeatTime()             { return pmesc_LastHeartBeatTime; 	}
		char*	mcfn_getClientIP()			{ return pmesc_ClientIP;		}
		long	mcfn_getClientPort()		 	{ return mesl_ClientPort;		}

		const	map<string,CInstanceServiceInfo*>& mcfn_getServiceMap()	{ return meC_ServiceInfoMap;	}

		void 	mcfn_incrementHeartBeatMissCount()			{ mesi_HeartBeatMissCount++;	}
		void 	mcfn_resetHeartBeatMissCount()				{ mesi_HeartBeatMissCount=0;	}

		void 	mcfn_deactivateInstance();
		void 	mcfn_activateInstance();
	
		bool 	mcfn_deactivateService(const string&);
		bool	mcfn_activateService(const string&);

		bool 	mcfn_insertService(const string&,char*,const string&,const int&,const char&);
		bool 	mcfn_deleteService(const string&);

		bool 	mcfn_getServiceDetailsFromServiceMap(const string&,char&,int&);

		bool 	mcfn_updateService(const string&,const int&);

		//TotalChannlBusyCount
		int     mcfn_getIBDBusyCount()          { return mesi_IBDBusyCount;     }
		int     mcfn_getOBDBusyCount()          { return mesi_OBDBusyCount;     }
		void    mcfn_setIBDBusyCount(const int& siL_Count)	{ mesi_IBDBusyCount = siL_Count;	}
		void    mcfn_setOBDBusyCount(const int& siL_Count)	{ mesi_OBDBusyCount = siL_Count;	}

		bool 	mcfn_checkAndIncrementIBDBusyCount();
		bool 	mcfn_checkAndDecrementIBDBusyCount();
		bool    mcfn_checkAndIncrementOBDBusyCount();
                bool    mcfn_checkAndDecrementOBDBusyCount();

		bool 	mcfn_findAndCheckForActiveService(const string&);




};

	inline 
void CInstanceInfo::mcfn_deactivateInstance()
{
	mesc_Status		= 'D';
	mesi_IBDBusyCount	= 0x00;
	mesi_OBDBusyCount	= 0x00;

	for( auto& lL_Itr : meC_ServiceInfoMap)
	{
		lL_Itr.second->mcfn_deActivateService();
	}
}

	inline
void CInstanceInfo::mcfn_activateInstance()
{
	mesc_Status		= 'A';
	mesi_IBDBusyCount       = 0x00;
        mesi_OBDBusyCount       = 0x00;
}
	inline 
bool CInstanceInfo::mcfn_deactivateService(const string& CL_ServiceType_Id)
{
	auto lL_Itr= meC_ServiceInfoMap.find(CL_ServiceType_Id);
	if(lL_Itr!=meC_ServiceInfoMap.end())
	{
		lL_Itr->second->mcfn_deActivateService();
		return true;
	}
	return false;
}
	inline 
bool CInstanceInfo::mcfn_activateService(const string& CL_ServiceType_Id)
{
	auto lL_Itr= meC_ServiceInfoMap.find(CL_ServiceType_Id);
	if(lL_Itr!=meC_ServiceInfoMap.end())
	{
		lL_Itr->second->mcfn_activateService();
		return true;
	}
	return false;
}

	inline
bool CInstanceInfo::mcfn_insertService(const string& CL_ServiceId,char* pscL_ServiceName,const string& CL_ServiceType,const int& siL_MaxLimit,const char& scL_Status = 'A')
{
	
	auto lL_Itr = meC_ServiceInfoMap.find(CL_ServiceType+"_"+CL_ServiceId);
	if(lL_Itr==meC_ServiceInfoMap.end())
	{
		CInstanceServiceInfo* pCL_ServiceInfo = new CInstanceServiceInfo();
		pCL_ServiceInfo->mcfn_setServiceId(CL_ServiceId);
		pCL_ServiceInfo->mcfn_setServiceName(pscL_ServiceName);
		pCL_ServiceInfo->mcfn_setServiceType(CL_ServiceType);
		pCL_ServiceInfo->mcfn_setMaxLimit(siL_MaxLimit);
		pCL_ServiceInfo->mcfn_setStatus(scL_Status);
		meC_ServiceInfoMap.insert({CL_ServiceType+"_"+CL_ServiceId,pCL_ServiceInfo});
		return true;
	}
	return false;
}

	inline
bool CInstanceInfo::mcfn_deleteService(const string& CL_ServiceType_Id)
{
	auto lL_Itr = meC_ServiceInfoMap.find(CL_ServiceType_Id);
	if(lL_Itr!=meC_ServiceInfoMap.end())
	{
		if(lL_Itr->second)
			delete lL_Itr->second;
		lL_Itr->second=nullptr;
		return true;
	}
	return false;

}
	inline
bool CInstanceInfo::mcfn_getServiceDetailsFromServiceMap(const string& CL_ServiceType_Id,char& scL_Status,int& siL_MaxLimit)
{
	auto lL_Itr = meC_ServiceInfoMap.find(CL_ServiceType_Id);
	if(lL_Itr!=meC_ServiceInfoMap.end())
	{
		scL_Status=lL_Itr->second->mcfn_getStatus();
		siL_MaxLimit=lL_Itr->second->mcfn_getMaxLimit();
		return true;
	}
	return false;
}
inline
bool CInstanceInfo::mcfn_updateService(const string& CL_ServiceType_Id,const int& siL_MaxLimit)
{
	auto lL_Itr = meC_ServiceInfoMap.find(CL_ServiceType_Id);
        if(lL_Itr!=meC_ServiceInfoMap.end())
        {
		lL_Itr->second->mcfn_setMaxLimit(siL_MaxLimit);
                return true;
        }
        return false;	
}
#endif
