/************************************************************************
*                                                                       *
* The information described in this file is a confidential and          *
* proprietary product of 6d Technologies.                               *
* (c) 2022 6d Technologies                                              *
* All Rights Reserved                                                   *
*                                                                       *
*************************************************************************
*
* AppDefines.h
*
*
* Rev Date      User      Comments
* --- --------- --------  -----------------------------------------------
*
*/


#if !(defined __6dAppDefines1_H)
#define __6dAppDefines1_H

extern int siG_InstanceID;
/**
 * Predefined Macros
 */

#define __entryFunction__ EVT_LOG((_6d::_Framework::CG_EventLog),LOG_VERBOSE,siG_InstanceID,"Function","Entry",this,"","%s>>>" ,__func__);
#define __return__(A) {EVT_LOG((_6d::_Framework::CG_EventLog),LOG_VERBOSE,siG_InstanceID,"Function","Exit",this,"","%s<<<" ,__func__); return A;}

/**
 * Extern Declarations
 */
#define MAXRETRY		3
#define DIGITS 			"9876543210"
#define ENQUIRY_TYPE		1
#define INITIAL_SYNKEY		1
#define MAXLIMIT_DEFAULT	-1
#define UNITDUR 1000
#define NOCONNECTIONSLEEPDUR            UNITDUR*500
#define DBFAILURESLEEPDUR		UNITDUR*500
#define POOLOUTAGEDUR			UNITDUR*500
#define MAXBUFFERSIZE 		1024
#define CONSUMERHANDLERPOOL	98327

/**
 * Status Codes
 */
#define SUCCESS 		0
#define STATUS_REG_FAILURE 	9

/**
 * Cmd Id
 */
#define CMD_REGISTER_INSTANCE_REQ               1
#define CMD_DEREGISTER_INSTANCE_REQ             2             
#define CMD_DEREGISTER_SERVICE_RESOURCE_REQ     3        
#define CMD_REGISTER_CONSUMER_REQ               4                  
#define CMD_DEREGISTER_CONSUMER_REQ             5                 
#define CMD_FETCH_RESOURCE_REQ                  6
#define CMD_RELEASE_RESOURCE_REQ                7
#define CMD_ENQUIRY_REQ                         8
#define CMD_BLOCK_SERVICE_RESOURCE_REQ          9                    
#define CMD_UNBLOCK_SERVICE_RESOURCE_REQ        10                  
#define CMD_BLOCK_SERVICE_REQ                   11                             
#define CMD_UNBLOCK_SERVICE_REQ                 12                           
#define CMD_SOCKET_RECONNECT_REQ		13

#define CMD_REGISTER_INSTANCE_RESP               14
#define CMD_DEREGISTER_INSTANCE_RESP             15            
#define CMD_DEREGISTER_SERVICE_RESOURCE_RESP     16       
#define CMD_REGISTER_CONSUMER_RESP               17                 
#define CMD_DEREGISTER_CONSUMER_RESP             18                
#define CMD_FETCH_RESOURCE_RESP                  19
#define CMD_RELEASE_RESOURCE_RESP                20
#define CMD_ENQUIRY_RESP                         21
#define CMD_BLOCK_SERVICE_RESOURCE_RESP          22                   
#define CMD_UNBLOCK_SERVICE_RESOURCE_RESP        23                  
#define CMD_BLOCK_SERVICE_RESP	             	 24                             
#define CMD_UNBLOCK_SERVICE_RESP                 25                           
#define CMD_GENERIC_ERROR_RESP                   26
#define CMD_SOCKET_RECONNECT_RESP                27

/**
 * Error Codes
 */
#define ER_SERIALIZING_DATA 		99
#define ER_PARSING_DATA     		98
#define ER_INVALID_CMDID		97
#define ER_NOSERVICELIST		96
#define ER_INSTANCEID_NOTREGISTERED	95
#define ER_NO_INSTANCE_REG		94
#define ER_RESOURCE_BUSY		93
#define ER_RELEASE_FALIURE		92
#define ER_NO_SERVICE			91
#define ER_CACHE_FALIURE		90
#define ER_NO_RESOURCES_AVAILABLE  	89


/**
 * Query Types
 */
#define QUERYTYPE_DEREGISTER_ALL_SERVICE 	1
#define QUERYTUPE_DEREGISTER_SINGLE_SERVICE 	2

#define QUERYTYPE_UPDATE_MAXLIMIT		1
#define QUERYTYPE_ACTIVATE_RESOURCE		2
#define QUERYTYPE_UPDATE_ALLOCATED_RESOURCE	3

#define QUERYTYPE_GENERIC_RESOURCE_UPDATE	1
#define QUERYTYPE_ACTIVATE_INSTANCE		2

/**
 * Events
 */
#define EVT_NETWORK 		1
#define EVT_ENQUIRY_TIMEOUT	2
#define EVT_REQUEST_TIMEOUT	3
#define EVT_PURGE_INSTANCE	4
#define EVT_PURGE_RESOURCE	5
#define EVT_ADD_RESOURCE	6
#define EVT_RESET_RESOURCE	7
#define EVT_UPDATE_TOTALCHANNEL	8
#define EVT_UPDATE_RESOURCE	9
#define EVT_DEACT_RESOURCE	10
#define EVT_ACT_RESOURCE	11
#define EVT_REMOVE_RESOURCE 	12
#define EVT_LOAD_RESOURCE	13
#define EVT_RESET_BUSYCOUNT	14
/**
 * Periodoic Retry Types
 */
#define PERIODIC_RETRY_TYPE_IMMEDIATE		1
#define PERIODIC_RETRY_TYPE_INTERVEL		2
#define PERIODIC_RETRY_TYPE_DEFAULT		3


/**
 * Query Builder Types
 */

#define QB_TYPE_TR 		1
#define QB_TYPE_IBD 		2
#define QB_TYPE_TR_IBD 		3
#define QB_TYPE_OBD 		4
#define QB_TYPE_TR_OBD 		5
#define QB_TYPE_IBD_OBD		6
#define QB_TYPE_TR_IBD_OBD 	7

#endif
