/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2020 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* InstanceServiceInfo.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dInstanceServiceInfo_H)
#define __6dInstanceServiceInfo_H

/**
 * Header Includes
 */
#include "AppHeaders.h"

/************************************************************************
 * Class : InstanceServiceInfo
 * Purpose : used to store the Service Details in cache
 ************************************************************************/


class CInstanceServiceInfo 
{

	private:
		/**
		 * Private Membesrs
		 */

		string	meC_ServiceType;
		string 	meC_ServiceId;

		int	mesi_MaxLimit;

		char 	mesc_Status;
		char 	pmesc_ServiceName[50];
	public:
		/**
		 * Public Membesrs
		 */

		//Constructor
		CInstanceServiceInfo()
		{
			memset(pmesc_ServiceName, 0x00, sizeof(pmesc_ServiceName));

			mesc_Status	 = 'A';
			meC_ServiceId	 = "";
			meC_ServiceType  = "";	
			mesi_MaxLimit 	 = 0x00;
		}

		//Destructor
		~CInstanceServiceInfo(){}


		// Setter Functions
		void 	mcfn_setServiceId(const string& CL_ServiceId)	{ meC_ServiceId=CL_ServiceId; 		}
		void 	mcfn_setServiceName(const char* pscL_Name) 	{ strcpy(pmesc_ServiceName, pscL_Name);	}
		void 	mcfn_setServiceType(const string& CL_Type) 	{ meC_ServiceType = CL_Type; 		}
		void 	mcfn_setMaxLimit(const int& siL_Limit) 		{ mesi_MaxLimit = siL_Limit;		}
		void 	mcfn_setStatus(const char& scL_Status) 		{ mesc_Status=scL_Status; 		}
		// Getter Functions
		string	mcfn_getServiceId() 	{ return meC_ServiceId;		}
		char* 	mcfn_getServiceName() 	{ return pmesc_ServiceName; 	}
		string 	mcfn_getServiceType() 	{ return meC_ServiceType; 	}
		int 	mcfn_getMaxLimit() 	{ return mesi_MaxLimit;	 	}
		char 	mcfn_getStatus() 	{ return mesc_Status; 		}

		void	 mcfn_deActivateService() 	{ mesc_Status = 'D'; }
                void 	 mcfn_activateService() 	{ mesc_Status = 'A'; }



};


#endif
