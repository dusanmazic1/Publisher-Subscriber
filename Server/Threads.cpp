#include "Threads.h"

#include <conio.h>


DWORD WINAPI ThreadPublisherFunction(LPVOID lpvThreadParam)
{
	ClientThradData publisherThreadData = *((ClientThradData*)lpvThreadParam);

	int clintId = publisherThreadData.clientId;
	Client_information **head_clientInfo = publisherThreadData.head_clientInfo;
	SOCKET acceptedSocket = publisherThreadData.acceptedSocket;
	Topic_node **topic_head = publisherThreadData.topic_head;

	



	char recvbuf[DEFAULT_BUFLEN];

	RecievePublisherMessage(head_clientInfo, topic_head, acceptedSocket, recvbuf, clintId);


	return 0;

}

DWORD WINAPI ThreadSubscriberFunction(LPVOID lpvThreadParam)
{

	ClientThradData subscriberThreadData = *((ClientThradData*)lpvThreadParam);

	int clientId = subscriberThreadData.clientId;
	Client_information **head_clientInfo = subscriberThreadData.head_clientInfo;
	SOCKET acceptedSocket = subscriberThreadData.acceptedSocket;
	Topic_node **topic_head = subscriberThreadData.topic_head;

	
	

	char recvbuf[DEFAULT_BUFLEN];

	RecieveSubsriberMessage(head_clientInfo, topic_head, acceptedSocket, recvbuf, clientId);


	SendMessageToSubscriber(head_clientInfo, topic_head, clientId);


	return 0;


}


