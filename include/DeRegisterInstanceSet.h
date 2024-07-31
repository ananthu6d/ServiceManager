/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* DeRegisterInstanceSet.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dDeRegisterInstanceSet_H)
#define __6dDeRegisterInstanceSet_H

/**
 * Header Includes
 */

#include "AppConfigParams.h"
#include "MySQLRecordSet.h"
#include "AppExterns.h"

using namespace _6d;
using namespace std;

/************************************************************************
 * Class : DeRegisterInstanceSet
 * Purpose : to perform deregister in InstanceMaster
 ************************************************************************/

class CDeRegisterInstanceSet: public CMySQLRecordSet
{
	private:
		/**
		 * Private Variables
		 */	

		//Column values

		long 	mesl_SignalingPort;
		char	pmesc_SignalingIp[64];	
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
			mesl_SignalingPort	= 0x00;
			pmesc_SignalingIp[0]	= 0x00;
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
                CDeRegisterInstanceSet();

		//Destructor
		~CDeRegisterInstanceSet();
		
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

			sprintf(pscL_Str, "UPDATE %s SET STATUS = 'D' WHERE SIGNALLING_PORT=%ld AND SIGNALLING_IP='%s'",CG_AppConfigParams.mcfn_getInstanceMasterTable(),mesl_SignalingPort,pmesc_SignalingIp);
			EVT_LOG(CG_EventLog, LOG_INFO | LOG_OPINFO,siG_InstanceID, "getSQLString","MySqlQuery", this,"", "%s",pscL_Str);

		}

		bool 	mcfn_deRegisterInstance(const char*,const long&);
		

			
};

#endif
