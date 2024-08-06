/************************************************************************
 *                                                                       *
 * The information described in this file is a confidential and          *
 * proprietary product of 6d Technologies.                               *
 * (c) 2020 6d Technologies                                              *
 * All Rights Reserved                                                   *
 *                                                                       *
 *************************************************************************
 *
 * InstanceHandler.cpp
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -----------------------------------------------
 *
 */


/**
 * Header Includes
 */

#include "InstanceHandler.h"
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
 * Class     : CInstanceHandler
 * Method    : ConstructorResourceDeRegistrationReq
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CInstanceHandler::CInstanceHandler()
{
	__entryFunction__;

	pmeS_ServiceManagerEventReq	= nullptr;
	pmeC_RequestTimer		= nullptr;
	pmeC_EnquiryTimer		= nullptr;
	mesl_ClientPort			= 0x00;
	mesi_TotalResourceCount         = 0x00;
        mesi_InboundResourceCount       = 0x00;
        mesi_OutboundResourceCount      = 0x00;
        mesi_InstanceId                 = 0x00;
        mesi_HeartBeatMissCount         = 0x00;
        mesi_SignalingPort              = 0x00;
        mesi_IBDBusyCount               = 0x00;
        mesi_OBDBusyCount               = 0x00;
        memset(pmesc_ClientIP,          0x00, sizeof(pmesc_ClientIP));
        memset(pmesc_LastHeartBeatTime, 0x00, sizeof(pmesc_LastHeartBeatTime));
        memset(pmesc_RegistrationTime,  0x00, sizeof(pmesc_RegistrationTime));
        memset(pmesc_InstanceName,      0x00, sizeof(pmesc_InstanceName));
	pmeC_RequestTimer = new CRequestTimer(this);
	pmeC_EnquiryTimer = new CEnquiryTimer(this);
	mefn_initialize();
	__return__();
}


/************************************************************************
 * Class     : CInstanceHandler
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CInstanceHandler::~CInstanceHandler()
{
	__entryFunction__;

	if(pmeC_RequestTimer)
	{
		delete pmeC_RequestTimer;
		pmeC_RequestTimer = nullptr;
	}

	if(pmeC_EnquiryTimer)
	{
		delete pmeC_EnquiryTimer;
		pmeC_EnquiryTimer = nullptr;
	}

	if(pmeS_ServiceManagerEventReq)
        {
                delete pmeS_ServiceManagerEventReq;
                pmeS_ServiceManagerEventReq=nullptr;
        }

	__return__();
}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_initialize
 * Purpose   : initialise the object
 * Arguments : none
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_initialize()
{
	mesi_StatusCode			= -1;
	pmesc_StatusDesc[0]		= 0x00;
	memset(&meS_ServiceManagerEventResp,0x00,sizeof(meS_ServiceManagerEventResp));

	if(pmeS_ServiceManagerEventReq)
	{
		delete pmeS_ServiceManagerEventReq;
		pmeS_ServiceManagerEventReq=nullptr;
	}
}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mcfn_printObject
 * Purpose   : print object
 * Arguments : none
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mcfn_printObject()
{
	__entryFunction__;
	__return__();

}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mcfn_subscribeEvent
 * Purpose   : subscribe for an event
 * Arguments : @param1: AppCallBack reference
 * 	     : @param2: EventCriteria reference
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mcfn_subscribeEvent(IAppCallBack *,CEventCriteria *)
{
	__entryFunction__;
	__return__();
}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mcfn_unsubscribeEvent
 * Purpose   : unsubscribe for an event
 * Arguments : @param1: AppCallBack reference
 *           : @param2: EventCriteria reference
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mcfn_unsubscribeEvent(IAppCallBack *,CEventCriteria *)
{
	__entryFunction__;
	__return__();
}


/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mcfn_onNotifyEvent
 * Purpose   : callback method for event reporting
 * Arguments : GenericEvent
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mcfn_onNotifyEvent(CGenericEvent& CL_GenericEvent,long slL_Val)
{
	__entryFunction__;

	mefn_initialize();
	SEventInfo* pSL_EventInfo=(SEventInfo*) CL_GenericEvent.mcfn_getEventObject();

	if(pSL_EventInfo == nullptr)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "onNotifyEvent", "Failure", this,"","No Data to Process from EventInfo");
		return;
	}

	EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "onNotifyEvent", "Success", this,"","Event Type:%d, EventInfo:%ld",pSL_EventInfo->mcsi_EventType,pSL_EventInfo->mcsl_EventInfo);

	switch (pSL_EventInfo->mcsi_EventType)
	{
		case EVT_NETWORK: 
			{
				if(strcmp(pmesc_ClientIp,pSL_EventInfo->pmcsc_ClientIp) != 0)
				{
					strcpy(pmesc_ClientIp,pSL_EventInfo->pmcsc_ClientIp);
				}
				if(mesl_ClientPort != pSL_EventInfo->mcsl_ClientPort)
				{
					mesl_ClientPort = pSL_EventInfo->mcsl_ClientPort;
				}

				pmeS_ServiceManagerEventReq = (SServiceManagerEvent*)pSL_EventInfo->mcsl_EventInfo;
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
				mefn_processEnquiryTimeout();
				break;
			}

		case EVT_REQUEST_TIMEOUT:
			{
				mefn_processRequestTimeout();
				break;
			}
		case EVT_FORCE_ENQUIRY:
			{
				mefn_processForceEnquiry();
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
 * Class     : CInstanceHandler
 * Method    : mefn_processNetworkEvent
 * Purpose   : process network events
 * Arguments : none
 * Returns   : none
 ************************************************************************/

bool CInstanceHandler::mefn_processNetworkEvent()
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
 * Class     : CInstanceHandler
 * Method    : mefn_processRegisterInstance
 * Purpose   : process registration of Instance
 * Arguments : const ResourceRegistrationReq&
 * Returns   : Success/Failure
 ************************************************************************/

bool CInstanceHandler::mefn_processRegisterInstance(const ResourceRegistrationReq& CL_ResourceRegistrationReq)
{
	__entryFunction__;

	//printing Alldetails in log
	if(!CL_ResourceRegistrationReq.ibdservicelist_size() > 0)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processRegisterInstance", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "ServiceList is empty, InstanceId :%d,SignalingIp:%s,Port:%d",CL_ResourceRegistrationReq.instanceid(),CL_ResourceRegistrationReq.signalingip().c_str(),CL_ResourceRegistrationReq.signalingport());
		mesi_StatusCode	= ER_NOSERVICELIST;
		sprintf(pmesc_StatusDesc,"ServiceList is Empty,Registration Failed!");	
		__return__(false);
	}

EVT_LOG(CG_EventLog, LOG_INFO |LOG_OPINFO, siG_InstanceID, "processRegisterInstance", "ReqData", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceID:%d,signalingip:%s,signalingport:%d,totalresourcecount:%d,inboundreourcecount:%d,outboundresourcecount:%d,ClientIP:%s,ClientPort:%d, ServiceList:%s",CL_ResourceRegistrationReq.instanceid(),CL_ResourceRegistrationReq.signalingip().c_str(),CL_ResourceRegistrationReq.signalingport(),CL_ResourceRegistrationReq.totalresourcecount(),CL_ResourceRegistrationReq.inboundresourcecount(),CL_ResourceRegistrationReq.outboundresourcecount(),pmesc_ClientIp,mesl_ClientPort,mefn_getServiceLists(CL_ResourceRegistrationReq).c_str());


	//Checking for wheather the request is for Reg or reReg
	if(mesi_InstanceId != 0x00)
	{
		__return__(mefn_processReregistrationResource(CL_ResourceRegistrationReq));
	}

	__return__(mefn_processRegistrationResource(CL_ResourceRegistrationReq));

}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_processRegistrationResource
 * Purpose   : process registration of Resource
 * Arguments : const ResourceRegistrationReq&
 * Returns   : Success/Failure
 ************************************************************************/

bool CInstanceHandler::mefn_processRegistrationResource(const ResourceRegistrationReq& CL_ResourceRegistrationReq)
{
	__entryFunction__;

	//SetInstance Values
	mefn_setInstanceInfo(CL_ResourceRegistrationReq);

	//Insert details to Database
	mefn_InsertInstanceInfoToDB();

	//Registering to ServiceHandler and Inserting to ServiceInfoMap
	for(const auto& lL_Itr : CL_ResourceRegistrationReq.ibdservicelist())
	{
		CServiceHandler* pCL_ServiceHandler=nullptr;
		if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(lL_Itr.second+"_"+lL_Itr.first,&pCL_ServiceHandler))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for ServiceTypeId:%s",lL_Itr.first.c_str());
			continue;
		}
	
		//Inserting to Cache
		CServiceInfo* pCL_ServiceInfo =  new CServiceInfo();
		pCL_ServiceInfo->mcfn_setServiceId(lL_Itr.second);
		pCL_ServiceInfo->mcfn_setServiceName(pCL_ServiceHandler->mcfn_getServiceName());
		pCL_ServiceInfo->mcfn_setServiceType(lL_Itr.first);
		meC_ServiceInfoMap.insert({lL_Itr.second+"_"+lL_Itr.first,pCL_ServiceInfo});

		//Dispatching to ServieHandler for Registration
		CServiceResource* pCL_ServiceResource= new CServiceResource();
		pCL_ServiceResource->mcfn_setSignalingIP(meC_SignalingIp);
		pCL_ServiceResource->mcfn_setSignalingPort(mesi_SignalingPort);
		pCL_ServiceResource->mcfn_setInstanceId(mesi_InstanceId);
		pCL_ServiceResource->mcfn_setTotalChannel(mesi_InboundResourceCount);
		pCL_ServiceResource->mcfn_setServiceId(atoi(lL_Itr.second.c_str()));
		pCL_ServiceResource->mcfn_setServiceType(lL_Itr.first);
		CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_ADD_RESOURCE,(long)pCL_ServiceResource);

	}

	pmeC_EnquiryTimer->mcfn_startEnquiryTimer(CG_AppConfigParams.mcfn_getPrimaryEquiryTimerMs());

	__return__(true);
}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_setInstanceInfo
 * Purpose   : set values of InstanceInfo
 * Arguments : const ResourceRegistrationReq&
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_setInstanceInfo(const ResourceRegistrationReq& CL_ResourceRegistrationReq)
{

	strcpy(pmesc_InstanceName,CL_ResourceRegistrationReq.instancename().c_str());
	strcpy(pmesc_RegistrationTime,CL_ResourceRegistrationReq.eventtimestamp().c_str());

	mesc_Status 			= 'A';
	meC_SignalingIp			= CL_ResourceRegistrationReq.signalingip();
	mesi_TotalResourceCount		= CL_ResourceRegistrationReq.totalresourcecount();
	mesi_InboundResourceCount	= CL_ResourceRegistrationReq.inboundresourcecount();
	mesi_OutboundResourceCount	= CL_ResourceRegistrationReq.outboundresourcecount();
	mesi_SignalingPort		= CL_ResourceRegistrationReq.signalingport();
	mesi_InstanceId			= CL_ResourceRegistrationReq.instanceid();

}
/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_InsertInstanceInfoToDB
 * Purpose   : insert InstanceInfo to db
 * Arguments : CInstanceInfo*
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_InsertInstanceInfoToDB()
{
	__entryFunction__;
	while(1)
	{
		CRegisterInstanceSet CL_RegisterInstanceSet;
		if(CL_RegisterInstanceSet.mcfn_insertIntoInstanceMaster(pmesc_InstanceName,pmesc_RegistrationTime,mesc_Status,meC_SignalingIp.c_str(),mesi_TotalResourceCount,mesi_InboundResourceCount,mesi_OutboundResourceCount,mesi_SignalingPort,mesi_InstanceId))
		{
			break;
		}
		usleep(DBFAILURESLEEPDUR);
	}	
	__return__();

}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_processReregistrationResource
 * Purpose   : process reregistration
 * Arguments : const ResourceRegistrationReq&,CInstanceInfo*
 * Returns   : Success/Failure
 ************************************************************************/

bool CInstanceHandler::mefn_processReregistrationResource(const ResourceRegistrationReq& CL_ResourceRegistrationReq)
{
	__entryFunction__;
	
	//Activating Instance If it Deactivated
	mefn_activateInstance();

	//update the Resource
	if(!mefn_updateResources(CL_ResourceRegistrationReq))
	{
		__return__(false);			
	}

	pmeC_EnquiryTimer->mcfn_startEnquiryTimer(CG_AppConfigParams.mcfn_getPrimaryEquiryTimerMs());
	__return__(true);
}

void CInstanceHandler::mefn_updateRegisterDataBase(const ResourceRegistrationReq& CL_ResourceRegistrationReq)
{
	__entryFunction__;
	char pscL_Query[256];
	pscL_Query[0]	= 0x00;
	int siL_Type	= 0x00;
	if( mesi_TotalResourceCount != CL_ResourceRegistrationReq.totalresourcecount() )
	{
		mesi_TotalResourceCount = CL_ResourceRegistrationReq.totalresourcecount();
		siL_Type |= 1;
	}

	if(mesi_InboundResourceCount != CL_ResourceRegistrationReq.inboundresourcecount())
	{
		mesi_InboundResourceCount = CL_ResourceRegistrationReq.inboundresourcecount();
		siL_Type |= 2;
	}

	if(mesi_OutboundResourceCount != CL_ResourceRegistrationReq.outboundresourcecount())
	{
		mesi_OutboundResourceCount = CL_ResourceRegistrationReq.outboundresourcecount();
		siL_Type |= 4;
	}

	if(siL_Type == 0x00)
	{
		while(1)
		{
			CUpdateInstanceSet CL_UpdateInstanceSet;
			if(CL_UpdateInstanceSet.mcfn_activateInstance(meC_SignalingIp.c_str(),mesi_SignalingPort,CL_ResourceRegistrationReq.eventtimestamp().c_str()))
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
			if(CL_UpdateInstanceSet.mcfn_resourceGenericUpdate(meC_SignalingIp.c_str(),mesi_SignalingPort,pscL_Query,CL_ResourceRegistrationReq.eventtimestamp().c_str()))
				break;
			usleep(DBFAILURESLEEPDUR);
		}
	}
	__return__();
}
/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_updateResources
 * Purpose   : update Resource in Db and Cache
 * Arguments : const ResourceRegistrationReq&
 * Returns   : Success/Failure
 ************************************************************************/

bool CInstanceHandler::mefn_updateResources(const ResourceRegistrationReq& CL_ResourceRegistrationReq)
{
	__entryFunction__;
	char scL_Status	= 0x00;
	int siL_MaxLimt	= 0x00;
	

	for (const auto& lL_Itr : meC_ServiceInfoMap)
	{
		if(CL_ResourceRegistrationReq.ibdservicelist().find(lL_Itr.second->mcfn_getServiceId()) == CL_ResourceRegistrationReq.ibdservicelist().end())
		{
			lL_Itr.second->mcfn_deActivateService();
			EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "updateResources", "Success", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "Deactivated ServiceId:%d",lL_Itr.first);
			CServiceHandler* pCL_ServiceHandler = nullptr;
			if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(lL_Itr.first,&pCL_ServiceHandler))
			{
				EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for ServiceTypeId:%s",lL_Itr.first.c_str());
			}
			CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_DEACT_RESOURCE,meC_SignalingIp.c_str(),mesi_SignalingPort);

		}	
	}


	CServiceHandler* pCL_ServiceHandler = nullptr;
	for(const auto& CL_ServiceDetails : CL_ResourceRegistrationReq.ibdservicelist())
	{

		if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first,&pCL_ServiceHandler))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for ServiceTypeId:%s_%s",CL_ServiceDetails.second.c_str(),CL_ServiceDetails.first.c_str());
			continue;
		}

		if(meC_ServiceInfoMap.find(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first) != meC_ServiceInfoMap.end())
		{
			if(scL_Status == 'D')
			{
				mefn_activateService(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first);
				CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_ACT_RESOURCE,meC_SignalingIp.c_str(),mesi_SignalingPort);

			}
			else
			{
				CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_RESET_BUSYCOUNT,meC_SignalingIp.c_str(),mesi_SignalingPort);
			}
		}
		else
		{
			EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "processReregistrationResource", "NewService", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "ServiceId:%d,ServiceType:%s",atoi(CL_ServiceDetails.first.c_str()),CL_ServiceDetails.second);

			//Dispatching to ServiceHandler
			CServiceResource* pCL_ServiceResource= new CServiceResource();
			pCL_ServiceResource->mcfn_setSignalingIP(meC_SignalingIp);
			pCL_ServiceResource->mcfn_setSignalingPort(mesi_SignalingPort);
			pCL_ServiceResource->mcfn_setInstanceId(mesi_InstanceId);
			pCL_ServiceResource->mcfn_setTotalChannel(mesi_InboundResourceCount);
			pCL_ServiceResource->mcfn_setServiceId(atoi(CL_ServiceDetails.first.c_str()));
			CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_ADD_RESOURCE,(long)pCL_ServiceResource);

			//Inserting to Cache
			CServiceInfo* pCL_ServiceInfo =  new CServiceInfo();
			pCL_ServiceInfo->mcfn_setServiceId(CL_ServiceDetails.second);
			pCL_ServiceInfo->mcfn_setServiceName(pCL_ServiceHandler->mcfn_getServiceName());
			pCL_ServiceInfo->mcfn_setServiceType(CL_ServiceDetails.first);
			meC_ServiceInfoMap.insert({CL_ServiceDetails.second+"_"+CL_ServiceDetails.first,pCL_ServiceInfo});

		}

	}
	__return__(true);
}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_serilizeResponse
 * Purpose   : serialize registration response
 * Arguments : const ResourceRegistrationReq&,siL_StatusCode,pscL_StatusDesc
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_serilizeResponse(const ResourceRegistrationReq& CL_ResourceRegistrationReq,int siL_StatusCode,char* pscL_StatusDesc)
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
 * Class     : CInstanceHandler
 * Method    : mefn_processDeRegisterInstance
 * Purpose   : process to deregister instance
 * Arguments : const ResourceDeRegistrationReq&
 * Returns   : Success/Failure
 ************************************************************************/

bool CInstanceHandler::mefn_processDeRegisterInstance(const ResourceDeRegistrationReq& CL_ResourceDeRegistrationReq)
{

	__entryFunction__;
	EVT_LOG(CG_EventLog, LOG_INFO |LOG_OPINFO, siG_InstanceID, "processDeRegisterInstance", "ReqData", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceID:%d,signalingip:%s,signalingport:%d,ClientIP:%s,ClientPort:%d",CL_ResourceDeRegistrationReq.instanceid(),CL_ResourceDeRegistrationReq.signalingip().c_str(),CL_ResourceDeRegistrationReq.signalingport(),pmesc_ClientIp,mesl_ClientPort);

	pmeC_RequestTimer->mcfn_stopRequestTimer();
	pmeC_EnquiryTimer->mcfn_stopEnquiryTimer();
	mefn_deActivateInstance();	

	while(1)
	{	
		CDeRegisterInstanceSet CL_DeRegisterInstanceSet;	
		if(CL_DeRegisterInstanceSet.mcfn_deRegisterInstance(meC_SignalingIp.c_str(),mesi_SignalingPort))
		{
			break;
		}
		usleep(DBFAILURESLEEPDUR);
	}

	for(const auto& lL_Itr : meC_ServiceInfoMap)
	{
		lL_Itr.second->mcfn_deActivateService();
		CServiceHandler* pCL_ServiceHandler=nullptr;
		if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(lL_Itr.first,&pCL_ServiceHandler))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for ServiceTypeId:%s",lL_Itr.first.c_str());
		}
		CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_REMOVE_RESOURCE,meC_SignalingIp.c_str(),mesi_SignalingPort);

	}
	__return__(true);
}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_processDeRegisterServiceResource
 * Purpose   : process to deregister service
 * Arguments : const ResourceDeRegistrationReq&
 * Returns   : Success/Failure
 ************************************************************************/

bool CInstanceHandler::mefn_processDeRegisterServiceResource(const ResourceDeRegistrationReq& CL_ResourceDeRegistrationReq)
{

	__entryFunction__;

	if(!CL_ResourceDeRegistrationReq.servicelist_size()<0)
        {
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processDeRegisterInstance", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "ServiceList is empty, InstanceId :%d,SignalingIp:%s,Port:%d",CL_ResourceDeRegistrationReq.instanceid(),CL_ResourceDeRegistrationReq.signalingip().c_str(),CL_ResourceDeRegistrationReq.signalingport());
                mesi_StatusCode = ER_NOSERVICELIST;
                sprintf(pmesc_StatusDesc,"ServiceList is Empty,Deregistration Failed!");
                __return__(false);

        }

	EVT_LOG(CG_EventLog, LOG_INFO |LOG_OPINFO, siG_InstanceID, "processDeRegisterServiceResource", "ReqData", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceID:%d,signalingip:%s,signalingport:%d,servicelistcount:%d,ClientIP:%s,ClientPort:%d, ServiceList:%s",CL_ResourceDeRegistrationReq.instanceid(),CL_ResourceDeRegistrationReq.signalingip().c_str(),CL_ResourceDeRegistrationReq.signalingport(),CL_ResourceDeRegistrationReq.servicelist_size(),pmesc_ClientIp,mesl_ClientPort,mefn_getServiceLists(CL_ResourceDeRegistrationReq).c_str());
	
	for(const auto& CL_ServiceDetails : CL_ResourceDeRegistrationReq.servicelist())
	{    
	       	mefn_deActivateService(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first);	
		CServiceHandler* pCL_ServiceHandler=nullptr;
		if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(CL_ServiceDetails.second+"_"+CL_ServiceDetails.first,&pCL_ServiceHandler))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for ServiceTypeId:%s_%s",CL_ServiceDetails.second.c_str(),CL_ServiceDetails.first.c_str());
			continue;
		}
		CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_REMOVE_RESOURCE,meC_SignalingIp.c_str(),mesi_SignalingPort);
	}
	__return__(true);
}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_serilizeResponse
 * Purpose   : serilize deregistration response
 * Arguments : const ResourceDeRegistrationReq&,statuscode,statusdesc,CmdId
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_serilizeResponse(const ResourceDeRegistrationReq& CL_ResourceDeRegistrationReq,int siL_StatusCode,char* pscL_StatusDesc,int siL_Cmdid)
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
 * Class     : CInstanceHandler
 * Method    : mefn_processRequestTimeout
 * Purpose   : process Request Timeout
 * Arguments : none
 * Returns   : none
 ************************************************************************/

void CInstanceHandler:: mefn_processRequestTimeout()
{

	__entryFunction__;

	if(mesi_HeartBeatMissCount+1 <= CG_AppConfigParams.mcfn_getMaxEnquiryMissCount())
	{
		EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "processRequestTimeout", "Success", this,"", "Count:%d,Type:%d",mesi_HeartBeatMissCount+1,CG_AppConfigParams.mcfn_getPeriodicEnquiryType());
		mesi_HeartBeatMissCount++;
		while(1)
		{
			CEnquiryMissCountIncrementSet CL_EnquiryMissCountIncrementSet;
			if(CL_EnquiryMissCountIncrementSet.mcfn_incrementEnquiryCount(meC_SignalingIp.c_str(),mesi_SignalingPort))
			{
				break;
			}
			usleep(DBFAILURESLEEPDUR);
		}
		switch(CG_AppConfigParams.mcfn_getPeriodicEnquiryType())
		{
			//send Enquiry Immediatly to Instance
			case PERIODIC_RETRY_TYPE_IMMEDIATE:
				{
					mefn_serilizeRequest(mesi_InstanceId,meC_SignalingIp,mesi_SignalingPort);

					meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_SyncKey = mesi_InstanceId;
					CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&meS_ServiceManagerEventResp,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength,pmesc_ClientIp,mesl_ClientPort);
					pmeC_RequestTimer->mcfn_startRequestTimer(CG_AppConfigParams.mcfn_getMaxEnqiuryRequestTimeMs());
					break;
				}

			//Send Enquiry Req from Configured Time
			case PERIODIC_RETRY_TYPE_INTERVEL:
				{

					auto lL_Itr = CG_AppConfigParams.mcC_PeriodicEnquiryDetailsMap.find(mesi_HeartBeatMissCount);
					if(lL_Itr != CG_AppConfigParams.mcC_PeriodicEnquiryDetailsMap.end())
					{
						pmeC_EnquiryTimer->mcfn_startEnquiryTimer(lL_Itr->second);
						break;
					}	
					EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "processRequestTimeout", "Failure", this,"","Finding Next Retrying TimeMs from PeriodicEnquiryDetailsMap,MissCount:%d",mesi_HeartBeatMissCount);
					break;
				}
			//Calculate New EnquiryTime and Send Enquiry Req
			case PERIODIC_RETRY_TYPE_DEFAULT:
				{
					int siL_NewEnquiryTimer = (CG_AppConfigParams.mcfn_getPrimaryEquiryTimerMs()/CG_AppConfigParams.mcfn_getMaxEnquiryMissCount())*(CG_AppConfigParams.mcfn_getMaxEnquiryMissCount() - mesi_HeartBeatMissCount + 1);

					if(siL_NewEnquiryTimer>0)
					{
						pmeC_EnquiryTimer->mcfn_startEnquiryTimer(siL_NewEnquiryTimer);
					}
					break;
				}
		}
		__return__();
	}


	//If MissCount is more than configured count, that means instance is down 
	mefn_deActivateInstance();
	while(1)
        {
		CDeRegisterInstanceSet CL_DeRegisterInstanceSet;
                if(CL_DeRegisterInstanceSet.mcfn_deRegisterInstance(meC_SignalingIp.c_str(),mesi_SignalingPort))
                {
                        break;
                }
                usleep(DBFAILURESLEEPDUR);
        }

        for(const auto& lL_Itr : meC_ServiceInfoMap)
        {
                lL_Itr.second->mcfn_deActivateService();
                CServiceHandler* pCL_ServiceHandler=nullptr;
                if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(lL_Itr.first,&pCL_ServiceHandler))
                {
                        EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,"No Handler Found for ServiceTypeId:%s",lL_Itr.first.c_str());
                }
                CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_REMOVE_RESOURCE,meC_SignalingIp.c_str(),mesi_SignalingPort);

        }
	__return__();
}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_processEnquiryTimeout
 * Purpose   : process Enquiry Timeout
 * Arguments : none
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_processEnquiryTimeout()
{

	__entryFunction__;
	mefn_serilizeRequest(mesi_InstanceId,meC_SignalingIp,mesi_SignalingPort);
	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_SyncKey = mesi_InstanceId;
	
	CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&meS_ServiceManagerEventResp,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength,pmesc_ClientIp,mesl_ClientPort);

	pmeC_RequestTimer->mcfn_startRequestTimer(CG_AppConfigParams.mcfn_getMaxEnqiuryRequestTimeMs());
	__return__();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Generic Response
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_serilizeResponse
 * Purpose   : serilize Generic Response
 * Arguments : siL_StatusCode,pscL_StatusDesc
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_serilizeResponse(int siL_StatusCode,char* pscL_StatusDesc)
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
 * Class     : CInstanceHandler
 * Method    : mefn_processEnquiryResponse
 * Purpose   : process EnquiryResponse
 * Arguments : const EnquireStatusResp&
 * Returns   : None
 ************************************************************************/

void CInstanceHandler::mefn_processEnquiryResponse(const EnquireStatusResp& CL_EnquireStatusResp)
{
	__entryFunction__;

	EVT_LOG(CG_EventLog, LOG_INFO |LOG_OPINFO, siG_InstanceID, "processEnquiryResponse", "RespData", this,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId, "InstanceId:%d,Signalingip:%s,Signalingport:%d,ClientIp:%s,ClientPort:%ld,ServiceBusyCountMap:%s",CL_EnquireStatusResp.instanceid(),CL_EnquireStatusResp.signalingip().c_str(),CL_EnquireStatusResp.signalingport(),pmesc_ClientIp,mesl_ClientPort,mefn_getServiceLists(CL_EnquireStatusResp).c_str());
	
	pmeC_RequestTimer->mcfn_stopRequestTimer();
	mesi_HeartBeatMissCount = 0x00;

	while(1)
	{
		CEnquiryUpdateSet CL_EnquiryUpdateSet;
		if(CL_EnquiryUpdateSet.mcfn_updateEnquiryTime(meC_SignalingIp.c_str(),mesi_SignalingPort))
		{
			break;
		}
		usleep(DBFAILURESLEEPDUR);
	}

	for (const auto& lL_Itr : meC_ServiceInfoMap)
	{
		CServiceHandler* pCL_ServiceHandler = nullptr;                        
		if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(lL_Itr.first,&pCL_ServiceHandler))
		{
			EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,"","No Handler Found for ServiceTypeId:%s",lL_Itr.first.c_str());
		}

		auto lL_ServiceItr = CL_EnquireStatusResp.ibdbusycount().find(lL_Itr.second->mcfn_getServiceId());
		if(lL_ServiceItr == CL_EnquireStatusResp.ibdbusycount().end())
		{
			lL_Itr.second->mcfn_deActivateService();
			CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_DEACT_RESOURCE,meC_SignalingIp.c_str(),mesi_SignalingPort);
		}
		else
		{
			CUDPEventMonitor::mcfn_getInstance()->mcfn_dispatchEvent(pCL_ServiceHandler,EVT_VALIDATE_BUSYCOUNT,meC_SignalingIp.c_str(),mesi_SignalingPort,lL_ServiceItr->second);	
		}
		
	}
	pmeC_EnquiryTimer->mcfn_startEnquiryTimer(CG_AppConfigParams.mcfn_getPrimaryEquiryTimerMs());
	__return__();


}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_serilizeResponse
 * Purpose   : serilize EnquiryStatusReq
 * Arguments : siL_InstanceId,string CL_SignalingIp,int siL_SignalingPort
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_serilizeRequest(int siL_InstanceId,string CL_SignalingIp,int siL_SignalingPort)
{
	__entryFunction__;
	EnquireStatusReq CL_EnquireStatusReq;

	CL_EnquireStatusReq.set_instanceid(siL_InstanceId);
	CL_EnquireStatusReq.set_signalingip(CL_SignalingIp);
	CL_EnquireStatusReq.set_signalingport(siL_SignalingPort);


	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength = CL_EnquireStatusReq.ByteSizeLong();
	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_CmdId 		 = CMD_ENQUIRY_REQ;

	if (!CL_EnquireStatusReq.SerializePartialToArray(meS_ServiceManagerEventResp.pmcsc_ProtoBuffEvent,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength))
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "serilizeResponse", "Failure", this,"", "Serializing Response Data");
	}

	EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "EnquireStatusReq", "Success", this,"", "InstanceId:%d",siL_InstanceId);
	__return__();
}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_sendResponse
 * Purpose   : sendResponse to Client
 * Arguments : none
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_sendResponse()
{

	__entryFunction__;
	meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_SyncKey = pmeS_ServiceManagerEventReq->mcS_EventHeader.mcsi_SyncKey;
	strcpy(meS_ServiceManagerEventResp.mcS_EventHeader.pmcsc_TransId,pmeS_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId);

	CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&meS_ServiceManagerEventResp,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength,pmesc_ClientIp,mesl_ClientPort);
	__return__();

}

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_UpdateQueryBuilder
 * Purpose   : build query to generic update
 * Arguments : pscL_Query,siL_Type,siL_TotalResourceCount,siL_InBoundCount,siL_OutBoundCount
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_UpdateQueryBuilder(char* pscL_Query,const int& siL_Type,const int& siL_TotalResourceCount,const int& siL_InBoundCount,const int& siL_OutBoundCount)
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

/************************************************************************
 * Class     : CInstanceHandler
 * Method    : mefn_processEnquiryTimeout
 * Purpose   : process Enquiry Timeout
 * Arguments : CInstanceInfo*
 * Returns   : none
 ************************************************************************/

void CInstanceHandler::mefn_processForceEnquiry()
{

	__entryFunction__;
	if(!pmeC_RequestTimer->mcfn_IsRequestTimerStarted())
	{
		pmeC_EnquiryTimer->mcfn_stopEnquiryTimer();
		mefn_serilizeRequest(mesi_InstanceId,meC_SignalingIp,mesi_SignalingPort);
		meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_SyncKey = mesi_InstanceId;

		CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&meS_ServiceManagerEventResp,meS_ServiceManagerEventResp.mcS_EventHeader.mcsi_ProtoBuffLength,pmesc_ClientIp,mesl_ClientPort);

		pmeC_RequestTimer->mcfn_startRequestTimer(CG_AppConfigParams.mcfn_getMaxEnqiuryRequestTimeMs());
	}
	__return__();

}

bool CInstanceHandler::mefn_activateService(const string& CL_ServiceType_Id)
{
        auto lL_Itr= meC_ServiceInfoMap.find(CL_ServiceType_Id);
        if(lL_Itr!=meC_ServiceInfoMap.end())
        {
                lL_Itr->second->mcfn_activateService();
                return true;
        }
        return false;
}

bool CInstanceHandler::mefn_deActivateService(const string& CL_ServiceType_Id)
{
        auto lL_Itr= meC_ServiceInfoMap.find(CL_ServiceType_Id);
        if(lL_Itr!=meC_ServiceInfoMap.end())
        {
                lL_Itr->second->mcfn_deActivateService();
                return true;
        }
        return false;
}


void CInstanceHandler::mefn_deActivateInstance()
{
        mesc_Status             = 'D';
        mesi_IBDBusyCount       = 0x00;
	mesi_OBDBusyCount       = 0x00;
}

void CInstanceHandler::mefn_activateInstance()
{
	if(mesc_Status == 'D')
		mesc_Status = 'A';
}

string CInstanceHandler::mefn_getServiceLists(const ResourceRegistrationReq& CL_ResourceRegistrationReq)
{
	string CL_ServiceList = "IBD:[ ";
        bool bL_IsFirstTime = true;

        for(const auto& lL_ServiceItr : CL_ResourceRegistrationReq.ibdservicelist())
        {
                if(!bL_IsFirstTime)
                {
                        CL_ServiceList.append(",");
                        bL_IsFirstTime = false;
                }

                CL_ServiceList.append(lL_ServiceItr.second);
        }
        CL_ServiceList.append(" ], OBD:[ ");

	bL_IsFirstTime = true;
	for(const auto& lL_ServiceItr : CL_ResourceRegistrationReq.obdservicelist())
        {
                if(!bL_IsFirstTime)
                {
                        CL_ServiceList.append(",");
                        bL_IsFirstTime = false;
                }

                CL_ServiceList.append(lL_ServiceItr.second);
        }
	CL_ServiceList.append(" ]");

	return CL_ServiceList;
}

string CInstanceHandler::mefn_getServiceLists(const ResourceDeRegistrationReq& CL_ResourceDeRegistrationReq)
{
	string CL_ServiceList = "[ ";
        bool bL_IsFirstTime = true;

        for(const auto& lL_ServiceItr : CL_ResourceDeRegistrationReq.servicelist())
        {
                if(!bL_IsFirstTime)
                {
                        CL_ServiceList.append(",");
                        bL_IsFirstTime = false;
                }

                CL_ServiceList.append(lL_ServiceItr.second);
        }
        CL_ServiceList.append(" ]");

	/*
	bL_IsFirstTime = true;
	for(const auto& lL_ServiceItr : CL_ResourceDeRegistrationReq.obdbusycount())
        {
                if(!bL_IsFirstTime)
                {
                        CL_ServiceList.append(",");
                        bL_IsFirstTime = false;
                }

                CL_ServiceList.append(lL_ServiceItr.second);
        }
	CL_ServiceList.append(" ]");

	*/
	return CL_ServiceList;
}

string CInstanceHandler::mefn_getServiceLists(const EnquireStatusResp& CL_EnquireStatusResp)
{
	string CL_ServiceBusyCount = "[ ";
        bool bL_IsFirstTime = true;

        for(const auto& lL_BusyCountItr : CL_EnquireStatusResp.ibdbusycount())
        {
                if(!bL_IsFirstTime)
                {
                        CL_ServiceBusyCount.append(",");
                        bL_IsFirstTime = false;
                }

                CL_ServiceBusyCount.append(lL_BusyCountItr.first+":"+to_string(lL_BusyCountItr.second));
        }
        CL_ServiceBusyCount.append(" ]");
	return  CL_ServiceBusyCount;
}
