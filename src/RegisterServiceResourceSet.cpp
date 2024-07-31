/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* RegisterServiceResourceSet.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */


#include "RegisterServiceResourceSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;


/************************************************************************
 * Class     : CRegisterServiceResourceSet
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CRegisterServiceResourceSet::CRegisterServiceResourceSet()
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
 * Class     : CRegisterServiceResourceSet
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CRegisterServiceResourceSet::~CRegisterServiceResourceSet()
{
	__entryFunction__;

	mcfn_close();
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);
	
	__return__();
}

/************************************************************************
 * Class     : CRegisterServiceResourceSet
 * Method    : mcfn_reInitailizeDB
 * Purpose   : Destroys and Releases the 
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CRegisterServiceResourceSet::mcfn_reInitailizeDB()
{
	__entryFunction__;
	
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);
	pmeC_Connection=CG_AppServiceManager.mcfn_getConnection();
	mcfn_setDBConnection(pmeC_Connection);
	mcfn_setDBOperation(1);

	__return__();

}
/************************************************************************
 * Class     : CRegisterServiceResourceSet
 * Method    : mdfn_bindColumns
 * Purpose   : bind varibles with query
 * Arguments : None
 * Returns   : None
 ************************************************************************/
void CRegisterServiceResourceSet::mdfn_bindColumns()
{
	 __entryFunction__;	

	__return__();
}

/************************************************************************
 *  Class     : CRegisterServiceResourceSet    
 *  Method    : mcfn_insertIntoServiceInstanceMapping
 *  Purpose   : to insert into ServiceInstanceMapping table
 *  Arguments : int siL_ServiceId,int siL_InstanceId,string CL_SignalingIpPort,siL_MaxLimt,char* pscL_ServiceType
 *  Returns   : success/failure
 ************************************************************************/

bool CRegisterServiceResourceSet:: mcfn_insertIntoServiceInstanceMapping(int siL_ServiceId,int siL_InstanceId,string CL_SignalingIpPort,int siL_MaxLimt,char* pscL_ServiceType)
{

	 __entryFunction__;
	mesi_ServiceId		= siL_ServiceId;
	mesi_InstanceId		= siL_InstanceId;
	mesi_MaxLimit		= siL_MaxLimt;
	meC_SignalingIpPort 	= CL_SignalingIpPort;
	strcpy(pmesc_ServiceType,pscL_ServiceType);
	
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "insertIntoServiceInstanceMapping","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "insertIntoServiceInstanceMapping","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "insertIntoServiceInstanceMapping","Exception", this,"", "Connection: %p, ServiceId:%d, InstanceId:%d, SignalingIpPort:%s, ErrorCode: %d, ErrorMsg: %s", pmeC_Connection,mesi_ServiceId,mesi_InstanceId,meC_SignalingIpPort.c_str(),CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "insertIntoServiceInstanceMapping","UnknownException", this,"", "Connection: %p, ServiceId:%d, InstanceId:%d,SignalingIpPort:%s", pmeC_Connection,mesi_ServiceId,mesi_InstanceId,meC_SignalingIpPort.c_str());
	}
	mcfn_close();
	__return__(false);

}


