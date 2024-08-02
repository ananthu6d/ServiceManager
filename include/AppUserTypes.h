/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* AppUserTypes.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dAppUserTypes_H)
#define __6dAppUserTypes_H

#include "AppDefines.h"
/**
 * type definition for unsigned char
 */
//typedef unsigned char U8;
/**
 * type definition for unsigned short
 */
//typedef unsigned short U16;
/**
 * type definition for unsigned int
 */
//typedef unsigned int U32;
/**
 * type definition for unsigned long
 */
//typedef unsigned long U64;
/*
enum class EStatus
{
        IDLE            = 0x00,
        PROCESSING      = 0x01,
        FINISH          = 0x02,
        DONE            = 0x03
};
*/

typedef struct _SEventInfo
{
	char pmcsc_ClientIp[64];
	char pmcsc_SignalingIpPort[70];
	long mcsl_ClientPort;
	long mcsl_EventInfo;
	int mcsi_TotalBusyCount; 
        int mcsi_EventType; // Event Type
	_SEventInfo()
	{
		pmcsc_ClientIp[0]		= 0x00;
		pmcsc_SignalingIpPort[0]	= 0x00;
		mcsl_ClientPort			= 0x00;
		mcsi_TotalBusyCount		= 0x00;
		mcsi_EventType			= 0x00;
	}
}SEventInfo;


typedef struct _SServiceManagerEventHeader
{
	int mcsi_CmdId;
	int mcsi_SyncKey;//InstanceId or ServiceId
	int mcsi_ProtoBuffLength;
	char pmcsc_TransId[16];
	_SServiceManagerEventHeader()
	{
		pmcsc_TransId[0]=0x00;
	}
}SServiceManagerEventHeader;

typedef struct _SServiceManagerEvent
{
	SServiceManagerEventHeader mcS_EventHeader;
	char pmcsc_ProtoBuffEvent[MAXBUFFERSIZE];
	_SServiceManagerEvent()
	{
		pmcsc_ProtoBuffEvent[0]=0x00;
	}
}SServiceManagerEvent;


typedef struct _SResourceCacheEvent
{
	string mcC_ServiceTypeId;
	string mcC_SignalingIpPort;
	int mcsi_BusyCount;
	int mcsi_Type;
        _SResourceCacheEvent()
        {
		mcC_ServiceTypeId	= "";
		mcC_SignalingIpPort	= "";
		mcsi_BusyCount		= 0x00;
		mcsi_Type		= 0x00;
        }
}SResourceCacheEvent;

#endif
