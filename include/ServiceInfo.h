/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2020 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* ServiceInfo.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dServiceInfo_H)
#define __6dServiceInfo_H

/**
 * Header Includes
 */
#include "AppHeaders.h"

/************************************************************************
 * Class : ServiceInfo
 * Purpose : used to store the Service Details in cache
 ************************************************************************/


class CServiceInfo 
{

	private:
		/**
		 * Private Membesrs
		 */

		string	meC_ServiceType;
		string 	meC_ServiceId;

		char 	mesc_Status;
		char 	pmesc_ServiceName[50];
	public:
		/**
		 * Public Membesrs
		 */

		//Constructor
		CServiceInfo()
		{
			memset(pmesc_ServiceName, 0x00, sizeof(pmesc_ServiceName));
			mesc_Status	 = 'A';
			meC_ServiceId	 = "";
			meC_ServiceType  = "";	
		}

		//Destructor
		~CServiceInfo(){}

		// Setter Functions
		void 	mcfn_setServiceId(const string& CL_ServiceId)	{ meC_ServiceId = CL_ServiceId; 	}
		void 	mcfn_setServiceName(const char* pscL_Name) 	{ strcpy(pmesc_ServiceName, pscL_Name);	}
		void 	mcfn_setServiceType(const string& CL_Type) 	{ meC_ServiceType = CL_Type; 		}
		void 	mcfn_setStatus(const char& scL_Status) 		{ mesc_Status = scL_Status; 		}
		// Getter Functions
		string	mcfn_getServiceId() 	{ return meC_ServiceId;		}
		char* 	mcfn_getServiceName() 	{ return pmesc_ServiceName; 	}
		string 	mcfn_getServiceType() 	{ return meC_ServiceType; 	}
		char 	mcfn_getStatus() 	{ return mesc_Status; 		}

		void	 mcfn_deActivateService()
		{
			if(mesc_Status == 'A')
				mesc_Status = 'D';

		}
		void 	 mcfn_activateService() 	
		{
			if(mesc_Status == 'D')
				mesc_Status = 'A'; 
		}

};


#endif
