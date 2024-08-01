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

bool CResourceCache::mcfn_insertBusyCount(const string& CL_ServiceKey,const string& CL_InstanceKey,const int& siL_BusyCount)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);

	int siL_ErrorCode = 0x00;
	if(pmeI_RedisCacheInterface->mcfn_setHashFieldValue("",CL_ServiceKey,CL_InstanceKey,to_string(siL_BusyCount),siL_ErrorCode))
	{
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Insert", this,"","ServiceKey:%s,InstanceKey:%s, BusyCount:%d",CL_ServiceKey.c_str(),CL_InstanceKey.c_str(),siL_BusyCount);
                return true;		
	}
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Insertion ServiceKey:%s, InstanceKey:%s, BusyCount:%d,RedisErrorCode:%d",CL_ServiceKey.c_str(),CL_InstanceKey.c_str(),siL_BusyCount,siL_ErrorCode);
	return false;
}
bool CResourceCache::mcfn_fetchBusyCount(const string& CL_ServiceKey,const string& CL_InstanceKey,int& siL_BusyCount)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);

	string CL_Value = "";
	if(pmeI_RedisCacheInterface->mcfn_getHashFieldValue("",CL_ServiceKey,CL_InstanceKey,CL_Value) == 0)
	{
		siL_BusyCount = atoi(CL_Value.c_str());	
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Insert", this,"","ServiceKey:%s,InstanceKey:%s, BusyCount:%d",CL_ServiceKey.c_str(),CL_InstanceKey.c_str(),siL_BusyCount);
		return true;
	}
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Insertion ServiceKey:%s,InstanceKey:%s, BusyCount:%d",CL_ServiceKey.c_str(),CL_InstanceKey.c_str(),siL_BusyCount);
	return false;	
}
bool CResourceCache::mcfn_incrementBusyCount(const string& CL_ServiceKey,const string& CL_InstanceKey,const int& siL_BusyCount)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);
	int siL_ErrorCode = 0x00;
	long slL_NewValue = 0x00;
	if(pmeI_RedisCacheInterface->mcfn_incrementExistingHashFieldIntValue("",CL_ServiceKey,CL_InstanceKey,siL_BusyCount,slL_NewValue,siL_ErrorCode))
	{
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Increment", this,"","ServiceKey:%s, InstanceKey:%s, NewCount:%ld",CL_ServiceKey.c_str(),CL_InstanceKey.c_str(),slL_NewValue);
		return true;
	}
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Increment ServiceKey:%s, InstanceKey:%s, RedisErrorCode:%d",CL_ServiceKey.c_str(),CL_InstanceKey.c_str(),siL_ErrorCode);
	return false;
}

bool CResourceCache::mcfn_decrementBusyCount(const string& CL_ServiceKey,const string& CL_InstanceKey,const int& siL_BusyCount)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);
	int siL_ErrorCode = 0x00;
	long slL_NewValue = 0x00;
	if(pmeI_RedisCacheInterface->mcfn_incrementExistingHashFieldIntValue("",CL_ServiceKey,CL_InstanceKey,(0-siL_BusyCount),slL_NewValue,siL_ErrorCode))
	{
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Decrement", this,"","ServiceKey:%s, InstanceKey:%s, NewCount:%ld",CL_ServiceKey.c_str(),CL_InstanceKey.c_str(),slL_NewValue);
		return true;
	}
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Decrement ServiceKey:%s, InstanceKey:%s, RedisErrorCode:%d",CL_ServiceKey.c_str(),CL_InstanceKey.c_str(),siL_ErrorCode);
	return false;
}


bool CResourceCache::mcfn_resetBusyCount(const string& CL_ServiceKey,const string& CL_InstanceKey)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);

	int siL_ErrorCode = 0x00;
	string CL_Value	  = "0";
	if(pmeI_RedisCacheInterface->mcfn_updateExistingHashFieldValue("",CL_ServiceKey,CL_InstanceKey,CL_Value,siL_ErrorCode) == 0)
	{
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Reset", this,"","ServiceKey:%s,InstanceKey:%s",CL_ServiceKey.c_str(),CL_InstanceKey.c_str());
		return true;
	}

	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","Reset ServiceKey:%s,InstanceKey:%s, RedisErrorCode:%d",CL_ServiceKey.c_str(),CL_InstanceKey.c_str(),siL_ErrorCode);
	return false;
}

bool CResourceCache::mcfn_removeFromResourceCache(const string& CL_ServiceKey,const string& CL_InstanceKey)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);

	int siL_ErrorCode = 0x00;
	if(pmeI_RedisCacheInterface->mcfn_deleteHashFieldValue("",CL_ServiceKey,CL_InstanceKey,siL_ErrorCode))
	{
		EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","Remove", this,"","ServiceKey:%s,InstanceKey:%s",CL_ServiceKey.c_str(),CL_InstanceKey.c_str());
		return true;
	}	
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","ServiceKey:%s,InstanceKey:%s ",CL_ServiceKey.c_str(),CL_InstanceKey.c_str());
	return false;
}

bool CResourceCache:: mcfn_getHashMapFromResourceCache(const string& CL_ServiceKey,unordered_map<string,string>& CL_HashMap)
{
	lock_guard<mutex> CL_Lock(meC_ResourceCacheLock);
        if(pmeI_RedisCacheInterface->mcfn_getAllHashFieldValue("",CL_ServiceKey,CL_HashMap) == 0)
        {
                EVT_LOG(CG_EventLog, LOG_INFO,siG_InstanceID, "ResourceCache","FetchAllHash", this,"","ServiceKey:%s",CL_ServiceKey.c_str());
                return true;
        }
	EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "ResourceCache","Failure", this,"","GetHashMap ServiceKey:%s",CL_ServiceKey.c_str());
	return false;
}

