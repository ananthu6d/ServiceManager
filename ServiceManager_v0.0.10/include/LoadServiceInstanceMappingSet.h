/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* LoadServiceInstanceMappingSet.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
* 
*/


#if !(defined __6dLoadServiceInstanceMappingSet_H)
#define __6dLoadServiceInstanceMappingSet_H

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
 * Class : LoadServiceInstanceMappingSet
 * Purpose : to load ServiceInstanceMapping table
 ************************************************************************/

class CLoadServiceInstanceMappingSet: public CMySQLRecordSet
{
	private:
		/**
		 * Private Variables
		 */	

		//Column values

		char 	pmesc_ServiceType[5];
		char 	pmesc_SignalingIpPort[30];
                int 	mesi_ServiceId;
                int 	mesi_InstanceId;
                int 	mesi_TotalCap;
                int 	mesi_AllotedCap;

		char	mesc_Status;
		
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
			mesc_Status		= 0x00;
			mesi_ServiceId		= 0x00;
			mesi_InstanceId		= 0x00;
			mesi_TotalCap		= 0x00;
			mesi_AllotedCap		= 0x00;
			pmesc_SignalingIpPort[0]= 0x00;
			pmesc_ServiceType[0]	= 0x00;
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
                CLoadServiceInstanceMappingSet();

		//Destructor
		~CLoadServiceInstanceMappingSet();
		
		//pure virtual functions of MySQL RecordSet
		void		mdfn_getINParamCount(int &siL_Count)	{ siL_Count = 0x00;						}
		void  		mdfn_getOUTParamCount(int &siL_Count)	{ siL_Count = 0x07;						}
                const char*	mdfn_getUserName() const 		{ return CG_AppConfigParams.mcfn_getDBUserName(); 		}
                const char*	mdfn_getPassword() const 		{ return CG_AppConfigParams.mcfn_getDBPassword(); 		}
                const char*	mdfn_getServerName() const 		{ return CG_AppConfigParams.mcfn_getPrimaryDBServerIP();	}
                const char*	mdfn_getSID() const 			{ return CG_AppConfigParams.mcfn_getDBSID(); 			}
		unsigned int 	mdfn_getPortNumber() const 		{ return CG_AppConfigParams.mcfn_getDBServerPort(); 		}
		void 		mdfn_bindColumns();

		void 	mdfn_getSQLString(char *pscL_Str)
		{

			sprintf(pscL_Str, "SELECT SERVICE_ID, INSTANCE_ID, STATUS, ALLOCATED_RESOURCE, RESOURCE_LIMIT,RESOURCE_ADDRESS,SERVICE_TYPE FROM  %s",CG_AppConfigParams.mcfn_getServiceInstanceMappingTable());
			EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "getSQLString","MySqlQuery", this,"", "%s",pscL_Str);

		}

		bool 	mcfn_loadServiceInstanceMapping();
			
};

#endif
