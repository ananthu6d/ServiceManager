/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* LoadInstanceMasterSet.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */


#include "LoadInstanceMasterSet.h"
#include "AppExterns.h"
#include "InstanceInfo.h"
#include "InstanceRegistry.h"
using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;


/************************************************************************
 * Class     : CLoadInstanceMasterSet
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CLoadInstanceMasterSet::CLoadInstanceMasterSet()
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
 * Class     : CLoadInstanceMasterSet
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CLoadInstanceMasterSet::~CLoadInstanceMasterSet()
{
	__entryFunction__;
	mcfn_close();
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);
	__return__();
}

/************************************************************************
 * Class     : CLoadInstanceMasterSet
 * Method    : mdfn_bindColumns
 * Purpose   : bind varibles with query
 * Arguments : None
 * Returns   : None
 ************************************************************************/
void CLoadInstanceMasterSet::mdfn_bindColumns()
{
	__entryFunction__;

	mdfn_bindColumn(&mesi_InstanceId, sizeof(mesi_InstanceId), MYSQL_TYPE_LONG, 1, 0);
	mdfn_bindColumn(pmesc_InstanceName, sizeof(pmesc_InstanceName), MYSQL_TYPE_STRING, 2, 0);
	mdfn_bindColumn(pmesc_SignalingIP, sizeof(pmesc_SignalingIP), MYSQL_TYPE_STRING, 3, 0);
	mdfn_bindColumn(&mesi_SignalingPort, sizeof(mesi_SignalingPort), MYSQL_TYPE_LONG, 4, 0);
	mdfn_bindColumn(&mesi_TotalResourceCount, sizeof(mesi_TotalResourceCount), MYSQL_TYPE_LONG, 5, 0);
	mdfn_bindColumn(&mesi_InboundResourceCount, sizeof(mesi_InboundResourceCount), MYSQL_TYPE_LONG, 6, 0);
	mdfn_bindColumn(&mesi_OutboundResourceCount, sizeof(mesi_OutboundResourceCount), MYSQL_TYPE_LONG, 7, 0);
	mdfn_bindColumn(pmesc_RegistrationTime, sizeof(pmesc_RegistrationTime), MYSQL_TYPE_LONG, 8, 0);
	mdfn_bindColumn(&mesc_Status, sizeof(mesi_TotalResourceCount), MYSQL_TYPE_STRING, 9, 0);
	mdfn_bindColumn(&mesi_HeartBeatMissCount, sizeof(mesi_HeartBeatMissCount), MYSQL_TYPE_LONG, 10, 0);
	mdfn_bindColumn(pmesc_LastHeartBeatTime, sizeof(pmesc_LastHeartBeatTime), MYSQL_TYPE_LONG, 11, 0);
	__return__();
}

/************************************************************************
 *  Class     : CLoadInstanceMasterSet    
 *  Method    : mcfn_loadInstanceMaster
 *  Purpose   : to load data from InstanceMaster
 *  Arguments : Nil
 *  Returns   : success/failure
 ************************************************************************/

bool CLoadInstanceMasterSet:: mcfn_loadInstanceMaster()
{

	__entryFunction__;
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadInstanceMaster","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadInstanceMaster","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);
		}

		int siL_InstanceCounter=0;
		while(mcfn_moveNext())
		{
			siL_InstanceCounter++;

			/*
			CInstanceInfo* pCL_InstanceInfo = new CInstanceInfo();
			pCL_InstanceInfo->mcfn_setSignalingIP(pmesc_SignalingIP);
			pCL_InstanceInfo->mcfn_setInstanceName(pmesc_InstanceName);
			pCL_InstanceInfo->mcfn_setTotalResourceCount(mesi_TotalResourceCount);
			pCL_InstanceInfo->mcfn_setInboundResourceCount(mesi_InboundResourceCount);
			pCL_InstanceInfo->mcfn_setOutboundResourceCount(mesi_OutboundResourceCount);
			pCL_InstanceInfo->mcfn_setSignalingPort(mesi_SignalingPort);
			pCL_InstanceInfo->mcfn_setInstanceId(mesi_InstanceId);
			pCL_InstanceInfo->mcfn_setRegistrationTime(pmesc_RegistrationTime);
			pCL_InstanceInfo->mcfn_setStatus(mesc_Status);
			pCL_InstanceInfo->mcfn_setHeartBeatMissCount(mesi_HeartBeatMissCount);

			string CL_SignalingIpPort=pmesc_SignalingIP;
			CL_SignalingIpPort.append(":"+to_string(mesi_SignalingPort));
			if(!CInstanceRegistry::mcfn_getInstance()->mcfn_insertInstanceInfo(CL_SignalingIpPort,pCL_InstanceInfo))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "CacheInsert", "Failure", this,"", "InstanceId :%d",mesi_InstanceId);
			}

			mefn_initDataSet();
			*/
		}

		EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "loadInstanceMaster","Success", this,"", "Instances loaded in Cache:%d, InstancesMaster in DB:%d, Connection:%p",CInstanceRegistry::mcfn_getInstance()->mcfn_getRegistrySize(), siL_InstanceCounter,pmeC_Connection);

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{

		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadInstanceMaster","Exception", this,"", "Connection: %p, InstanceId:%d, ErrorCode:%d, ErrorMsg:%s", pmeC_Connection,mesi_InstanceId,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "loadInstanceMaster","UnknownException", this,"", "Connection:%p, InstanceId:%d", pmeC_Connection,mesi_InstanceId);
	}
	mcfn_close();
	__return__(false);

}


