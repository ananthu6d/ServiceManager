/************************************************************************
 *                                                                       *
 * The information described in this file is a confidential and          *
 * proprietary product of 6d Technologies.                               *
 * (c) 2020 6d Technologies                                              *
 * All Rights Reserved                                                   *
 *                                                                       *
 *************************************************************************
 *
 * ServiceResource.cpp
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -----------------------------------------------
 *
 */


/**
 * Header Includes
 */

#include "ServiceResource.h"
#include "AppExterns.h"
#include "UDPEventMonitor.h"
#include "DeRegisterServiceResourceSet.h"
#include "RegisterServiceResourceSet.h"
#include "UpdateServiceResourceSet.h"
#include "ServiceMasterSet.h"
#include "ResourceCache.h"

/**
 * Global Variables
 */

/************************************************************************
 * Class     : CServiceResource
 * Method    : mefn_initialize
 * Purpose   : initialise the object
 * Arguments : none
 * Returns   : none
 ************************************************************************/
bool CServiceResource:: mcfn_getNextResource(char* pscL_Ip,long& slL_Port, int& siL_InstanceId)
{
	
	/*
	if(!CResourceCache::mcfn_getInstance()->mcfn_incrementBusyCount(meC_ServiceType+"_"+to_string(mesi_ServiceId),meC_SignalingIP+":"+to_string(mesl_SignalingPort),mesi_TotalChannel))
	{
		return false;
	}
	*/

	strcpy(pscL_Ip,meC_SignalingIP.c_str());
	slL_Port        = mesl_SignalingPort;
	siL_InstanceId  = mesi_InstanceId;
	return true;
}

bool CServiceResource::mcfn_releaseResource()
{

	return true;
	/*
	return CResourceCache::mcfn_getInstance()->mcfn_decrementBusyCount(meC_ServiceType+"_"+to_string(mesi_ServiceId),meC_SignalingIP+":"+to_string(mesl_SignalingPort));

	int siL_CurrentBusyResourceCount = 0x00;

	if(!CResourceCache::mcfn_getInstance()->mcfn_fetchResourceCache(meC_SignalingIP+":"+to_string(mesl_SignalingPort),mesi_ServiceId,siL_CurrentBusyResourceCount))
	{
		return false;
	}

	if(siL_CurrentBusyResourceCount <= 0x00)
	{
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "releaseResource","Release", this,"","Resource overflowed InstanceId:%d, ServiceId:%d,AllotedResorceCap:%d, ResourceBusyCount:%d",mesi_InstanceId,mesi_ServiceId,mesi_AllotedResourceCap,siL_CurrentBusyResourceCount);
		return false;
	}

	EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "releaseResource","Release", this,"","InstanceId:%d, ServiceId:%d, AllotedResorceCap:%d, BusyResourceCount:%d",mesi_InstanceId,mesi_ServiceId,mesi_AllotedResourceCap,siL_CurrentBusyResourceCount-1);
	CResourceCache::mcfn_getInstance()->mcfn_decrementBusyResourceCount(meC_SignalingIP+":"+to_string(mesl_SignalingPort),mesi_ServiceId);
	*/
}
