#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdlib.h>
#include <stdio.h>
#include <conio.h>



#include "Helper.h"
#include "../Common/Common.h"

#define DEFAULT_PORT 27016
#define NUMBER_OF_TOPICS 5




// configurable constanst for test
#define MAX_THREADS 5000
#define NUMBER_OF_PUBLISHER_MESSAGE 1000



typedef struct Client_thread_data {

	int clientId;
	Client_information **head_clientInfo;
	Topic_node **topic_head;
	SOCKET acceptedSocket;


}ClientThradData;



typedef struct Important_data {

	Topic_node **head;
	SOCKET connectSocket;

}Important_data;







// Initializes WinSock2 library
// Returns true if succeeded, false otherwise.
bool InitializeWindowsSockets();

DWORD WINAPI ThreadRecieveFunction(LPVOID lpvThreadParam);
DWORD WINAPI CreatePublisher(LPVOID lpvThreadParam);
DWORD WINAPI CreateSubscriber(LPVOID lpvThreadParam);


HANDLE handleFinishSignal;



int __cdecl main() 
{
    
	int numberOfPublishers = 0;
	int numberOfSubscribers = 0;

	InitializeTopicListCriticalSection();


	
	
  
    if(InitializeWindowsSockets() == false)
    {
		// we won't log anything since it will be logged
		// by InitializeWindowsSockets() function
		return 1;
    }
	printf("\n\nWelcome to StressTest\n\n");

	do {
		printf("\Max number of clients = %d\n", MAX_THREADS);

		do {

			printf("Enter number of publishers: ");
			scanf("%d", &numberOfPublishers);

		} while (numberOfPublishers <= 1);


		do {

			printf("Enter number of subscribers: ");
			scanf("%d", &numberOfSubscribers);

		} while (numberOfSubscribers <= 1);

	} while (numberOfPublishers + numberOfSubscribers > MAX_THREADS);

	int allThreadsSize = numberOfPublishers + numberOfSubscribers;
	int threadCounter = 0;

	HANDLE allThreads[MAX_THREADS];

	handleFinishSignal = CreateSemaphore(0, 0, numberOfSubscribers, NULL);

	


	printf("\nPress any key to start stress test..\n");

	char startStressTest = getch();



	DWORD lpThreadId = -1;

	

	
	for (int i = 0; i < numberOfSubscribers; i++)
	{
		allThreads[threadCounter] = CreateThread(NULL, 0, &CreateSubscriber, NULL, 0, &lpThreadId); // create thread to simulate subscriber
		threadCounter++;

		Sleep(150);

	}


	Sleep(5000);
	
	for (int i = 0; i < numberOfPublishers; i++)
	{
		allThreads[threadCounter] = CreateThread(NULL, 0, &CreatePublisher, NULL, 0, &lpThreadId); // create thread to simulate publisher
		threadCounter++;

		Sleep(150);
	}

	

	


	//printf("\nPress any key to close StressTest..\n");


	char endServer;

	printf("\n\nPress ESC to close StressTest..\n\n");

	do {

		
		endServer = getch();
	

	} while (endServer != 27);

	printf("\StressTest is closing down..\n");

	

	
	ReleaseSemaphore(handleFinishSignal, numberOfSubscribers, NULL);


	Sleep(5000);

	for (int i = 0; i < numberOfPublishers + numberOfSubscribers; i++)
		CloseHandle(allThreads[i]);


	CloseHandle(handleFinishSignal);


	printf("\n\nStressTest succesfully closed..\n");
	char endStressTest = getchar();
	endStressTest = getchar();
   

    return 0;
}

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






DWORD WINAPI CreatePublisher(LPVOID lpvThreadParam)
{





	// socket used to communicate with server
	SOCKET connectSocket = INVALID_SOCKET;
	// variable used to store function return value
	int iResult;
	

	char *messageToSend = (char*)malloc(sizeof(char) * DEFAULT_BUFLEN);

	



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

	for (int i = 0; i < NUMBER_OF_PUBLISHER_MESSAGE; i++)
	{

		int selectedOption = generateRandomTopic(NUMBER_OF_TOPICS);

		memset(messageToSend, 0, MAX_MESSAGE_LENGTH);

		int randomMessage = generateRandomMessage(3);

		if (randomMessage == 0)
			strcpy(messageToSend, "SHORT MESSAGE");
		else if (randomMessage == 1)
			strcpy(messageToSend, "LONG MESSAGE RANDOM CHARACTERS: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAOPPPPPPPPP LONG MESSAGE ");
		else
			strcpy(messageToSend, "NORMAL MESSAGE USED FOR STREES TEST");



		messageToSend[strlen(messageToSend)] = '\n';





		strcpy(node->message, messageToSend);
		node->messageLength = strlen(node->message);
		node->topicId = selectedOption;


		serializedMessage = SerializePublisherData(node);

		int size = 2 * sizeof(int) + node->messageLength;


		bool end = SendPublisherMessageToServer(connectSocket, serializedMessage, size);



		if (end)
			break;

	   Sleep(300);
	}



	Sleep(10000);
	//free allocated memory

	free(messageToSend);
	free(node);
	free(serializedMessage);
	







	





	return 0;
}




DWORD WINAPI CreateSubscriber(LPVOID lpvThreadParam)
{
	InitializeTopicListCriticalSection();

	// socket used to communicate with server
	SOCKET connectSocket = INVALID_SOCKET;
	// variable used to store function return value
	int iResult;
	





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

	PutSocketInNonblockingMode(&connectSocket);

	int subscriber = 1; //  // when we send 1 server know that subscriber connected
	iResult = send(connectSocket, (char*)&subscriber, sizeof(int), 0);


	FILE *topicsFile = SafeOpen("Topics.txt", "r");
	Topic_node* head = NULL;
	int counter;
	ReadAllTopics(topicsFile, &head, &counter);
	fclose(topicsFile);


	int choice = -1;
	int *subscribedTopics = (int*)malloc(counter * sizeof(int));

	memset(subscribedTopics, 0, counter * sizeof(int));

	for (int i = 0; i < counter; i++)           // subscribe to all topics
		subscribedTopics[i] = 1;


	bool isSubsribed = false;
	//int subscribedTopicsCounter = 0;


	//printf("\n%d\n", subscribedTopicsCounter);


	// treba da posaljem serveru informacije o temama na koje je subsribovan

	int size = (counter + 1) * sizeof(int);

	char *initialMessageToServer = (char*)malloc(size);
	*(int*)(initialMessageToServer) = counter;
	memcpy(initialMessageToServer + sizeof(int), subscribedTopics, counter * sizeof(int));

	int bytesSent = 0;

	do {
	

		iResult = send(connectSocket, initialMessageToServer + bytesSent, size - bytesSent, 0);


		if (iResult == SOCKET_ERROR || iResult == 0)
		{
			break;
		}

		bytesSent += iResult;

	} while (bytesSent < size);



	

	printf("\n\nPress ESC to close client subscriber...\n\n");

	DWORD lpThreadId;
	HANDLE hThreadRecieve;

	Important_data *importantData = (Important_data*)malloc(sizeof(Important_data));
	importantData->connectSocket = connectSocket;
	importantData->head = &head;


	hThreadRecieve = CreateThread(NULL, 0, &ThreadRecieveFunction, importantData, 0, &lpThreadId);

	if (hThreadRecieve == NULL)
	{
		printf("\nCreating thread for receiving messages failed!\n");

		return -1;
	}









	WaitForSingleObject(handleFinishSignal, INFINITE);


	printf("\n\nSubscriber closed successfully!\n");



	CloseHandle(hThreadRecieve);

	free(initialMessageToServer);
	free(subscribedTopics);
	









	return 0;


}


DWORD WINAPI ThreadRecieveFunction(LPVOID lpvThreadParam)
{

	Important_data *importantData = (Important_data*)lpvThreadParam;

	SOCKET connectSocket = importantData->connectSocket;
	Topic_node **head = importantData->head;

	free(lpvThreadParam);

	char recvbuf[DEFAULT_BUFLEN];



	printf("\n\n----------------------------------------------------------------\n");
	printf("Waiting for news from publisher..\n\n");

	SelectFunction(connectSocket, READ);
	RecieveServerMessage(head, connectSocket, recvbuf);


	return 0;
}


