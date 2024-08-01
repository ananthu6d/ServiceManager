/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.                             *
 *  (c) 2008 6d Technologies                                            *
 *  All Rights Reserved                                                 *
 *                                                                      *
 ************************************************************************
 *
 *  ServiceMaster.cpp
 *
 *
 * Rev Date      User      Comments
: * --- --------- --------  -------------------------------------------------------
 *   1 24Sep08   RKM       Created
 */



/**
* Header Includes
*/

#include "AppExterns.h"
#include "AppDefines.h"
#include "ServiceMaster.h"
#include "ResourceCache.h"
#include "InstanceRegistry.h"

using namespace _6d;
/**
 * Initializing the Singleton Reference Object to NULL
 */
CServiceMaster *CServiceMaster::pmeC_ServiceMaster = nullptr;

/************************************************************************
* Class     : CServiceMaster                                       	  
* Method    : Constructor                                               
* Purpose   : Creates Object and initializes its variables              
* Arguments : Nil                                                       
* Returns   : Nil                                                       
*************************************************************************/

CServiceMaster::CServiceMaster()
{
	__entryFunction__;

	__return__();
}

/************************************************************************
* Class     : CServiceMaster                                         
* Method    : Destructor                                                
* Purpose   : releases its variables and destroys the object            
* Arguments : Nil                                                       
* Returns   : Nil                                                       
*************************************************************************/

CServiceMaster::~CServiceMaster()
{
	__entryFunction__;

	__return__();
}

/************************************************************************
* Class     : CServiceMaster
* Method    : mcfn_insertServiceHandler
* Purpose   : insert ServiceHandler
* Arguments : const string& CL_ServiceId_Type,CServiceHandler* pCL_ServiceHandler
* Returns   : Success/Failure
*************************************************************************/

bool CServiceMaster::mcfn_insertServiceHandler(const string& CL_ServiceId_Type,CServiceHandler* pCL_ServiceHandler)
{
	lock_guard<std::mutex> CL_Lock(meC_ServiceMasterLock);
	auto lL_Itr = meC_ServiceHandlerMap.find(CL_ServiceId_Type);
	if(lL_Itr == meC_ServiceHandlerMap.end())
	{
		if(meC_ServiceHandlerMap.insert({CL_ServiceId_Type,pCL_ServiceHandler}).second)
		{
			return true;
		}
	}
	return false;	
}

/************************************************************************
* Class     : CServiceMaster
* Method    : mcfn_getServiceHandler
* Purpose   : get serviceHandler
* Arguments : const string& CL_ServiceId_Type,CServiceHandler** pCL_ServiceHandler
* Returns   : Success/Failure
*************************************************************************/

bool CServiceMaster::mcfn_getServiceHandler(const string& CL_ServiceId_Type,CServiceHandler** pCL_ServiceHandler)
{
	lock_guard<std::mutex> CL_Lock(meC_ServiceMasterLock);
	auto lL_Itr = meC_ServiceHandlerMap.find(CL_ServiceId_Type);
	if(lL_Itr != meC_ServiceHandlerMap.end())
	{
		*pCL_ServiceHandler = lL_Itr->second;
		return true;
	}
	return false;

}

/************************************************************************
* Class     : CServiceMaster
* Method    : mcfn_isServiceHandlerExist
* Purpose   : check the existance of ServiceHandler
* Arguments : const string& siL_ServiceI
* Returns   : Success/Failure
*************************************************************************/

bool CServiceMaster::mcfn_isServiceHandlerExist(const string& CL_ServiceId_Type)
{
	lock_guard<std::mutex> CL_Lock(meC_ServiceMasterLock);
	auto lL_Itr = meC_ServiceHandlerMap.find(CL_ServiceId_Type);
	if(lL_Itr != meC_ServiceHandlerMap.end())
	{
		return true;
	}
	return false;
}

/************************************************************************
* Class     : CServiceMaster
* Method    : mcfn_deleteServiceHandler
* Purpose   : delete ServiceHandler
* Arguments : CL_ServiceId_Type
* Returns   : Success/Failure
*************************************************************************/

bool CServiceMaster::mcfn_deleteServiceHandler(const string& CL_ServiceId_Type)
{
	lock_guard<std::mutex> CL_Lock(meC_ServiceMasterLock);
	auto lL_Itr = meC_ServiceHandlerMap.find(CL_ServiceId_Type);
	if(lL_Itr != meC_ServiceHandlerMap.end())
	{
		if(lL_Itr->second)
		{
			delete lL_Itr->second;
			lL_Itr->second=nullptr;
		}
		meC_ServiceHandlerMap.erase(CL_ServiceId_Type);
		return true;
	}
	return false;
}

/************************************************************************
 * Class     : CServiceMaster
 * Method    : mcfn_getServiceNameAnd
 * Purpose   : get ServieName and Type
 * Arguments : CL_ServiceId_Type,char* pscL_ServiceName
 * Returns   : Success/Failure
 *************************************************************************/

bool CServiceMaster::mcfn_getServiceName(const string& CL_ServiceId_Type,char* pscL_ServiceName)
{
	lock_guard<std::mutex> CL_Lock(meC_ServiceMasterLock);
	auto lL_Itr = meC_ServiceHandlerMap.find(CL_ServiceId_Type);
	if(lL_Itr != meC_ServiceHandlerMap.end())
	{
		strcpy(pscL_ServiceName,lL_Itr->second->mcfn_getServiceName());
		return true;
	}
	return false;	
}

bool CServiceMaster::mcfn_loadTotalBusyCount()
{
	__entryFunction__;
	unordered_map<string,string> CL_InstanceBusyMap;
	map<string,int> CL_TotalIBDInstanceBusyCountMap;
	map<string,int> CL_TotalOBDInstanceBusyCountMap;
	int siL_ServiceTotal = 0x00;
	for(const auto& lL_ServiceItr : meC_ServiceHandlerMap)
	{
		siL_ServiceTotal = 0x00;
		if(CResourceCache::mcfn_getInstance()->mcfn_getHashMapFromResourceCache(lL_ServiceItr.first,CL_InstanceBusyMap))
		{
			for (const auto& lL_InstanceItr : CL_InstanceBusyMap) 
			{
				siL_ServiceTotal += atoi(lL_InstanceItr.second.c_str());
				if(lL_ServiceItr.second->mcfn_getServiceType() == "IBD")
				{
					CL_TotalIBDInstanceBusyCountMap[lL_InstanceItr.first] += atoi(lL_InstanceItr.second.c_str());
				}
				else
				{
					CL_TotalOBDInstanceBusyCountMap[lL_InstanceItr.first] += atoi(lL_InstanceItr.second.c_str());
				}

				lL_ServiceItr.second->mcfn_setResourceBusyCount(lL_InstanceItr.first,atoi(lL_InstanceItr.second.c_str()));
				EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "mcfn_loadTotalBusyCount", "Resource", this,"", "ServiceTypeId:%s,SiganalingIpPort:%s,TotalResourceBusyCount:%s",lL_ServiceItr.first.c_str(),lL_InstanceItr.first.c_str(),lL_InstanceItr.second.c_str());

			}
			EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "mcfn_loadTotalBusyCount", "Service", this,"", "ServiceTypeId:%s,TotalBusyCount:%d",lL_ServiceItr.first.c_str(),siL_ServiceTotal);
			lL_ServiceItr.second->mcfn_setTotalServiceBusyCount(siL_ServiceTotal);
		}	
	}

	for(const auto& lL_InstanceItr : CL_TotalIBDInstanceBusyCountMap)
	{
		if(!CInstanceRegistry::mcfn_getInstance()->mcfn_setOBDTotalChannelCount(lL_InstanceItr.first,lL_InstanceItr.second))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "mcfn_loadTotalBusyCount", "Set Failure", this,"", "SignalingIpPort:%s,OBDTotalChannelCount:%d",lL_InstanceItr.first.c_str(),lL_InstanceItr.second);
		}
		EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "mcfn_loadTotalBusyCount", "Instance", this,"", "SignalingIpPort:%s,OBDTotalChannelCount:%d",lL_InstanceItr.first.c_str(),lL_InstanceItr.second);
	}

	for(const auto& lL_InstanceItr : CL_TotalIBDInstanceBusyCountMap)
        {
                if(!CInstanceRegistry::mcfn_getInstance()->mcfn_setIBDTotalChannelCount(lL_InstanceItr.first,lL_InstanceItr.second))
                {
                        EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "mcfn_loadTotalBusyCount", "Set Failure", this,"", "SignalingIpPort:%s,IBDTotalChannelCount:%d",lL_InstanceItr.first.c_str(),lL_InstanceItr.second);
                }
		EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "mcfn_loadTotalBusyCount", "Instance", this,"", "SignalingIpPort:%s,IBDTotalChannelCount:%d",lL_InstanceItr.first.c_str(),lL_InstanceItr.second);
        }
	__return__(true);	
}
