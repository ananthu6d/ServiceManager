/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* LoadServiceInstanceMappingSet.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */


#include "LoadServiceInstanceMappingSet.h"
#include "AppExterns.h"
#include "InstanceRegistry.h"
using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;


/************************************************************************
 * Class     : CLoadServiceInstanceMappingSet
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CLoadServiceInstanceMappingSet::CLoadServiceInstanceMappingSet()
{
	__entryFunction__;

	pmeC_Connection = nullptr;
	mefn_initDataSet();
	//Getting Connection
	pmeC_Connection=CG_AppServiceManager.mcfn_getConnection();
	mcfn_setDBConnection(pmeC_Connection);
	mcfn_setDBOperation(1);
	__return__();
}


/************************************************************************
 * Class     : CLoadServiceInstanceMappingSet
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CLoadServiceInstanceMappingSet::~CLoadServiceInstanceMappingSet()
{
	__entryFunction__;
	mcfn_close();
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);
	__return__();
}

/************************************************************************
 * Class     : CLoadServiceInstanceMappingSet
 * Method    : mdfn_bindColumns
 * Purpose   : bind varibles with query
 * Arguments : None
 * Returns   : None
 ************************************************************************/
void CLoadServiceInstanceMappingSet::mdfn_bindColumns()
{
	 __entryFunction__;
	
	 mdfn_bindColumn(&mesi_ServiceId, sizeof(mesi_ServiceId), MYSQL_TYPE_LONG, 1, 0);
	 mdfn_bindColumn(&mesi_InstanceId, sizeof(mesi_InstanceId), MYSQL_TYPE_LONG, 2, 0);
	 mdfn_bindColumn(&mesc_Status, sizeof(mesc_Status), MYSQL_TYPE_STRING, 3, 0);
	 mdfn_bindColumn(&mesi_AllotedCap, sizeof(mesi_AllotedCap), MYSQL_TYPE_LONG, 4, 0);
	 mdfn_bindColumn(&mesi_TotalCap, sizeof(mesi_TotalCap), MYSQL_TYPE_LONG, 5, 0);
	 mdfn_bindColumn(pmesc_SignalingIpPort, sizeof(pmesc_SignalingIpPort), MYSQL_TYPE_STRING, 6, 0);
	 mdfn_bindColumn(pmesc_ServiceType, sizeof(pmesc_ServiceType), MYSQL_TYPE_STRING, 7, 0);
	__return__();
}

/************************************************************************
 *  Class     : CLoadServiceInstanceMappingSet    
 *  Method    : mcfn_loadServiceInstanceMapping
 *  Purpose   : to load data from ServiceMaster
 *  Arguments : Nil
 *  Returns   : success/failure
 ************************************************************************/

bool CLoadServiceInstanceMappingSet:: mcfn_loadServiceInstanceMapping()
{

	__entryFunction__;
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceInstanceMapping","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceInstanceMapping","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
                        __return__(false);
		}

		while(mcfn_moveNext())
		{
			if(!CInstanceRegistry::mcfn_getInstance()->mcfn_addService(pmesc_SignalingIpPort,to_string(mesi_ServiceId),pmesc_ServiceType,mesi_TotalCap,mesi_AllotedCap,mesc_Status))	
			{
				EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceInstanceMapping","CacheFailure", this,"", "Inserting Mapping ServiceId:%d, InstanceId:%d, Connection:%p",mesi_ServiceId,mesi_InstanceId,pmeC_Connection);
			}


			mefn_initDataSet();
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{

		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceInstanceMapping","Exception", this,"", "Connection: %p, ServiceId:%d, InstanceId:%d ErrorCode:%d, ErrorMsg:%s", pmeC_Connection,mesi_ServiceId,mesi_InstanceId,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceInstanceMapping","UnknownException", this,"", "Connection:%p, ServiceId:%d InstanceId:%d", pmeC_Connection,mesi_ServiceId,mesi_InstanceId);
	}
	mcfn_close();
	__return__(false);

}


