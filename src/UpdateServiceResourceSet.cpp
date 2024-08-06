/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* UpdateServiceResourceSet.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */


#include "UpdateServiceResourceSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;


/************************************************************************
 * Class     : CUpdateServiceResourceSet
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CUpdateServiceResourceSet::CUpdateServiceResourceSet()
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
 * Class     : CUpdateServiceResourceSet
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CUpdateServiceResourceSet::~CUpdateServiceResourceSet()
{
	__entryFunction__;

	mcfn_close();
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);
	__return__();
}

/************************************************************************
 * Class     : CUpdateServiceResourceSet
 * Method    : mcfn_reInitailizeDB
 * Purpose   : Destroys and Releases the
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CUpdateServiceResourceSet::mcfn_reInitailizeDB()
{
        __entryFunction__;
        CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);
        pmeC_Connection=CG_AppServiceManager.mcfn_getConnection();
        mcfn_setDBConnection(pmeC_Connection);
        mcfn_setDBOperation(1);
        __return__();

}

/************************************************************************
 * Class     : CUpdateServiceResourceSet
 * Method    : mdfn_bindColumns
 * Purpose   : bind varibles with query
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CUpdateServiceResourceSet::mdfn_bindColumns()
{
	 __entryFunction__;	

	__return__();
}

/************************************************************************
 *  Class     : CUpdateServiceResourceSet 
 *  Method    : mcfn_activateResource
 *  Purpose   : activate resource
 *  Arguments : string CL_SignalingIpPort,const int& siL_ServiceId
 *  Returns   : success/failure
 ************************************************************************/

bool CUpdateServiceResourceSet:: mcfn_activateResource(string CL_SignalingIpPort,const int& siL_ServiceId)
{
	__entryFunction__;
	mesi_QueryType		= QUERYTYPE_ACTIVATE_RESOURCE;
	meC_SignalingIpPort	= CL_SignalingIpPort;
	mesi_ServiceId		= siL_ServiceId;
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "activateResource","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "activateResource","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{

		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "activateResource","Exception", this,"", "Connection: %p, SignalingIpPort:%s, ServiceId:%d, ErrorCode: %d, ErrorMsg: %s", pmeC_Connection,meC_SignalingIpPort.c_str(),mesi_ServiceId,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "activateResource","UnknownException", this,"", "Connection: %p, SignalingIpPort:%s, ServiceId:%d", pmeC_Connection,meC_SignalingIpPort.c_str(),mesi_ServiceId);
	}
	mcfn_close();
	__return__(false);

}

/************************************************************************
 *  Class     : CUpdateServiceResourceSet 
 *  Method    : mcfn_updateResource
 *  Purpose   : update resource
 *  Arguments : string CL_SignalingIpPort,const int& siL_ServiceId
 *  Returns   : success/failure
 ************************************************************************/

bool CUpdateServiceResourceSet:: mcfn_updateResource(string CL_SignalingIpPort,const int& siL_ServiceId,const int& siL_Value,const int& siL_Type)
{
	__entryFunction__;
	mesi_QueryType=siL_Type;
	meC_SignalingIpPort=CL_SignalingIpPort;
	mesi_ServiceId=siL_ServiceId;
	switch(mesi_QueryType)
	{
		case QUERYTYPE_UPDATE_MAXLIMIT:
			{
				mesi_MaxLimit=siL_Value;
				break;
			}
		case QUERYTYPE_UPDATE_ALLOCATED_RESOURCE:
			{
				mesi_AllocatedResource=siL_Value;
				break;
			}
	}
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "updateResource","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "updateResource","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{

		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "updateResource","Exception", this,"", "Connection: %p, SignalingIpPort:%s, ServiceId:%d, ErrorCode: %d, ErrorMsg: %s", pmeC_Connection,meC_SignalingIpPort.c_str(),mesi_ServiceId,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "updateResource","UnknownException", this,"", "Connection: %p, SignalingIpPort:%s, ServiceId:%d", pmeC_Connection,meC_SignalingIpPort.c_str(),mesi_ServiceId);
	}
	mcfn_close();
	__return__(false);

}

