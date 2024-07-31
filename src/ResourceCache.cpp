/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.                             *
 *  (c) 2008 6d Technologies                                            *
 *  All Rights Reserved                                                 *
 *                                                                      *
 ************************************************************************
 *
 *  ResourceCache.cpp
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
//#include "ResourceCacheDefines.h"
#include "ResourceCache.h"
#include "AppHeaders.h"

using namespace _6d;
/**
 * Initializing the Singleton Reference Object to NULL
 */
CResourceCache *CResourceCache::pmeC_ResourceCache = nullptr;

/************************************************************************
 * Class     : CResourceCache                                         
 * Method    : Constructor                                               
 * Purpose   : Creates Object and initializes its variables              
 * Arguments : Nil                                                       
 * Returns   : Nil                                                       
 *************************************************************************/

CResourceCache::CResourceCache()
{
	pmeI_RedisCacheInterface = nullptr;
	if(CG_AppConfigParams.mcfn_getClusterMode())
	{
		pmeI_RedisCacheInterface = CRedisController::mcfn_getInstance()->mcfn_getRedisClusterInstance(CG_AppConfigParams.mcfn_getPrimaryHost(),\
				CG_AppConfigParams.mcfn_getPrimaryPort(),\
				CG_AppConfigParams.mcfn_getPrimaryUserName(), CG_AppConfigParams.mcfn_getPrimaryPassword(),\
				CG_AppConfigParams.mcfn_getPrimaryPoolSize(), CG_AppConfigParams.mcfn_getSocketTimeOut(),\
				CG_AppConfigParams.mcfn_getAuthenticationStatus());
	}
	else
	{
		pmeI_RedisCacheInterface = CRedisController::mcfn_getInstance()->mcfn_getRedisStandAloneInstance(CG_AppConfigParams.mcfn_getPrimaryHost(),\
				CG_AppConfigParams.mcfn_getPrimaryPort(),\
				CG_AppConfigParams.mcfn_getPrimaryUserName(), CG_AppConfigParams.mcfn_getPrimaryPassword(),\
				CG_AppConfigParams.mcfn_getPrimaryPoolSize(), CG_AppConfigParams.mcfn_getSocketTimeOut(),\
				CG_AppConfigParams.mcfn_getAuthenticationStatus());
	}
}

/************************************************************************
 * Class     : CResourceCache                                         
 * Method    : Destructor                                                
 * Purpose   : releases its variables and destroys the object            
 * Arguments : Nil                                                       
 * Returns   : Nil                                                       
 *************************************************************************/

CResourceCache::~CResourceCache()
{
	
}

bool CResourceCache::mcfn_insertBusyCount(const string& CL_MapKey,const int& siL_BusyCount)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);

	int siL_ErrorCode = 0x00;
	if(pmeI_RedisCacheInterface->mcfn_setKeyValue("",CL_MapKey,to_string(siL_BusyCount),CG_AppConfigParams.mcfn_getSessionExpiryTime(),siL_ErrorCode))
	{
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Insert", this,"","MapKey:%s, BusyCount:%d",CL_MapKey.c_str(),siL_BusyCount);
                return true;		
	}
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Insertion MapKey:%s, BusyCount:%d",CL_MapKey.c_str(),siL_BusyCount);
	return false;
}

bool CResourceCache::mcfn_fetchBusyCount(const string& CL_MapKey,int& siL_BusyCount)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);

	string CL_Value = "";
	if(pmeI_RedisCacheInterface->mcfn_getValue("",CL_MapKey,CL_Value) == 0)
	{
		siL_BusyCount = atoi(CL_Value.c_str());	
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Insert", this,"","MapKey:%s, BusyCount:%d",CL_MapKey.c_str(),siL_BusyCount);
		return true;
	}
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Insertion MapKey:%s, BusyCount:%d",CL_MapKey.c_str(),siL_BusyCount);
	return false;	
}
bool CResourceCache::mcfn_incrementBusyCount(const string& CL_MapKey,const int& siL_MaxLimit,const int& siL_BusyCount)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);
	
	string CL_Value = "";
	if(pmeI_RedisCacheInterface->mcfn_getValue("",CL_MapKey,CL_Value) != 0)
	{
		return false;
	}
	if(atoi(CL_Value.c_str()) < siL_MaxLimit)
	{
		long slL_NewValue = 0x00;
		int siL_ErrorCode = 0x00;
		if(pmeI_RedisCacheInterface->mcfn_incrementExistingIntValue("",CL_MapKey,siL_BusyCount,slL_NewValue,siL_ErrorCode))
		{
			EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Increment", this,"","MapKey:%s, NewCount:%ld,MaxLimit:%d",CL_MapKey.c_str(),slL_NewValue,siL_MaxLimit);
			return true;
		}
	}
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Increment MapKey:%s",CL_MapKey.c_str());
	return false;
}

bool CResourceCache::mcfn_decrementBusyCount(const string& CL_MapKey,const int& siL_BusyCount)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);

	string CL_Value = "";
        if(pmeI_RedisCacheInterface->mcfn_getValue("",CL_MapKey,CL_Value) != 0)
        {
                return false;
        }

	if(atoi(CL_Value.c_str()) > 0x00)
        {
                long slL_NewValue = 0x00;
                int siL_ErrorCode = 0x00;
                if(pmeI_RedisCacheInterface->mcfn_incrementExistingIntValue("",CL_MapKey,(0-siL_BusyCount),slL_NewValue,siL_ErrorCode))
                {
                        EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Decrement", this,"","MapKey:%s, NewCount:%ld",CL_MapKey.c_str(),slL_NewValue);
                        return true;
                }
        }
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Decrement MapKey:%s",CL_MapKey.c_str());
	return false;
}


bool CResourceCache::mcfn_resetBusyCount(const string& CL_MapKey)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);

	int siL_ErrorCode = 0x00;
	string CL_Value	  = "0";
	if(pmeI_RedisCacheInterface->mcfn_updateExistingKeyValue("",CL_MapKey,CL_Value,CG_AppConfigParams.mcfn_getSessionExpiryTime(),siL_ErrorCode) == 0)
	{
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Reset", this,"","MapKey:%s",CL_MapKey.c_str());
		return true;
	}

	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Reset MapKey:%s",CL_MapKey.c_str());
	return false;
}

bool CResourceCache::mcfn_removeFromResourceCache(const string& CL_MapKey)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);

	int siL_ErrorCode = 0x00;
	if(pmeI_RedisCacheInterface->mcfn_deleteKeyValue("",CL_MapKey,siL_ErrorCode))
	{
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Remove", this,"","MapKey:%s",CL_MapKey.c_str());
		return true;
	}	
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Remove MapKey:%s, ",CL_MapKey.c_str());
	return false;
}


