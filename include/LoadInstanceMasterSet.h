/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* LoadInstanceMasterSet.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
* 
*/


#if !(defined __6dLoadInstanceMasterSet_H)
#define __6dLoadInstanceMasterSet_H

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
 * Class : LoadInstanceMasterSet
 * Purpose : to load instance master into cache
 ************************************************************************/

class CLoadInstanceMasterSet: public CMySQLRecordSet
{
	private:
		/**
		 * Private Variables
		 */	

		//Column values

		char    pmesc_SignalingIP[64];
                char    pmesc_InstanceName[30];
                char    pmesc_RegistrationTime[20];
                char    pmesc_LastHeartBeatTime[20];

                char    mesc_Status;

                int     mesi_TotalResourceCount;
                int     mesi_InboundResourceCount;
                int     mesi_OutboundResourceCount;
                int     mesi_SignalingPort;
                int     mesi_InstanceId;
                int     mesi_HeartBeatMissCount;

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
			mesc_Status			= 0x00;
			mesi_TotalResourceCount         = 0x00;
			mesi_InboundResourceCount       = 0x00;
			mesi_OutboundResourceCount      = 0x00;
			mesi_InstanceId                 = 0x00;
			mesi_HeartBeatMissCount         = 0x00;
			mesi_SignalingPort              = 0x00;
			memset(pmesc_SignalingIP,       0x00, sizeof(pmesc_SignalingIP));
			memset(pmesc_LastHeartBeatTime, 0x00, sizeof(pmesc_LastHeartBeatTime));
			memset(pmesc_RegistrationTime,  0x00, sizeof(pmesc_RegistrationTime));
			memset(pmesc_InstanceName,      0x00, sizeof(pmesc_InstanceName));

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
                CLoadInstanceMasterSet();

		//Destructor
		~CLoadInstanceMasterSet();
		
		//pure virtual functions of MySQL RecordSet
		void		mdfn_getINParamCount(int &siL_Count)	{ siL_Count = 0x00;						}
		void  		mdfn_getOUTParamCount(int &siL_Count)	{ siL_Count = 0xB;						}
                const char*	mdfn_getUserName() const 		{ return CG_AppConfigParams.mcfn_getDBUserName(); 		}
                const char*	mdfn_getPassword() const 		{ return CG_AppConfigParams.mcfn_getDBPassword(); 		}
                const char*	mdfn_getServerName() const 		{ return CG_AppConfigParams.mcfn_getPrimaryDBServerIP();	}
                const char*	mdfn_getSID() const 			{ return CG_AppConfigParams.mcfn_getDBSID(); 			}
		unsigned int 	mdfn_getPortNumber() const 		{ return CG_AppConfigParams.mcfn_getDBServerPort(); 		}
		void 		mdfn_bindColumns();

		void 	mdfn_getSQLString(char *pscL_Str)
		{

			sprintf(pscL_Str, "SELECT INSTANCE_ID, INSTANCE_NAME, SIGNALLING_IP, SIGNALLING_PORT, TOTAL_RESOURCE, TOTAL_INBOUND_RESOURCE, TOTAL_OUTBOUND_RESOURCE, REGISTRATION_TIME, STATUS, HEARTBEAT_MISS_COUNT, LAST_HEARTBEAT_TIME FROM %s",CG_AppConfigParams.mcfn_getInstanceMasterTable());
			EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "getSQLString","MySqlQuery", this,"", "%s",pscL_Str);

		}

		bool 	mcfn_loadInstanceMaster();
			
};

#endif
