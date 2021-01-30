#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

//#include "../Common/TopicRepository.h"
//#include "../Common/Common.h"
#include "PublisherHelper.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27016

// Initializes WinSock2 library
// Returns true if succeeded, false otherwise.
//bool InitializeWindowsSockets();



bool InitializeWindowsSockets();

int __cdecl main() 
{
	
	InitializeTopicListCriticalSection();
	// socket used to communicate with server
	SOCKET connectSocket = INVALID_SOCKET;
	// variable used to store function return value
	int iResult;
	

	char *messageToSend = (char*)malloc(sizeof(char) * DEFAULT_BUFLEN);          




	if (InitializeWindowsSockets() == false)
	{
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
	}

	// create a socket
	connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// create and initialize address structure
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(DEFAULT_PORT);

	// connect to server specified in serverAddress and socket connectSocket
	if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connectSocket);
		WSACleanup();
	}

	unsigned long mode = 1;
	if (ioctlsocket(connectSocket, FIONBIO, &mode) == SOCKET_ERROR) {
		printf("ioctl failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	int publisher = 0;  // when we send 0 server know that publisher connected
	iResult = send(connectSocket, (char*)&publisher, sizeof(int), 0);


	// reading topics from file and store it in linked list
	FILE *topicsFile = SafeOpen("Topics.txt", "r");
	Topic_node* head = NULL;
	int counter;
	ReadAllTopics(topicsFile, &head, &counter);

	fclose(topicsFile);

	//PrintTopicList(&head);
	//PrintTopicListWithCounter(&head);
	//printf("%d", counter);

	PublisherNode *node = (PublisherNode*)malloc(sizeof(PublisherNode));
	char *serializedMessage = (char*)malloc(sizeof(DEFAULT_BUFLEN));

	do {


		

		PrintPublisherMenu(&head);
		int selectedOption = SelectSpecificTopic(counter);
		//printf("\nOVO JE IZABRAO:%d", selectedOption);

		if (selectedOption == 0)
			break;

		EnterMessage(messageToSend);

		//printf("\nOvo je hteo da posalje:%s\n", messageToSend);
		

		
		
		//PublisherNode *node = (PublisherNode*)malloc(sizeof(PublisherNode));
		strcpy(node->message, messageToSend);
		node->messageLength = strlen(node->message);
		node->topicId = selectedOption - 1; //ako selektuje 1 to znaci da je 0 indeks


		serializedMessage = SerializePublisherData(node);

		int size = 2 * sizeof(int) + node->messageLength;

	
		

		bool end = SendPublisherMessageToServer(connectSocket, serializedMessage, size);

		if (end)
			break;




	} while (true);
	

	printf("\n\nPress any key to close publisher...\n\n");
	char c = getchar();
	c = getchar();

	//free allocated memory

	free(messageToSend);
	free(node);		
	free(serializedMessage);
	FreeTopicList(&head);

	

	



	// cleanup
	closesocket(connectSocket);
	WSACleanup();

	return 0;
    

   
}

/*
bool InitializeWindowsSockets()
{
    WSADATA wsaData;
	// Initialize windows sockets library for this process
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return false;
    }
	return true;
}
*/

bool InitializeWindowsSockets()
{
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}