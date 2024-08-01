/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* AppExterns.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dAppExterns_H)
#define __6dAppExterns_H

/**
 * Header Includes
 */

#include "AppServiceManager.h"
#include "GSPEventLog.h"
#include "AppConfigParams.h"
#include "InstanceInfo.h"
//#include "ICache.h"

/**
 * Extern Declarations
 */
extern CAppConfigParams CG_AppConfigParams;

extern CAppServiceManager CG_AppServiceManager;

extern long slG_SynKey;

extern int siG_InstanceID;

//extern ICache* pCG_ResourceCache;
#endif
