/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* ServiceMasterSet.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */


#include "ServiceMasterSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;


/************************************************************************
 * Class     : CServiceMasterSet
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CServiceMasterSet::CServiceMasterSet()
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
 * Class     : CServiceMasterSet
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CServiceMasterSet::~CServiceMasterSet()
{
	__entryFunction__;
	mcfn_close();
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);
	__return__();
}

/************************************************************************
 * Class     : CServiceMasterSet
 * Method    : mdfn_bindColumns
 * Purpose   : bind varibles with query
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CServiceMasterSet::mdfn_bindColumns()
{
	 __entryFunction__;	

	__return__();
}

/************************************************************************
 *  Class     : CServiceMasterSet    
 *  Method    : mcfn_updateTotalAllocatedResource
 *  Purpose   : update Total Allocated Resource
 *  Arguments : const CInstanceInfo& CL_InstanceInfo
 *  Returns   : success/failure
 ************************************************************************/

bool CServiceMasterSet:: mcfn_updateTotalAllocatedResource(const int& siL_ServiceId,const int& siL_AllocatedResource)
{

	__entryFunction__;
	mesi_ServiceId		= siL_ServiceId;
	mesi_AllocatedResource 	= siL_AllocatedResource;
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "updateTotalAllocatedResource","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "updateTotalAllocatedResource","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
                        __return__(false);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{

		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "updateTotalAllocatedResource","Exception", this,"", "Connection: %p, ServiceId:%d, ErrorCode: %d, ErrorMsg: %s", pmeC_Connection,mesi_ServiceId,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "updateTotalAllocatedResource","UnknownException", this,"", "Connection: %p, ServiceId:%d", pmeC_Connection,mesi_ServiceId);
	}
	mcfn_close();
	__return__(false);

}


