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
