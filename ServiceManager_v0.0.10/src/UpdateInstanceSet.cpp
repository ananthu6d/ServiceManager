/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* UpdateInstanceSet.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */


#include "UpdateInstanceSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;


/************************************************************************
 * Class     : CUpdateInstanceSet
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CUpdateInstanceSet::CUpdateInstanceSet()
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
 * Class     : CUpdateInstanceSet
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CUpdateInstanceSet::~CUpdateInstanceSet()
{
	__entryFunction__;

	mcfn_close();
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);

	__return__();
}

/************************************************************************
 * Class     : CUpdateInstanceSet
 * Method    : mdfn_bindColumns
 * Purpose   : bind varibles with query
 * Arguments : None
 * Returns   : None
 ************************************************************************/
void CUpdateInstanceSet::mdfn_bindColumns()
{
	 __entryFunction__;	

	__return__();
}

/************************************************************************
 *  Class     : CUpdateInstanceSet    
 *  Method    : mcfn_resourceGenericUpdate
 *  Purpose   : update in InstanceMaster Table
 *  Arguments : const char* pscL_SignalingIp,const long& slL_SignalingPort,char* pscL_Query,char* pscL_RegistrationTime
 *  Returns   : success/failure
 ************************************************************************/

bool CUpdateInstanceSet:: mcfn_resourceGenericUpdate(const char* pscL_SignalingIp,const long& slL_SignalingPort,char* pscL_Query,char* pscL_RegistrationTime)
{

	__entryFunction__;	
	strcpy(pmesc_RegistrationTime,pscL_RegistrationTime);
	strcpy(pmesc_SignalingIp,pscL_SignalingIp);
	strcpy(pmesc_Query,pscL_Query);
	mesl_SignalingPort = slL_SignalingPort;
	siL_QueryType=QUERYTYPE_GENERIC_RESOURCE_UPDATE;
	try
	{
		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "resourceGenericUpdate","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "resourceGenericUpdate","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{
	
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "resourceGenericUpdate","Exception", this,"", "Connection: %p, SignalingIp:%s, SignalingPort:%ld, ErrorCode: %d, ErrorMsg: %s", pmeC_Connection,pmesc_SignalingIp,mesl_SignalingPort,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "resourceGenericUpdate","UnknownException", this,"", "Connection: %p, SignalingIp:%s, SignalingPort:%ld", pmeC_Connection,pmesc_SignalingIp,mesl_SignalingPort);
	}
	mcfn_close();
	__return__(false);

}


/************************************************************************
 *  Class     : CUpdateInstanceSet    
 *  Method    : mcfn_activateInstance
 *  Purpose   : update in InstanceMaster Table
 *  Arguments : const char* pscL_SignalingIp,const long& slL_SignalingPort,char* pscL_RegistrationTime
 *  Returns   : success/failure
 ************************************************************************/

bool CUpdateInstanceSet:: mcfn_activateInstance(const char* pscL_SignalingIp,const long& slL_SignalingPort,char* pscL_RegistrationTime)
{

	__entryFunction__;	
	strcpy(pmesc_RegistrationTime,pscL_RegistrationTime);
	siL_QueryType=QUERYTYPE_ACTIVATE_INSTANCE;
	mesl_SignalingPort = slL_SignalingPort;
	strcpy(pmesc_SignalingIp,pscL_SignalingIp);
	try
	{
		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "activateInstance","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "activateInstance","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{
	
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "activateInstance","Exception", this,"", "Connection: %p, SignalingIp:%s, SignalingPort:%ld, ErrorCode: %d, ErrorMsg: %s", pmeC_Connection,pmesc_SignalingIp,mesl_SignalingPort,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "activateInstance","UnknownException", this,"", "Connection: %p, SignalingIp:%s, SignalingPort:%ld", pmeC_Connection,pmesc_SignalingIp,mesl_SignalingPort);
	}
	mcfn_close();
	__return__(false);

}
