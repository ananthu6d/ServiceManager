/************************************************************************
 *                                                                       *
 * The information described in this file is a confidential and          *
 * proprietary product of 6d Technologies.                               *
 * (c) 2020 6d Technologies                                              *
 * All Rights Reserved                                                   *
 *                                                                       *
 *************************************************************************
 *
 * ServiceHandler.cpp
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -----------------------------------------------
 *
 */


/**
 * Header Includes
 */

#include "ServiceHandler.h"
#include "AppExterns.h"
#include "UDPEventMonitor.h"
#include "DeRegisterServiceResourceSet.h"
#include "RegisterServiceResourceSet.h"
#include "UpdateServiceResourceSet.h"
#include "ServiceMasterSet.h"
#include "InstanceRegistry.h"
#include "ResourceCache.h"

/**
 * Global Variables
 */


/************************************************************************
 * Class     : CServiceHandler
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CServiceHandler::CServiceHandler()
{
	__entryFunction__;

	meC_ResourceMap.clear();
	meC_CurrentResource	= meC_ResourceMap.begin();
	mesi_AllotedCap 	= 0x00;
	mesc_Status		= 'A';

	__return__();
}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CServiceHandler::~CServiceHandler()
{
	__entryFunction__;
	__return__();
}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : mefn_initialize
 * Purpose   : initialise the object
 * Arguments : none
 * Returns   : none
 ************************************************************************/

void CServiceHandler::mefn_initialize()
{
	__entryFunction__;

	__return__();
}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : mcfn_printObject
 * Purpose   : print object
 * Arguments : none
 * Returns   : none
 ************************************************************************/

void CServiceHandler::mcfn_printObject()
{
	__entryFunction__;
	__return__();

}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : mcfn_subscribeEvent
 * Purpose   : subscribe for an event
 * Arguments : @param1: AppCallBack reference
 * 	     : @param2: EventCriteria reference
 * Returns   : none
 ************************************************************************/

void CServiceHandler::mcfn_subscribeEvent(IAppCallBack *,CEventCriteria *)
{
	__entryFunction__;
	__return__();
}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : mcfn_unsubscribeEvent
 * Purpose   : unsubscribe for an event
 * Arguments : @param1: AppCallBack reference
 *           : @param2: EventCriteria reference
 * Returns   : none
 ************************************************************************/

void CServiceHandler::mcfn_unsubscribeEvent(IAppCallBack *,CEventCriteria *)
{
	__entryFunction__;
	__return__();
}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : mcfn_onNotifyEvent
 * Purpose   : callback method for event reporting
 * Arguments : GenericEvent
 * Returns   : none
 ************************************************************************/

void CServiceHandler::mcfn_onNotifyEvent(CGenericEvent& CL_GenericEvent,long slL_Val)
{
	__entryFunction__;

	SEventInfo* pSL_EventInfo=(SEventInfo*) CL_GenericEvent.mcfn_getEventObject();

	if(pSL_EventInfo==nullptr)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "onNotifyEvent", "Failure", this,"","No Data to Process from EventInfo");
		return;
	}

	switch (pSL_EventInfo->mcsi_EventType)
	{
		case EVT_ADD_RESOURCE:
			{
				mefn_processAddResource((CServiceResource*)pSL_EventInfo->mcsl_EventInfo);
				break;
			}
		case EVT_LOAD_RESOURCE:
			{
				mefn_processLoadResource((CServiceResource*)pSL_EventInfo->mcsl_EventInfo);
				break;
			}
		case EVT_REMOVE_RESOURCE:
			{
				mefn_processRemoveResource(pSL_EventInfo->pmcsc_SignalingIpPort);
				break;
			}
		case EVT_ACT_RESOURCE:
			{
				mefn_processActivateService(pSL_EventInfo->pmcsc_SignalingIpPort);
				break;
			}
		case EVT_DEACT_RESOURCE:
			{

				mefn_processDeactivateService(pSL_EventInfo->pmcsc_SignalingIpPort);
				break;
			}
		case EVT_UPDATE_TOTALCHANNEL:
			{
				mefn_updateTotalChannelCount();
				break;
			}
		case EVT_RESET_BUSYCOUNT:
			{
				mefn_processResetBusyCount(pSL_EventInfo->pmcsc_SignalingIpPort);
				break;
			}


	}
	/*
	   case AddNewResource://dispatched from RegisterHandler
	   {
	   member_lock();
	   create and insert new resource with dummy value
	   }
	case ResetResourceCount://dispatched from RegisterHandler
	{
		member_lock();
		reset count to dummy value
	}
	case BlockService://dispatched from Monitor
	{
		member_lock();
		state = blocked;
	}

	case UnblockService://dispatched from Monitor
	{
		member_lock();
		state = Active;
		recalculate resource for entire service
	}

	case calculateResource://dispatched from RegisterHandler
CL_ServiceKey	{
		member_lock();
		recalculate resource for entire service
	}

	case purge resource:
	{
		member_lock();

		pCL_ServiceResource = meC_ResourceMap.find(instance_id);

		if(meC_CurrentItr == meC_ResourceMap.find(instance_id))
			++meC_CurrentItr;

		meC_ResourceMap.erase(instance_id);
		member_unlock();

		pCL_ServiceResource->mcfn_deleteFromCache();
		delete from SERVICE_INSTANCE_MAPPING where status = 'D' and INSTANCE_ID == instance_id AND SERVICE_ID == serviceId;
		delete pCL_ServiceResource;

	}
	case EVT_ADD_RESOURCE:		//add a resource 
	case EVT_UPDATE_RESOURCE:	//update resource
	case EVT_DEACT_RESOURCE:	//deactivate a resource
	case EVT_ACT_RESOURCE:		//activate a resource

*/

	__return__();
}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : mefn_processAddResource
 * Purpose   : Add new resource
 * Arguments : CServiceResource*
 * Returns   : None
 ************************************************************************/

void CServiceHandler::mefn_processAddResource(CServiceResource* pCL_ServiceResource)
{
	__entryFunction__;
	lock_guard<mutex> CL_Lock(meC_ResourceMutex);
	auto lL_Itr = meC_ResourceMap.find(pCL_ServiceResource->mcfn_getSignalingIP()+":"+to_string(pCL_ServiceResource->mcfn_getSignalingPort()));
	if(lL_Itr!=meC_ResourceMap.end())
	{
		
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processAddResource", "Failure", this,"", "Instance:%d,SignalingIpPort:%s:%ld is Already Registered",pCL_ServiceResource->mcfn_getInstanceId(),pCL_ServiceResource->mcfn_getSignalingIP().c_str(),pCL_ServiceResource->mcfn_getSignalingPort());

		if(pCL_ServiceResource)
			delete pCL_ServiceResource;
		pCL_ServiceResource=nullptr;
		__return__();	

	}


	meC_ResourceMap.insert({pCL_ServiceResource->mcfn_getSignalingIP()+":"+to_string(pCL_ServiceResource->mcfn_getSignalingPort()),pCL_ServiceResource});

	pCL_ServiceResource->mcfn_InsertIntoExternalCache();

	CRegisterServiceResourceSet CL_RegisterServiceResourceSet;

	while(1)
	{
		if(CL_RegisterServiceResourceSet.mcfn_insertIntoServiceInstanceMapping(mesi_ServiceId,pCL_ServiceResource->mcfn_getInstanceId(),pCL_ServiceResource->mcfn_getSignalingIP()+":"+to_string(pCL_ServiceResource->mcfn_getSignalingPort()),0,meC_ServiceType.c_str()))
		{
			break;
		}
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processAddResource", "Failure", this,"", "ServiceId:%d and InstanceId :%d",mesi_ServiceId,pCL_ServiceResource->mcfn_getInstanceId());
		CL_RegisterServiceResourceSet.mcfn_reInitailizeDB();
		usleep(DBFAILURESLEEPDUR);
	}


	//mefn_calculateResourceCount(pCL_ServiceResource->mcfn_getSignalingIP()+":"+to_string(pCL_ServiceResource->mcfn_getSignalingPort()));

	__return__();
}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : mefn_processLoadResource
 * Purpose   : load resources
 * Arguments : CServiceResource*
 * Returns   : None
 ************************************************************************/

void CServiceHandler::mefn_processLoadResource(CServiceResource* pCL_ServiceResource)
{
	__entryFunction__;
	lock_guard<mutex> CL_Lock(meC_ResourceMutex);
	auto lL_Itr = meC_ResourceMap.find(pCL_ServiceResource->mcfn_getSignalingIP()+":"+to_string(pCL_ServiceResource->mcfn_getSignalingPort()));
	if(lL_Itr!=meC_ResourceMap.end())
	{

		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processLoadResource", "Failure", this,"", "Instance:%d,SignalingIpPort:%s:%ld is Already Registered",pCL_ServiceResource->mcfn_getInstanceId(),pCL_ServiceResource->mcfn_getSignalingIP().c_str(),pCL_ServiceResource->mcfn_getSignalingPort());	

		if(pCL_ServiceResource)
			delete pCL_ServiceResource;
		pCL_ServiceResource=nullptr;
		__return__();	

	}
	
	meC_ResourceMap.insert({pCL_ServiceResource->mcfn_getSignalingIP()+":"+to_string(pCL_ServiceResource->mcfn_getSignalingPort()),pCL_ServiceResource});

	pCL_ServiceResource->mcfn_InsertIntoExternalCache();

	//mefn_calculateResourceCount(pCL_ServiceResource->mcfn_getSignalingIP()+":"+to_string(pCL_ServiceResource->mcfn_getSignalingPort()));

	__return__();
}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : mefn_processRemoveResource
 * Purpose   : remove a resource
 * Arguments : string CL_SignalingIpPort
 * Returns   : None
 ************************************************************************/

void CServiceHandler::mefn_processRemoveResource(string CL_SignalingIpPort)
{
	__entryFunction__;
	lock_guard<mutex> CL_Lock(meC_ResourceMutex);
	auto lL_Itr = meC_ResourceMap.find(CL_SignalingIpPort);
	if(lL_Itr!=meC_ResourceMap.end())
	{

		//lL_Itr->second->mcfn_deActivateResource();
		//lL_Itr->second->mcfn_resetAllotedResourceCap();
		while(1)
		{
			CDeRegisterServiceResourceSet CL_DeRegisterServiceResourceSet;
			if(CL_DeRegisterServiceResourceSet.mcfn_deRegisterServiceResource(CL_SignalingIpPort,mesi_ServiceId))
			{
				break;
			}
			usleep(DBFAILURESLEEPDUR);
		}
		//mefn_calculateResourceCount(CL_SignalingIpPort);
		__return__();

	}
	EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processRemoveResource", "Failure", this,"", "Resource Not Found with SignalingIpPort:%s",CL_SignalingIpPort.c_str());
	__return__();

}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : mefn_processActivateService
 * Purpose   : activate Service
 * Arguments : meC_SignalingIpPort
 * Returns   : None
 ************************************************************************/

void CServiceHandler::mefn_processActivateService(string CL_SignalingIpPort)
{
	__entryFunction__;
	lock_guard<mutex> CL_Lock(meC_ResourceMutex);

	auto lL_Itr = meC_ResourceMap.find(CL_SignalingIpPort);
	if(lL_Itr!=meC_ResourceMap.end())
	{
		lL_Itr->second->mcfn_activateAndResetResource();

		while(1)
		{
			CUpdateServiceResourceSet CL_UpdateServiceResourceSet;
			if(CL_UpdateServiceResourceSet.mcfn_activateResource(CL_SignalingIpPort,mesi_ServiceId))
				break;
		}

		//mefn_calculateResourceCount(CL_SignalingIpPort);
		__return__();
	}
	EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processActivateService", "Failure", this,"", "Resource Not Found with InstanceId :%d",CL_SignalingIpPort);
	__return__();

}

/************************************************************************
 * Class     : CServiceHandler
 * Method    : mefn_processDeactivateService
 * Purpose   : deactivate service
 * Arguments : string meC_SignalingIpPort
 * Returns   : None
 ************************************************************************/

void CServiceHandler::mefn_processDeactivateService(string CL_SignalingIpPort)
{
	__entryFunction__;
	lock_guard<mutex> CL_Lock(meC_ResourceMutex);

	auto lL_Itr = meC_ResourceMap.find(CL_SignalingIpPort);
	if(lL_Itr!=meC_ResourceMap.end())
	{
		//lL_Itr->second->mcfn_deActivateResource();
		//lL_Itr->second->mcfn_resetAllotedResourceCap();
	}

	//mefn_calculateResourceCount(CL_SignalingIpPort);
	__return__();
}

bool CServiceHandler::mcfn_fetchResource(char* pscL_SignalingIp,long& slL_SignalingPort,int& siL_InstanceId,int& siL_ErrorCode)
{
	__entryFunction__;
	lock_guard<mutex> CL_Lock(meC_ResourceMutex);

	if(meC_ResourceMap.empty())
	{
		siL_ErrorCode = ER_NO_INSTANCE_REG;
		__return__(false);
	}


	/*
	if(!CResourceCache::mcfn_getInstance()->mcfn_incrementBusyCount(meC_ServiceType+"_"+to_string(mesi_ServiceId),mesi_LicenseCap))
	{
		siL_ErrorCode = ER_RESOURCE_BUSY;
		__return__(false);	
	}

	*/
	int siL_ResourceCount = meC_ResourceMap.size();

	bool bL_IsInstanceAvailable = false;
	while(siL_ResourceCount--)
	{
		if(meC_CurrentResource == meC_ResourceMap.end())
		{
			meC_CurrentResource = meC_ResourceMap.begin();
		}

		if(meC_CurrentResource->second->mcfn_getStatus()=='A')
		{
			bL_IsInstanceAvailable = true;
			if(meC_CurrentResource->second->mcfn_getNextResource(pscL_SignalingIp,slL_SignalingPort,siL_InstanceId))
			{
				++meC_CurrentResource;
				__return__(true);
			}
		}
		++meC_CurrentResource;
	}

	if(!bL_IsInstanceAvailable)
	{
		siL_ErrorCode = ER_NO_RESOURCES_AVAILABLE;
		__return__(false);
	}
	siL_ErrorCode = ER_RESOURCE_BUSY;
	__return__(false);

}
bool CServiceHandler::mcfn_releaseResource(string CL_SignalingIpPort,int& siL_ErrorCode)
{

        lock_guard<mutex> CL_Lock(meC_ResourceMutex);
	auto lL_Itr = meC_ResourceMap.find(CL_SignalingIpPort);
	if(lL_Itr == meC_ResourceMap.end())
	{
		siL_ErrorCode = ER_NO_INSTANCE_REG;
		return false;
	}

	if(!lL_Itr->second->mcfn_releaseResource())
	{
		siL_ErrorCode = ER_RELEASE_FALIURE;
		return false;
	}
	//CResourceCache::mcfn_getInstance()->mcfn_decrementBusyCount(meC_ServiceType+"_"+to_string(mesi_ServiceId),CL_SignalingIpPort);
	return true;
}

void CServiceHandler::mefn_processResetBusyCount(string CL_SignalingIpPort)
{

        lock_guard<mutex> CL_Lock(meC_ResourceMutex);
        auto lL_Itr = meC_ResourceMap.find(CL_SignalingIpPort);
        if(lL_Itr != meC_ResourceMap.end())
        {
        	lL_Itr->second->mcfn_resetBusyCountInExternalCache();
	}
}

void CServiceHandler::mefn_updateTotalChannelCount()
{

	lock_guard<mutex> CL_Lock(meC_ResourceMutex);
	if(meC_ServiceType=="IBD")
	{
		mesi_LicenseCap = mef_LicenseCapPercentage * CInstanceRegistry::mcfn_getInstance()->mcfn_getIBDTotalChannelCount();
	}
	else
	{
		mesi_LicenseCap = mef_LicenseCapPercentage * CInstanceRegistry::mcfn_getInstance()->mcfn_getIBDTotalChannelCount();
	}
}
