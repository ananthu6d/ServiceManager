/************************************************************************
 *                                                                       *
 * The information described in this file is a confidential and          *
 * proprietary product of 6d Technologies.                               *
 * (c) 2020 6d Technologies                                              *
 * All Rights Reserved                                                   *
 *                                                                       *
 *************************************************************************
 *
 * ConsumerHandler.cpp
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -----------------------------------------------
 *
 */


/**
 * Header Includes
 */

#include "ConsumerHandler.h"
#include "AppExterns.h"
#include "ServiceHandler.h"
#include "ServiceMaster.h"
#include "UDPEventMonitor.h"

/**
 * Global Variables
 */


/************************************************************************
 * Class     : CConsumerHandler
 * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CConsumerHandler::CConsumerHandler()
{
	__entryFunction__;

	mefn_initialize(); 

	__return__();
}


/************************************************************************
 * Class     : CConsumerHandler
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CConsumerHandler::~CConsumerHandler()
{
	__entryFunction__;
	__return__();
}

/************************************************************************
 * Class     : CConsumerHandler
 * Method    : mefn_initialize
 * Purpose   : initialise the object
 * Arguments : none
 * Returns   : none
 ************************************************************************/
void CConsumerHandler::mefn_initialize()
{
	__entryFunction__;
	
	mesl_SynKey	= 0x00;
	
	__return__();
}

/************************************************************************
 * Class     : CConsumerHandler
 * Method    : mcfn_printObject
 * Purpose   : print object
 * Arguments : none
 * Returns   : none
 ************************************************************************/
void CConsumerHandler::mcfn_printObject()
{
	__entryFunction__;
	__return__();

}

/************************************************************************
 * Class     : CConsumerHandler
 * Method    : mcfn_subscribeEvent
 * Purpose   : subscribe for an event
 * Arguments : @param1: AppCallBack reference
 * 	     : @param2: EventCriteria reference
 * Returns   : none
 ************************************************************************/
void CConsumerHandler::mcfn_subscribeEvent(IAppCallBack *,CEventCriteria *)
{
	__entryFunction__;
	__return__();
}

/************************************************************************
 * Class     : CConsumerHandler
 * Method    : mcfn_unsubscribeEvent
 * Purpose   : unsubscribe for an event
 * Arguments : @param1: AppCallBack reference
 *           : @param2: EventCriteria reference
 * Returns   : none
 ************************************************************************/
void CConsumerHandler::mcfn_unsubscribeEvent(IAppCallBack *,CEventCriteria *)
{
	__entryFunction__;
	__return__();
}
/************************************************************************
 * Class     : CConsumerHandler
 * Method    : mcfn_onNotifyEvent
 * Purpose   : callback method for event reporting
 * Arguments : GenericEvent
 * Returns   : none
 ************************************************************************/
void CConsumerHandler::mcfn_onNotifyEvent(CGenericEvent& CL_GenericEvent,long slL_Val)
{
	__entryFunction__;

	SEventInfo* pSL_EventInfo=(SEventInfo*) CL_GenericEvent.mcfn_getEventObject();

        if(pSL_EventInfo == nullptr)
        {
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "onNotifyEvent", "Failure", this,"","No Data to Process from EventInfo");
		return;
	}
	//Getting Ip and Port
	//strcpy(pmesc_ClientIp,pSL_EventInfo->pmcsc_ClientIp);
	//mesl_ClientPort = pSL_EventInfo->mcsl_ClientPort;

	EVT_LOG(CG_EventLog, LOG_INFO, siG_InstanceID, "onNotifyEvent", "Success", this,"","Event Type:%d, Client Ip:%s Port:%d, EventInfo:%ld",pSL_EventInfo->mcsi_EventType,pSL_EventInfo->pmcsc_ClientIp,pSL_EventInfo->mcsl_ClientPort,pSL_EventInfo->mcsl_EventInfo);


	SServiceManagerEvent* pCL_ServiceManagerEventReq=(SServiceManagerEvent*)pSL_EventInfo->mcsl_EventInfo;
	if(pCL_ServiceManagerEventReq == nullptr)
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "onNotifyEvent", "Failure", this,"","Empty ServiceManagerEvent Request");
		return;
	}

	switch(pCL_ServiceManagerEventReq->mcS_EventHeader.mcsi_CmdId)
	{

		case CMD_FETCH_RESOURCE_REQ:
			{
		
				FetchInstanceReq CL_FetchInstanceReq;

				if(!CL_FetchInstanceReq.ParsePartialFromArray(pCL_ServiceManagerEventReq->pmcsc_ProtoBuffEvent,pCL_ServiceManagerEventReq->mcS_EventHeader.mcsi_ProtoBuffLength))
				{
					mefn_serilizeResponseAndSend(ER_PARSING_DATA,"Parsing Failed",pCL_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,pSL_EventInfo->pmcsc_ClientIp,pSL_EventInfo->mcsl_ClientPort);
					__return__(false);
				}
				mcfn_processFetchResource(CL_FetchInstanceReq,pCL_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,pSL_EventInfo->pmcsc_ClientIp,pSL_EventInfo->mcsl_ClientPort);					
			}
			break;
		case CMD_RELEASE_RESOURCE_REQ:
			{
				ReleaseResourceReq CL_ReleaseResourceReq;

                                if(!CL_ReleaseResourceReq.ParsePartialFromArray(pCL_ServiceManagerEventReq->pmcsc_ProtoBuffEvent,pCL_ServiceManagerEventReq->mcS_EventHeader.mcsi_ProtoBuffLength))
                                {
					mefn_serilizeResponseAndSend(ER_PARSING_DATA,"Parsing Failed",pCL_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,pSL_EventInfo->pmcsc_ClientIp,pSL_EventInfo->mcsl_ClientPort);
                                        __return__(false);
                                }
				mcfn_processReleaseResource(CL_ReleaseResourceReq,pCL_ServiceManagerEventReq->mcS_EventHeader.pmcsc_TransId,pSL_EventInfo->pmcsc_ClientIp,pSL_EventInfo->mcsl_ClientPort);
			}
			break;
	}	
	__return__();
}

void    CConsumerHandler::mcfn_processFetchResource(const FetchInstanceReq& CL_FetchInstanceReq,char* pscL_Tid,char* pscL_ClientIp,long slL_ClientPort)
{
	__entryFunction__;
	CServiceHandler* pCL_ServiceHandler=nullptr;
	if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(CL_FetchInstanceReq.servicetype()+"_"+CL_FetchInstanceReq.serviceid(),&pCL_ServiceHandler))
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pscL_Tid,"No Handler Found for SericeId:%d",atoi(CL_FetchInstanceReq.serviceid().c_str()));
		mefn_serilizeResponseAndSend(0,pscL_Tid,ER_NO_SERVICE,"No Service","",0,pscL_ClientIp,slL_ClientPort);
			__return__();
	}

	CServiceResource* pCL_ServiceResource = nullptr;

	char pscL_SignalingIp[64];
	pscL_SignalingIp[0]	= 0x00;
	long slL_SignalingPort	= 0x00;
	int siL_InstanceId	= 0x00;	
	int siL_ErrorCode	= 0;	

	if(pCL_ServiceHandler->mcfn_fetchResource(pscL_SignalingIp,slL_SignalingPort,siL_InstanceId,siL_ErrorCode))
	{

		if(mefn_serilizeResponseAndSend(siL_InstanceId,pscL_Tid,SUCCESS,"Fetch Successfull",pscL_SignalingIp,slL_SignalingPort,pscL_ClientIp,slL_ClientPort))
		{
			__return__();
		}
		else
		{
			int siL_Temp=0;
			string CL_SignalingIpPort = pscL_SignalingIp;
			CL_SignalingIpPort.append(":"+to_string(slL_SignalingPort));
			pCL_ServiceHandler->mcfn_releaseResource(CL_SignalingIpPort,siL_Temp);
			__return__();
		}
	}
	switch(siL_ErrorCode)
	{
		case ER_NO_INSTANCE_REG:
			{
				mefn_serilizeResponseAndSend(siL_InstanceId,pscL_Tid,ER_NO_INSTANCE_REG,"Instance is Not Registered","",0x00,pscL_ClientIp,slL_ClientPort);
			}
			break;
		case ER_RESOURCE_BUSY:
			{
				mefn_serilizeResponseAndSend(siL_InstanceId,pscL_Tid,ER_RESOURCE_BUSY,"Every Resource Is Busy","",0x00,pscL_ClientIp,slL_ClientPort);
			}
		case ER_NO_RESOURCES_AVAILABLE:
			{	
				mefn_serilizeResponseAndSend(siL_InstanceId,pscL_Tid,ER_NO_RESOURCES_AVAILABLE,"No Resources Available","",0x00,pscL_ClientIp,slL_ClientPort);
			}
	}
	__return__();

}
void    CConsumerHandler::mcfn_processReleaseResource(const ReleaseResourceReq& CL_ReleaseResourceReq,char* pscL_Tid,char* pscL_ClientIp,long slL_ClientPort)
{
	__entryFunction__;
	CServiceHandler* pCL_ServiceHandler=nullptr;
	if(!CServiceMaster::mcfn_getInstance()->mcfn_getServiceHandler(CL_ReleaseResourceReq.servicetype()+"_"+CL_ReleaseResourceReq.serviceid(),&pCL_ServiceHandler))
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "ServiceHandler", "Failure", this,pscL_Tid,"No Handler Found for SericeId:%d",atoi(CL_ReleaseResourceReq.serviceid().c_str()));
		mefn_serilizeResponseAndSend(CL_ReleaseResourceReq.instanceid(),pscL_Tid,ER_NO_SERVICE,"No Service with the ID",pscL_ClientIp,slL_ClientPort);
			__return__();
	}

	int siL_ErrorCode       = 0;
	if(pCL_ServiceHandler->mcfn_releaseResource(CL_ReleaseResourceReq.signalingip()+":"+CL_ReleaseResourceReq.signalingport(),siL_ErrorCode))
	{
		mefn_serilizeResponseAndSend(CL_ReleaseResourceReq.instanceid(),pscL_Tid,SUCCESS,"Resource Released",pscL_ClientIp,slL_ClientPort);
		__return__();

	}

	switch(siL_ErrorCode)
	{
		case ER_NO_INSTANCE_REG:
			{
				mefn_serilizeResponseAndSend(CL_ReleaseResourceReq.instanceid(),pscL_Tid,ER_NO_INSTANCE_REG,"No Instance is Registered",pscL_ClientIp,slL_ClientPort);
			}
			break;
		case ER_RELEASE_FALIURE:
			{
				mefn_serilizeResponseAndSend(CL_ReleaseResourceReq.instanceid(),pscL_Tid,ER_RELEASE_FALIURE,"Release Failed",pscL_ClientIp,slL_ClientPort);
			}
	}
	__return__();
}

bool CConsumerHandler::mefn_serilizeResponseAndSend(const int& siL_InstanceId,char* pscL_Tid,const int& siL_StatusCode,char* pscL_StatusDesc,char* pscL_SignalingIp, long slL_SignalingPort,char* pscL_ClientIp,long slL_ClientPort)
{
	__entryFunction__;

	FetchInstanceResp CL_FetchInstanceResp;

	CL_FetchInstanceResp.set_instanceid(siL_InstanceId);
	CL_FetchInstanceResp.set_transactionid(pscL_Tid);
	CL_FetchInstanceResp.set_statuscode(siL_StatusCode);
	CL_FetchInstanceResp.set_statusdescription(pscL_StatusDesc);
	CL_FetchInstanceResp.set_signalingip(pscL_SignalingIp);
	CL_FetchInstanceResp.set_signalingport(to_string(slL_SignalingPort));

	SServiceManagerEvent SL_ServiceManagerEvent;

	SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength = CL_FetchInstanceResp.ByteSizeLong();
	SL_ServiceManagerEvent.mcS_EventHeader.mcsi_CmdId           = CMD_FETCH_RESOURCE_RESP;
	strcpy(SL_ServiceManagerEvent.mcS_EventHeader.pmcsc_TransId,pscL_Tid);

	if (!CL_FetchInstanceResp.SerializePartialToArray(SL_ServiceManagerEvent.pmcsc_ProtoBuffEvent,SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength))
	{
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "serilizeResponseAndSend", "Failure", this,pscL_Tid, "Serializing Response Data");
		__return__(false);
	}

	if(!CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&SL_ServiceManagerEvent,SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength,pscL_ClientIp,slL_ClientPort))
	{
		__return__(false);	
	}
	EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "FetchInstanceResp", "Success", this,pscL_Tid, "InstanceId:%d, StatusCode:%d, StatusDesc:%s, SignalingIp:%s, SignalingPort:%ld ",siL_InstanceId,siL_StatusCode,pscL_StatusDesc,pscL_SignalingIp,slL_SignalingPort);
	__return__(true);
}

bool CConsumerHandler::mefn_serilizeResponseAndSend(const int& siL_InstanceId,char* pscL_Tid,int siL_StatusCode,char* pscL_StatusDesc,char* pscL_ClientIp,long slL_ClientPort)
{
	__entryFunction__;
	ReleaseResourceResp CL_ReleaseResourceResp;

	CL_ReleaseResourceResp.set_transactionid(pscL_Tid);
	CL_ReleaseResourceResp.set_instanceid(siL_InstanceId);
        CL_ReleaseResourceResp.set_statuscode(siL_StatusCode);
        CL_ReleaseResourceResp.set_statusdescription(pscL_StatusDesc);

	SServiceManagerEvent SL_ServiceManagerEvent;

        SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength = CL_ReleaseResourceResp.ByteSizeLong();
        SL_ServiceManagerEvent.mcS_EventHeader.mcsi_CmdId           = CMD_RELEASE_RESOURCE_RESP;
	strcpy(SL_ServiceManagerEvent.mcS_EventHeader.pmcsc_TransId,pscL_Tid);

        if (!CL_ReleaseResourceResp.SerializePartialToArray(SL_ServiceManagerEvent.pmcsc_ProtoBuffEvent,SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength))
        {
                EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "serilizeResponseAndSend", "Failure", this,pscL_Tid, "Serializing Response Data");
		__return__();
        }

        CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&SL_ServiceManagerEvent,SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength,pscL_ClientIp,slL_ClientPort);
	EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "ReleaseResourceResp", "Success", this,pscL_Tid, "InstanceId:%d, StatusCode:%d, StatusDesc:%s",siL_InstanceId,siL_StatusCode,pscL_StatusDesc);
        __return__();
}

bool CConsumerHandler::mefn_serilizeResponseAndSend(int siL_StatusCode,char* pscL_StatusDesc,char* pscL_Tid,char* pscL_ClientIp,long slL_ClientPort)
{
	__entryFunction__;
	GenericError CL_GenericError;

        CL_GenericError.set_errorcode(siL_StatusCode);
        CL_GenericError.set_errordescription(pscL_StatusDesc);

	SServiceManagerEvent SL_ServiceManagerEvent;

	SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength = CL_GenericError.ByteSizeLong();
	SL_ServiceManagerEvent.mcS_EventHeader.mcsi_CmdId           = CMD_GENERIC_ERROR_RESP;
	strcpy(SL_ServiceManagerEvent.mcS_EventHeader.pmcsc_TransId,pscL_Tid);

	if (!CL_GenericError.SerializePartialToArray(SL_ServiceManagerEvent.pmcsc_ProtoBuffEvent,SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength))
	{       
		EVT_LOG(CG_EventLog, LOG_ERROR, siG_InstanceID, "serilizeResponseAndSend", "Failure", this,pscL_Tid, "Serializing Response Data");
		__return__();
	}
        CUDPEventMonitor::mcfn_getInstance()->mcfn_sendTo((char*)&SL_ServiceManagerEvent,SL_ServiceManagerEvent.mcS_EventHeader.mcsi_ProtoBuffLength,pscL_ClientIp,slL_ClientPort);
	EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO, siG_InstanceID, "GenericErrorResp", "Success", this,pscL_Tid, "StatusCode:%d, StatusDesc:%s",siL_StatusCode,pscL_StatusDesc);
        __return__();

}

