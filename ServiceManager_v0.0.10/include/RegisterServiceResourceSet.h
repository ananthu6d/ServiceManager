/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* RegisterServiceResourceSet.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dRegisterServiceResourceSet_H)
#define __6dRegisterServiceResourceSet_H

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
 * Class : RegisterServiceResourceSet
 * Purpose : to register ServiceResoures
 ************************************************************************/

class CRegisterServiceResourceSet: public CMySQLRecordSet
{
	private:
		/**
		 * Private Variables
		 */	

		//Column values

		int 	mesi_InstanceId;
		int 	mesi_ServiceId;
		int 	mesi_MaxLimit;
		char	pmesc_ServiceType[5];
		string 	meC_SignalingIpPort;
	
		
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
			pmesc_ServiceType[0]= 0x00;
			meC_SignalingIpPort = "";
			mesi_InstanceId	= 0x00;
			mesi_ServiceId 	= 0x00;
			mesi_MaxLimit 	= 0x00;
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
                CRegisterServiceResourceSet();

		//Destructor
		~CRegisterServiceResourceSet();
		
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

			sprintf(pscL_Str, "INSERT INTO %s (SERVICE_ID, INSTANCE_ID,RESOURCE_LIMIT,RESOURCE_ADDRESS,SERVICE_TYPE) VALUES(%d,%d,%d,'%s','%s') ON DUPLICATE KEY UPDATE STATUS='A',ALLOCATED_RESOURCE=0,RESOURCE_LIMIT=%d,RESOURCE_ADDRESS='%s',SERVICE_TYPE='%s'",CG_AppConfigParams.mcfn_getServiceInstanceMappingTable(),mesi_ServiceId,mesi_InstanceId,mesi_MaxLimit,meC_SignalingIpPort.c_str(),pmesc_ServiceType,mesi_MaxLimit,meC_SignalingIpPort.c_str(),pmesc_ServiceType);
			EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "getSQLString","MySqlQuery", this,"", "%s",pscL_Str);

		}

		bool 	mcfn_insertIntoServiceInstanceMapping(int,int,string,int,char*);
		void 	mcfn_reInitailizeDB();
		

			
};

#endif
