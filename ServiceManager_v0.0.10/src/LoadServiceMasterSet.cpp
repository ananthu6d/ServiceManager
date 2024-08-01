/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* LoadServiceMasterSet.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */


#include "LoadServiceMasterSet.h"
#include "AppExterns.h"
#include "ServiceHandler.h"
#include "ServiceMaster.h"
#include "ResourceCache.h"

using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;


/************************************************************************
 * Class     : CLoadServiceMasterSet
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CLoadServiceMasterSet::CLoadServiceMasterSet()
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
 * Class     : CLoadServiceMasterSet
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CLoadServiceMasterSet::~CLoadServiceMasterSet()
{
	__entryFunction__;
	mcfn_close();
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);
	__return__();
}

/************************************************************************
 * Class     : CLoadServiceMasterSet
 * Method    : mdfn_bindColumns
 * Purpose   : bind varibles with query
 * Arguments : None
 * Returns   : None
 ************************************************************************/
void CLoadServiceMasterSet::mdfn_bindColumns()
{
	 __entryFunction__;
	
	 mdfn_bindColumn(&mesi_ServiceId, sizeof(mesi_ServiceId), MYSQL_TYPE_LONG, 1, 0);
	 mdfn_bindColumn(pmesc_ServiceName, sizeof(pmesc_ServiceName), MYSQL_TYPE_STRING, 2, 0);
	 mdfn_bindColumn(pmesc_ServiceType, sizeof(pmesc_ServiceType), MYSQL_TYPE_STRING, 3, 0);
	 mdfn_bindColumn(&mesi_LicenseCap, sizeof(mesi_LicenseCap), MYSQL_TYPE_LONG, 4, 0);
	 mdfn_bindColumn(&mesi_AllotedCap, sizeof(mesi_AllotedCap), MYSQL_TYPE_LONG, 5, 0);
	__return__();
}

/************************************************************************
 *  Class     : CLoadServiceMasterSet    
 *  Method    : mcfn_loadServiceMaster
 *  Purpose   : to load data from ServiceMaster
 *  Arguments : Nil
 *  Returns   : success/failure
 ************************************************************************/

bool CLoadServiceMasterSet:: mcfn_loadServiceMaster()
{

	__entryFunction__;
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceMaster","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceMaster","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
                        __return__(false);
		}

		int siL_ServiceCounter=0;
		while(mcfn_moveNext())
		{
			siL_ServiceCounter++;

			string CL_Key = pmesc_ServiceType;
			CL_Key.append("_"+to_string(mesi_ServiceId));

			if(CServiceMaster::mcfn_getInstance()->mcfn_isServiceHandlerExist(CL_Key))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceMaster","CacheDataExist", this,"", "ServiceId:%d, Connection:%p",mesi_ServiceId, pmeC_Connection);
				continue;
			}

			CServiceHandler* pCL_CServiceHandler = new CServiceHandler();
			pCL_CServiceHandler->mcfn_setSynKey(slG_SynKey);
			slG_SynKey++;
			pCL_CServiceHandler->mcfn_setServiceName(pmesc_ServiceName);	
			pCL_CServiceHandler->mcfn_setServiceId(mesi_ServiceId);
			pCL_CServiceHandler->mcfn_setServiceType(pmesc_ServiceType);
			pCL_CServiceHandler->mcfn_setLicenseCap(mesi_LicenseCap);
			pCL_CServiceHandler->mcfn_setAllotedCap(mesi_AllotedCap);
			pCL_CServiceHandler->mcfn_setLicenseCapPercentage(mesi_LicenseCap/100.0);

			/*
			if(!CResourceCache::mcfn_getInstance()->mcfn_insertBusyCount(pCL_CServiceHandler->mcfn_getServiceType()+"_"+to_string(mesi_ServiceId)))
			{
				mefn_initDataSet();
				continue;
			}

			*/
			if(!CServiceMaster::mcfn_getInstance()->mcfn_insertServiceHandler(pCL_CServiceHandler->mcfn_getServiceType()+"_"+to_string(mesi_ServiceId),pCL_CServiceHandler))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceMaster","CacheFailure", this,"", "Inserting ServiceId:%d,Type:%s Connection:%p",mesi_ServiceId,pmesc_ServiceType, pmeC_Connection);
				if(pCL_CServiceHandler)
				{
					delete pCL_CServiceHandler;
					pCL_CServiceHandler=nullptr;
				}
			}
			mefn_initDataSet();
		}

		EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "loadServiceMaster","Success", this,"", "Services loaded in Cache:%d, ServicesMaster in DB:%d, Connection:%p",CServiceMaster::mcfn_getInstance()->mcfn_ServiceHandlerSize(), siL_ServiceCounter,pmeC_Connection);

		if(CServiceMaster::mcfn_getInstance()->mcfn_ServiceHandlerSize()==0)
		{
			EVT_LOG(CG_EventLog, LOG_CRITICAL,siG_InstanceID, "loadServiceMaster","Failure", this,"", "Empty ServiceList,Connection:%p",CServiceMaster::mcfn_getInstance()->mcfn_ServiceHandlerSize(),pmeC_Connection);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{

		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceMaster","Exception", this,"", "Connection: %p, ServiceId:%d, ErrorCode:%d, ErrorMsg:%s", pmeC_Connection,mesi_ServiceId,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadServiceMaster","UnknownException", this,"", "Connection:%p, ServiceId:%d", pmeC_Connection,mesi_ServiceId);
	}
	mcfn_close();
	__return__(false);

}


