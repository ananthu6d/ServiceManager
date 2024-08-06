/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* UpdateInstanceSet.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dUpdateInstanceSet_H)
#define __6dUpdateInstanceSet_H

/**
 * Header Includes
 */

#include "AppConfigParams.h"
#include "MySQLRecordSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace std;

/************************************************************************
 * Class : UpdateInstanceSet
 * Purpose : to update IntanceMaster set
 ************************************************************************/

class CUpdateInstanceSet: public CMySQLRecordSet
{
	private:
		/**
		 * Private Variables
		 */	

		//Column values

		char 	pmesc_SignalingIp[64];
		long	mesl_SignalingPort;
		int 	siL_QueryType;
		
		char 	pmesc_RegistrationTime[20];
		char 	pmesc_Query[256];
		
		CMySQLConnection* pmeC_Connection;

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
			mesl_SignalingPort = 0x00;
			memset(pmesc_SignalingIp,0x00,sizeof(pmesc_SignalingIp));
			memset(pmesc_RegistrationTime,0x00,sizeof(pmesc_RegistrationTime));
			memset(pmesc_Query,0x00,sizeof(pmesc_Query));
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
                CUpdateInstanceSet();

		//Destructor
		~CUpdateInstanceSet();

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

			switch(siL_QueryType)
			{
				case QUERYTYPE_GENERIC_RESOURCE_UPDATE:
					{	
						sprintf(pscL_Str, "UPDATE %s SET %s,STATUS = 'A',HEARTBEAT_MISS_COUNT=0,LAST_HEARTBEAT_TIME=NOW(),REGISTRATION_TIME='%s' WHERE SIGNALLING_IP='%s' AND SIGNALLING_PORT=%ld",CG_AppConfigParams.mcfn_getInstanceMasterTable(),pmesc_Query,pmesc_RegistrationTime,pmesc_SignalingIp,mesl_SignalingPort);
					}
					break;
				case QUERYTYPE_ACTIVATE_INSTANCE:
					{

						sprintf(pscL_Str, "UPDATE %s SET STATUS = 'A',HEARTBEAT_MISS_COUNT=0,LAST_HEARTBEAT_TIME=NOW(),REGISTRATION_TIME='%s' WHERE SIGNALLING_IP='%s' AND SIGNALLING_PORT=%ld",CG_AppConfigParams.mcfn_getInstanceMasterTable(),pmesc_RegistrationTime,pmesc_SignalingIp,mesl_SignalingPort);
					}

			}

			EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "getSQLString","MySqlQuery", this,"", "%s",pscL_Str);

		}

		bool 	mcfn_resourceGenericUpdate(const char*,const long&,char*,char*);
		bool 	mcfn_activateInstance(const char*,const long&,char*);
		

			
};

#endif
