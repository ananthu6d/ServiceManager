/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* DeRegisterServiceResourceSet.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */


#include "DeRegisterServiceResourceSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;


/************************************************************************
 * Class     : CDeRegisterServiceResourceSet
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CDeRegisterServiceResourceSet::CDeRegisterServiceResourceSet()
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
 * Class     : CDeRegisterServiceResourceSet
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CDeRegisterServiceResourceSet::~CDeRegisterServiceResourceSet()
{
	__entryFunction__;
	
	mcfn_close();
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);

	__return__();
}

/************************************************************************
 * Class     : CDeRegisterServiceResourceSet
 * Method    : mcfn_reInitailizeDB
 * Purpose   : Destroys and Releases the
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CDeRegisterServiceResourceSet::mcfn_reInitailizeDB()
{
        __entryFunction__;
	
        CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);
        pmeC_Connection=CG_AppServiceManager.mcfn_getConnection();
        mcfn_setDBConnection(pmeC_Connection);
        mcfn_setDBOperation(1);

        __return__();

}

/************************************************************************
 * Class     : CDeRegisterServiceResourceSet
 * Method    : mdfn_bindColumns
 * Purpose   : bind varibles with query
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CDeRegisterServiceResourceSet::mdfn_bindColumns()
{
	 __entryFunction__;	

	__return__();
}

/************************************************************************
 *  Class     : CDeRegisterServiceResourceSet    
 *  Method    : mcfn_deRegisterInstance
 *  Purpose   : deregister instance
 *  Arguments : string CL_SignalingIpPort
 *  Returns   : success/failure
 ************************************************************************/

bool CDeRegisterServiceResourceSet:: mcfn_deRegisterInstance(string CL_SignalingIpPort)
{
	__entryFunction__;
	mesi_QueryType=QUERYTYPE_DEREGISTER_ALL_SERVICE;
	meC_SignalingIpPort=meC_SignalingIpPort;
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "deRegisterInstance","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "deRegisterInstance","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{

		if(1062 == CL_Ex.mcfn_getErrorCode())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,  siG_InstanceID, "deRegisterInstance","DataExist",this, "", "SignalingIpPort:%s", meC_SignalingIpPort.c_str());
			mcfn_close();
			__return__(true);
		}

		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "deRegisterInstance","Exception", this,"", "Connection: %p, SignalingIpPort:%s, ErrorCode: %d, ErrorMsg: %s", pmeC_Connection,meC_SignalingIpPort.c_str(),CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "deRegisterInstance","UnknownException", this,"", "Connection: %p, SignalingIpPort:%s", pmeC_Connection,meC_SignalingIpPort.c_str());
	}
	mcfn_close();
	__return__(false);

}

/************************************************************************
 *  Class     : CDeRegisterServiceResourceSet 
 *  Method    : mcfn_deRegisterServiceResource
 *  Purpose   : reregister Service
 *  Arguments : string CL_SignalingIpPort,const int& siL_ServiceId
 *  Returns   : success/failure
 ************************************************************************/

bool CDeRegisterServiceResourceSet:: mcfn_deRegisterServiceResource(string CL_SignalingIpPort,const int& siL_ServiceId)
{
	__entryFunction__;
	mesi_QueryType=QUERYTUPE_DEREGISTER_SINGLE_SERVICE;
	meC_SignalingIpPort = CL_SignalingIpPort;
	mesi_ServiceId=siL_ServiceId;
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "deRegisterServiceResource","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "deRegisterServiceResource","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{

		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "deRegisterServiceResource","Exception", this,"", "Connection: %p, SignalingIpPort:%s, ServiceId:%d, ErrorCode: %d, ErrorMsg: %s", pmeC_Connection,meC_SignalingIpPort.c_str(),mesi_ServiceId,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "deRegisterServiceResource","UnknownException", this,"", "Connection: %p, SignalingIpPort:%s, ServiceId:%d", pmeC_Connection,meC_SignalingIpPort.c_str(),mesi_ServiceId);
	}
	mcfn_close();
	__return__(false);

}


