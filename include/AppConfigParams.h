/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* AppConfigParams.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*  1  18June24	Ananthu    Created
*/


#if !(defined __6dAppConfigParams_H)
#define __6dAppConfigParams_H

/**
 * Header Includes
 */

#include "AppHeaders.h"
#include "AppDefines.h"
#include "AppUserTypes.h"

using namespace _6d;
using namespace _Framework;
using namespace _EventLog;


/**
 * Configuration parameter with data value as number
 */
#define CFGINT 0
/**
 * Configuration parameter with data value as String
 */
#define CFGSTRING 1
/**
 * Configuration parameter with data value as boolean
 */
#define CFGBOOL 2
/**
 * Configuration parameter with data value as char
 */
#define CFGCHAR 3

/**
 * Configuration parameter with data value as char
 */
#define CFGLONG 4



/************************************************************************
 * Class : AppConfigParams
 * Purpose : Configuration Class for App specific detail
 ************************************************************************/

class CAppConfigParams : public IConfigParams
{
	private:
		/**
                 * Protected Variables
                 */
		int 	mesi_UDPServerPort;
		char 	pmesc_UDPServerIP[256];



		//DATABASE Part
		
		//Variable to store DBServerIP
                char    pmesc_PrimaryDBServerIP[50];
                //Variable to store DBSID
                char    pmesc_DBSID[50];
                //Variable to store DBUserName
                char    pmesc_DBUserName[30];
                //Variable to store DBPassword
                char    pmesc_DBPassword[30];
                //Variable to store SecondaryDBServerIP
                char    pmesc_SecondaryDBServerIP[50];

                //Variable to store DBServerPort
                int     mesi_DBServerPort;
                //Variable to store DBConnMonitorInterval
                int     mesi_DBConnMonitorInterval;
                //Variable to store DBConnectionpool size
                int     mesi_DBConnectionpool;


		//Tables
		char 	pmesc_InstanceMasterTable[50];
		char 	pmesc_ServiceInstanceMappingTable[50];
		char    pmesc_ServiceMasterTable[50];


		long 	mesi_MaxEnqiuryRequestTimeMs;
		long	mesi_PrimaryEquiryTimerMs;
		int	mesi_MaxEnquiryMissCount;

		int 	mesi_PeriodicEnquiryType;

		int 	mesi_ConsumerHandlerCount;

		int 	mesi_ResourceExhaustLimit;

		//Variable to store cluster mode
                bool meb_ClusterMode;

                //Variable to store authentication status
                bool meb_IsAuthenticationEnabled;

                //Variables to store redis cache details

                //Variable to store Primary port
                int mesi_PrimaryPort;

                //Variable to store primary conn pool size
                int mesi_PrimaryPoolSize;

                //Variable to store SocketTimeOut in milliseconds
                int mesi_SocketTimeOutMS;

                //Variable to store session expirytime
                int mesi_SessionExpiryTime;

		//Variable to store Primary Host address
                char pmesc_PrimaryHost[50];

                //Variable to store Primary UserName
                char pmesc_PrimaryUserName[50];

                //Varible to store primary password
                char pmesc_PrimaryPassword[50];



	protected:
		/**
		 * Protected Variables
		 */

	public:
		/**
		 * Public Variables
		 */

		map<int,int> mcC_PeriodicEnquiryDetailsMap;
	private:
		/**
		 * Private Member Functions
		 */

		//Read configuration and assign the value
		void 	mefn_readCfgParam(XMLNode &,const char *,int ,void *);
		void    mefn_readCfgAttributeParam(XMLNode &,const char *,int ,void *);
		void 	mefn_readUDPIpAndPort(XMLNode &);
		void 	mefn_readPeriodicEnquiryDetails(XMLNode &);
	protected:
		/**
		 * Protected Member Functions
		 */

	public:
		/**
		 * Public Member Functions
		 */

		//Constructor
		CAppConfigParams();
		//Destructor
		~CAppConfigParams();
		//Method to read initial configurations
		void 	mcfn_readConfigurations();

		//Method to handle the dynamic configuration change
		void 	mcfn_invokeConfigChange(CGenericEvent &);

		//Method to get the Config CLSID
		UUCID	mcfn_getConfigCLSID()				{ return DUMMY_UUCID; 				}

		char*	mcfn_getClassName()				{ return pscG_ConfigKey;			}


		//Fro DataBase
		char*   mcfn_getPrimaryDBServerIP()                     { return pmesc_PrimaryDBServerIP;       	}
		char*   mcfn_getDBSID()                                 { return pmesc_DBSID;                   	}
		char*   mcfn_getDBUserName()                            { return pmesc_DBUserName;              	}
		char*   mcfn_getDBPassword()                            { return pmesc_DBPassword;              	}
		char*   mcfn_getSecondaryDBServerIP()                   { return pmesc_SecondaryDBServerIP;     	}

		char*   mcfn_getInstanceMasterTable()                   { return pmesc_InstanceMasterTable;     	}
		char*   mcfn_getServiceInstanceMappingTable()        	{ return pmesc_ServiceInstanceMappingTable;     }
		char*   mcfn_getServiceMasterTable()        		{ return pmesc_ServiceMasterTable;		}

		int     mcfn_getDBConnMonitorInterval()                 { return mesi_DBConnMonitorInterval;    	}
		int     mcfn_getDBServerPort()                          { return mesi_DBServerPort;             	}
		int     mcfn_getDBConnectionpool()      		{ return mesi_DBConnectionpool;         	}
		
		//For UDP Server
		char* 	mcfn_getUDPServerIP() 				{ return pmesc_UDPServerIP;			}
		int 	mcfn_getUDPServerPort() 			{ return mesi_UDPServerPort;			}


		int 	mcfn_getMaxEnqiuryRequestTimeMs() 		{ return mesi_MaxEnqiuryRequestTimeMs;			}
		int 	mcfn_getPrimaryEquiryTimerMs() 			{ return mesi_PrimaryEquiryTimerMs;			}
		int 	mcfn_getMaxEnquiryMissCount() 			{ return mesi_MaxEnquiryMissCount;			}
		int 	mcfn_getPeriodicEnquiryType()			{ return mesi_PeriodicEnquiryType;			}
		int 	mcfn_getConsumerHandlerCount()			{ return mesi_ConsumerHandlerCount;			}
		int 	mcfn_getResourceExhaustLimit()			{ return mesi_ResourceExhaustLimit;			}

		//Redis Cache:
		//returns ClusterMode
                bool mcfn_getClusterMode()		{ return meb_ClusterMode;		}
		
                //returns AuthenticationStatus
                bool mcfn_getAuthenticationStatus()	{ return meb_IsAuthenticationEnabled;	}
		
                //returns PrimaryPort
                int mcfn_getPrimaryPort()		{ return mesi_PrimaryPort;		}
		
                //returns Primary conn pool size
                int mcfn_getPrimaryPoolSize()		{ return mesi_PrimaryPoolSize;		}
		
                //returns session expiry time
                int mcfn_getSessionExpiryTime()		{ return mesi_SessionExpiryTime;	}
		
		//returns primary host
                char* mcfn_getPrimaryHost()		{ return pmesc_PrimaryHost;		}
		
                //returns primary user name
                char* mcfn_getPrimaryUserName()		{ return pmesc_PrimaryUserName;		}

                //returns primary password
                char* mcfn_getPrimaryPassword()		{ return pmesc_PrimaryPassword;		}

		int mcfn_getSocketTimeOut()		{ return mesi_SocketTimeOutMS;		}
};

#endif
