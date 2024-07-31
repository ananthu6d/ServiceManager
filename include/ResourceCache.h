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
using namespace std;
/**
 * This resourceCache
 */
class CResourceCache 
{
	private:
		/**
                 * Public Variables
                 */
		//Singleton Reference Object
		static CResourceCache *pmeC_ResourceCache;
	
		IRedisInterface* pmeI_RedisCacheInterface;

		mutex meC_ResourceCacheLock;
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

		//Method to create Singleton Reference Object
		static CResourceCache* mcfn_getInstance()
		{
			if(pmeC_ResourceCache == nullptr)
				pmeC_ResourceCache = new CResourceCache();

			return pmeC_ResourceCache;
		}

		bool	mcfn_insertBusyCount(const string& CL_MapKey,const int& siL_BusyCount = 0);
		bool	mcfn_fetchBusyCount(const string& CL_MapKey,int&);

		bool	mcfn_incrementBusyCount(const string& CL_MapKey,const int& siL_MaxLimit,const int& siL_BusyCount = 1);
		bool	mcfn_decrementBusyCount(const string& CL_MapKey,const int& siL_BusyCount = 1);	

		bool	mcfn_resetBusyCount(const string& CL_MapKey);

		bool	mcfn_removeFromResourceCache(const string& CL_MapKey);
}; // end of class ResourceCache

#endif // end of _6dResourceCache_H
