/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* LoadServiceMasterSet.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
* 
*/


#if !(defined __6dLoadServiceMasterSet_H)
#define __6dLoadServiceMasterSet_H

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
 * Class : LoadServiceMasterSet
 * Purpose : to load ServiceMaster
 ************************************************************************/

class CLoadServiceMasterSet: public CMySQLRecordSet
{
	private:
		/**
		 * Private Variables
		 */	

		//Column values

		char 	pmesc_ServiceName[30];
                char 	pmesc_ServiceType[5];
                int 	mesi_ServiceId;
                int 	mesi_LicenseCap;
                int 	mesi_AllotedCap;
		
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
			mesi_ServiceId 		= 0x00;
			mesi_LicenseCap	 	= 0x00;
			mesi_AllotedCap 	= 0x00;
			pmesc_ServiceName[0] 	= 0x00;
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
                CLoadServiceMasterSet();

		//Destructor
		~CLoadServiceMasterSet();
		
		//pure virtual functions of MySQL RecordSet
		void		mdfn_getINParamCount(int &siL_Count)	{ siL_Count = 0x00;						}
		void  		mdfn_getOUTParamCount(int &siL_Count)	{ siL_Count = 0x05;						}
                const char*	mdfn_getUserName() const 		{ return CG_AppConfigParams.mcfn_getDBUserName(); 		}
                const char*	mdfn_getPassword() const 		{ return CG_AppConfigParams.mcfn_getDBPassword(); 		}
                const char*	mdfn_getServerName() const 		{ return CG_AppConfigParams.mcfn_getPrimaryDBServerIP();	}
                const char*	mdfn_getSID() const 			{ return CG_AppConfigParams.mcfn_getDBSID(); 			}
		unsigned int 	mdfn_getPortNumber() const 		{ return CG_AppConfigParams.mcfn_getDBServerPort(); 		}
		void 		mdfn_bindColumns();

		void 	mdfn_getSQLString(char *pscL_Str)
		{

			sprintf(pscL_Str, "SELECT SERVICE_ID,SERVICE_NAME,SERVICE_TYPE,LICENSE_COUNT,ALLOCATED_RESOURCE_COUNT FROM %s",CG_AppConfigParams.mcfn_getServiceMasterTable());
			EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "getSQLString","MySqlQuery", this,"", "%s",pscL_Str);

		}

		bool 	mcfn_loadServiceMaster();
			
};

#endif
