#ifndef CLIENTINFORMATIONLIST_H_
#define CLIENTINFORMATIONLIST_H_


#include "../Common/ListTopic.h"
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"
#include <ws2tcpip.h>

typedef struct Client_information {

	int clientID;
	SOCKET clientSocket;
	DWORD lpThreadId;
	HANDLE handleClientThread;
	HANDLE queueSemaphore;
	Queue* subscriberMessages;

	struct Client_information* next;

}Client_information;




void InsertFirstClientInformation(Client_information** head, int clientID, SOCKET clientSocket, DWORD lpThreadId, HANDLE handleClientThread);  // insert client information at the beginning of the list
void InsertEndClientInformation(Client_information** head, int clientID, SOCKET clientSocket, DWORD lpThreadId, HANDLE handleClientThread);  // insert client information at the end of the list
bool IsEmptyClientInformation(Client_information**); // check if list is empty
struct Client_information* FindElementClientInformation(Client_information** head, int clientID); // return element of the list with clientId 
void PrintClientInformationList(Client_information** head);  // print all list
void DeleteElementClientInformation(Client_information **head, int clientId); // delete element with clientId from list
void FreeClientInformationList(Client_information **head); // free allocated space for clientInformationList
void InitializeClientInformationCriticalSection();
void CloseAllHandles(Client_information **head);




#endif


