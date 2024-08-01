/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* DeRegisterServiceResourceSet.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dDeRegisterServiceResourceSet_H)
#define __6dDeRegisterServiceResourceSet_H

/**
 * Header Includes
 */

#include "AppConfigParams.h"
#include "MySQLRecordSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace std;

/************************************************************************
 * Class : DeRegisterServiceResourceSet
 * Purpose : to perform dereg for services
 ************************************************************************/

class CDeRegisterServiceResourceSet: public CMySQLRecordSet
{
	private:
		/**
		 * Private Variables
		 */	

		//Column values

		int 	mesi_QueryType;
		string 	meC_SignalingIpPort;
		int 	mesi_ServiceId;	
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
			mesi_ServiceId	= 0x00;
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
                CDeRegisterServiceResourceSet();

		//Destructor
		~CDeRegisterServiceResourceSet();

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
				case QUERYTUPE_DEREGISTER_SINGLE_SERVICE:
					{
						sprintf(pscL_Str, "UPDATE %s SET STATUS = 'D',ALLOCATED_RESOURCE=0 WHERE RESOURCE_ADDRESS='%s' AND SERVICE_ID=%d",CG_AppConfigParams.mcfn_getServiceInstanceMappingTable(),meC_SignalingIpPort.c_str(),mesi_ServiceId);
					}
					break;
				case QUERYTYPE_DEREGISTER_ALL_SERVICE:
					{

						sprintf(pscL_Str, "UPDATE %s SET STATUS = 'D',ALLOCATED_RESOURCE=0 WHERE RESOURCE_ADDRESS='%s'",CG_AppConfigParams.mcfn_getServiceInstanceMappingTable(),meC_SignalingIpPort.c_str());
					}
					break;
			}
			EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "getSQLString","MySqlQuery", this,"", "%s",pscL_Str);

		}

		bool 	mcfn_deRegisterInstance(string);
		bool 	mcfn_deRegisterServiceResource(string,const int&);

		void 	mcfn_reInitailizeDB();

			
};

#endif
