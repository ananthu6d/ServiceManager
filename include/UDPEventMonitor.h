/************************************************************************
 *                                                                      *
 *  The information described in this file is a confidential and        *
 *  proprietary product of 6d Technologies.                             *
 *  (c) 2008 6d Technologies                                            *
 *  All Rights Reserved                                                 *
 *                                                                      *
 ************************************************************************
 *
 *  IPEventMonitor.h
 *
 *
 * Rev Date      User      Comments
 * --- --------- --------  -------------------------------------------------------
 *   
 */


#if !defined(_UDPEventMonitor_H)
#define _UDPEventMonitor_H


/**
* Header Includes
*/
#include "Runnable.h"
#include "EventCriteria.h"
#include <map>
#include "ISocket.h"
#include "string"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ResourceManager.h"
#include "AppDefines.h"
#include "InstanceInfo.h"
#include "AppUserTypes.h"
#include "ServiceResource.h"
#include "ServiceManagerInterface.pb.h"

using namespace std;
using namespace _6d;
using namespace _6d::_Utils;
using namespace _6d::_Framework;

/************************************************************************
 * Class : CUDPEventMonitor
 * Purpose : Monitor events and dispatch
 ************************************************************************/

class CUDPEventMonitor : public IEventMonitor , public CRunnable 
{
	private:
		//Singleton Reference Object
		static CUDPEventMonitor *pmeC_EventMonitor;

		/**
		 * Variable to hold socket file descriptor
		 */
		int      mesi_Socket;

		/**
		 * Variable to hold socket address
		 */
		sockaddr_in meS_SockAddr;

		//CUDPPacketInfo meC_UDPPacketInfo;
		
		SServiceManagerEvent* pmeS_ServiceManagerEvent;

		char 	pmesc_ClientIp[64];
		long 	mesl_ClientPort;



	private:
		/**
		 * Constructor
		 */

		CUDPEventMonitor();

		bool 	mefn_create();
		bool 	mefn_bind();
		void 	mefn_initalize();

	public:
		/**
		 * Destructor
		 */

		~CUDPEventMonitor();

		/**
		 * Implementation of Base pure virtual Method
		 * Method to register Listeners for the events
		 * @param1 is the reference to Event Listener to be triggered when event
		 * occurs
		 * @param2 is reference of Event criteria to be met to trigger the event
		 */

		void 	mcfn_addListener(IEventListener *,CEventCriteria *);

		/**
		 * Implementation of Base pure virtual Method
		 * Method to unregister Listeners for the events
		 * @param1 is the reference to Event Listener to be removed
		 */

		void 	mcfn_removeListener(IEventListener *);

		/**
		 * Method to dispatch the Events to the Event listeners
		 */

		void 	mcfn_initializeForEvents();
		bool 	mcfn_receiveEvents();

		void 	mcfn_dispatchEvent();

		void 	mcfn_dispatchEvent(IEventListener*,int,int,CInstanceInfo*);

		//void mcfn_dispatchEvent(IEventListener*,int,CServiceResource*);
		//
		void 	mcfn_dispatchEvent(IEventListener*,int,long);


		 void 	mcfn_dispatchEvent(IEventListener*,int,char*,long);

		/**
		 * Implementation of Base pure virtual to print the Object Contents
		 */

		void 	mcfn_printObject();

		/**
		 * Implementation of Base pure virtual function
		 * It is the thread function
		 */
		void 	mcfn_run();
		void 	mcfn_close();
		bool 	mcfn_sendTo(char*,int,char*,long);

                //Method to create Singleton Reference Object
                static CUDPEventMonitor *mcfn_getInstance();
		


}; // end of class IPCEventMonitor

#endif // end of _6dIPEventMonitor_H
