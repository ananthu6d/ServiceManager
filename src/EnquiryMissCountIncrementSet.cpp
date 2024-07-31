/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* EnquiryMissCountIncrementSet.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */


#include "EnquiryMissCountIncrementSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;


/************************************************************************
 * Class     : CEnquiryMissCountIncrementSet
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CEnquiryMissCountIncrementSet::CEnquiryMissCountIncrementSet()
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
 * Class     : CEnquiryMissCountIncrementSet
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CEnquiryMissCountIncrementSet::~CEnquiryMissCountIncrementSet()
{
	__entryFunction__;

	mcfn_close();
	CG_AppServiceManager.mcfn_releaseConnection(pmeC_Connection);

	__return__();
}

/************************************************************************
 * Class     : CEnquiryMissCountIncrementSet
 * Method    : mdfn_bindColumns
 * Purpose   : bind varibles with query
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CEnquiryMissCountIncrementSet::mdfn_bindColumns()
{
	 __entryFunction__;	

	__return__();
}

/************************************************************************
 *  Class     : CEnquiryMissCountIncrementSet    
 *  Method    : mcfn_incrementEnquiryCount
 *  Purpose   : increment heartbeat count
 *  Arguments : const char* pscL_SignalingIp,const long& slL_SignalingPort
 *  Returns   : success/failure
 ************************************************************************/

bool CEnquiryMissCountIncrementSet:: mcfn_incrementEnquiryCount(const char* pscL_SignalingIp,const long& slL_SignalingPort)
{

	__entryFunction__;
	mesl_SignalingPort = slL_SignalingPort;
        strcpy(pmesc_SignalingIp,pscL_SignalingIp);
	try
	{

		if(!mcfn_open())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "incrementEnquiryCount","OpenFailure", this,""," Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);

		}
		if(!mcfn_execute())
		{
			EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "incrementEnquiryCount","Failure", this,"", "Execution,Connection: %p", pmeC_Connection);
			mcfn_close();
			__return__(false);
		}

		mcfn_close();
		__return__(true);

	}
	catch(CException CL_Ex)
	{	

		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "incrementEnquiryCount","Exception", this,"", "Connection: %p, SignalingIp:%s, SignalingPort:%ld, ErrorCode: %d, ErrorMsg: %s", pmeC_Connection,pmesc_SignalingIp,mesl_SignalingPort,CL_Ex.mcfn_getErrorCode(), CL_Ex.mcfn_getErrorMessage());

	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "incrementEnquiryCount","UnknownException", this,"", "Connection: %p, SignalingIp:%s, SignalingPort:%ld", pmeC_Connection,pmesc_SignalingIp,mesl_SignalingPort);
	}
	mcfn_close();
	__return__(false);

}


