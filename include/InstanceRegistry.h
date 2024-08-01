/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.                             *
 *  (c) 2008 6d Technologies                                            *
 *  All Rights Reserved                                                 *
 *                                                                      *
 ************************************************************************
 *
 *  InstanceRegistry.h
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -------------------------------------------------------
 *   1 2ndJune09   RKM       Created
 */


#if !defined(_InstanceRegistry_H)
#define _InstanceRegistry_H


/**
 * Header Includes
 */

#include "AppHeaders.h"
#include "InstanceInfo.h"
#include <shared_mutex>
#include <mutex>

/************************************************************************
 * Class : CInstanceRegistry
 * Purpose : internal cache holds instanceInfo
 ************************************************************************/

class CInstanceRegistry  
{
	private:
		/**
                 * Public Variables
                 */
		//Singleton Reference Object
		static CInstanceRegistry *pmeC_InstanceRegistry;

		std::map<string,CInstanceInfo*>* pmeC_Registry;

		//std::shared_timed_mutex  meC_RegistryLock;
		//
		std::mutex meC_RegistryLock;
		// map<ipPort,busycount> 
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

		CInstanceRegistry();

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

		~CInstanceRegistry();
		/**
		 * Implementation of Base pure virtual function
		 * It is the thread function
		 */

		//Method to create Singleton Reference Object
		static CInstanceRegistry *mcfn_getInstance()
		{
			if(pmeC_InstanceRegistry == nullptr)
				pmeC_InstanceRegistry = new CInstanceRegistry();

			return pmeC_InstanceRegistry;
		}

		int	mcfn_getRegistrySize() { return pmeC_Registry->size(); }
		bool 	mcfn_insertInstanceInfo(const string& CL_SignalingIpPort, CInstanceInfo& CL_InstanceInfo);
		bool 	mcfn_isInstanceInfoExist(const string& CL_SignalingIpPort);
		bool 	mcfn_insertInstanceInfo(const string& CL_SignalingIpPort,CInstanceInfo* pCL_Instance);
		bool 	mcfn_getInstanceInfo(const string& CL_SignalingIpPort, CInstanceInfo& CL_InstanceInfo);
		bool 	mcfn_getInstanceInfo(const string& CL_SignalingIpPort, CInstanceInfo** pCL_Instance);
		bool 	mcfn_deActivateInstanceInfo(const string& CL_SignalingIpPort);
		bool 	mcfn_activateInstanceInfo(const string& CL_SignalingIpPort);
		
		bool 	mcfn_deActivateService(const string& CL_SignalingIpPort,const string& CL_ServiceType_Id);
		bool 	mcfn_activateService(const string& CL_SignalingIpPort,const string& CL_ServiceType_Id);
		
		bool 	mcfn_eraseService(const string& CL_SignalingIpPort, const string& CL_ServiceType_Id);

		bool 	mcfn_addService(const string& CL_SignalingIpPort, const string& CL_ServiceId,const string& CL_ServiceType, const int& siL_MaxLimit,const int& siL_AllotedCap,const char& scL_Status);
		bool 	mcfn_updateService(const string& CL_SignalingIpPort, const string& CL_ServiceType_Id, const int& siL_MaxLimit);

		bool 	mcfn_updateClientAddress(const string& CL_SignalingIpPort, char* pscL_IpAddress, const int& siL_Port);

		void 	mcfn_eraseInstanceInfo(const string& CL_SignalingIpPort);
		bool 	mcfn_loadIntoServiceResource();

		int 	mcfn_getOBDTotalChannelCount(const string&);
		int 	mcfn_getIBDTotalChannelCount(const string&);

		bool 	mcfn_checkAndIncrement(const string&, const string&);
		bool	mcfn_checkAndDecrement(const string&, const string&);

		bool 	mcfn_setOBDTotalChannelCount(const string&,const int&);
		bool    mcfn_setIBDTotalChannelCount(const string&,const int&);

}; // end of class InstanceRegistry

#endif // end of _6dInstanceRegistry_H
