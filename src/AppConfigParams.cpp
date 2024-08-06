/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* AppConfigParams.cpp
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


/**
 * Header Includes
 */

#include "AppConfigParams.h"
#include "AppExterns.h"


using namespace _6d;


/**
 * Global Variables
 */

/************************************************************************
 * Class     : CAppConfigParams
cout<<"\nEnquiry InstanceId: "<<CL_EnquireStatusReq.instanceid(); * Method    : Constructor
 * Purpose   : Creates and Initializes the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CAppConfigParams::CAppConfigParams()
{
	mdfn_register(mcfn_getConfigCLSID(), true);
	pmesc_UDPServerIP[0]		= 0x00;
	mesi_UDPServerPort		= 0x00;
	mesi_ConsumerHandlerCount	= 0x00;


}
/************************************************************************
 * Class     : CAppConfigParams
 * Method    : Destructor
 * Purpose   : Destroys and Releases the object
 * Arguments : None
 * Returns   : None
 ************************************************************************/

CAppConfigParams::~CAppConfigParams()
{
	mdfn_register(mcfn_getConfigCLSID(), false);
}


/************************************************************************
 * Class     : CAppConfigParams
 * Method    : mefn_readCfgParam
 * Purpose   : Read configuration and assign the value
 * Arguments : None
 * Returns   : void
 ************************************************************************/

void CAppConfigParams::mefn_readCfgParam(XMLNode &xMainNode,const char *pscL_CfgParamKey,int siL_DataType,void *pL_Value)
{
	XMLNode xNode;
	xNode = xMainNode.getChildNode(pscL_CfgParamKey);
	if(xNode.isEmpty())
	{
		char pscL_ErrorStr[100];

		sprintf(pscL_ErrorStr, "%s missing in section %s in %s.xml", pscL_CfgParamKey,xMainNode.getText(),pscG_ConfigKey);
		cout << pscL_ErrorStr << endl;
		throw CException(1, pscL_ErrorStr);
	}
	if(siL_DataType == CFGLONG)
        {
                 long *pslL_Value = (long *)pL_Value;
                *pslL_Value = atol(xNode.getText());
        }
	if(siL_DataType == CFGINT)
	{
		 int *psiL_Value = (int *)pL_Value;
		*psiL_Value = atoi(xNode.getText());
	}
	if(siL_DataType == CFGFLOAT)
        {
                 float *psiL_Value = (float *)pL_Value;
                *psiL_Value = atof(xNode.getText());
        }
	if(siL_DataType == CFGSTRING)
	{
		((char*)pL_Value)[0] = 0x00;
		strcpy((char *)pL_Value, xNode.getText());
	}
	if(siL_DataType == CFGBOOL)
	{
		*((bool *)pL_Value) = (bool)atoi(xNode.getText());
	}
}

void CAppConfigParams::mefn_readCfgAttributeParam(XMLNode &xMainNode,const char *pscL_CfgParamKey,int siL_DataType,void *pL_Value)
{
	char pscL_Value[128];
	memset(pscL_Value,0x00, sizeof(pscL_Value));
	for(int i = 0; i < xMainNode.nAttribute(); i++)
	{
		if(strcasecmp(xMainNode.getAttributeName(i),pscL_CfgParamKey) == 0x00)
		{
			if (xMainNode.getAttributeValue(i) == nullptr)
			{
				char pscL_ErrorStr[100];

				sprintf(pscL_ErrorStr, "%s missing in section %s in Schedule.xml", pscL_CfgParamKey,xMainNode.getName());
				cout << pscL_ErrorStr << endl;
				throw CException(1, pscL_ErrorStr);
			}
			strcpy(pscL_Value, xMainNode.getAttributeValue(i));
		}
	}

	if(pscL_Value[0] == 0x00)
	{
		char pscL_ErrorStr[100];

		sprintf(pscL_ErrorStr, "%s missing in section %s in Schedule.xml", pscL_CfgParamKey,xMainNode.getName());
		cout << pscL_ErrorStr << endl;
		throw CException(1, pscL_ErrorStr);

	}
	cout << pscL_CfgParamKey << "		:	" << pscL_Value << endl;
	if(siL_DataType == CFGINT)
	{
		 int *psiL_Value = (int *)pL_Value;
		*psiL_Value = atoi(pscL_Value);
	}
	if(siL_DataType == CFGLONG)
        {
                 long *pslL_Value = (long *)pL_Value;
                *pslL_Value = atol(pscL_Value);
        }
	if(siL_DataType == CFGFLOAT)
        {
                 float *pslL_Value = (float *)pL_Value;
                *pslL_Value = atol(pscL_Value);
        }
	if(siL_DataType == CFGSTRING)
	{
		((char*)pL_Value)[0] = 0x00;
		strcpy((char *)pL_Value, pscL_Value);
	}
	if(siL_DataType == CFGCHAR)
	{
		((char*)pL_Value)[0] = 0x00;
		((char*)pL_Value)[0] = pscL_Value[0];
	}

	if(siL_DataType == CFGBOOL)
	{
		*((bool *)pL_Value) = (bool)atoi(pscL_Value);
	}
}
/************************************************************************
 * Class     : CAppConfigParams
 * Method    : mcfn_readConfigurations
 * Purpose   : Method to read initial configuration
 * Arguments : None
 * Returns   : void
 ************************************************************************/

void CAppConfigParams::mcfn_readConfigurations()
{
	cout << endl << "CAppConfigParams::mcfn_readConfigurations >>>" << endl;
	cout << "Section Name:" << pscG_ConfigKey << endl;

	char pscL_AppConfigFile[100];
	sprintf(pscL_AppConfigFile,"%s.xml",pscG_ConfigKey);

/**
 * Reading Main configurations
 */
	XMLResults CL_results;
	XMLNode xMainNode = XMLNode::parseFile(pscL_AppConfigFile,pscG_ConfigKey,&CL_results);
	XMLNode xNode;
	XMLNode xSubNode;

	if(xMainNode.isEmpty())
	{
		xMainNode = XMLNode::parseFile(pscL_AppConfigFile,"ServerManager",&CL_results);
		if(xMainNode.isEmpty())
		{
			char pscL_ErrorStr[100];
			sprintf(pscL_ErrorStr,"Error in ServerManager Default %s %d",pscG_ConfigKey,CL_results.error);
			throw CException(10,pscL_ErrorStr);
		}
	}


	mefn_readUDPIpAndPort(xMainNode);
	mefn_readCfgParam(xMainNode,"ConsumerHandlerCount",CFGINT,&mesi_ConsumerHandlerCount);
	mefn_readCfgParam(xMainNode,"ResourceExhaustLimit",CFGINT,&mesi_ResourceExhaustLimit);
	mefn_readCfgParam(xMainNode,"ForceEnquiryPercentageLimit",CFGINT,&mesi_ForceEnquiryPercentageLimit);

	xNode = xMainNode.getChildNode("PeriodicEnquiryDetails");
        if(xNode.isEmpty())
        {
                char pscL_ErrorStr[100];

                sprintf(pscL_ErrorStr, "%s missing in section %s in %s.xml", "DataBase",xNode.getName(),pscG_ConfigKey);
                cout << pscL_ErrorStr << endl;
                throw CException(1, pscL_ErrorStr);

        }	
	mefn_readPeriodicEnquiryDetails(xNode);

	xNode = xMainNode.getChildNode("DataBase");
        if(xNode.isEmpty())
        {
                char pscL_ErrorStr[100];

                sprintf(pscL_ErrorStr, "%s missing in section %s in %s.xml", "DataBase",xNode.getName(),pscG_ConfigKey);
                cout << pscL_ErrorStr << endl;
                throw CException(1, pscL_ErrorStr);

	}


	mefn_readCfgParam(xNode,"DBConnMonitorInterval",CFGINT,&mesi_DBConnMonitorInterval);
	mefn_readCfgParam(xNode,"DBConnectionpool",CFGINT,&mesi_DBConnectionpool);
	mefn_readCfgParam(xNode,"DBInstanceMasterTable",CFGSTRING,pmesc_InstanceMasterTable);
	mefn_readCfgParam(xNode,"DBServiceInstanceMappingTable",CFGSTRING,pmesc_ServiceInstanceMappingTable);
	mefn_readCfgParam(xNode,"DBServiceMasterTable",CFGSTRING,pmesc_ServiceMasterTable);

        xSubNode = xNode.getChildNode("DataBaseConnection");
        if(xSubNode.isEmpty())
        {
                char pscL_ErrorStr[100];

                sprintf(pscL_ErrorStr, "%s missing in section %s in %s.xml", "DataBaseConnection",xNode.getName(),pscG_ConfigKey);
                cout << pscL_ErrorStr << endl;
                throw CException(1, pscL_ErrorStr);

        }

        mefn_readCfgAttributeParam(xSubNode,"PrimaryDBServerIP",CFGSTRING,pmesc_PrimaryDBServerIP);
        mefn_readCfgAttributeParam(xSubNode,"SecondaryDBServerIP",CFGSTRING,pmesc_SecondaryDBServerIP);
        mefn_readCfgAttributeParam(xSubNode,"DBServerPort",CFGINT,&mesi_DBServerPort);
        mefn_readCfgAttributeParam(xSubNode,"DBSID",CFGSTRING,pmesc_DBSID);
        mefn_readCfgAttributeParam(xSubNode,"DBUserName",CFGSTRING,pmesc_DBUserName);
        mefn_readCfgAttributeParam(xSubNode,"DBPassword",CFGSTRING,pmesc_DBPassword);

	
	xNode = xMainNode.getChildNode("RedisCacheDetails");
        if(xNode.isEmpty())
        {
                char pscL_ErrorStr[100];
		sprintf(pscL_ErrorStr, "%s missing in section %s in %s.xml", "DataBase",xNode.getName(),pscG_ConfigKey);
                throw CException(1,pscL_ErrorStr);
        }

        mefn_readCfgParam(xNode,"PrimaryHost",CFGSTRING,pmesc_PrimaryHost);
        mefn_readCfgParam(xNode,"PrimaryPoolSize",CFGINT,&mesi_PrimaryPoolSize);
        mefn_readCfgParam(xNode,"PrimaryPort",CFGINT,&mesi_PrimaryPort);
        mefn_readCfgParam(xNode,"IsAuthenticationEnabled",CFGBOOL,&meb_IsAuthenticationEnabled);
        if(meb_IsAuthenticationEnabled == true)
        {
		xSubNode = xNode.getChildNode("PrimaryUserName");
                if(!xSubNode.isEmpty() && xSubNode.getText()!=nullptr)
                {
                        strcpy(pmesc_PrimaryUserName, xSubNode.getText());
                }
                mefn_readCfgParam(xNode,"PrimaryPassword",CFGSTRING,pmesc_PrimaryPassword);
        }
        mefn_readCfgParam(xNode,"ClusterMode",CFGBOOL,&meb_ClusterMode);
        mefn_readCfgParam(xNode,"SessionExpiryTime",CFGINT,&mesi_SessionExpiryTime);
        mefn_readCfgParam(xNode,"SocketTimeOutMS",CFGINT,&mesi_SocketTimeOutMS);


	cout << "CAppConfigParams::mcfn_readConfigurations <<<" << endl;
}
void CAppConfigParams::mefn_readPeriodicEnquiryDetails(XMLNode & xMainNode)
{

	mefn_readCfgParam(xMainNode,"MaxEnqiuryRequestTimeMs",CFGINT,&mesi_MaxEnqiuryRequestTimeMs);
	mefn_readCfgParam(xMainNode,"PrimaryEquiryTimerMs",CFGINT,&mesi_PrimaryEquiryTimerMs);
	mefn_readCfgParam(xMainNode,"MaxEnquiryMissCount",CFGINT,&mesi_MaxEnquiryMissCount);

	XMLNode xNode = xMainNode.getChildNode("EnquiryRetry");
	if(xNode.isEmpty())
	{
		cout <<"\nPeriodicEnquiry Type is set to Default"<< endl;
		mesi_PeriodicEnquiryType=PERIODIC_RETRY_TYPE_DEFAULT;
		return;
	}
	int siL_Itr=-1;
	int siL_TimerMs;
	int siL_Retry;
	XMLNode xRetry;
	while(1)
	{
		siL_Itr++;
		xRetry = xNode.getChildNode(siL_Itr);
		if(xRetry.isEmpty())
			break;
		mefn_readCfgAttributeParam(xRetry,"Retry",CFGINT,&siL_Retry);
		mefn_readCfgAttributeParam(xRetry,"TimerMs",CFGINT,&siL_TimerMs);
		mcC_PeriodicEnquiryDetailsMap.insert({siL_Retry,siL_TimerMs});
	}
	
	if(mcC_PeriodicEnquiryDetailsMap.size()==mesi_MaxEnquiryMissCount)
	{
		mesi_PeriodicEnquiryType=PERIODIC_RETRY_TYPE_INTERVEL;
		return;
	}
	mesi_PeriodicEnquiryType=PERIODIC_RETRY_TYPE_IMMEDIATE;

}
void CAppConfigParams::mefn_readUDPIpAndPort(XMLNode & xMainNode)
{

	char pscL_ErrorStr[100];
	pscL_ErrorStr[0]=0x00;
	XMLNode xNode;
	xNode = xMainNode.getChildNode("UDPServer");
	if(xNode.isEmpty())
	{
		sprintf(pscL_ErrorStr, "%s missing in section %s in %s.xml","UDPServer",xMainNode.getText(),pscG_ConfigKey);
		cout << pscL_ErrorStr << endl;
		throw CException(10, pscL_ErrorStr);
	}

	if(strcasecmp(xNode.getAttributeName(0),"IP")==0 && strcasecmp(xNode.getAttributeName(1),"PORT")==0)
	{
		strcpy(pmesc_UDPServerIP,xNode.getAttributeValue(0));
		mesi_UDPServerPort= atoi(xNode.getAttributeValue(1));
	}
	else if(strcasecmp(xNode.getAttributeName(1),"IP")==0 && strcasecmp(xNode.getAttributeName(0),"PORT")==0)
	{
		strcpy(pmesc_UDPServerIP,xNode.getAttributeValue(1));
                mesi_UDPServerPort= atoi(xNode.getAttributeValue(0));
	}
	else
	{
		sprintf(pscL_ErrorStr, "%s missing in section %s in %s.xml","UDPServer",xMainNode.getText(),pscG_ConfigKey);
                cout << pscL_ErrorStr << endl;
                throw CException(10, pscL_ErrorStr);		
	}	

	

}
/************************************************************************
 * Class     : CAppConfigParams
 * Method    : mcfn_invokeConfigChange
 * Purpose   : Method to handle Dynamic Configuration changes
 * Arguments : CGenericEvent Obj containing telnet connection
 * Returns   : void
 ************************************************************************/

void CAppConfigParams::mcfn_invokeConfigChange(CGenericEvent &CL_GenericEvent)
{
	__entryFunction__;


        char pscL_UUCID[100];
        sprintf(pscL_UUCID,"%ld",mcfn_getConfigCLSID());

        long slL_ParamReference;
        memcpy(&slL_ParamReference,CL_GenericEvent.mcfn_getEventObject(),sizeof(long));

        long slL_Root = 0, slL_ReqData = 0, slL_ReqHdrData = 0;

        unsigned char *pu8L_Pptr = (unsigned char *)slL_ParamReference;

        memcpy(&slL_Root, pu8L_Pptr,sizeof(long));
        pu8L_Pptr += sizeof(long);

        memcpy(&slL_ReqHdrData,pu8L_Pptr,sizeof(long));
        pu8L_Pptr += sizeof(long);

        memcpy(&slL_ReqData,pu8L_Pptr,sizeof(long));
        std::map<string,string> *pCL_ReqHeadersMap =  (std::map<string,string> *)(slL_ReqHdrData);
        string *pCL_ReqData = (string *)slL_ReqData;

        json_t *pSL_ApplicationJson = nullptr;
        if((*pCL_ReqHeadersMap)[":method"] == "GET")
        {
                json_t *root = (json_t *)slL_Root;

                pSL_ApplicationJson = json_object();

                json_object_set_new(root,pscL_UUCID,pSL_ApplicationJson);

                json_object_set_new(pSL_ApplicationJson,"Type",json_string(pscG_ConfigKey));
                json_object_set_new(pSL_ApplicationJson,"Parameters",json_string("None"));
        }
        if((*pCL_ReqHeadersMap)[":method"] == "PUT")
        {
                json_t *root = nullptr;
                json_error_t error;

                EVT_LOG(_6d::_Framework::CG_EventLog,LOG_INFO,siG_InstanceID,"invokeConfigChange","JSONRequest",this,"","%s", pCL_ReqData->c_str());

                root = json_loads(pCL_ReqData->c_str(),0,&error);
                if(root == nullptr)
                {
                        EVT_LOG(_6d::_Framework::CG_EventLog,LOG_ERROR,siG_InstanceID,"invokeConfigChange","Failure",this,"","Could not create JSON from buffer");
                        EVT_LOG(_6d::_Framework::CG_EventLog,LOG_VERBOSE,siG_InstanceID,"invokeConfigChange","Failure",this,"","mcfn_invokeConfigChange <<<");
                        return;
                }
                const char *str = nullptr;
                json_t *pSL_ApplicationJSON;
                if((*pCL_ReqHeadersMap)[":path"] == "/")
                {
                        pSL_ApplicationJSON = json_object_get(root,pscL_UUCID);
                        if(pSL_ApplicationJSON == nullptr)
                        {
                                EVT_LOG(_6d::_Framework::CG_EventLog,LOG_ERROR,siG_InstanceID,"invokeConfigChange","Failure",this,0,"Could not get  Application JSON Object");

                                EVT_LOG(_6d::_Framework::CG_EventLog,LOG_VERBOSE,siG_InstanceID,"invokeConfigChange","Failure",this,"","mcfn_invokeConfigChange <<<");
                                return;

                        }
                }
                else
                {
                        pSL_ApplicationJSON = root;
                }
                const char *key;
                json_t *value;
                json_t *RespRoot = (json_t *)slL_Root;

                pSL_ApplicationJson = json_object();
                json_object_set_new(RespRoot,pscL_UUCID,pSL_ApplicationJson);


                json_object_foreach(pSL_ApplicationJSON, key, value)
                {
                        EVT_LOG(_6d::_Framework::CG_EventLog,LOG_INFO|LOG_OPINFO,siG_InstanceID,"invokeConfigChange","Config",this,0,"Key:%s", key);
                }
                json_object_set_new(pSL_ApplicationJson,"Response",json_string("Failure"));
        }


	__return__();
}



