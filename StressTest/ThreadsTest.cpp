#include "ThreadsTest.h"

#include <conio.h>
#include "Helper.h"
//#include "../ClientSubscriber/SubscriberHelper.h"






/*

DWORD WINAPI CreatePublisher(LPVOID lpvThreadParam)
{

	
	
	
	
	// socket used to communicate with server
	SOCKET connectSocket = INVALID_SOCKET;
	// variable used to store function return value
	int iResult;
	// message to send
	//char *messageToSend = "Ja sam publisher";
	//char messageToSend[DEFAULT_BUFLEN];

	char *messageToSend = (char*)malloc(sizeof(char) * DEFAULT_BUFLEN);

	//printf("\n\n***********************Ono sto sam ja: %d***********************\n\n", sizeof(DEFAULT_BUFLEN));
	//printf("\n\n************************Ono sto treba: %d***********************\n\n", sizeof(char)*DEFAULT_BUFLEN);



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

		Sleep(700);
	}

	

	Sleep(10000);
	//free allocated memory

	free(messageToSend);
	free(node);
	free(serializedMessage);
	//FreeTopicList(&head);







	// cleanup
	//closesocket(connectSocket);
	//WSACleanup();

	

	

	return 0;
}




DWORD WINAPI CreateSubscriber(LPVOID lpvThreadParam)
{
	InitializeTopicListCriticalSection();

	// socket used to communicate with server
	SOCKET connectSocket = INVALID_SOCKET;
	// variable used to store function return value
	int iResult;
	// message to send
	//char *messageToSend = "Ja sam subscriber";





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
		//printf("Test\n");

		iResult = send(connectSocket, initialMessageToServer + bytesSent, size - bytesSent, 0);


		if (iResult == SOCKET_ERROR || iResult == 0)
		{
			break;
		}

		bytesSent += iResult;

	} while (bytesSent < size);



	//printf("Bytes Sent: %ld\n", iResult);







	//ThreadRecieveFunction( &head, connectSocket, recvbuf);

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

	






	/*

	// Send an prepared message with null terminator included
	iResult = send(connectSocket, messageToSend, (int)strlen(messageToSend) + 1, 0);

	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	

	//char c = getchar();
	//c = getchar();

	char c;

	do {

		c = getch();


	} while (c != 27);


	printf("\n\nSubscriber closed successfully!\n");

	char end = getchar();
	end = getchar();

	CloseHandle(hThreadRecieve);

	free(initialMessageToServer);
	free(subscribedTopics);
	FreeTopicList(&head);




	// cleanup

	shutdown(connectSocket, SD_BOTH);
	//closesocket(connectSocket);
	//WSACleanup();






	return 0;


}

*/