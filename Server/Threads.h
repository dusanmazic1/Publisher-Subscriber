#ifndef THREADS_H_
#define THREADS_H_

#include <ws2tcpip.h>
#include "../Common/Common.h"

typedef struct Client_thread_data {

	int clientId;
	Client_information **head_clientInfo;
	Topic_node **topic_head;
	SOCKET acceptedSocket;
	

}ClientThradData;


DWORD WINAPI ThreadPublisherFunction(LPVOID lpvThreadParam);

DWORD WINAPI ThreadSubscriberFunction(LPVOID lpvThreadParam);






#endif


