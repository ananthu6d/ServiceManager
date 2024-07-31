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
	mesi_TotalResourceCount         = 0;
	mesi_InboundResourceCount       = 0;
	mesi_OutboundResourceCount      = 0;
	mesi_InstanceId                 = 0;
	mesi_HeartBeatMissCount         = 0;
	mesl_ClientPort                 = 0;
	mesi_SignalingPort              = 0x00;
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


