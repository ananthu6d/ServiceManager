/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.                             *
 *  (c) 2008 6d Technologies                                            *
 *  All Rights Reserved                                                 *
 *                                                                      *
 ************************************************************************
 *
 *  CCEventMonitor.cpp
 *
 *
 * Rev Date      User      Comments
: * --- --------- --------  -------------------------------------------------------
 *   1 24Sep08   RKM       Created
 */



/**
 Header Includes
*/

#include "AppConfigParams.h"
#include "AppExterns.h"
#include "UDPEventMonitor.h"
#include "AppHeaders.h"
#include "RegistryEventHandler.h"

extern CRegistryEventHandler* pCG_RegistryEventHandler;

using namespace _6d;
/**
 * Initializing the Singleton Reference Object to NULL
 */
CUDPEventMonitor *CUDPEventMonitor::pmeC_EventMonitor = NULL;

/************************************************************************
* Class     : CUDPEventMonitor                                         *
* Method    : Constructor                                               *
* Purpose   : Creates Object and initializes its variables              *
* Arguments : Nil                                                       *
* Returns   : Nil                                                       *
*************************************************************************/

CUDPEventMonitor::CUDPEventMonitor()
{

	pmeS_ServiceManagerEvent=nullptr;
        //Get the DispatchManager
        CDispatchManager *pCL_DispatchManager = CDispatchManager::mcfn_getInstance();

        //If DispatchManager is not available then throw exception
        if(!pCL_DispatchManager)
                throw CException(22,"Could not get DispatchManager");

	mesi_Socket = -1;

        /**
        * register the monitor with the DispatchManager
        * @param1 UUCID of CCEventMonitor
        * @param2 CCEventMonitor reference
        */

        //pCL_DispatchManager->mcfn_addEventMonitor(5,this);

	//mcfn_start();


}

/************************************************************************
* Class     : CUDPEventMonitor                                         *
* Method    : Destructor                                                *
* Purpose   : releases its variables and destroys the object            *
* Arguments : Nil                                                       *
* Returns   : Nil                                                       *
*************************************************************************/

CUDPEventMonitor::~CUDPEventMonitor()
{
        //Get the DispatchManager
       	CDispatchManager *pCL_DispatchManager = CDispatchManager::mcfn_getInstance();

        if(pCL_DispatchManager)
        {
                /**
                * If DispatchManager is available, unregister the Event Monitor
                * @param1 UUCID of CCEventMonitor
                */

		//               pCL_DispatchManager->mcfn_removeEventMonitor(5);
	}

	mcfn_close();
	
	//mcfn_join();
}

/************************************************************************
 * Class     : CUDPEventMonitor
 * Method    : mcfn_getInstance
 * Purpose   : Creates a Singleton Reference Object
 * Arguments : None
 * Returns   : CUDPEventMonitor*
 ************************************************************************/

CUDPEventMonitor* CUDPEventMonitor::mcfn_getInstance()
{
        if(pmeC_EventMonitor == NULL)
                pmeC_EventMonitor =  new CUDPEventMonitor();

        return pmeC_EventMonitor;
}




/************************************************************************
* Class     : CUDPEventMonitor                                         *
* Method    : mcfn_addListener                                          *
* Purpose   : Allow Listeners to register for the events                *
* Arguments : @param1 reference to the Event Listener                   *
*             @param2 reference to the Event criteria                   *
* Returns   : Nil                                                       *
*************************************************************************/

void CUDPEventMonitor::mcfn_addListener(IEventListener *pIL_EventListener,CEventCriteria *pCL_EventCriteria)
{
} //end of mcfn_addListener

/************************************************************************
 * Class     : CUDPEventMonitor                                         *
 * Method    : mcfn_removeListener                                       *
 * Purpose   : Allow Listeners to unregister for the events              *
 * Arguments : @param1 reference to the Event Listener                   *
 * Returns   : Nil                                                       *
 *************************************************************************/

void CUDPEventMonitor::mcfn_removeListener(IEventListener *pIL_EventListener)
{
   
} // end of mcfn_removeListener

/************************************************************************
 * Class     : CUDPEventMonitor                                         *
 * Method    : mcfn_initializeForEvents                                  *
 * Purpose   : Startup initializations before retrieving the events      *
 * Arguments : Nil                                                       *
 * Returns   : Nil                                                       *
 *************************************************************************/

void CUDPEventMonitor::mcfn_initializeForEvents()
{

	__entryFunction__;
	try
	{
		while(1)
		{
			if(!mefn_create())
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "InitializedFailed", this, "", "Error Creating Socket");
				sleep(5);
				continue;

			}
			if(!mefn_bind())
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "InitializedFailed", this, "", "Error in Binding...Retrying...");
				sleep(1);
				continue;

			}
			break;
		}
	}
	catch(CException e)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Exception", this, "", "%s", e.mcfn_getErrorMessage());
	}
	catch(...)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Exception", this, "", "Unknown Exception");

	}
	__return__();


} // end of mcfn_initializeForEvents

/************************************************************************
 * Class     : CUDPEventMonitor                                         *
 * Method    : mcfn_dispatchEvent                                        *
 * Purpose   : Method to retrieve the events                             *
 * Arguments : Nil                                                       *
 * Returns   : true if event present else false                          *
 *************************************************************************/

void CUDPEventMonitor::mcfn_dispatchEvent()
{

	__entryFunction__;
	EVENTOBJECT SL_EventObject;
	CDispatchManager *pCL_DispatchManager = CDispatchManager::mcfn_getInstance();
	CResourceManager *pCL_ResourceManager = CResourceManager::mcfn_getInstance();

	switch(pmeS_ServiceManagerEvent->mcS_EventHeader.mcsi_CmdId)
	{
		case CMD_REGISTER_INSTANCE_REQ:
		case CMD_DEREGISTER_INSTANCE_REQ:
		case CMD_DEREGISTER_SERVICE_RESOURCE_REQ:
		case CMD_ENQUIRY_RESP:
		case CMD_SOCKET_RECONNECT_REQ:
			{
				try
				{
					SL_EventObject.pmcI_EventListener = (IEventListener*)pCG_RegistryEventHandler;

					SEventInfo SL_EventInfo;
					strcpy(SL_EventInfo.pmcsc_ClientIp,pmesc_ClientIp);
					SL_EventInfo.mcsl_ClientPort	= mesl_ClientPort;
					SL_EventInfo.mcsi_EventType	= EVT_NETWORK;
					SL_EventInfo.mcsl_EventInfo	= (long)pmeS_ServiceManagerEvent;

					while((SL_EventObject.pmcC_EventInfo = (CGenericEvent*)pCL_ResourceManager->mcfn_getObjectFromPool(EVENTOBJECTPOOL)) == NULL)
					{
						EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "dispatchError", this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId, "EventOBject outtage");
						usleep(POOLOUTAGEDUR);
					}

					SL_EventObject.pmcC_EventInfo->mcfn_setEventObject(&SL_EventInfo, sizeof(SL_EventInfo)+1);

					EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "UDPEventMonitor", "dispatch", this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId, "EventListener : %p",SL_EventObject.pmcI_EventListener);
					pCL_DispatchManager->mcfn_insertEventObject(SL_EventObject);

				}
				catch(CException CL_Ex)
				{
					EVT_LOG(CG_EventLog,LOG_ERROR,siG_InstanceID,"UDPEventMonitor","Exception",this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId,"Error Code:%d, ErrorDesc:%s",CL_Ex.mcfn_getErrorCode(),CL_Ex.mcfn_getErrorMessage());
				}
				catch(...)
				{
					EVT_LOG(CG_EventLog,LOG_ERROR,siG_InstanceID,"UDPEventMonitor","Exception",this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId,"Unknown Exception");
				}
			}
			break;
		case CMD_REGISTER_CONSUMER_REQ:
		case CMD_DEREGISTER_CONSUMER_REQ:
		case CMD_FETCH_RESOURCE_REQ:
		case CMD_RELEASE_RESOURCE_REQ:
			{
				try
				{

					SEventInfo SL_EventInfo;
					strcpy(SL_EventInfo.pmcsc_ClientIp,pmesc_ClientIp);
					SL_EventInfo.mcsl_ClientPort	= mesl_ClientPort;
					SL_EventInfo.mcsi_EventType	= EVT_NETWORK;
					SL_EventInfo.mcsl_EventInfo	= (long)pmeS_ServiceManagerEvent;

					while((SL_EventObject.pmcI_EventListener = (IEventListener*)pCL_ResourceManager->mcfn_getObjectFromPool(CONSUMERHANDLERPOOL)) == NULL)
					{
						EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "dispatchError", this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId, "ConsumerHandler outage");
						usleep(POOLOUTAGEDUR);
					}

					pCL_ResourceManager->mcfn_addObject2Pool(CONSUMERHANDLERPOOL, SL_EventObject.pmcI_EventListener);

					while((SL_EventObject.pmcC_EventInfo = (CGenericEvent*)pCL_ResourceManager->mcfn_getObjectFromPool(EVENTOBJECTPOOL)) == NULL)
					{
						EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "dispatchError", this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId, "EventOBject outage");
						usleep(POOLOUTAGEDUR);
					}

					SL_EventObject.pmcC_EventInfo->mcfn_setEventObject(&SL_EventInfo, sizeof(SL_EventInfo)+1);

					EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "UDPEventMonitor", "dispatch", this, "", "EventListener : %p",SL_EventObject.pmcI_EventListener);
					pCL_DispatchManager->mcfn_insertEventObject(SL_EventObject);
				}
				catch(CException CL_Ex)
				{
					EVT_LOG(CG_EventLog,LOG_ERROR,siG_InstanceID,"UDPEventMonitor","Exception",this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId,"Error Code:%d, ErrorDesc:%s",CL_Ex.mcfn_getErrorCode(),CL_Ex.mcfn_getErrorMessage());
					if(SL_EventObject.pmcC_EventInfo)
					{
						pCL_ResourceManager->mcfn_addObject2Pool(CONSUMERHANDLERPOOL, SL_EventObject.pmcC_EventInfo);
					}
				}
				catch(...)
				{
					EVT_LOG(CG_EventLog,LOG_ERROR,siG_InstanceID,"UDPEventMonitor","Exception",this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId,"Unknown Exception");
					if(SL_EventObject.pmcC_EventInfo)
					{
						pCL_ResourceManager->mcfn_addObject2Pool(CONSUMERHANDLERPOOL, SL_EventObject.pmcC_EventInfo);
					}
				}


			}
			break;
		default:
			{
				EVT_LOG(CG_EventLog,LOG_ERROR,siG_InstanceID,"UDPEventMonitor","Failure",this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId,"Invalid CmdId:%d",pmeS_ServiceManagerEvent->mcS_EventHeader.mcsi_CmdId);

				SServiceManagerEvent SL_ServiceManagerEvent;
				GenericError CL_GenericError;

				CL_GenericError.set_errorcode(ER_INVALID_CMDID);
				CL_GenericError.set_errordescription("Invalide CmdId");

				SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength = CL_GenericError.ByteSizeLong();
				SL_ServiceManagerEvent.mcS_EventHeader.mcsi_CmdId           = CMD_GENERIC_ERROR_RESP;

				if (!CL_GenericError.SerializePartialToArray(SL_ServiceManagerEvent.pmcsc_ProtoBuffEvent,SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength))
				{
					EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Failure", this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId, "Serializing Response Data");
				}

				mcfn_sendTo((char*)&SL_ServiceManagerEvent,SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength,pmesc_ClientIp,mesl_ClientPort);

			}

	}


}// end of mcfn_dispatchEvent

/************************************************************************
 * Class     : CUDPEventMonitor
 * Method    : mcfn_dispatchEvent
 * Purpose   : pIL_EventHandler,siL_CategoryType,siL_TimerCategory,pCL_InstanceInfo
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CUDPEventMonitor::mcfn_dispatchEvent(IEventListener* pIL_EventHandler,int siL_CategoryType,int siL_TimerCategory,CInstanceInfo* pCL_InstanceInfo)
{

	__entryFunction__;
        try
        {

		EVENTOBJECT SL_EventObject;
		CDispatchManager *pCL_DispatchManager = CDispatchManager::mcfn_getInstance();
		CResourceManager *pCL_ResourceManager = CResourceManager::mcfn_getInstance();

		SEventInfo SL_EventInfo;
		SL_EventInfo.mcsi_EventType		= siL_CategoryType;
		SL_EventInfo.mcsl_EventInfo		= (long)pCL_InstanceInfo;
		SL_EventInfo.pmcsc_ClientIp[0]		= 0x00;
		SL_EventInfo.mcsl_ClientPort    	= 0x00;
		SL_EventObject.pmcI_EventListener	= (IEventListener*)pIL_EventHandler;

		while((SL_EventObject.pmcC_EventInfo = (CGenericEvent*)pCL_ResourceManager->mcfn_getObjectFromPool(EVENTOBJECTPOOL)) == NULL)
		{
                        EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "dispatchError", this,"", "EventOBject outtage");
                        sleep(1);
                }

                SL_EventObject.pmcC_EventInfo->mcfn_setEventObject(&SL_EventInfo, sizeof(SL_EventInfo));

                pCL_DispatchManager->mcfn_insertEventObject(SL_EventObject);


        }
	catch(CException e)
        {
                EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Exception", this, "", "%s", e.mcfn_getErrorMessage());
        }
        catch(...)
        {
                EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Exception", this, "", "Unknown Exception");

        }
        __return__();

}

/************************************************************************
 * Class     : CUDPEventMonitor
 * Method    : mcfn_dispatchEvent
 * Purpose   : pIL_EventHandler,siL_EventType,slL_EventInfo
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CUDPEventMonitor::mcfn_dispatchEvent(IEventListener* pIL_EventHandler,int siL_EventType,long slL_EventInfo)
{

	__entryFunction__;
        try
        {
                EVENTOBJECT SL_EventObject;
                CDispatchManager *pCL_DispatchManager = CDispatchManager::mcfn_getInstance();
		CResourceManager *pCL_ResourceManager = CResourceManager::mcfn_getInstance();

		SEventInfo SL_EventInfo;
		SL_EventInfo.mcsi_EventType	= siL_EventType;
		SL_EventInfo.mcsl_EventInfo	= slL_EventInfo;

		SL_EventObject.pmcI_EventListener = (IEventListener*)pIL_EventHandler;

		while((SL_EventObject.pmcC_EventInfo = (CGenericEvent*)pCL_ResourceManager->mcfn_getObjectFromPool(EVENTOBJECTPOOL)) == NULL)
		{
                        EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "dispatchError", this,"", "EventOBject outtage");
                        sleep(1);
                }

                SL_EventObject.pmcC_EventInfo->mcfn_setEventObject(&SL_EventInfo, sizeof(SL_EventInfo));

                pCL_DispatchManager->mcfn_insertEventObject(SL_EventObject);

        }
	catch(CException e)
        {
                EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Exception", this, "", "%s", e.mcfn_getErrorMessage());
        }
        catch(...)
        {
                EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Exception", this, "", "Unknown Exception");

        }
        __return__();

}

/************************************************************************
 * Class     : CUDPEventMonitor
 * Method    : mcfn_dispatchEvent
 * Purpose   : pIL_EventHandler,siL_EventType,pscL_SignalingIp,slL_SignalingPort,siL_MaxLimit=MAXLIMIT_DEFAULT
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CUDPEventMonitor::mcfn_dispatchEvent(IEventListener* pIL_EventHandler,int siL_EventType,char* pscL_SignalingIp,long slL_SignalingPort)
{

	__entryFunction__;
        try
        {
                EVENTOBJECT SL_EventObject;
                CDispatchManager *pCL_DispatchManager = CDispatchManager::mcfn_getInstance();
		CResourceManager *pCL_ResourceManager = CResourceManager::mcfn_getInstance();

		SEventInfo SL_EventInfo;
		SL_EventInfo.mcsi_EventType 	  = siL_EventType;
		
		sprintf(SL_EventInfo.pmcsc_SignalingIpPort,"%s:%ld",pscL_SignalingIp,slL_SignalingPort);

		SL_EventObject.pmcI_EventListener = (IEventListener*)pIL_EventHandler;

		while((SL_EventObject.pmcC_EventInfo = (CGenericEvent*)pCL_ResourceManager->mcfn_getObjectFromPool(EVENTOBJECTPOOL)) == NULL)
		{
                        EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "dispatchError", this,"", "EventOBject outtage");
                        sleep(1);
                }

                SL_EventObject.pmcC_EventInfo->mcfn_setEventObject(&SL_EventInfo, sizeof(SL_EventInfo));

                pCL_DispatchManager->mcfn_insertEventObject(SL_EventObject);

        }
	catch(CException e)
        {
                EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Exception", this, "", "%s", e.mcfn_getErrorMessage());
        }
        catch(...)
        {
                EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Exception", this, "", "Unknown Exception");

        }
        __return__();

}
/************************************************************************
 * Class     : CUDPEventMonitor                                         *
 * Method    : mcfn_printObject                                          *
 * Purpose   : Method to print the Object                                *
 * Arguments : Nil                                                       *
 * Returns   : Nil                                                       *
 *************************************************************************/

void CUDPEventMonitor::mcfn_printObject()
{

}

/************************************************************************
 * Class     : CUDPEventMonitor                                         *
 * Method    : mcfn_run                                                  *
 * Purpose   : Thread Function                                           *
 * Arguments : Nil                                                       *
 * Returns   : Nil                                                       *
 *************************************************************************/

void CUDPEventMonitor::mcfn_run()
{

        EVT_LOG(CG_EventLog, LOG_VERBOSE, siG_InstanceID, "UDPEventMonitor", "ThreadEntry", this, "", "mcfn_run>>>");

	while(1)
	{
		mcfn_close();
		//invoke function to initialize before accepting the events
		mcfn_initializeForEvents();
		cout<<"\nUDP Server Started with IP: "<<CG_AppConfigParams.mcfn_getUDPServerIP()<<" PORT: "<<CG_AppConfigParams.mcfn_getUDPServerPort()<<"\n";

		//Infinite loop for retrieving the events
		while(1)
		{
			try
			{

				pmeS_ServiceManagerEvent=new SServiceManagerEvent;

				//invoke method to receive the events
				if(!mcfn_receiveEvents())
				{
					if(pmeS_ServiceManagerEvent)
					{
						delete pmeS_ServiceManagerEvent;
						pmeS_ServiceManagerEvent = nullptr;
					}
					break;
				}
				//invoke method to dispatch the events
				mcfn_dispatchEvent();
				pmeS_ServiceManagerEvent = nullptr;
			}
			catch(CException e)
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Exception", this, "", "%s", e.mcfn_getErrorMessage());
				if(pmeS_ServiceManagerEvent)
				{
					delete pmeS_ServiceManagerEvent;
					pmeS_ServiceManagerEvent = nullptr;
				}
			}
			catch(...)
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "Exception", this, "", "Unknown Exception");
				if(pmeS_ServiceManagerEvent)
				{
					delete pmeS_ServiceManagerEvent;
					pmeS_ServiceManagerEvent = nullptr;
				}
				break;
			}
                }
        }
        EVT_LOG(CG_EventLog, LOG_VERBOSE, siG_InstanceID, "UDPEventMonitor", "ThreadExit", this, "", "mcfn_run<<<");

}// end of mcfn_run

/************************************************************************
 * Class     : CUDPEventMonitor
 * Method    : mcfn_sendTo
 * Purpose   : send respoonse to Client
 * Arguments : pscL_Data,siL_Len,pscL_Ip,slL_Port
 * Returns   : Success
 ************************************************************************/

bool CUDPEventMonitor::mcfn_sendTo(char* pscL_Data,int siL_Len,char* pscL_Ip,long slL_Port)
{
	__entryFunction__;

	struct sockaddr_in SL_ClientAddr;

	SL_ClientAddr.sin_family 	= AF_INET;
	SL_ClientAddr.sin_port 		= htons(slL_Port);
	SL_ClientAddr.sin_addr.s_addr 	= inet_addr(pscL_Ip);

	if(sendto(mesi_Socket,pscL_Data,siL_Len+sizeof(SServiceManagerEventHeader),0, (const struct sockaddr *)&SL_ClientAddr,sizeof(SL_ClientAddr))== -1)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "sendTo", "Failure", this, "", "Client IP:%s,Client Port:%d",pscL_Ip,slL_Port);
		__return__(false);
	}

	EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "sendTo", "Success", this, "", "Client IP:%s,Client Port:%ld,Protobuff Length:%d, DataLength:%d",pscL_Ip,slL_Port,siL_Len,siL_Len+sizeof(SServiceManagerEventHeader));
	__return__(true);
}

/************************************************************************
 * Class     : CUDPEventMonitor
 * Method    : mefn_initalize
 * Purpose   : initalize variables
 * Arguments : None
 * Returns   : None
 ************************************************************************/

void CUDPEventMonitor::mefn_initalize()
{
	memset(pmesc_ClientIp,0x00,sizeof(pmesc_ClientIp));
	mesl_ClientPort = 0;
	//memset(&pmeS_ServiceManagerEvent,0x00,sizeof(pmeS_ServiceManagerEvent));
}

/************************************************************************
 * Class     : CUDPEventMonitor
 * Method    : mcfn_receiveEvents
 * Purpose   : receive Events
 * Arguments : None
 * Returns   : None
 ************************************************************************/

bool CUDPEventMonitor::mcfn_receiveEvents()
{
	__entryFunction__;

	while(1)
	{
		mefn_initalize();
		struct sockaddr_in SL_ClientAddr;
		socklen_t u32_ClientAddrLen = sizeof(SL_ClientAddr);
		int siL_NoOfByte	    = 0;

		SServiceManagerEvent SL_ServiceManagerEvent;

		siL_NoOfByte=recvfrom(mesi_Socket,(char *)pmeS_ServiceManagerEvent,sizeof(SServiceManagerEventHeader)+MAXBUFFERSIZE , 0 , ( struct sockaddr*)&SL_ClientAddr,&u32_ClientAddrLen);
		if(siL_NoOfByte==-1)
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "receiveFrom", "Failure", this, "", "Failed to Receive from Client");
			__return__(false);
		}

		inet_ntop(AF_INET,&SL_ClientAddr.sin_addr,pmesc_ClientIp,sizeof(pmesc_ClientIp));
		mesl_ClientPort=ntohs(SL_ClientAddr.sin_port);

		EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "receiveFrom", "Success", this,pmeS_ServiceManagerEvent->mcS_EventHeader.pmcsc_TransId, "Client IP:%s,Client Port:%ld,Received Length:%d,DataLength:%d,CmdId:%d,SynKey:%d",pmesc_ClientIp,mesl_ClientPort,siL_NoOfByte,pmeS_ServiceManagerEvent->mcS_EventHeader.mcsi_ProtoBuffLength,pmeS_ServiceManagerEvent->mcS_EventHeader.mcsi_CmdId,pmeS_ServiceManagerEvent->mcS_EventHeader.mcsi_SyncKey);
	
		if( siL_NoOfByte == sizeof(SServiceManagerEventHeader)+pmeS_ServiceManagerEvent->mcS_EventHeader.mcsi_ProtoBuffLength )
			__return__(true);
	}


}
void CUDPEventMonitor::mcfn_close()
{

        if(mesi_Socket == -1)
                return;

        shutdown(mesi_Socket, SHUT_RDWR);

#ifdef __LINUX__
        if(close(mesi_Socket) == 0)
#else
        if(closesocket(mesi_Socket) == 0)
#endif
        mesi_Socket = -1;

        return;
}

bool CUDPEventMonitor::mefn_create()
{
        meS_SockAddr.sin_family      = AF_INET;
        meS_SockAddr.sin_port	     = htons(CG_AppConfigParams.mcfn_getUDPServerPort());
        meS_SockAddr.sin_addr.s_addr = inet_addr(CG_AppConfigParams.mcfn_getUDPServerIP());
        if((mesi_Socket = socket(AF_INET,SOCK_DGRAM,0)) == -1)
        {
                EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "SocketError", this, "", "socket creation failed");
                return(false);
        }

        return(true);
}

bool CUDPEventMonitor::mefn_bind()
{
	if(bind(mesi_Socket, (struct sockaddr *)&meS_SockAddr, sizeof(meS_SockAddr)) != 0)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UDPEventMonitor", "connectError", this, "", "socket connect failed");
		return(false);
	}

	return(true);
}

