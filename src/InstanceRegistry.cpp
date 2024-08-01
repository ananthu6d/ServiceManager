/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.                             *
 *  (c) 2008 6d Technologies                                            *
 *  All Rights Reserved                                                 *
 *                                                                      *
 ************************************************************************
 *
 *  InstanceRegistry.cpp
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
#include "InstanceRegistry.h"
#include "ServiceMaster.h"
#include "UDPEventMonitor.h"
using namespace _6d;
/**
 * Initializing the Singleton Reference Object to NULL
 */
CInstanceRegistry *CInstanceRegistry::pmeC_InstanceRegistry = nullptr;

/************************************************************************
* Class     : CInstanceRegistry                                         
* Method    : Constructor                                               
* Purpose   : Creates Object and initializes its variables              
* Arguments : Nil                                                       
* Returns   : Nil                                                       
*************************************************************************/

CInstanceRegistry::CInstanceRegistry()
{
	__entryFunction__;

	pmeC_Registry = new std::map<string,CInstanceInfo*>;
	mesi_IBDTotalChannelCount = 0x00;
	mesi_OBDTotalChannelCount = 0x00;
	__return__();
}

/************************************************************************
* Class     : CInstanceRegistry                                         
* Method    : Destructor                                                
* Purpose   : releases its variables and destroys the object            
* Arguments : Nil                                                       
* Returns   : Nil                                                       
*************************************************************************/

CInstanceRegistry::~CInstanceRegistry()
{
	__entryFunction__;

	if(pmeC_Registry)
		delete pmeC_Registry;
	pmeC_Registry = nullptr;

	__return__();
}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : mcfn_insertInstanceInfo
* Purpose   : insert InstanceInfo
* Arguments : const string& CL_SignalingIpPort,CInstanceInfo& CL_InstanceInfo
* Returns   : Success/Failure
*************************************************************************/

bool CInstanceRegistry::mcfn_insertInstanceInfo(const string& CL_SignalingIpPort,CInstanceInfo& CL_InstanceInfo)
{
	lock_guard<mutex> CL_Lock(meC_RegistryLock);
	auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
	if(lL_Itr == pmeC_Registry->end())
	{
		if(pmeC_Registry->insert({CL_SignalingIpPort,&CL_InstanceInfo}).second)
		{
			//mesi_IBDTotalChannelCount += pCL_InstanceInfo->mcfn_getInboundResourceCount();
			return true;
		}
	}
	return false;	
}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : mcfn_isInstanceInfoExist
* Purpose   : Is Instance Exists
* Arguments : const string& CL_SignalingIpPort
* Returns   : Success/Failure
*************************************************************************/

bool CInstanceRegistry::mcfn_isInstanceInfoExist(const string& CL_SignalingIpPort)
{
        lock_guard<mutex> CL_Lock(meC_RegistryLock);
        auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
        if(lL_Itr != pmeC_Registry->end())
        {
                        return true;
        }
        return false;
}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : mcfn_insertInstanceInfo
* Purpose   : insert InstanceInfo
* Arguments : const string& CL_SignalingIpPort,CInstanceInfo* CL_InstanceInfo
* Returns   : Success/Failure
*************************************************************************/

bool CInstanceRegistry::mcfn_insertInstanceInfo(const string& CL_SignalingIpPort, CInstanceInfo* pCL_InstanceInfo)
{
	lock_guard<mutex> CL_Lock(meC_RegistryLock);
        auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
        if(lL_Itr == pmeC_Registry->end())
        {
                if(pmeC_Registry->insert({CL_SignalingIpPort,pCL_InstanceInfo}).second)
                {
			mesi_IBDTotalChannelCount += pCL_InstanceInfo->mcfn_getInboundResourceCount();	
                        return true;
                }
        }
        return false;
}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : mcfn_getInstanceInfo
* Purpose   : get InstanceInfo
* Arguments : const string& CL_SignalingIpPort,CInstanceInfo& CL_InstanceInfo
* Returns   : Success/Failure
*************************************************************************/

bool CInstanceRegistry::mcfn_getInstanceInfo(const string& CL_SignalingIpPort, CInstanceInfo& CL_InstanceInfo)
{
	{
		lock_guard<mutex> CL_Lock(meC_RegistryLock);
		auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
		if(lL_Itr != pmeC_Registry->end())
		{
			CL_InstanceInfo = *(lL_Itr->second);
			return true;
		}
	}
	return false;

}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : mcfn_getInstanceInfo
* Purpose   : get InstanceInfo
* Arguments : const string& CL_SignalingIpPort, CInstanceInfo** pCL_Instance
* Returns   : Success/Failure
*************************************************************************/

bool CInstanceRegistry::mcfn_getInstanceInfo(const string& CL_SignalingIpPort, CInstanceInfo** pCL_Instance)
{
	{
		lock_guard<mutex> CL_Lock(meC_RegistryLock);
		auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
		if(lL_Itr != pmeC_Registry->end())
		{

			*pCL_Instance = lL_Itr->second;
			return true;
		}
	}
	return false;

}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : Destructor
* Purpose   : remove InstanceInfo
* Arguments : const string& CL_SignalingIpPort
* Returns   : None
*************************************************************************/

void CInstanceRegistry::mcfn_eraseInstanceInfo(const string& CL_SignalingIpPort)
{
	{
		lock_guard<mutex> CL_Lock(meC_RegistryLock);
                auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
                if(lL_Itr != pmeC_Registry->end())
                {
                        if(lL_Itr->second)
			{
				delete lL_Itr->second;
				lL_Itr->second=nullptr;
			}
			pmeC_Registry->erase(CL_SignalingIpPort);
                        return true;
                }
        }
        return false;
}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : Destructor
* Purpose   : deactivate InstanceInfo
* Arguments : const string& CL_SignalingIpPort
* Returns   : Success/Failure
*************************************************************************/

bool CInstanceRegistry::mcfn_deActivateInstanceInfo(const string& CL_SignalingIpPort)
{
	lock_guard<mutex> CL_Lock(meC_RegistryLock);
	auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
	if(lL_Itr != pmeC_Registry->end())
	{
		mesi_IBDTotalChannelCount -= lL_Itr->second->mcfn_getInboundResourceCount();
		lL_Itr->second->mcfn_deactivateInstance();
		return true;
	}
	return false;
}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : mcfn_activateInstanceInfo
* Purpose   : activate InstanceInfo
* Arguments : const string& CL_SignalingIpPort
* Returns   : Success/Failure
*************************************************************************/

bool CInstanceRegistry::mcfn_activateInstanceInfo(const string& CL_SignalingIpPort)
{
	lock_guard<mutex> CL_Lock(meC_RegistryLock);
        auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
        if(lL_Itr != pmeC_Registry->end())
        {
		mesi_IBDTotalChannelCount += lL_Itr->second->mcfn_getInboundResourceCount();
                lL_Itr->second->mcfn_activateInstance();
                return true;
	}
	return false;
}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : mcfn_deActivateService
* Purpose   : deactivate service
* Arguments : const string& CL_SignalingIpPort,const stringconst string& CL_ServiceType_Id
* Returns   : Success/Failure
*************************************************************************/

bool CInstanceRegistry::mcfn_deActivateService(const string& CL_SignalingIpPort,const string& CL_ServiceType_Id)
{
	lock_guard<mutex> CL_Lock(meC_RegistryLock);
	auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
	if(lL_Itr != pmeC_Registry->end())
	{
		if(lL_Itr->second->mcfn_deactivateService(CL_ServiceType_Id))
			return true;
	}
	return false;
}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : mcfn_activateService
* Purpose   : activate Service
* Arguments : const string& CL_SignalingIpPort,const string& CL_ServiceType_Id
* Returns   : Success/Failure
*************************************************************************/

bool CInstanceRegistry::mcfn_activateService(const string& CL_SignalingIpPort,const string& CL_ServiceType_Id)
{
	lock_guard<mutex> CL_Lock(meC_RegistryLock);
	auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
	if(lL_Itr != pmeC_Registry->end())
	{
		if(lL_Itr->second->mcfn_activateService(CL_ServiceType_Id))
			return true;
	}
        return false;
}
bool CInstanceRegistry::mcfn_eraseService(const string& CL_SignalingIpPort, const string& CL_ServiceType_Id)
{
}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : mcfn_addService
* Purpose   : add Service
* Arguments : const string& CL_SignalingIpPort, const int& CL_ServiceTypeId, const int& siL_MaxLimit
* Returns   : Success/Failure
*************************************************************************/
bool CInstanceRegistry::mcfn_addService(const string& CL_SignalingIpPort, const string& CL_ServiceId,const string& CL_ServiceType,const int& siL_MaxLimit,const int& siL_AllotedCap,const char& scL_Status)
{
	lock_guard<mutex> CL_Lock(meC_RegistryLock);
	auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
	if(lL_Itr != pmeC_Registry->end())
	{
		char pscL_ServiceName[30];
		pscL_ServiceName[0] = 0x00;
		if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceName(CL_ServiceType+"_"+CL_ServiceId,pscL_ServiceName))
		{
			return false;
                }
		if(lL_Itr->second->mcfn_insertService(CL_ServiceId,pscL_ServiceName,CL_ServiceType,siL_MaxLimit,scL_Status))
                        return true;
        }
        return false;
}

/************************************************************************
* Class     : CInstanceRegistry
* Method    : mcfn_updateService
* Purpose   : update Service
* Arguments : const string& CL_SignalingIpPort, const string& CL_ServiceType_Id, const int& siL_MaxLimit
* Returns   : Success/Failure
*************************************************************************/

bool CInstanceRegistry::mcfn_updateService(const string& CL_SignalingIpPort, const string& CL_ServiceType_Id, const int& siL_MaxLimit)
{
	lock_guard<mutex> CL_Lock(meC_RegistryLock);
        auto lL_Itr = pmeC_Registry->find(CL_SignalingIpPort);
        if(lL_Itr != pmeC_Registry->end())
        {
                if(lL_Itr->second->mcfn_updateService(CL_ServiceType_Id,siL_MaxLimit))
                        return true;
	}
	return false;
}

bool CInstanceRegistry::mcfn_updateClientAddress(const string& CL_SignalingIpPort, char* pscL_IpAddress, const int& siL_Port)
{
}

bool CInstanceRegistry::mcfn_loadIntoServiceResource()
{
	lock_guard<mutex> CL_Lock(meC_RegistryLock);
	for(const auto& lL_InstanceItr : *pmeC_Registry)
	{
		for(const auto& lL_ServiceItr : lL_InstanceItr.second->mcfn_getServiceMap())
		{
			CServiceHandler* pCL_ServiceHandler = nullptr;
			if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(lL_ServiceItr.first,&pCL_ServiceHandler))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,"","No Handler Found for SericeId:%d",lL_ServiceItr.first);
				return false;
			}

			CServiceResource* pCL_ServiceResource= new CServiceResource();
			pCL_ServiceResource->mcfn_setSignalingIP(lL_InstanceItr.second->mcfn_getSignalingIP().c_str());
			pCL_ServiceResource->mcfn_setSignalingPort(lL_InstanceItr.second->mcfn_getSignalingPort());
			pCL_ServiceResource->mcfn_setInstanceId(lL_InstanceItr.second->mcfn_getInstanceId());

			pCL_ServiceResource->mcfn_setStatus(lL_ServiceItr.second->mcfn_getStatus());

			//pCL_ServiceResource->mcfn_setMaxResourceCap(lL_ServiceItr.second->mcfn_getMaxLimit());
			pCL_ServiceResource->mcfn_setServiceId(atoi(lL_ServiceItr.second->mcfn_getServiceId().c_str()));
			pCL_ServiceResource->mcfn_setServiceType(lL_ServiceItr.second->mcfn_getServiceType());

			CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_LOAD_RESOURCE,(long)pCL_ServiceResource);

		}
	}
	return true;
}
	
void CInstanceRegistry::mcfn_incrementTotalChannelCount(const CInstanceInfo* pCL_InstanceInfo)
{
	for(const auto& lL_Itr : pCL_InstanceInfo->mcfn_getServiceMap())
	{
		if(lL_Itr.second->mcfn_getServiceType() == "IBD")
		{
			mesi_IBDTotalChannelCount += pCL_InstanceInfo->mcfn_getInboundResourceCount();
		}
		else
		{
			mesi_OBDTotalChannelCount += pCL_InstanceInfo->mcfn_getOutboundResourceCount();
		}
	}

}
void CInstanceRegistry::mcfn_decrementTotalChannelCount(const CInstanceInfo* pCL_InstanceInfo)
{
        for(const auto& lL_Itr : pCL_InstanceInfo->mcfn_getServiceMap())
        {
                if(lL_Itr.second->mcfn_getServiceType() == "IBD")
		{
			mesi_IBDTotalChannelCount -= pCL_InstanceInfo->mcfn_getInboundResourceCount();
		}
		else
		{
			mesi_OBDTotalChannelCount -= pCL_InstanceInfo->mcfn_getOutboundResourceCount();
		}
	}
}

bool CInstanceRegistry::mcfn_updateTotalChannelCount()
{
	lock_guard<mutex> CL_Lock(meC_RegistryLock);
	mesi_OBDTotalChannelCount = 0x00;
	mesi_IBDTotalChannelCount = 0x00;
	for(const auto& lL_InstanceItr : *pmeC_Registry)
	{
		if(lL_InstanceItr.second->mcfn_getStatus() == 'A')
		{
			for(const auto& lL_Itr : lL_InstanceItr.second->mcfn_getServiceMap())
			{
				if(lL_Itr.second->mcfn_getStatus() == 'A')
				{
					if(lL_Itr.second->mcfn_getServiceType() == "IBD")
					{
						mesi_IBDTotalChannelCount += lL_InstanceItr.second->mcfn_getInboundResourceCount();
					}
					else
					{
						mesi_OBDTotalChannelCount += lL_InstanceItr.second->mcfn_getOutboundResourceCount();
					}
				}
			}
		}	
	}
	EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "InstanceRegistry", "updateTotalChannelCount", this,"","IBDTotalChannelCount:%d, OBDTotalChannelCount:%d",mesi_IBDTotalChannelCount,mesi_OBDTotalChannelCount);
	return true;
}


