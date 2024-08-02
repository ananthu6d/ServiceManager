/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.							    *
 *  (c) 2008 6d Technologies		                                    *
 *  All Rights Reserved				                                    *
 *                                                                      *
 ************************************************************************
 *
 *  MediaTimer.cpp
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -------------------------------------------------------
 *   1 23Sep08   RKM       Created
 */


/**
* Header Includes
*/
#include "InstanceInfo.h"
#include "AppHeaders.h"
#include "UDPEventMonitor.h"
#include "ServiceResource.h"
#include "ServiceMaster.h"
#include "DeRegisterInstanceSet.h"
#include "DeRegisterServiceResourceSet.h"
/************************************************************************
 * Class     : CInstanceInfo                                             
 * Method    : Constructor                                               
 * Purpose   : Creates Object and initializes its variables              
 * Arguments : Nil                                                       
 * Returns   : Nil                                                       
 *************************************************************************/
CInstanceInfo::CInstanceInfo()
{
	pmcC_RequestTimer               = nullptr;
	pmcC_EnquiryTimer               = nullptr;
	pmcC_EnquiryTimer               = new CEnquiryTimer();
	pmcC_RequestTimer               = new CRequestTimer();
	mesi_TotalResourceCount         = 0x00;
	mesi_InboundResourceCount       = 0x00;
	mesi_OutboundResourceCount      = 0x00;
	mesi_InstanceId                 = 0x00;
	mesi_HeartBeatMissCount         = 0x00;
	mesl_ClientPort                 = 0x00;
	mesi_SignalingPort              = 0x00;
	mesi_IBDBusyCount		= 0x00;
	mesi_OBDBusyCount		= 0x00;
	memset(pmesc_ClientIP,          0x00, sizeof(pmesc_ClientIP));
	memset(pmesc_LastHeartBeatTime, 0x00, sizeof(pmesc_LastHeartBeatTime));
	memset(pmesc_RegistrationTime,  0x00, sizeof(pmesc_RegistrationTime));
	memset(pmesc_InstanceName,      0x00, sizeof(pmesc_InstanceName));

}


/************************************************************************
 * Class     : CInstanceInfo                                            
 * Method    : Destructor                                               
 * Purpose   : releases its variables and destroys the object            
 * Arguments : Nil                                                       
 * Returns   : Nil                                                       
 *************************************************************************/

CInstanceInfo::~CInstanceInfo()
{
	if(pmcC_RequestTimer)
	{
		delete pmcC_RequestTimer;
		pmcC_RequestTimer = nullptr;
	}
	if(pmcC_EnquiryTimer)
	{
		delete pmcC_EnquiryTimer;
		pmcC_EnquiryTimer = nullptr;
	}

	for(auto lL_Itr : meC_ServiceInfoMap)
	{
		if(lL_Itr.second)
			delete lL_Itr.second;
		lL_Itr.second=nullptr;
	}
} // end of Destructor

bool CInstanceInfo::mcfn_checkAndIncrementIBDBusyCount()
{
	lock_guard<mutex> CL_Lock(meC_InstanceLock);
	if(mesi_IBDBusyCount < mesi_InboundResourceCount)
	{
		mesi_IBDBusyCount++;
		mefn_checkForForceEnquiry(BD_TYPE_IBD);
		return true;
	}
	return false;

}
bool CInstanceInfo::mcfn_checkAndIncrementOBDBusyCount()
{
	lock_guard<mutex> CL_Lock(meC_InstanceLock);
	if(mesi_OBDBusyCount < mesi_OutboundResourceCount)
	{
		mesi_OBDBusyCount++;
		mefn_checkForForceEnquiry(BD_TYPE_OBD);
		return true;
	}
	return false;

}
bool CInstanceInfo::mcfn_checkAndDecrementIBDBusyCount()
{
	lock_guard<mutex> CL_Lock(meC_InstanceLock);
	if(mesi_IBDBusyCount > 0x00)
	{
		mesi_IBDBusyCount--;
		return true;
	}
	return false;
}
bool CInstanceInfo::mcfn_checkAndDecrementOBDBusyCount()
{
	lock_guard<mutex> CL_Lock(meC_InstanceLock);
	if(mesi_OBDBusyCount > 0x00)
	{
		mesi_OBDBusyCount--;
		return true;
	}
	return false;
}
bool CInstanceInfo::mcfn_findAndCheckForActiveService(const string& CL_ServiceTypeId)
{
	lock_guard<mutex> CL_Lock(meC_InstanceLock);
	auto lL_Itr= meC_ServiceInfoMap.find(CL_ServiceTypeId);
	if(lL_Itr!=meC_ServiceInfoMap.end())
	{
		if(lL_Itr->second->mcfn_getStatus() == "A")
		{
			return true;
		}	
	}
	return true;
}

void CInstanceInfo::mefn_checkForForceEnquiry(int siL_BoundType)
{
	switch(siL_BoundType)
	{
		case BD_TYPE_IBD:
			{
				if((mesi_IBDBusyCount*100)/mesi_InboundResourceCount >= CG_AppConfigParams.mcfn_getForceEnquiryPercentageLimit())
				{
					CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(EVT_FORCE_ENQUIRY,(long)this);
				}
			}
			break;
		case BD_TYPE_OBD:
			{
				if((mesi_OBDBusyCount*100)/mesi_OutboundResourceCount >= CG_AppConfigParams.mcfn_getForceEnquiryPercentageLimit())
				{                                        
					CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(EVT_FORCE_ENQUIRY,(long)this);
				}
			}
	}
}

