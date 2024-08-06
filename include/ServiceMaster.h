/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.                             *
 *  (c) 2008 6d Technologies                                            *
 *  All Rights Reserved                                                 *
 *                                                                      *
 ************************************************************************
 *
 *  ServiceMaster.h
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -------------------------------------------------------
 *   
 */


#if !defined(_ServiceMaster_H)
#define _ServiceMaster_H


/**
 * Header Includes
 */

#include "AppHeaders.h"
#include "InstanceInfo.h"
#include "ServiceHandler.h"
#include <unordered_map>

/************************************************************************
 * Class : CServiceMaster
 * Purpose : holds ServiceHandlers
 ************************************************************************/

class CServiceMaster  
{
	private:
		/**
                 * Public Variables
                 */
		//Singleton Reference Object
		static CServiceMaster*	pmeC_ServiceMaster;

		mutex	meC_ServiceMasterLock;
		
		std::map<string,CServiceHandler*> meC_ServiceHandlerMap;

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
                 * Private Functions
                 */

		/**
		 * Constructor
		 */

		CServiceMaster();
        protected:
                /**
                 * Protected Functions 
                 */


	public:
		/**
                 * Public Functions
                 */

		/**
		 * Destructor
		 */

		~CServiceMaster();
		/**
		 * Implementation of Base pure virtual function
		 * It is the thread function
		 */

		//Method to create Singleton Reference Object
		static CServiceMaster *mcfn_getInstance()
		{
			if(pmeC_ServiceMaster == nullptr)
				pmeC_ServiceMaster = new CServiceMaster();

			return pmeC_ServiceMaster;
		}

		bool mcfn_insertServiceHandler(const string& CL_ServiceId_Type,CServiceHandler* pCL_ServiceHandler);
		bool mcfn_getServiceHandler(const string& CL_ServiceId_Type,CServiceHandler** pCL_ServiceHandler);
		bool mcfn_deleteServiceHandler(const string& CL_ServiceId_Type);
		bool mcfn_isServiceHandlerExist(const string& CL_ServiceId_Type);

		int mcfn_ServiceHandlerSize() { return meC_ServiceHandlerMap.size(); }

		bool mcfn_getServiceName(const string& CL_ServiceId_Type,char* pscL_ServiceName);

		bool mcfn_loadTotalBusyCount();


		
}; // end of class ServiceMaster

#endif // end of _6dServiceMaster_H
