/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* RegisterInstanceSet.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */


#include "RegisterInstanceSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;


/************************************************************************
 * Class     : CRegisterInstanceSet
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CRegisterInstanceSet::CRegisterInstanceSet()
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
 * Class     : CRegisterInstanceSet
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CRegisterInstanceSet::~CRegisterInstanceSet()
{
	__entryFunction__;
	
	mcfn_close();
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);

	__return__();
}

/************************************************************************
 * Class     : CRegisterInstanceSet
 * Method    : mdfn_bindColumns
 * Purpose   : bind varibles with query
 * Arguments : None
 * Returns   : None
 ************************************************************************/
void CRegisterInstanceSet::mdfn_bindColumns()
{
	 __entryFunction__;	

	__return__();
}

/************************************************************************
 *  Class     : CRegisterInstanceSet    
 *  Method    : mcfn_insertIntoInstanceMaster
 *  Purpose   : to insert data into InstanceMaster
 *  Arguments : const CInstanceInfo& CL_InstanceInfo
 *  Returns   : success/failure
 ************************************************************************/

bool CRegisterInstanceSet:: mcfn_insertIntoInstanceMaster(char* pscL_InstanceName,char* pscL_RegistrationTime,char scL_Status,char* pscL_SignalingIp,int siL_TotalResourceCount,int siL_InboundResourceCount,int siL_OutboundResourceCount,int siL_SignalingPort,int siL_InstanceId)
{
	__entryFunction__;
	mefn_setData(pscL_InstanceName,pscL_RegistrationTime,scL_Status,pscL_SignalingIp,siL_TotalResourceCount,siL_InboundResourceCount,siL_OutboundResourceCount,siL_SignalingPort,siL_InstanceId);
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "insertIntoInstanceMaster","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "insertIntoInstanceMaster","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
                        __return__(false);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{

		/*
		mesi_ErrorCode=CL_Ex.mcfn_getErrorCode();
		if(1062 == CL_Ex.mcfn_getErrorCode())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,  siG_InstanceID, "insertIntoInstanceMaster","DataExist",this, "", "InstanceId: %d", mesi_InstanceId);
			mcfn_close();
			__return__(true);
		}
		*/

		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "insertIntoInstanceMaster","Exception", this,"", "Connection: %p, IP:%s, PORT:%d ErrorCode: %d, ErrorMsg: %s", pmeC_Connection,pmesc_SignalingIp,mesi_SignalingPort,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "insertIntoInstanceMaster","UnknownException", this,"", "Connection: %p, IP:%s, PORT:%d", pmeC_Connection,pmesc_SignalingIp,mesi_SignalingPort);
	}
	mcfn_close();
	__return__(false);

}


