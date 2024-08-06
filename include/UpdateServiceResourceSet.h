/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* UpdateServiceResourceSet.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dUpdateServiceResourceSet_H)
#define __6dUpdateServiceResourceSet_H

/**
 * Header Includes
 */

#include "AppConfigParams.h"
#include "MySQLRecordSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace std;

/************************************************************************
 * Class : UpdateServiceResourceSet
 * Purpose : to perform operation on subscriber MSISDN table
 ************************************************************************/

class CUpdateServiceResourceSet: public CMySQLRecordSet
{
	private:
		/**
		 * Private Variables
		 */	

		//Column values

		int 	mesi_QueryType;
		string 	meC_SignalingIpPort;
		int 	mesi_ServiceId;
		int 	mesi_MaxLimit;
		int 	mesi_AllocatedResource;

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
			meC_SignalingIpPort	= "";
			mesi_ServiceId		= 0x00;
			mesi_MaxLimit		= 0x00;
			mesi_AllocatedResource	= 0x00;
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
                CUpdateServiceResourceSet();

		//Destructor
		~CUpdateServiceResourceSet();

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

			switch(mesi_QueryType)
			{
				case QUERYTYPE_ACTIVATE_RESOURCE:
					{
						sprintf(pscL_Str, "UPDATE %s SET STATUS = 'A',ALLOCATED_RESOURCE=0 WHERE RESOURCE_ADDRESS='%s' AND SERVICE_ID=%d",CG_AppConfigParams.mcfn_getServiceInstanceMappingTable(),meC_SignalingIpPort.c_str(),mesi_ServiceId);
					}
					break;
				case QUERYTYPE_UPDATE_MAXLIMIT:
					{

						sprintf(pscL_Str, "UPDATE %s SET RESOURCE_LIMIT=%d,STATUS = 'A',ALLOCATED_RESOURCE=0 WHERE RESOURCE_ADDRESS='%s' AND SERVICE_ID=%d",CG_AppConfigParams.mcfn_getServiceInstanceMappingTable(),mesi_MaxLimit,meC_SignalingIpPort.c_str(),mesi_ServiceId);
					}
					break;
				case QUERYTYPE_UPDATE_ALLOCATED_RESOURCE:
                                        {

                                                sprintf(pscL_Str, "UPDATE %s SET ALLOCATED_RESOURCE=%d WHERE RESOURCE_ADDRESS='%s' AND SERVICE_ID=%d",CG_AppConfigParams.mcfn_getServiceInstanceMappingTable(),mesi_AllocatedResource,meC_SignalingIpPort.c_str(),mesi_ServiceId);
                                        }
                                        break;
			}
			EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "getSQLString","MySqlQuery", this,"", "%s",pscL_Str);

		}


		bool 	mcfn_activateResource(string,const int&);

		bool 	mcfn_updateResource(string,const int&,const int&,const int&);
		
		void 	mcfn_reInitailizeDB();

			
};

#endif
