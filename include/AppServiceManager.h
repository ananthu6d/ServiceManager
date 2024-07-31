/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* AppServiceManager.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dRNGServiceApp_H)
#define __6dRNGServiceApp_H

/**
 * Header Includes
 */

#include "AppConfigParams.h"
#include "DBConnectionPool.h"

using namespace _6d;
using namespace std;


/************************************************************************
 * Class : RNGServiceApp
 * Purpose : Main Application class
 ************************************************************************/

class CAppServiceManager : public CFramework
{
	private:
		/**
		 * Private Variables
		 */
		CDBConnectionPool* pmeC_DBConnectionPool;

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

	protected:
		/**
		 * Protected Member Functions
		 */

	public:
		/**
		 * Public Member Functions
		 */

		//Constructor
		CAppServiceManager();
		//Destructor
		~CAppServiceManager();
		//Implementation of base class virtual method
		void mcfn_printObject(){}
		//Entry point for the application to initialize the application resources
		bool mcfn_initInstance();
		//Method to get the version of the application
		void mcfn_getVersion();
		//Exit point for application to release application resources
		void mcfn_closeApplication();

		void mcfn_onSetLogSink();


		CMySQLConnection* mcfn_getConnection();

                void mcfn_releaseConnection(CMySQLConnection*& pCL_Conn);

		//load details from DB
		bool mcfn_loadFromDB();

		

};

#endif
