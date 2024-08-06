/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.                             *
 *  (c) 2008 6d Technologies                                            *
 *  All Rights Reserved                                                 *
 *                                                                      *
 ************************************************************************
 *
 *  ResourceCache.h
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -------------------------------------------------------
 *   1 2ndJune09   RKM       Created
 */


#if !defined(_ResourceCache_H)
#define _ResourceCache_H


/**
 * Header Includes
 */

#include <shared_mutex>
#include <mutex>
#include "RedisController.h"
#include "Runnable.h"
#include "AppUserTypes.h"
#include "STSMsgQue.h"
#include <unordered_map>
using namespace std;
/**
 * This resourceCache
 */
class CResourceCache : public CRunnable 
{
	private:
		/**
		 * Public Variables
		 */
		//Singleton Reference Object
		static CResourceCache *pmeC_ResourceCache;

		IRedisInterface* pmeI_RedisCacheInterface;

		mutex meC_ResourceCacheLock;

		CMsgQue<SResourceCacheEvent> meC_ResourceCacheQueue;
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
		bool 	mefn_processincrementBusyCount(const SResourceCacheEvent& SL_ResourceCacheEvent);
		bool	mefn_processdecrementBusyCount(const SResourceCacheEvent& SL_ResourceCacheEvent);
		bool 	mefn_processresetBusyCount(const SResourceCacheEvent& SL_ResourceCacheEvent);


		/**
		 * Constructor
		 */

		CResourceCache();
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

		~CResourceCache();
		/**
		 * Implementation of Base pure virtual function
		 * It is the thread function
		 */

		void mcfn_run();
		//Method to create Singleton Reference Object
		static CResourceCache* mcfn_getInstance()
		{
			if(pmeC_ResourceCache == nullptr)
				pmeC_ResourceCache = new CResourceCache();

			return pmeC_ResourceCache;
		}

		bool	mcfn_insertBusyCount(const string& CL_ServiceKey,const string& CL_InstanceKey,const int& siL_BusyCount = 0);
		bool	mcfn_fetchBusyCount(const string& CL_ServiceKey,const string& CL_InstanceKey,int&);

		bool	mcfn_incrementBusyCount(const string& CL_ServiceKey,const string& CL_InstanceKey,const int& siL_BusyCount = 1);
		bool	mcfn_decrementBusyCount(const string& CL_ServiceKey,const string& CL_InstanceKey,const int& siL_BusyCount = 1);	

		bool	mcfn_resetBusyCount(const string& CL_ServiceKey,const string& CL_InstanceKey);

		bool	mcfn_removeFromResourceCache(const string& CL_ServiceKey,const string& CL_InstanceKey);

		bool 	mcfn_getHashMapFromResourceCache(const string& CL_ServiceKey,std::unordered_map<std::string,std::string>&);


}; // end of class ResourceCache

#endif // end of _6dResourceCache_H
