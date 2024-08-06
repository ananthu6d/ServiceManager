/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* RegisterInstanceSet.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dRegisterInstanceSet_H)
#define __6dRegisterInstanceSet_H

/**
 * Header Includes
 */

#include "AppConfigParams.h"
#include "MySQLRecordSet.h"
#include "AppExterns.h"
#include "InstanceInfo.h"

using namespace _6d;
using namespace std;

/************************************************************************
 * Class : RegisterInstanceSet
 * Purpose : Register InstanceMaster 
 ************************************************************************/

class CRegisterInstanceSet: public CMySQLRecordSet
{
	private:
		/**
		 * Private Variables
		 */	

		//Column values

		int 	mesi_InstanceId;
		char 	pmesc_InstanceName[30];
		char	pmesc_SignalingIp[20];
		int 	mesi_SignalingPort;
		int 	mesi_TotalResource;
		int 	mesi_TotalInBoundResource;
		int 	mesi_TotalOutBoundResource;
		char	pmesc_RegistrationTime[20];
		char 	mesc_Status;

		CMySQLConnection* pmeC_Connection;

		//int mesi_ErrorCode;

	protected:
		/**
		 * Protected Variables
		 */

	public:
		/**
		 * Public Variables
		 */

	private:
		/**
		 * Private Member Functions
		 */
		void mefn_initDataSet()	
		{
			mesi_InstanceId 		= 0x00;
			mesi_TotalResource 		= 0x00;
			mesi_TotalInBoundResource 	= 0x00;
			mesi_TotalOutBoundResource 	= 0x00;
			mesi_SignalingPort		= 0x00;

			memset(pmesc_InstanceName, 	0x00, sizeof(pmesc_InstanceName));
			memset(pmesc_SignalingIp, 	0x00, sizeof(pmesc_SignalingIp));
			memset(pmesc_RegistrationTime, 	0x00, sizeof(pmesc_RegistrationTime));
		}

		void mefn_setData(char* pscL_InstanceName,char* pscL_RegistrationTime,char scL_Status,char* pscL_SignalingIp,int siL_TotalResourceCount,int siL_InboundResourceCount,int siL_OutboundResourceCount,int siL_SignalingPort,int siL_InstanceId)
		{

			strcpy(pmesc_InstanceName,pscL_InstanceName);
			strcpy(pmesc_RegistrationTime,pscL_RegistrationTime);
			strcpy(pmesc_SignalingIp,pscL_SignalingIp);
			mesi_SignalingPort		= siL_SignalingPort;
			mesi_TotalResource 		= siL_TotalResourceCount;
			mesi_TotalInBoundResource 	= siL_InboundResourceCount;
			mesi_TotalOutBoundResource 	= siL_OutboundResourceCount;
			mesc_Status			= scL_Status;
			mesi_InstanceId 		= siL_InstanceId;

		}


	protected:
		/**
		 * Protected Member Functions
		 */

	public:
		/**
		 * Public Member Functions
		 */
		
		//Constructor
                CRegisterInstanceSet();

		//Destructor
		~CRegisterInstanceSet();
		
		//pure virtual functions of MySQL RecordSet
		void		mdfn_getINParamCount(int &siL_Count)	{ siL_Count = 0x00;						}
		void  		mdfn_getOUTParamCount(int &siL_Count)	{ siL_Count = 0x00;						}
                const char*	mdfn_getUserName() const 		{ return CG_AppConfigParams.mcfn_getDBUserName(); 		}
                const char*	mdfn_getPassword() const 		{ return CG_AppConfigParams.mcfn_getDBPassword(); 		}
                const char*	mdfn_getServerName() const 		{ return CG_AppConfigParams.mcfn_getPrimaryDBServerIP();	}
                const char*	mdfn_getSID() const 			{ return CG_AppConfigParams.mcfn_getDBSID(); 			}
		unsigned int 	mdfn_getPortNumber() const 		{ return CG_AppConfigParams.mcfn_getDBServerPort(); 		}
		void 		mdfn_bindColumns();
		void mdfn_getSQLString(char *pscL_Str)
		{

			sprintf(pscL_Str, "INSERT INTO %s (INSTANCE_ID, INSTANCE_NAME, SIGNALLING_IP, SIGNALLING_PORT,TOTAL_RESOURCE, TOTAL_INBOUND_RESOURCE, TOTAL_OUTBOUND_RESOURCE,REGISTRATION_TIME, STATUS,LAST_HEARTBEAT_TIME) VALUES(%d, '%s','%s',%d,%d,%d, %d,'%s','%c',NOW()) ON DUPLICATE KEY UPDATE TOTAL_RESOURCE=%d, TOTAL_INBOUND_RESOURCE=%d, TOTAL_OUTBOUND_RESOURCE=%d,REGISTRATION_TIME='%s', STATUS='%c',LAST_HEARTBEAT_TIME=NOW()",CG_AppConfigParams.mcfn_getInstanceMasterTable(),mesi_InstanceId,pmesc_InstanceName,pmesc_SignalingIp,mesi_SignalingPort,mesi_TotalResource,mesi_TotalInBoundResource,mesi_TotalOutBoundResource,pmesc_RegistrationTime,mesc_Status,mesi_TotalResource,mesi_TotalInBoundResource,mesi_TotalOutBoundResource,pmesc_RegistrationTime,mesc_Status);
			
			EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "getSQLString","MySqlQuery", this,"", "%s",pscL_Str);

		}

		bool 	mcfn_insertIntoInstanceMaster(char*,char*,char,char*,int,int,int,int,int);

		//getErrorCode
		//int mcfn_getErrorCode(){return mesi_ErrorCode;}

			
};

#endif
