/************************************************************************
 *                                                                       *
 * The information described in this file is a confidential and          *
 * proprietary product of 6d Technologies.                               *
 * (c) 2020 6d Technologies                                              *
 * All Rights Reserved                                                   *
 *                                                                       *
 *************************************************************************
 *
 * RegistryEventHandler.cpp
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -----------------------------------------------
 *
 */


/**
 * Header Includes
 */

#include "RegistryEventHandler.h"
#include "RegisterInstanceSet.h"
#include "RegisterServiceResourceSet.h"
#include "DeRegisterInstanceSet.h"
#include "DeRegisterServiceResourceSet.h"
#include "EnquiryMissCountIncrementSet.h"
#include "EnquiryUpdateSet.h"
#include "ServiceMaster.h"
#include "InstanceRegistry.h"
#include "ServiceResource.h"
#include "UpdateInstanceSet.h"
/**
 * Global Variables
 */


/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : ConstructorResourceDeRegistrationReq
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CRegistryEventHandler::CRegistryEventHandler()
{
	__entryFunction__;

	pmeS_ServiceManagerEventReq=nullptr;
	mefn_initialize(); 

	__return__();
}


/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CRegistryEventHandler::~CRegistryEventHandler()
{
	__entryFunction__;
	__return__();
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_initialize
 * Purpose   : initialise the object
 * Arguments : none
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mefn_initialize()
{
	memset(&meS_ServiceManagerEventResp, 0x00, sizeof(meS_ServiceManagerEventResp));
	memset(pmesc_ClientIp,0x00,sizeof(pmesc_ClientIp));
	mesl_ClientPort		= 0x00;
	mesi_StatusCode		= -1;
	pmesc_StatusDesc[0]	= 0x00;

	if(pmeS_ServiceManagerEventReq)
	{
		delete pmeS_ServiceManagerEventReq;
		pmeS_ServiceManagerEventReq=nullptr;
	}
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mcfn_printObject
 * Purpose   : print object
 * Arguments : none
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mcfn_printObject()
{
	__entryFunction__;
	__return__();

}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mcfn_subscribeEvent
 * Purpose   : subscribe for an event
 * Arguments : @param1: AppCallBack reference
 * 	     : @param2: EventCriteria reference
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mcfn_subscribeEvent(IAppCallBack *,CEventCriteria *)
{
	__entryFunction__;
	__return__();
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mcfn_unsubscribeEvent
 * Purpose   : unsubscribe for an event
 * Arguments : @param1: AppCallBack reference
 *           : @param2: EventCriteria reference
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mcfn_unsubscribeEvent(IAppCallBack *,CEventCriteria *)
{
	__entryFunction__;
	__return__();
}


/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mcfn_onNotifyEvent
 * Purpose   : callback method for event reporting
 * Arguments : GenericEvent
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mcfn_onNotifyEvent(CGenericEvent& CL_GenericEvent,long slL_Val)
{
	__entryFunction__;

	mefn_initialize();
	SEventInfo* pSL_EventInfo=(SEventInfo*) CL_GenericEvent.mcfn_getEventObject();

	if(pSL_EventInfo == nullptr)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "onNotifyEvent", "Failure", this,"","No Data to Process from EventInfo");
		return;
	}
	//Getting Ip and Port
	strcpy(pmesc_ClientIp,pSL_EventInfo->pmcsc_ClientIp);
	mesl_ClientPort	= pSL_EventInfo->mcsl_ClientPort;

	EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "onNotifyEvent", "Success", this,"","Event Type:%d, Client Ip:%s Port:%d, EventInfo:%ld",pSL_EventInfo->mcsi_EventType,pSL_EventInfo->pmcsc_ClientIp,pSL_EventInfo->mcsl_ClientPort,pSL_EventInfo->mcsl_EventInfo);

	switch (pSL_EventInfo->mcsi_EventType)
	{
		case EVT_NETWORK: 
			{
				pmeS_ServiceManagerEventReq=(SServiceManagerEvent*)pSL_EventInfo->mcsl_EventInfo;
				if(pmeS_ServiceManagerEventReq == nullptr)
				{
					EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "onNotifyEvent", "Failure", this,"","Empty ServiceManagerEvent Request");
					return;
				}
				if(!mefn_processNetworkEvent())
				{
					EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processNetworkEvent", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"Parsing ResourceRegistrationReq");
					mefn_serilizeResponse(ER_PARSING_DATA,"Parsing Request Proto Failed");
					mefn_sendResponse();
				}
				break;
			}

		case EVT_ENQUIRY_TIMEOUT:
			{
				mefn_processEnquiryTimeout((CInstanceInfo*)pSL_EventInfo->mcsl_EventInfo);
				break;
			}

		case EVT_REQUEST_TIMEOUT:
			{
				mefn_processRequestTimeout((CInstanceInfo*)pSL_EventInfo->mcsl_EventInfo);
				break;
			}
		case EVT_PURGE_INSTANCE:
			{

				/*TODO:
				 * CInstance pCL_Instance = CG_Registry.find(instance_id);
				 * if(found)
				 * {
				 *	for(service: pCL_Instance->meC_SericeMap)
				 *	{
				 *		ServiceHandler = CG_ServiceMap.find(service.service_id)
				 *		 dispatchEvent(Service,instance_id, EVT_PURGE_RESOURCE);
				 *	}
				 *	CG_Registry.erase(instance_id);
				 *	delete from INSTANCE_MASTER where INSTANCE_ID = instance_id;
				 *	delete pCL_Instance;
				 *	}
				 */
			}
			break;	
	}
	__return__();

}
/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_processNetworkEvent
 * Purpose   : process network events
 * Arguments : none
 * Returns   : none
 ************************************************************************/

bool CRegistryEventHandler::mefn_processNetworkEvent()
{
	__entryFunction__;
	switch (pmeS_ServiceManagerEventReq->mcS_EventHeader.mcsi_CmdId)
	{
		case CMD_REGISTER_INSTANCE_REQ:
			{
				ResourceRegistrationReq CL_ResourceRegistrationReq;

				if(!CL_ResourceRegistrationReq.ParsePartialFromArray(pmeS_ServiceManagerEventReq->pmcsc_ProtoBuffEvent,pmeS_ServiceManagerEventReq->mcS_EventHeader.mcsi_ProtoBuffLength))
				{
					__return__(false);
				}

				if(!mefn_processRegisterInstance(CL_ResourceRegistrationReq))
				{
					mefn_serilizeResponse(CL_ResourceRegistrationReq,mesi_StatusCode,pmesc_StatusDesc);
					mefn_sendResponse();
					__return__(true);

				}
				mefn_serilizeResponse(CL_ResourceRegistrationReq,SUCCESS,"Registration Successfull!");
				mefn_sendResponse();
				break;
			}
		case CMD_DEREGISTER_INSTANCE_REQ:
			{
				ResourceDeRegistrationReq CL_ResourceDeRegistrationReq;

				if(!CL_ResourceDeRegistrationReq.ParsePartialFromArray(pmeS_ServiceManagerEventReq->pmcsc_ProtoBuffEvent,pmeS_ServiceManagerEventReq->mcS_EventHeader.mcsi_ProtoBuffLength))
				{
					__return__(false);
				}

				if(!mefn_processDeRegisterInstance(CL_ResourceDeRegistrationReq))
				{
					mefn_serilizeResponse(CL_ResourceDeRegistrationReq,mesi_StatusCode,pmesc_StatusDesc,CMD_DEREGISTER_INSTANCE_RESP);
					mefn_sendResponse();
					__return__(true);
				}
				mefn_serilizeResponse(CL_ResourceDeRegistrationReq,SUCCESS,"Deregistered Successfull!",CMD_DEREGISTER_INSTANCE_RESP);
				mefn_sendResponse();
				break;
			}
		case CMD_DEREGISTER_SERVICE_RESOURCE_REQ:
			{
				ResourceDeRegistrationReq CL_ResourceDeRegistrationReq;

				if(!CL_ResourceDeRegistrationReq.ParsePartialFromArray(pmeS_ServiceManagerEventReq->pmcsc_ProtoBuffEvent,pmeS_ServiceManagerEventReq->mcS_EventHeader.mcsi_ProtoBuffLength))
				{
					__return__(false);
				}
				if(!mefn_processDeRegisterServiceResource(CL_ResourceDeRegistrationReq))
				{
					mefn_serilizeResponse(CL_ResourceDeRegistrationReq,mesi_StatusCode,pmesc_StatusDesc,CMD_DEREGISTER_SERVICE_RESOURCE_RESP);
					mefn_sendResponse();
					__return__(true);

				}
				mefn_serilizeResponse(CL_ResourceDeRegistrationReq,SUCCESS,"Deregistered Successfull!",CMD_DEREGISTER_SERVICE_RESOURCE_RESP);
                                mefn_sendResponse();
				break;
			}
		case CMD_ENQUIRY_RESP:	
			{
				EnquireStatusResp CL_EnquireStatusResp;
				if(!CL_EnquireStatusResp.ParsePartialFromArray(pmeS_ServiceManagerEventReq->pmcsc_ProtoBuffEvent,pmeS_ServiceManagerEventReq->mcS_EventHeader.mcsi_ProtoBuffLength))
				{
					__return__(false);
				}
				mefn_processEnquiryResponse(CL_EnquireStatusResp);
				break;
			}
		case CMD_SOCKET_RECONNECT_REQ:
			{
				SocketReconnectReq CL_SocketReconnectReq;
				if(!CL_SocketReconnectReq.ParsePartialFromArray(pmeS_ServiceManagerEventReq->pmcsc_ProtoBuffEvent,pmeS_ServiceManagerEventReq->mcS_EventHeader.mcsi_ProtoBuffLength))
				{
					__return__(false);
				}

				if(!mefn_processSocketReconnect(CL_SocketReconnectReq))
				{
					mefn_serilizeResponse(CL_SocketReconnectReq,mesi_StatusCode,pmesc_StatusDesc);
					mefn_sendResponse();
					__return__(true);
				}
				mefn_serilizeResponse(CL_SocketReconnectReq,SUCCESS,"Reconnecting Successfull");
				mefn_sendResponse();
				break;
			}

		default:
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "onNotifyEvent", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Invalide CmdId:%d",pmeS_ServiceManagerEventReq->mcS_EventHeader.mcsi_CmdId);
				mefn_serilizeResponse(ER_INVALID_CMDID,"Invalid CmdId");
				mefn_sendResponse();
				break;
			}
	}

	__return__(true);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Registration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_processRegisterInstance
 * Purpose   : process registration of Instance
 * Arguments : const ResourceRegistrationReq&
 * Returns   : Success/Failure
 ************************************************************************/

bool CRegistryEventHandler::mefn_processRegisterInstance(const ResourceRegistrationReq& CL_ResourceRegistrationReq)
{
	__entryFunction__;
	//printing Alldetails in log

	if(!CL_ResourceRegistrationReq.ibdservicelist_size() > 0)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processRegisterInstance", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "No ServiceList to DeRegister InstanceId :%d,SignalingIp:%s,Port:%s",CL_ResourceRegistrationReq.instanceid(),CL_ResourceRegistrationReq.signalingip().c_str(),CL_ResourceRegistrationReq.signalingport().c_str());
		mesi_StatusCode	= ER_NOSERVICELIST;
		sprintf(pmesc_StatusDesc,"ServiceList is Empty,Registration Failed!");	
		__return__(false);
	}

	EVT_LOG(CG_EventLog, LOG_INFO |LOG_OPINFO, siG_InstanceID, "processRegisterInstance", "ReqData", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceID:%d,signalingip:%s,signalingport:%s,totalresourcecount:%d,inboundreourcecount:%d,outboundresourcecount:%d,ClientIP:%s,ClientPort:%d",CL_ResourceRegistrationReq.instanceid(),CL_ResourceRegistrationReq.signalingip().c_str(),CL_ResourceRegistrationReq.signalingport().c_str(),CL_ResourceRegistrationReq.totalresourcecount(),CL_ResourceRegistrationReq.inboundresourcecount(),CL_ResourceRegistrationReq.outboundresourcecount(),pmesc_ClientIp,mesl_ClientPort);


	//Checking for wheather the request is for Reg or reReg
	CInstanceInfo* pCL_InstanceInfo = nullptr;
	if(CInstanceRegistry::mcfn_getInstance()->mcfn_getInstanceInfo(CL_ResourceRegistrationReq.signalingip()+":"+CL_ResourceRegistrationReq.signalingport(),&pCL_InstanceInfo))
	{
		__return__(mefn_processReregistrationResource(CL_ResourceRegistrationReq,pCL_InstanceInfo));
	}

	__return__(mefn_processRegistrationResource(CL_ResourceRegistrationReq));

}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_processRegistrationResource
 * Purpose   : process registration of Resource
 * Arguments : const ResourceRegistrationReq&
 * Returns   : Success/Failure
 ************************************************************************/

bool CRegistryEventHandler::mefn_processRegistrationResource(const ResourceRegistrationReq& CL_ResourceRegistrationReq)
{
	//set InstanceInfo

	CInstanceInfo* pCL_InstanceInfo	= new CInstanceInfo();

	if(!mefn_setInstanceInfo(CL_ResourceRegistrationReq,pCL_InstanceInfo))
		__return__(false);

	//inserting to Cache
	if(!CInstanceRegistry::mcfn_getInstance()->mcfn_insertInstanceInfo(CL_ResourceRegistrationReq.signalingip()+":"+CL_ResourceRegistrationReq.signalingport(),pCL_InstanceInfo))
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "CacheInsert", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceId :%d,SignalingIp:%s,Port:%s",CL_ResourceRegistrationReq.instanceid(),CL_ResourceRegistrationReq.signalingip().c_str(),CL_ResourceRegistrationReq.signalingport().c_str());

		mesi_StatusCode = ER_CACHE_FALIURE;
		sprintf(pmesc_StatusDesc,"Insertion of Instance With Id:%d, SignalingIp:%s, Port:%s into Cache, Failed!",CL_ResourceRegistrationReq.instanceid(),CL_ResourceRegistrationReq.signalingip().c_str(),CL_ResourceRegistrationReq.signalingport().c_str());
		
		if(pCL_InstanceInfo)
			delete pCL_InstanceInfo;
		pCL_InstanceInfo = nullptr;

		__return__(false);
	}

	EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "CacheInsert", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceId :%d,SignalingIp:%s,Port:%s",pCL_InstanceInfo->mcfn_getInstanceId(),CL_ResourceRegistrationReq.signalingip().c_str(),CL_ResourceRegistrationReq.signalingport().c_str());


	//Insert to Db
	mefn_InsertInstanceInfoToDB(pCL_InstanceInfo);

	//TODO Update in Total IBD,OBD count in Registry
	for(const auto& lL_Itr : pCL_InstanceInfo->mcfn_getServiceMap())
	{
		CServiceHandler* pCL_ServiceHandler=nullptr;
		if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(lL_Itr.first,&pCL_ServiceHandler))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for SericeId:%d",lL_Itr.first);
			//mesi_StatusCode = ER_NO_SERVICE;
			//sprintf(pmesc_StatusDesc,"Service With Id:%d is Not Available,Registration Failed!",lL_Itr.first);
			//__return__(false)
			//;
		}
		
		CServiceResource* pCL_ServiceResource= new CServiceResource();
		pCL_ServiceResource->mcfn_setSignalingIP(pCL_InstanceInfo->mcfn_getSignalingIP().c_str());
		pCL_ServiceResource->mcfn_setSignalingPort(pCL_InstanceInfo->mcfn_getSignalingPort());
		pCL_ServiceResource->mcfn_setInstanceId(pCL_InstanceInfo->mcfn_getInstanceId());

		pCL_ServiceResource->mcfn_setStatus(lL_Itr.second->mcfn_getStatus());

		pCL_ServiceResource->mcfn_setTotalChannel(pCL_InstanceInfo->mcfn_getInboundResourceCount());

		pCL_ServiceResource->mcfn_setServiceId(atoi(lL_Itr.first.c_str()));
		
		CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_ADD_RESOURCE,(long)pCL_ServiceResource);

	}

	//TODO Recalculate Disptach repective to IBD,OBD
	pCL_InstanceInfo->pmcC_EnquiryTimer->mcfn_startEnquiryTimer(this,pCL_InstanceInfo,CG_AppConfigParams.mcfn_getPrimaryEquiryTimerMs());

	__return__(true);
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_setInstanceInfo
 * Purpose   : set values of InstanceInfo
 * Arguments : const ResourceRegistrationReq&,CInstanceInfo*
 * Returns   : Success/Failure
 ************************************************************************/

bool CRegistryEventHandler::mefn_setInstanceInfo(const ResourceRegistrationReq& CL_ResourceRegistrationReq,CInstanceInfo* pCL_InstanceInfo)
{
	pCL_InstanceInfo->mcfn_setSignalingIP(CL_ResourceRegistrationReq.signalingip().c_str());
	pCL_InstanceInfo->mcfn_setInstanceName(CL_ResourceRegistrationReq.instancename().c_str());
	pCL_InstanceInfo->mcfn_setTotalResourceCount(CL_ResourceRegistrationReq.totalresourcecount());
	pCL_InstanceInfo->mcfn_setInboundResourceCount(CL_ResourceRegistrationReq.inboundresourcecount());
	pCL_InstanceInfo->mcfn_setOutboundResourceCount(CL_ResourceRegistrationReq.outboundresourcecount());
	pCL_InstanceInfo->mcfn_setSignalingPort(atoi(CL_ResourceRegistrationReq.signalingport().c_str()));
	pCL_InstanceInfo->mcfn_setInstanceId(CL_ResourceRegistrationReq.instanceid());
	pCL_InstanceInfo->mcfn_setRegistrationTime(CL_ResourceRegistrationReq.eventtimestamp().c_str());
	pCL_InstanceInfo->mcfn_setStatus('A');
	pCL_InstanceInfo->mcfn_setHeartBeatMissCount(0);
	pCL_InstanceInfo->mcfn_setClientIP(pmesc_ClientIp);
	pCL_InstanceInfo->mcfn_setClientPort(mesl_ClientPort);

	int siL_ServiceType = 0x00;
	char pscL_ServiceName[30];
	pscL_ServiceName[0] = 0x00;
	for (const auto& CL_ServiceDetails : CL_ResourceRegistrationReq.ibdservicelist())
	{
		EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "RequestList", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceId :%d,ServiceId:%s, ServiceType:%s",pCL_InstanceInfo->mcfn_getInstanceId(),CL_ServiceDetails.first.c_str(),CL_ServiceDetails.second.c_str());
		pscL_ServiceName[0]	= 0x00;
		siL_ServiceType		= 0x00;

		//TODO GetServiceName using type and id
		if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceName(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first,pscL_ServiceName))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "setInstanceInfo", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Serice is Not Registered Serivce:%d",atoi(CL_ServiceDetails.first.c_str()));
			mesi_StatusCode = ER_NO_SERVICE;
			sprintf(pmesc_StatusDesc,"Service With Id:%s is Not Available,Registration Failed!",CL_ServiceDetails.first.c_str());
			return false;
		}

		//TODO Calculate the Total
		if(!pCL_InstanceInfo->mcfn_insertService(CL_ServiceDetails.first,pscL_ServiceName,CL_ServiceDetails.second,1000))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "setInstanceInfo", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "To Insert into Cache ServiceId:%d",atoi(CL_ServiceDetails.first.c_str()));
			mesi_StatusCode = ER_CACHE_FALIURE;
			sprintf(pmesc_StatusDesc,"Insertion of Service With Id:%s into Cache, Failed!",CL_ServiceDetails.first.c_str());
			return false;
		}

	}
	return true;
	
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_InsertInstanceInfoToDB
 * Purpose   : insert InstanceInfo to db
 * Arguments : CInstanceInfo*
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mefn_InsertInstanceInfoToDB(const CInstanceInfo* pCL_InstanceInfo)
{
	__entryFunction__;
	while(1)
	{
		CRegisterInstanceSet CL_RegisterInstanceSet;
		if(CL_RegisterInstanceSet.mcfn_insertIntoInstanceMaster(pCL_InstanceInfo))
		{
			break;
		}
		usleep(DBFAILURESLEEPDUR);
	}	
	__return__();

}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_processReregistrationResource
 * Purpose   : process reregistration
 * Arguments : const ResourceRegistrationReq&,CInstanceInfo*
 * Returns   : Success/Failure
 ************************************************************************/

bool CRegistryEventHandler::mefn_processReregistrationResource(const ResourceRegistrationReq& CL_ResourceRegistrationReq,const CInstanceInfo* pCL_InstanceInfo)
{
	__entryFunction__;

	if(strcmp(pmesc_ClientIp,pCL_InstanceInfo->mcfn_getClientIP())!=0)
	{
		pCL_InstanceInfo->mcfn_setClientIP(pmesc_ClientIp);
		EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "ClientInfo", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "New Ip:%s, InstanceId:%d",pmesc_ClientIp,pCL_InstanceInfo->mcfn_getInstanceId());
	}

	if(mesl_ClientPort != pCL_InstanceInfo->mcfn_getClientPort())
	{
		pCL_InstanceInfo->mcfn_setClientPort(mesl_ClientPort);
		EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "ClientInfo", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "New Port:%ld, InstanceId:%d",mesl_ClientPort,pCL_InstanceInfo->mcfn_getInstanceId());
	}


	if(pCL_InstanceInfo->mcfn_getStatus() == 'D')
	{
		//pCL_InstanceInfo->mcfn_activateInstance();
		CInstanceRegistry::mcfn_getInstance()->mcfn_deActivateInstanceInfo(CL_ResourceRegistrationReq.signalingip()+":"+CL_ResourceRegistrationReq.signalingport());
		//TODO Recal TotalResource
	}

	char pscL_Query[256];
	pscL_Query[0]	= 0x00;
	int siL_Type	= 0x00;
	if( pCL_InstanceInfo->mcfn_getTotalResourceCount() != CL_ResourceRegistrationReq.totalresourcecount() )
	{
		pCL_InstanceInfo->mcfn_setTotalResourceCount(CL_ResourceRegistrationReq.totalresourcecount());
		siL_Type |= 1;
	}
	if(pCL_InstanceInfo->mcfn_getInboundResourceCount() != CL_ResourceRegistrationReq.inboundresourcecount())
	{
		pCL_InstanceInfo->mcfn_setInboundResourceCount(CL_ResourceRegistrationReq.inboundresourcecount());
		siL_Type |= 2;
	}

	if(pCL_InstanceInfo->mcfn_getOutboundResourceCount() != CL_ResourceRegistrationReq.outboundresourcecount())
	{
		pCL_InstanceInfo->mcfn_setOutboundResourceCount(CL_ResourceRegistrationReq.outboundresourcecount());
		siL_Type |= 4;
	}

	//TODO Flag 
	if(siL_Type == 0x00)
	{
		while(1)
		{
			CUpdateInstanceSet CL_UpdateInstanceSet;
			if(CL_UpdateInstanceSet.mcfn_activateInstance(pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort(),CL_ResourceRegistrationReq.eventtimestamp().c_str()))
				break;
			usleep(DBFAILURESLEEPDUR);
		}
	}
	else
	{
		mefn_UpdateQueryBuilder(pscL_Query,siL_Type,CL_ResourceRegistrationReq.totalresourcecount(),CL_ResourceRegistrationReq.inboundresourcecount(),CL_ResourceRegistrationReq.outboundresourcecount());
		while(1)
		{
			CUpdateInstanceSet CL_UpdateInstanceSet;
			if(CL_UpdateInstanceSet.mcfn_resourceGenericUpdate(pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort(),pscL_Query,CL_ResourceRegistrationReq.eventtimestamp().c_str()))
				break;
			usleep(DBFAILURESLEEPDUR);
		}
	}

	//TODO check and call recal
	if(!mefn_updateResources(CL_ResourceRegistrationReq,pCL_InstanceInfo))
	{
		__return__(false);			
	}

	pCL_InstanceInfo->pmcC_EnquiryTimer->mcfn_startEnquiryTimer(this,pCL_InstanceInfo,CG_AppConfigParams.mcfn_getPrimaryEquiryTimerMs());
	__return__(true);
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_updateResources
 * Purpose   : update Resource in Db and Cache
 * Arguments : const ResourceRegistrationReq&,CInstanceInfo*
 * Returns   : Success/Failure
 ************************************************************************/

bool CRegistryEventHandler::mefn_updateResources(const ResourceRegistrationReq& CL_ResourceRegistrationReq,const CInstanceInfo* pCL_InstanceInfo)
{
	__entryFunction__;
	char scL_Status	= 0x00;
	int siL_MaxLimt	= 0x00;

	
	for (const auto& lL_Itr : pCL_InstanceInfo->mcfn_getServiceMap())
	{
		if(CL_ResourceRegistrationReq.ibdservicelist().find(lL_Itr.first) == CL_ResourceRegistrationReq.ibdservicelist().end())
		{
			pCL_InstanceInfo->mcfn_deactivateService(lL_Itr.first);
			EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "updateResources", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Deactivated ServiceId:%d",lL_Itr.first);
			CServiceHandler* pCL_ServiceHandler = nullptr;
			if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(lL_Itr.second->mcfn_getServiceType()+"_"+lL_Itr.first,&pCL_ServiceHandler))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for SericeId:%d",lL_Itr.first);
			}
			CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_DEACT_RESOURCE,pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort());

		}	
	}

	for(const auto& CL_ServiceDetails : CL_ResourceRegistrationReq.ibdservicelist())
	{

		if(pCL_InstanceInfo->mcfn_getServiceDetailsFromServiceMap(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first,scL_Status,siL_MaxLimt))
		{
			EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "processReregistrationResource", "FetchSuccess", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "ServiceId:%d, Status:%c, MaxLimitFromCache:%d,MaxLimitFromReq:%d",atoi(CL_ServiceDetails.first.c_str()),scL_Status,siL_MaxLimt,CL_ServiceDetails.second);

			CServiceHandler* pCL_ServiceHandler = nullptr;
			if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first,&pCL_ServiceHandler))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for SericeId:%d",atoi(CL_ServiceDetails.first.c_str()));
			}


			if(scL_Status == 'D')
			{
				pCL_InstanceInfo->mcfn_activateService(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first);

				/*
				if(siL_MaxLimt != CL_ServiceDetails.second)
				{					
					CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_ACT_RESOURCE,pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort(),CL_ServiceDetails.second);
					pCL_InstanceInfo->mcfn_updateService(atoi(CL_ServiceDetails.first.c_str()),CL_ServiceDetails.second);
				}
				else
				{
				*/
				
					CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_ACT_RESOURCE,pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort());
				/*
				}
				
				*/

			}
			else
			{
				/*
				if(siL_MaxLimt != CL_ServiceDetails.second)
				{       
				 			
					CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_ACT_RESOURCE,pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort(),CL_ServiceDetails.second);
					pCL_InstanceInfo->mcfn_updateService(atoi(CL_ServiceDetails.first.c_str()),CL_ServiceDetails.second);
				}		
					
				else
				{
				*/
				
					CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_RESET_BUSYCOUNT,pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort());

				/*
				}	
				*/
			}

		}
		else
		{

			EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "processReregistrationResource", "NewService", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "ServiceId:%d,ServiceType:%s",atoi(CL_ServiceDetails.first.c_str()),CL_ServiceDetails.second);

			
			CServiceHandler* pCL_ServiceHandler = nullptr;
			if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first,&pCL_ServiceHandler))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for SericeId:%d",atoi(CL_ServiceDetails.first.c_str()));
				mesi_StatusCode = ER_NO_SERVICE;
                                sprintf(pmesc_StatusDesc,"Service With Id:%s is Not Available,Registration Failed!",CL_ServiceDetails.first);
				__return__(false);
			}

			
			CServiceResource* pCL_ServiceResource= new CServiceResource();
			pCL_ServiceResource->mcfn_setSignalingIP(pCL_InstanceInfo->mcfn_getSignalingIP().c_str());
			pCL_ServiceResource->mcfn_setSignalingPort(pCL_InstanceInfo->mcfn_getSignalingPort());
			pCL_ServiceResource->mcfn_setInstanceId(pCL_InstanceInfo->mcfn_getInstanceId());

			pCL_ServiceResource->mcfn_setTotalChannel(pCL_InstanceInfo->mcfn_getInboundResourceCount());

			pCL_ServiceResource->mcfn_setServiceId(atoi(CL_ServiceDetails.first.c_str()));

			CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_ADD_RESOURCE,(long)pCL_ServiceResource);


			int siL_ServiceType = 0x00;
			char pscL_ServiceName[30];
			pscL_ServiceName[0] = 0x00;
			if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceName(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first,pscL_ServiceName))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "setInstanceInfo", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Serice is Not Registered Serivce:%d",atoi(CL_ServiceDetails.first.c_str()));
				mesi_StatusCode = ER_NO_SERVICE;
                                sprintf(pmesc_StatusDesc,"Service With Id:%s is Not Available,Registration Failed!",CL_ServiceDetails.first);
				__return__(false);
			}
			if(!pCL_InstanceInfo->mcfn_insertService(CL_ServiceDetails.first,pscL_ServiceName,CL_ServiceDetails.second,siL_ServiceType,1000))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processReregistrationResource", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "ToInsert into Cache ServiceId:%d",atoi(CL_ServiceDetails.first.c_str()));
				mesi_StatusCode = ER_CACHE_FALIURE;
                                sprintf(pmesc_StatusDesc,"Insertion of Service With Id:%s into Cache, Failed!",CL_ServiceDetails.first);
				__return__(false);
			}

		}

	}

	//TODO Recal Dispatch (IBD,OBD)
	__return__(true);
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_serilizeResponse
 * Purpose   : serialize registration response
 * Arguments : const ResourceRegistrationReq&,siL_StatusCode,pscL_StatusDesc
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mefn_serilizeResponse(const ResourceRegistrationReq& CL_ResourceRegistrationReq,int siL_StatusCode,char* pscL_StatusDesc)
{
	__entryFunction__;
	ResourceRegistrationResp CL_ResourceRegistrationResp;

	CL_ResourceRegistrationResp.set_instanceid(CL_ResourceRegistrationReq.instanceid());
	CL_ResourceRegistrationResp.set_statuscode(siL_StatusCode);
	CL_ResourceRegistrationResp.set_statusdescription(pscL_StatusDesc);

	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength = CL_ResourceRegistrationResp.ByteSizeLong();
	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_CmdId 		 = CMD_REGISTER_INSTANCE_RESP;

	if(!CL_ResourceRegistrationResp.SerializePartialToArray(meS_ServiceManagerEventResp.pmcsc_ProtoBuffEvent,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength))
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "serilizeResponse", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Serializing Response Data");
	}

	EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "ResourceRegistrationResp", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "StatusCode:%d, StatusDesc:%s",siL_StatusCode,pscL_StatusDesc);
	__return__();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DeRegistration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_processDeRegisterInstance
 * Purpose   : process to deregister instance
 * Arguments : const ResourceDeRegistrationReq&
 * Returns   : Success/Failure
 ************************************************************************/

bool CRegistryEventHandler::mefn_processDeRegisterInstance(const ResourceDeRegistrationReq& CL_ResourceDeRegistrationReq)
{

	__entryFunction__;
	EVT_LOG(CG_EventLog, LOG_INFO |LOG_OPINFO, siG_InstanceID, "processDeRegisterInstance", "ReqData", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceID:%d,signalingip:%s,signalingport:%s,ClientIP:%s,ClientPort:%d",CL_ResourceDeRegistrationReq.instanceid(),CL_ResourceDeRegistrationReq.signalingip().c_str(),CL_ResourceDeRegistrationReq.signalingport().c_str(),pmesc_ClientIp,mesl_ClientPort);
	CInstanceInfo* pCL_InstanceInfo = nullptr;

	if(CInstanceRegistry::mcfn_getInstance()->mcfn_getInstanceInfo(CL_ResourceDeRegistrationReq.signalingip()+":"+CL_ResourceDeRegistrationReq.signalingport(),&pCL_InstanceInfo))
	{
		pCL_InstanceInfo->pmcC_RequestTimer->mcfn_stopRequestTimer();
		pCL_InstanceInfo->pmcC_EnquiryTimer->mcfn_stopEnquiryTimer();

		CInstanceRegistry::mcfn_getInstance()->mcfn_deActivateInstanceInfo(CL_ResourceDeRegistrationReq.signalingip()+":"+CL_ResourceDeRegistrationReq.signalingport());
		//pCL_InstanceInfo->mcfn_deactivateInstance();
		while(1)
		{
		
			CDeRegisterInstanceSet CL_DeRegisterInstanceSet;	
			if(CL_DeRegisterInstanceSet.mcfn_deRegisterInstance(CL_ResourceDeRegistrationReq.signalingip().c_str(),atol(CL_ResourceDeRegistrationReq.signalingport().c_str())))
			{
				break;
			}
			usleep(DBFAILURESLEEPDUR);
		}

		for(const auto& lL_Itr : pCL_InstanceInfo->mcfn_getServiceMap())
		{
			lL_Itr.second->mcfn_deActivateService();
			CServiceHandler* pCL_ServiceHandler=nullptr;
			if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(lL_Itr.first,&pCL_ServiceHandler))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for SericeId:%d",lL_Itr.first);
			}
			CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_REMOVE_RESOURCE,pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort());

		}
		__return__(true);

	}

	mesi_StatusCode = ER_INSTANCEID_NOTREGISTERED;
	sprintf(pmesc_StatusDesc,"Instance With Id:%d is Not Registered,Deregistration Failed!",CL_ResourceDeRegistrationReq.instanceid());
	__return__(false);

}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_processDeRegisterServiceResource
 * Purpose   : process to deregister service
 * Arguments : const ResourceDeRegistrationReq&
 * Returns   : Success/Failure
 ************************************************************************/

bool CRegistryEventHandler::mefn_processDeRegisterServiceResource(const ResourceDeRegistrationReq& CL_ResourceDeRegistrationReq)
{

	__entryFunction__;

	EVT_LOG(CG_EventLog, LOG_INFO |LOG_OPINFO, siG_InstanceID, "processDeRegisterServiceResource", "ReqData", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceID:%d,signalingip:%s,signalingport:%s,servicelistcount:%d,ClientIP:%s,ClientPort:%d",CL_ResourceDeRegistrationReq.instanceid(),CL_ResourceDeRegistrationReq.signalingip().c_str(),CL_ResourceDeRegistrationReq.signalingport().c_str(),CL_ResourceDeRegistrationReq.servicelist_size(),pmesc_ClientIp,mesl_ClientPort);

	if(!CInstanceRegistry::mcfn_getInstance()->mcfn_isInstanceInfoExist(CL_ResourceDeRegistrationReq.signalingip()+":"+CL_ResourceDeRegistrationReq.signalingport()))
	{
		mesi_StatusCode = ER_INSTANCEID_NOTREGISTERED;
		sprintf(pmesc_StatusDesc,"Instance With Id:%d is Not Registered,Deregistration Failed!",CL_ResourceDeRegistrationReq.instanceid());
		__return__(false);
	}

	if(!CL_ResourceDeRegistrationReq.servicelist_size()<0)
	{
		mesi_StatusCode = ER_NOSERVICELIST;
		sprintf(pmesc_StatusDesc,"ServiceList is Empty,Deregistration Failed!");
		__return__(false);

	}

	for(const auto& CL_ServiceDetails : CL_ResourceDeRegistrationReq.servicelist())
	{     
		CInstanceRegistry::mcfn_getInstance()->mcfn_deActivateService(CL_ResourceDeRegistrationReq.signalingip()+":"+CL_ResourceDeRegistrationReq.signalingport(),CL_ServiceDetails.second+"_"+CL_ServiceDetails.first);
		CServiceHandler* pCL_ServiceHandler=nullptr;
		if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first,&pCL_ServiceHandler))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for SericeId:%d",atoi(CL_ServiceDetails.first.c_str()));
		}
		CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_REMOVE_RESOURCE,CL_ResourceDeRegistrationReq.signalingip().c_str(),atol(CL_ResourceDeRegistrationReq.signalingport().c_str()));
	}

	__return__(true);
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_serilizeResponse
 * Purpose   : serilize deregistration response
 * Arguments : const ResourceDeRegistrationReq&,statuscode,statusdesc,CmdId
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mefn_serilizeResponse(const ResourceDeRegistrationReq& CL_ResourceDeRegistrationReq,int siL_StatusCode,char* pscL_StatusDesc,int siL_Cmdid)
{
	__entryFunction__;
	ResourceDeRegistrationResp CL_ResourceDeRegistrationResp;

	CL_ResourceDeRegistrationResp.set_instanceid(CL_ResourceDeRegistrationReq.instanceid());
	CL_ResourceDeRegistrationResp.set_statuscode(siL_StatusCode);
	CL_ResourceDeRegistrationResp.set_statusdescription(pscL_StatusDesc);

	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength = CL_ResourceDeRegistrationResp.ByteSizeLong();
	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_CmdId 		 = siL_Cmdid;

	if (!CL_ResourceDeRegistrationResp.SerializePartialToArray(meS_ServiceManagerEventResp.pmcsc_ProtoBuffEvent,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength))
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "serilizeResponse(", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Serializing Response Data");
	}

	EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "ResourceDeRegistrationResp", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "StatusCode:%d, StatusDesc:%s",siL_StatusCode,pscL_StatusDesc);
	__return__();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Request TimeOut
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_processRequestTimeout
 * Purpose   : process Request Timeout
 * Arguments : CInstanceInfo*
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler:: mefn_processRequestTimeout(CInstanceInfo* pCL_InstanceInfo)
{

	__entryFunction__;

	if(pCL_InstanceInfo->mcfn_getHeartBeatMissCount()+1 <= CG_AppConfigParams.mcfn_getMaxEnquiryMissCount())
	{
		//TODO increment miss count in cache and db
		EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "processRequestTimeout", "Success", this,"", "Count:%d,Type:%d",pCL_InstanceInfo->mcfn_getHeartBeatMissCount()+1,CG_AppConfigParams.mcfn_getPeriodicEnquiryType());
		pCL_InstanceInfo->mcfn_incrementHeartBeatMissCount();
		while(1)
		{
			CEnquiryMissCountIncrementSet CL_EnquiryMissCountIncrementSet;
			if(CL_EnquiryMissCountIncrementSet.mcfn_incrementEnquiryCount(pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort()))
			{
				break;
			}
			usleep(DBFAILURESLEEPDUR);
		}
		switch(CG_AppConfigParams.mcfn_getPeriodicEnquiryType())
		{

			case PERIODIC_RETRY_TYPE_IMMEDIATE:
				{
					mefn_serilizeRequest(pCL_InstanceInfo->mcfn_getInstanceId(),pCL_InstanceInfo->mcfn_getSignalingIP(),pCL_InstanceInfo->mcfn_getSignalingPort());

					meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_SyncKey = pCL_InstanceInfo->mcfn_getInstanceId();
					CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&meS_ServiceManagerEventResp,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength,pCL_InstanceInfo->mcfn_getClientIP(),pCL_InstanceInfo->mcfn_getClientPort());
					pCL_InstanceInfo->pmcC_RequestTimer->mcfn_startRequestTimer(this,pCL_InstanceInfo,CG_AppConfigParams.mcfn_getMaxEnqiuryRequestTimeMs());
					break;
				}

			case PERIODIC_RETRY_TYPE_INTERVEL:
				{

					auto lL_Itr = CG_AppConfigParams.mcC_PeriodicEnquiryDetailsMap.find(pCL_InstanceInfo->mcfn_getHeartBeatMissCount());
					if(lL_Itr != CG_AppConfigParams.mcC_PeriodicEnquiryDetailsMap.end())
					{
						pCL_InstanceInfo->pmcC_EnquiryTimer->mcfn_startEnquiryTimer(this,pCL_InstanceInfo,lL_Itr->second);
						break;
					}	
					EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processRequestTimeout", "Failure", this,"","Finding Next Retrying TimeMs from PeriodicEnquiryDetailsMap,MissCount:%d",pCL_InstanceInfo->mcfn_getHeartBeatMissCount());
					break;
				}
			case PERIODIC_RETRY_TYPE_DEFAULT:
				{
					int siL_NewEnquiryTimer = (CG_AppConfigParams.mcfn_getPrimaryEquiryTimerMs()/CG_AppConfigParams.mcfn_getMaxEnquiryMissCount())*(CG_AppConfigParams.mcfn_getMaxEnquiryMissCount()-pCL_InstanceInfo->mcfn_getHeartBeatMissCount()+1);

					if(siL_NewEnquiryTimer>0)
					{
						pCL_InstanceInfo->pmcC_EnquiryTimer->mcfn_startEnquiryTimer(this,pCL_InstanceInfo,siL_NewEnquiryTimer);
					}
					break;
				}
		}
		__return__();
	}

	//pCL_InstanceInfo->mcfn_deactivateInstance();

	CInstanceRegistry::mcfn_getInstance()->mcfn_deActivateInstanceInfo(pCL_InstanceInfo->mcfn_getSignalingIP()+":"+to_string(pCL_InstanceInfo->mcfn_getSignalingPort()));

	while(1)
	{
		CDeRegisterInstanceSet CL_DeRegisterInstanceSet;
		if(CL_DeRegisterInstanceSet.mcfn_deRegisterInstance(pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort()))
		{
			break;
		}
		usleep(DBFAILURESLEEPDUR);
	}

	for(const auto& lL_Itr : pCL_InstanceInfo->mcfn_getServiceMap())
	{
		lL_Itr.second->mcfn_deActivateService();
		CServiceHandler* pCL_ServiceHandler=nullptr;
		if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(lL_Itr.first,&pCL_ServiceHandler))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,"","No Handler Found for SericeId:%d",lL_Itr.first);
			__return__();
		}
		CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_REMOVE_RESOURCE,pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort());

	}

	__return__();
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_processEnquiryTimeout
 * Purpose   : process Enquiry Timeout
 * Arguments : CInstanceInfo*
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mefn_processEnquiryTimeout(CInstanceInfo* pCL_InstanceInfo)
{

	__entryFunction__;
	mefn_serilizeRequest(pCL_InstanceInfo->mcfn_getInstanceId(),pCL_InstanceInfo->mcfn_getSignalingIP(),pCL_InstanceInfo->mcfn_getSignalingPort());
	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_SyncKey = pCL_InstanceInfo->mcfn_getInstanceId();
	
	CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&meS_ServiceManagerEventResp,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength,pCL_InstanceInfo->mcfn_getClientIP(),pCL_InstanceInfo->mcfn_getClientPort());

	pCL_InstanceInfo->pmcC_RequestTimer->mcfn_startRequestTimer(this,pCL_InstanceInfo,CG_AppConfigParams.mcfn_getMaxEnqiuryRequestTimeMs());
	__return__();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Generic Response
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_serilizeResponse
 * Purpose   : serilize Generic Response
 * Arguments : siL_StatusCode,pscL_StatusDesc
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mefn_serilizeResponse(int siL_StatusCode,char* pscL_StatusDesc)
{
	__entryFunction__;
	GenericError CL_GenericError;

	CL_GenericError.set_errorcode(siL_StatusCode);
	CL_GenericError.set_errordescription(pscL_StatusDesc);

	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength = CL_GenericError.ByteSizeLong();
	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_CmdId 		 = CMD_GENERIC_ERROR_RESP;

	if (!CL_GenericError.SerializePartialToArray(meS_ServiceManagerEventResp.pmcsc_ProtoBuffEvent,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength))
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "serilizeResponse", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Serializing Response Data");
	}

	EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "GenericErrorResp", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "StatusCode:%d, StatusDesc:%s",siL_StatusCode,pscL_StatusDesc);
	__return__();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Enquriry Response
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_processEnquiryResponse
 * Purpose   : process EnquiryResponse
 * Arguments : const EnquireStatusResp&
 * Returns   : None
 ************************************************************************/

void CRegistryEventHandler::mefn_processEnquiryResponse(const EnquireStatusResp& CL_EnquireStatusResp)
{
	__entryFunction__;

	EVT_LOG(CG_EventLog, LOG_INFO |LOG_OPINFO, siG_InstanceID, "processEnquiryResponse", "RespData", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceId:%d,Signalingip:%s,Signalingport:%s,ClientIp:%s,ClientPort:%ld",CL_EnquireStatusResp.instanceid(),CL_EnquireStatusResp.signalingip().c_str(),CL_EnquireStatusResp.signalingport().c_str(),pmesc_ClientIp,mesl_ClientPort);
	
	CInstanceInfo* pCL_InstanceInfo;
	if(!CInstanceRegistry::mcfn_getInstance()->mcfn_getInstanceInfo(CL_EnquireStatusResp.signalingip()+":"+CL_EnquireStatusResp.signalingport(),&pCL_InstanceInfo))
	{	
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processEnquiryResponse", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No InstanceInfo Found for InstanceId:%d,Signalingip:%s,Signalingport:%s",CL_EnquireStatusResp.instanceid(),CL_EnquireStatusResp.signalingip().c_str(),CL_EnquireStatusResp.signalingport().c_str());
		__return__();
	}

	pCL_InstanceInfo->pmcC_RequestTimer->mcfn_stopRequestTimer();
	pCL_InstanceInfo->mcfn_resetHeartBeatMissCount();

	while(1)
	{
		CEnquiryUpdateSet CL_EnquiryUpdateSet;
		if(CL_EnquiryUpdateSet.mcfn_updateEnquiryTime(pCL_InstanceInfo->mcfn_getSignalingIP().c_str(),pCL_InstanceInfo->mcfn_getSignalingPort()))
		{
			break;
		}
		usleep(DBFAILURESLEEPDUR);
	}
	pCL_InstanceInfo->pmcC_EnquiryTimer->mcfn_startEnquiryTimer(this,pCL_InstanceInfo,CG_AppConfigParams.mcfn_getPrimaryEquiryTimerMs());
	__return__();


}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_serilizeResponse
 * Purpose   : serilize EnquiryStatusReq
 * Arguments : siL_InstanceId,string CL_SignalingIp,int siL_SignalingPort
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mefn_serilizeRequest(int siL_InstanceId,string CL_SignalingIp,int siL_SignalingPort)
{
	__entryFunction__;
	EnquireStatusReq CL_EnquireStatusReq;

	CL_EnquireStatusReq.set_instanceid(siL_InstanceId);
	CL_EnquireStatusReq.set_signalingip(CL_SignalingIp);
	CL_EnquireStatusReq.set_signalingport(to_string(siL_SignalingPort));


	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength = CL_EnquireStatusReq.ByteSizeLong();
	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_CmdId 		 = CMD_ENQUIRY_REQ;

	if (!CL_EnquireStatusReq.SerializePartialToArray(meS_ServiceManagerEventResp.pmcsc_ProtoBuffEvent,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength))
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "serilizeResponse", "Failure", this,"", "Serializing Response Data");
	}

	EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "EnquireStatusReq", "Success", this,"", "InstanceId:%d",siL_InstanceId);
	__return__();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Process SocketReconnect
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_processEnquiryResponse
 * Purpose   : process EnquiryResponse
 * Arguments : const EnquireStatusResp&
 * Returns   : Success/Failure
 ************************************************************************/
bool CRegistryEventHandler::mefn_processSocketReconnect(const SocketReconnectReq& CL_SocketReconnectReq)
{
	__entryFunction__;

	EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "processSocketReconnect", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceId:%d, ClientIp:%s, ClientPort:%ld",CL_SocketReconnectReq.instanceid(),pmesc_ClientIp,mesl_ClientPort);

	CInstanceInfo* pCL_InstanceInfo = nullptr;
	if(CInstanceRegistry::mcfn_getInstance()->mcfn_getInstanceInfo(CL_SocketReconnectReq.signalingip()+":"+CL_SocketReconnectReq.signalingport(),&pCL_InstanceInfo))
	{

		if(strcmp(pmesc_ClientIp,pCL_InstanceInfo->mcfn_getClientIP())!=0)
		{
			pCL_InstanceInfo->mcfn_setClientIP(pmesc_ClientIp);
			EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "ClientInfo", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "New Ip:%s, InstanceId:%d",pmesc_ClientIp,pCL_InstanceInfo->mcfn_getInstanceId());
		}

		if(mesl_ClientPort != pCL_InstanceInfo->mcfn_getClientPort())
		{
			pCL_InstanceInfo->mcfn_setClientPort(mesl_ClientPort);
			EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "ClientInfo", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "New Port:%ld, InstanceId:%d",mesl_ClientPort,pCL_InstanceInfo->mcfn_getInstanceId());
		}


		if(pCL_InstanceInfo->pmcC_RequestTimer->mcfn_IsRequestTimerStarted())
		{
			pCL_InstanceInfo->pmcC_RequestTimer->mcfn_stopRequestTimer();

			mefn_serilizeRequest(pCL_InstanceInfo->mcfn_getInstanceId(),pCL_InstanceInfo->mcfn_getSignalingIP(),pCL_InstanceInfo->mcfn_getSignalingPort());
			meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_SyncKey = pCL_InstanceInfo->mcfn_getInstanceId();

			CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&meS_ServiceManagerEventResp,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength,pCL_InstanceInfo->mcfn_getClientIP(),pCL_InstanceInfo->mcfn_getClientPort());

			pCL_InstanceInfo->pmcC_RequestTimer->mcfn_startRequestTimer(this,pCL_InstanceInfo,CG_AppConfigParams.mcfn_getMaxEnqiuryRequestTimeMs());

		}
		__return__(true);	
	}
	mesi_StatusCode = ER_INSTANCEID_NOTREGISTERED;
	sprintf(pmesc_StatusDesc,"Instance With Id:%d is Not Registered,Reconnecting Failed!",CL_SocketReconnectReq.instanceid());
	__return__(false);
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_serilizeResponse
 * Purpose   : serilizeResponse
 * Arguments : const EnquireStatusResp&
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mefn_serilizeResponse(const SocketReconnectReq& CL_SocketReconnectReq,int siL_StatusCode,char* pscL_StatusDesc)
{
        __entryFunction__;
        SocketReconnectResp CL_SocketReconnectResp;

	CL_SocketReconnectResp.set_instanceid(CL_SocketReconnectReq.instanceid());
        CL_SocketReconnectResp.set_statuscode(siL_StatusCode);
        CL_SocketReconnectResp.set_statusdescription(pscL_StatusDesc);

        meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength = CL_SocketReconnectResp.ByteSizeLong();
        meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_CmdId           = CMD_SOCKET_RECONNECT_RESP;

        if (!CL_SocketReconnectResp.SerializePartialToArray(meS_ServiceManagerEventResp.pmcsc_ProtoBuffEvent,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength))
        {
                EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "serilizeResponse", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Serializing Response Data");
        }
	EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "SocketReconnectResp", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "StatusCode:%d, StatusDesc:%s",siL_StatusCode,pscL_StatusDesc);
        __return__();
}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_sendResponse
 * Purpose   : sendResponse to Client
 * Arguments : none
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mefn_sendResponse()
{

	__entryFunction__;
	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_SyncKey = pmeS_ServiceManagerEventReq->mcS_EventHeader.mcsi_SyncKey;
	strcpy(meS_ServiceManagerEventResp.mcS_EventHeader.pmcsc_TransId,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId);

	CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&meS_ServiceManagerEventResp,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength,pmesc_ClientIp,mesl_ClientPort);
	__return__();

}

/************************************************************************
 * Class     : CRegistryEventHandler
 * Method    : mefn_UpdateQueryBuilder
 * Purpose   : build query to generic update
 * Arguments : pscL_Query,siL_Type,siL_TotalResourceCount,siL_InBoundCount,siL_OutBoundCount
 * Returns   : none
 ************************************************************************/

void CRegistryEventHandler::mefn_UpdateQueryBuilder(char* pscL_Query,const int& siL_Type,const int& siL_TotalResourceCount,const int& siL_InBoundCount,const int& siL_OutBoundCount)
{
	__entryFunction__;
	switch(siL_Type)
	{
		case QB_TYPE_TR: sprintf(pscL_Query,"TOTAL_RESOURCE=%d",siL_TotalResourceCount);
			break;
		case QB_TYPE_IBD: sprintf(pscL_Query,"TOTAL_INBOUND_RESOURCE=%d",siL_InBoundCount);
			break;
		case QB_TYPE_TR_IBD: sprintf(pscL_Query,"TOTAL_RESOURCE=%d,TOTAL_INBOUND_RESOURCE=%d",siL_TotalResourceCount,siL_InBoundCount);
			break;
		case QB_TYPE_OBD: sprintf(pscL_Query,"TOTAL_OUTBOUND_RESOURCE=%d",siL_OutBoundCount);
			break;
		case QB_TYPE_TR_OBD: sprintf(pscL_Query,"TOTAL_RESOURCE=%d,TOTAL_OUTBOUND_RESOURCE=%d",siL_TotalResourceCount,siL_OutBoundCount);
			break;
		case QB_TYPE_IBD_OBD: sprintf(pscL_Query,"TOTAL_OUTBOUND_RESOURCE=%d,TOTAL_INBOUND_RESOURCE=%d",siL_OutBoundCount,siL_InBoundCount);
			break;
		case QB_TYPE_TR_IBD_OBD: sprintf(pscL_Query,"TOTAL_RESOURCE=%d,TOTAL_INBOUND_RESOURCE=%d,TOTAL_OUTBOUND_RESOURCE=%d",siL_TotalResourceCount,siL_InBoundCount,siL_OutBoundCount);
			break;
		default:
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "UpdateQueryBuilder", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Type:%d is not a valide Type",siL_Type);
	}

	EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "UpdateQueryBuilder", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Type:%d,Query:%s",siL_Type,pscL_Query);

	__return__();
}
