/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* AppServiceManager.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*  1  14June24  Ananthu    Created
*/


/**
 * Header Includes
 */

#include "AppServiceManager.h"
#include "AppHeaders.h"
#include "RegistryEventHandler.h"
#include "UDPEventMonitor.h"
#include "DBConnectionPool.h"
#include "InstanceInfo.h"
#include "ServiceHandler.h"
#include "LoadServiceMasterSet.h"
#include "LoadInstanceMasterSet.h"
#include "ServiceMaster.h"
#include "ConsumerHandler.h"
#include "LnxObjectLoader.h"
#include "InstanceRegistry.h"
//#include "ICache.h"
#include "LoadServiceInstanceMappingSet.h"
#include "ResourceCache.h"

#include <map>
using namespace _6d;
using namespace _6d::_EventLog;
using namespace _6d::_Framework;
using namespace _6d::_Utils;

/**
 * Global Variables
 */
extern int siG_InstanceID;

//ICache* CResourceCache::mcfn_getInstance() = nullptr;

/**
 * Main Application Object
 */
CAppServiceManager CG_AppServiceManager;

/**
 * Application Configuration Object
 */
CAppConfigParams CG_AppConfigParams;

CRegistryEventHandler* pCG_RegistryEventHandler=nullptr;

long slG_SynKey=INITIAL_SYNKEY;

/************************************************************************
 * Class     : CAppServiceManager
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CAppServiceManager::CAppServiceManager()
{
	__entryFunction__;

	__return__();
}


/************************************************************************
 * Class     : CAppServiceManager
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CAppServiceManager::~CAppServiceManager()
{
	__entryFunction__;
	__return__();
}


/************************************************************************
 * Class     : CAppServiceManager
 * Method    : mcfn_getVersion
 * Purpose   : Method to get the version of the application
 * Arguments : None
 * Returns   : void
 ************************************************************************/

void CAppServiceManager::mcfn_getVersion()
{
	__entryFunction__;

	CFramework::mcfn_getVersion();
	cout << endl << "==================================================================" << endl;
	cout << "Version : " << APPVERSION << endl;
	cout << "Release Date : " << APPRELDATE << endl;
	cout << "====================================================================" << endl;

	__return__();
}


/************************************************************************
 * Class     : CAppServiceManager
 * Method    : mcfn_initInstance
 * Purpose   : Entry point for the application to initialize the application resources
 * Arguments : None
 * Returns   : bool
 ************************************************************************/

bool CAppServiceManager::mcfn_initInstance()
{
	__entryFunction__;


	CResourceCache::mcfn_getInstance();

	/*
	CLnxObjectLoader* pCL_ObjectLoader=nullptr;

        pCL_ObjectLoader = pCG_Framework->mcfn_getObjectLoader("lib6dResourceCache.so.1.0.0");

	CResourceCache::mcfn_getInstance() = (ICache*)pCL_ObjectLoader->mcfn_createObject("ResourceCache");

	*/
	CServiceMaster::mcfn_getInstance();
	CInstanceRegistry::mcfn_getInstance();

	pmeC_DBConnectionPool = CDBConnectionPool::mcfn_getInstance();

        pmeC_DBConnectionPool->mcfn_setDBPoolParameters(CG_AppConfigParams.mcfn_getDBUserName(),\
                        CG_AppConfigParams.mcfn_getDBPassword(),CG_AppConfigParams.mcfn_getDBSID(),\
                        CG_AppConfigParams.mcfn_getPrimaryDBServerIP(),\
                        CG_AppConfigParams.mcfn_getSecondaryDBServerIP(), CG_AppConfigParams.mcfn_getDBConnectionpool(),\
                        CG_AppConfigParams.mcfn_getDBConnMonitorInterval(),CG_AppConfigParams.mcfn_getDBServerPort());

	//TODO:
	//1. load services from db - one CServiceHandler per one service

	if(!mcfn_loadFromDB())
	{
		cout<<"\nFailed To Load Datas From DB!\n";
		return false;
	}

	//2. load instance master from db - InstanceRegistry
	//3. load service mapping from db - store each mapping into CServiceHandler::CServiceResource
	//4. check external cache for each CServiceResource in all CServiceHandler & insert if required
	//5. create Consumer handler for configured count.
	
	//creating RegistryHandler
	pCG_RegistryEventHandler = new CRegistryEventHandler();
	pCG_RegistryEventHandler->mcfn_setHandlerId(slG_SynKey);
	slG_SynKey++;

	//creating ConsumerHandlers
	CResourceManager* pCL_ResourceManager = CResourceManager::mcfn_getInstance();
	for(int siL_Itr = 0; siL_Itr < CG_AppConfigParams.mcfn_getConsumerHandlerCount(); siL_Itr++)
	{
		CConsumerHandler* pCL_ConsumerHandler = new CConsumerHandler();
		pCL_ConsumerHandler->mcfn_setSynKey(slG_SynKey);
		slG_SynKey++;

		IEventListener* pIL_EventListener = (IEventListener*)pCL_ConsumerHandler;
		pCL_ResourceManager->mcfn_addObject2Pool(CONSUMERHANDLERPOOL, pIL_EventListener);

	}
	CUDPEventMonitor::mcfn_getInstance()->mcfn_start();
	__return__(true);
}
/************************************************************************
 * Class     : CAppServiceManager
 * Method    : mcfn_closeApplication
 * Purpose   : Exit point for application to release application resources
 * Arguments : None
 * Returns   : void
 ************************************************************************/

void CAppServiceManager::mcfn_closeApplication()
{
	__entryFunction__;

	//delete CInstanceCache::mcfn_getInstance();
	delete CUDPEventMonitor::mcfn_getInstance();
	delete CInstanceRegistry::mcfn_getInstance();
	if(pCG_RegistryEventHandler)
	{
		delete pCG_RegistryEventHandler;
		pCG_RegistryEventHandler=nullptr;
	}
	
	CFramework::mcfn_closeApplication();

	__return__();
}

/************************************************************************
 * Class     : CAppServiceManager
 * Method    : mcfn_onSetLogSink
 * Purpose   : function to set log sink for logging
 * Arguments : None
 * Returns   : void
 ************************************************************************/
void CAppServiceManager::mcfn_onSetLogSink()
{
	cout << endl << "CAppServiceManager::mcfn_onSetLogSink>>>" << endl;

	switch(CG_FrwkConfig.mcfn_getLogMode())
	{
		case 2:
			{
				std::shared_ptr<CLogStashSink> pIL_LogSink = std::make_shared<CLogStashSink>();
				std::shared_ptr<CEventJsonFormatter> pIL_EventFormatter = std::make_shared<CEventJsonFormatter>();

				pmdI_LogSink = pIL_LogSink;
				pmdI_EventFormatter = pIL_EventFormatter;
				CIOMultiplexer::mcfn_getInstance();

				pIL_LogSink->mcfn_openSink(CG_FrwkConfig.mcfn_getLogStashIPPort());

				CG_EventLog.mcfn_setLogSink(pIL_LogSink);
				CG_EventLog.mcfn_setEventFormatter(pIL_EventFormatter);
			}
			break;
		case 1:
			{
				std::shared_ptr<CFileSink> pIL_FileSink = std::make_shared<CFileSink>();
				std::shared_ptr<CEventLineFormatter> pIL_EventFormatter = std::make_shared<CEventLineFormatter>();

				pIL_FileSink->mcfn_setLogGenerationOnSize(CG_FrwkConfig.mcfn_getLogFileSize());
				pIL_FileSink->mcfn_setLogGenerationInterval(CG_FrwkConfig.mcfn_getLogFileDuration());
				pIL_FileSink->mcfn_setLogPath(CG_FrwkConfig.mcfn_getLogDir());

				pmdI_LogSink = pIL_FileSink;
				pmdI_EventFormatter = pIL_EventFormatter;

				pIL_FileSink->mcfn_openSink(CG_FrwkConfig.mcfn_getAppTag());

				CG_EventLog.mcfn_setLogSink(pIL_FileSink);
				CG_EventLog.mcfn_setEventFormatter(pIL_EventFormatter);
			}
	}
	CG_EventLog.mcfn_setAppTag(CG_FrwkConfig.mcfn_getAppTag());

	cout << endl << "CAppServiceManager::mcfn_onSetLogSink<<<" << endl;
}

/*************************************************************************
 * Class     : CAppServiceManager                                        *
 * Method    : mcfn_loadFromDB		                                 *
 * Purpose   : load the details from DB				         *
 * Arguments : Nil	                                                 *
 * Returns   : Success/Failure	                                         *
 *************************************************************************/
bool CAppServiceManager::mcfn_loadFromDB()
{
	while(1)
	{
		CLoadServiceMasterSet CL_LoadServiceMasterSet;
		if(CL_LoadServiceMasterSet.mcfn_loadServiceMaster())
			break;	
	}
	while(1)
	{
		CLoadInstanceMasterSet CL_LoadInstanceMasterSet;
		if(CL_LoadInstanceMasterSet.mcfn_loadInstanceMaster())
			break;
	}
	while(1)
	{
		CLoadServiceInstanceMappingSet CL_LoadServiceInstanceMappingSet;
		if(CL_LoadServiceInstanceMappingSet.mcfn_loadServiceInstanceMapping())
			break;
	}
	if(!CInstanceRegistry::mcfn_getInstance()->mcfn_loadIntoServiceResource())
		return false;

	return true;
}


/*************************************************************************
 * Class     : CAppServiceManager                                        *
 * Method    : mcfn_getConnection                                        *
 * Purpose   : create a new connection. release a connction              *
 * Arguments : Old connection                                            *
 * Returns   : new connection                                            *
 *************************************************************************/

CMySQLConnection* CAppServiceManager::mcfn_getConnection()
{
	__entryFunction__;

	CMySQLConnection* pCL_Connection = nullptr;
	while(1)
	{
		try
		{
			pCL_Connection = pmeC_DBConnectionPool->mcfn_getDBConnection();
			while(pCL_Connection == nullptr)
			{
				pCL_Connection = pmeC_DBConnectionPool->mcfn_getDBConnection();
				EVT_LOG(CG_EventLog, LOG_ERROR,siG_InstanceID, "getConnection","Failure", this,"", "DB Connection Establishment Failed");
				usleep(NOCONNECTIONSLEEPDUR);
			}

			EVT_LOG(CG_EventLog, LOG_INFO|LOG_OPINFO,siG_InstanceID, "getConnection","CurrentConnection", this,"", "%p", pCL_Connection);
			__return__(pCL_Connection);
		}
		catch(CException CL_Exception)
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID,"getConnection", "Exception", this, "", "ErrorCode:%d ErrorMessage:%s", CL_Exception.mcfn_getErrorCode(), CL_Exception.mcfn_getErrorMessage());
		}
		catch(...)
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID,"getConnection", "Exception", this, "", "Unknown");

		}
		if(pCL_Connection)
		{
			pmeC_DBConnectionPool->mcfn_releaseDBConnection(pCL_Connection);
			pCL_Connection = nullptr;
		}
	}

}
void CAppServiceManager::mcfn_releaseConnection(CMySQLConnection*& pCL_Conn)
{
	if(pCL_Conn != nullptr)
	{
		EVT_LOG(CG_EventLog, LOG_INFO|LOG_OPINFO,siG_InstanceID, "releaseConnection","CurrentConnection", this,"", "%p", pCL_Conn);
		pmeC_DBConnectionPool->mcfn_releaseDBConnection(pCL_Conn);
	}

	pCL_Conn = nullptr;
}


