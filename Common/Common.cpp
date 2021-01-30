#include "pch.h"
#include "Common.h"

#include <stdio.h>
#include <stdlib.h>


/*
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
*/

void SelectFunction(SOCKET socket, SelectOption option)
{
	while (1)
	{
		// Initialize select parameters
		FD_SET set;
		timeval timeVal;

		FD_ZERO(&set);
		// Add socket we will wait to read from
		FD_SET(socket, &set);
		// Set timeouts to zero since we want select to return
		// instantaneously

		timeVal.tv_sec = 0;
		timeVal.tv_usec = 0;

		int iResult;

		if (option == READ)
			iResult = select(0 /* ignored */, &set, NULL, NULL, &timeVal);
		else if (option == WRITE)
			iResult = select(0 /* ignored */, NULL, &set, NULL, &timeVal);
		else
		{
			printf("\nUnknown SelectOption!\n");
			return;
		}


		if (iResult == 0) {
			// vreme za cekanje je isteklo
			//printf("Isteklooo");
			Sleep(100);
			continue;
		}
		else if (iResult == SOCKET_ERROR) {
			//desila se greska prilikom poziva funkcije
			//printf("\nError occured in select function..\n");
			printf("\nError occured in select function.. %d\n", WSAGetLastError());
		}
		else {
			// rezultat je jednak broju soketa koji su zadovoljili uslov
			return;
		}
	}


	
}

int SelectFunctionRW(SOCKET socket)
{



	while (1)
	{
		// Initialize select parameters
		FD_SET set;
		FD_SET write;
		timeval timeVal;

		FD_ZERO(&set);
		FD_ZERO(&write);

		// Add socket we will wait to read from
		FD_SET(socket, &set);
		FD_SET(socket, &write);
		// Set timeouts to zero since we want select to return
		// instantaneously

		timeVal.tv_sec = 0;
		timeVal.tv_usec = 0;

		int iResult;

	
		
		iResult = select(0 /* ignored */, &set, &write, NULL, &timeVal);
	

		


		if (iResult == 0) {
			// vreme za cekanje je isteklo
			//printf("Isteklooo");
			Sleep(100);
			continue;
		}
		else if (iResult == SOCKET_ERROR) {
			//desila se greska prilikom poziva funkcije
			printf("\nError occured in select function.. %d\n", WSAGetLastError());
		}
		else {
			// rezultat je jednak broju soketa koji su zadovoljili uslov
			if (FD_ISSET(socket, &set))
			{
				return 0;
			}
			else if (FD_ISSET(socket, &write))
			{
				return 1;
			}
		}
	}



}

int SelectFunctionServer(SOCKET socket)
{


	while (1)
	{
		// Initialize select parameters
		FD_SET set;
		//FD_SET write;
		timeval timeVal;

		FD_ZERO(&set);
		//FD_ZERO(&write);

		// Add socket we will wait to read from
		FD_SET(socket, &set);
		

		timeVal.tv_sec = 0;
		timeVal.tv_usec = 0;

		int iResult;



		iResult = select(0 /* ignored */, &set, NULL, NULL, &timeVal);





		if (iResult == 0) {
			// vreme za cekanje je isteklo
			//printf("Isteklooo");
			Sleep(100);
			//continue;

			return 0;
		}
		else if (iResult == SOCKET_ERROR) {
			//desila se greska prilikom poziva funkcije
			printf("\nError occured in select function.. %d\n", WSAGetLastError());
		}
		else {
			// rezultat je jednak broju soketa koji su zadovoljili uslov
			
			return 1;
		}
	}


}





void RecievePublisherMessage(Client_information **client_info, Topic_node **head, SOCKET acceptedSocket, char* recieveBuffer, int clintId)
{
	int iResult = -1;
	



	while (true)
	{
		SelectFunction(acceptedSocket, READ);
		iResult = recv(acceptedSocket, recieveBuffer, sizeof(int)*2, 0); // pokupim informaciju o topicu i duzini poruke
	

		PublisherNode *node = (PublisherNode*)malloc(sizeof(PublisherNode));

		if (iResult > 0)
		{
			//printf("Message received from client: %s.\n", recieveBuffer);

			
			DeserializePublisherNode(node, recieveBuffer);


			int bytesRecieved = 0;
			int tempMessageLength = node->messageLength;

			do
			{
				iResult = recv(acceptedSocket, node->message + bytesRecieved, tempMessageLength - bytesRecieved, 0);
				if (iResult == SOCKET_ERROR || iResult == 0)
				{
					
					break;
				}
				bytesRecieved += iResult;

			} while (bytesRecieved < tempMessageLength);

			node->message[node->messageLength] = '\0';

			//printf("\n\n%d %d %s\n\n\n", node->topicId, node->messageLength, node->message);
			printf("\n\nPublisher sent: %s", node->message);
			printf("Forwarding message to subscribed clients..");


			char* topicName = FindTopicByIdTopicList(head, node->topicId);

			Node* clientsSubscribed =  GetDictionaryValue(topicName);   // return clients subsrcibed to specific topic

			//PrintClientList(&clientsSubscribed);


			Node* current = clientsSubscribed;
		

			while (current != NULL)
			{
				
				//printf("\n\nID Subsribera kojem saljem: %d", current->clientID);
				Client_information *clientSubsrbiedToTopic = FindElementClientInformation(client_info, current->clientID);
				

				if (clientSubsrbiedToTopic != NULL)
				{
					Enqueue(&(clientSubsrbiedToTopic->subscriberMessages), node->topicId, node->message);
					ReleaseSemaphore(clientSubsrbiedToTopic->queueSemaphore, 1, NULL);
				}

				current = current->next;
			}

			//PrintClientInformationList(client_info);



			/*
			
			Client_information *test = FindElementClientInformation(client_info, 0);

			char* poruka = Dequeue(&(test->subscriberMessages));

			printf("\n\nPORUKA KOJA JE DODATA NA QUEUE: %s\n\n", poruka);
			*/
			

			

			Sleep(1000); 

			
			free(node);
		}
		else if (iResult == 0)
		{
			// connection was closed gracefully
			iResult = shutdown(acceptedSocket, SD_SEND);
			if (iResult == SOCKET_ERROR)
			{
				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(acceptedSocket);
				WSACleanup();
				
			}


			
			printf("\n\nConnection with publisher closed.\n");
			closesocket(acceptedSocket);

			
			DeleteElementClientInformation(client_info, clintId);
			//PrintClientInformationList(client_info);
			
			free(node);

			
			break;
		}
		else
		{
			int ierr = WSAGetLastError();
			if (ierr == WSAEWOULDBLOCK) {  // currently no data available
				Sleep(100);  // wait and try again
				//printf("OCEKIVANO PONASANJE!");
				continue;
			}
			else
			{
				// there was an error during recv
				//printf("recv failed with error: %d\n", WSAGetLastError());
				printf("\n\nConnection with publisher closed.\n");
				closesocket(acceptedSocket);


				DeleteElementClientInformation(client_info, clintId);	
				//PrintClientInformationList(client_info);
				free(node);
				break;
			}
			
		}
	}

	Sleep(100);



	


} 

void RecieveSubsriberMessage(Client_information** client_info, Topic_node** head, SOCKET acceptedSocket, char * recieveBuffer, int clientId)
{
	int iResult = -1;
	int topicCounter = -1;
	int subsribedTopicCounter = -1;



	while (true)
	{
		SelectFunction(acceptedSocket, READ);
		iResult = recv(acceptedSocket, recieveBuffer, sizeof(int), 0); // pokupim informaciju o broj topica
		

		topicCounter = *(int*)recieveBuffer;

		if (iResult > 0)
		{
			//printf("Message received from client: %s.\n", recieveBuffer);


		


			int bytesRecieved = 0;
			int topicsLength = topicCounter *sizeof(int);

			char* topics = (char*)malloc(topicsLength);

			do
			{
				iResult = recv(acceptedSocket, topics + bytesRecieved, topicsLength - bytesRecieved, 0);
				if (iResult == SOCKET_ERROR || iResult == 0)
				{

					free(topics);
					
				}
				bytesRecieved += iResult;

			} while (bytesRecieved < topicsLength);

			
			//printf("\n\n%d\n\n\n", topicCounter);
			//printf("Subsriber subsribed to next topics:\n");

			int *topicsReal = (int*)topics;

			/*for (int i = 0; i < topicCounter; i++)
				printf("\t%d", topicsReal[i]);
				*/

			for (int i = 0; i < topicCounter; i++)
			{
				if (topicsReal[i] == 1) {
					char *topicName = FindTopicByIdTopicList(head, i);

					AddDictionaryElement(topicName, clientId);
				}
					
			}

			

			free(topics);

			break;
		


			

		}
		else if (iResult == 0)
		{
			// connection was closed gracefully
			iResult = shutdown(acceptedSocket, SD_SEND);
			if (iResult == SOCKET_ERROR)
			{

				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(acceptedSocket);
				WSACleanup();

			}

			
			closesocket(acceptedSocket);


			DeleteElementClientInformation(client_info, clientId);
			//PrintClientInformationList(client_info);
			printf("\nConnection with subscriber closed.\n");
			
			break;
		}
		else
		{
			int ierr = WSAGetLastError();
			if (ierr == WSAEWOULDBLOCK) {  // currently no data available
				Sleep(100);  // wait and try again
				//printf("OCEKIVANO PONASANJE!");
				continue;
			}
			else
			{
				// there was an error during recv
				//printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(acceptedSocket);
				DeleteElementClientInformation(client_info, clientId);
				//PrintClientInformationList(client_info);
				printf("\nConnection with subscriber closed.\n");
				
				break;
			}

		}

		Sleep(100);
	}

	
}

void RecieveServerMessage(Topic_node **head, SOCKET socket, char* recieveBuffer)
{
	int iResult = -1;


	int topicId = -1;
	int messageLength = -1;
	

	while (true)
	{
		SelectFunction(socket, READ);
		iResult = recv(socket, recieveBuffer, 2*sizeof(int), 0); // get information about topicId and messageLength
		

		topicId = *(int*)recieveBuffer;
		messageLength = *(int*)(recieveBuffer + sizeof(int));
		

		if (iResult > 0)
		{
			

			int bytesRecieved = 0;
			

			char *messageFromPublisher = (char*)malloc(sizeof(char)*messageLength);

			do
			{
				iResult = recv(socket, messageFromPublisher + bytesRecieved, messageLength - bytesRecieved, 0);
				if (iResult == SOCKET_ERROR || iResult == 0)
				{				
					
					break;
				}
				bytesRecieved += iResult;

			} while (bytesRecieved < messageLength);

			messageFromPublisher[messageLength-1] = '\0';


			/*printf("\n\n%d\n\n\n", topicCounter);
			printf("Subsriber subsribed to next topics:\n");

			int *topicsReal = (int*)topics;
			for (int i = 0; i < topicCounter; i++)
				printf("\t%d", topicsReal[i]);*/

		

			// dobavim niz id jeva topica

			char *topic = FindTopicByIdTopicList(head, topicId);

			printf("Publisher %s released new information: %s\n", topic, messageFromPublisher);

			free(messageFromPublisher);





		}
		else if (iResult == 0)
		{
			// connection was closed gracefully
			iResult = shutdown(socket, SD_SEND);
			if (iResult == SOCKET_ERROR)
			{
				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(socket);
				WSACleanup();

			}


			printf("\nConnection with server closed.\n");
			closesocket(socket);
			break;
		}
		else
		{
			int ierr = WSAGetLastError();
			if (ierr == WSAEWOULDBLOCK) {  // currently no data available
				Sleep(100);  // wait and try again
				//printf("OCEKIVANO PONASANJE!");
				continue;
			}
			else
			{
				// there was an error during recv
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(socket);
				break;
			}

		}

		Sleep(100);
	}


	
	
}


int RecieveInitialMessage(SOCKET socket, char* recieveBuffer)
{
	int iResult = -1;
	

	int clientType = -1;


	while (true)
	{
		SelectFunction(socket, READ);
		iResult = recv(socket, recieveBuffer, sizeof(int), 0); // pokupim informaciju o topicu 
		

		if (iResult > 0)
		{


			clientType = *(int*)recieveBuffer;


			if (clientType == 0)
				printf("\nNew Publisher connected!\n");
			else if (clientType == 1)
				printf("\nNew Subscriber connected!\n");
			else
				printf("Unknown client connected!\n");

						
			break;
			


			





		}
		else if (iResult == 0)
		{
			// connection was closed gracefully
			iResult = shutdown(socket, SD_SEND);
			if (iResult == SOCKET_ERROR)
			{
				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(socket);
				WSACleanup();

			}


			printf("\nConnection with client closed.\n");
			closesocket(socket);
			break;
		}
		else
		{
			int ierr = WSAGetLastError();
			if (ierr == WSAEWOULDBLOCK) {  // currently no data available
				Sleep(100);  // wait and try again
				//printf("OCEKIVANO PONASANJE!");
				continue;
			}
			else
			{
				// there was an error during recv
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(socket);
				break;
			}

		}



		Sleep(100);
	}

	return clientType;
}



void SendMessageToSubscriber(Client_information** client_info, Topic_node** topic_head, int clientId)
{
	

	Client_information *client = NULL;

	
	int iResult = -1;
	bool end = false;

	
	
	while(true)
	{

		char *buffer = (char*)malloc(sizeof(int));
		client = FindElementClientInformation(client_info, clientId);

		if (client == NULL)
		{
			free(buffer);
			//printf("Client with %d id does not exists in ClientInformationList", clientId);
			break;
		}

		WaitForSingleObject(client->queueSemaphore, INFINITE);





		int topicId = -1;

		int rw = SelectFunctionRW(client->clientSocket);

		if (rw == 0)
		{
			iResult = recv(client->clientSocket, buffer, sizeof(int), 0);
			free(buffer);

			iResult = shutdown(client->clientSocket, SD_SEND);
			if (iResult == SOCKET_ERROR)
			{
				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(client->clientSocket);
				//WSACleanup();

			}

			printf("\n\nConnection with subsriber closed.\n\n");
			closesocket(client->clientSocket);
			//WSACleanup();
			DeleteElementClientInformation(client_info, client->clientID);
			//PrintClientInformationList(client_info);
			end = true;
			
			break;
		}


		if (client->subscriberMessages != NULL && !IsEmptyQueue(client->subscriberMessages))
		{

			char *messageFromQueue = (char*)malloc(sizeof(char) * MAX_MESSAGE_LEN);
			char *messageToSend = (char*)malloc(sizeof(char) * MAX_MESSAGE_LENGTH);

			Dequeue(&(client->subscriberMessages), messageFromQueue, &topicId);
			//printf("\n\n\nOvo cu da saljem subsriberu: %d %d %s\n", client->clientID, topicId, message);

			int bytesSent = 0;
			int size = strlen(messageFromQueue) + 2 * sizeof(int);

			*(int*)messageToSend = topicId;
			*(int*)(messageToSend + 4) = strlen(messageFromQueue);
			memcpy((messageToSend + 2 * sizeof(int)), messageFromQueue, size);

			//topic id i duzinu poruke

			do {
				

				SelectFunction(client->clientSocket, WRITE);
				iResult = send(client->clientSocket, messageToSend + bytesSent, size - bytesSent, 0);


				if (iResult == SOCKET_ERROR || iResult == 0)
				{
					iResult = shutdown(client->clientSocket, SD_SEND);
					if (iResult == SOCKET_ERROR)
					{
						printf("shutdown failed with error: %d\n", WSAGetLastError());
						closesocket(client->clientSocket);
						//WSACleanup();

					}


					printf("\n\nConnection with subsriber closed.\n\n");
					closesocket(client->clientSocket);
					DeleteElementClientInformation(client_info, client->clientID);
					//PrintClientInformationList(client_info);
					//WSACleanup();
					end = true;
					break;
				}

				bytesSent += iResult;

			} while (bytesSent < size);

			printf("\nMessage sent to subsriber with id = %d", client->clientID);

			free(messageToSend);
			free(messageFromQueue);

			

			if (end)
				break;
			


		}


		free(buffer);
		Sleep(100);


	}





	

}



bool SendPublisherMessageToServer(SOCKET socket, char* messageToSend, int size)
{
	int bytesSent = 0;
	int iResult = -1;


	char *buffer = (char*)malloc(sizeof(int));
	
	



	do {
		

		int rw = SelectFunctionRW(socket);
		if (rw == 0)
		{
			//printf("\n\nServer poslao nesto!\n\n");
			iResult = recv(socket, buffer, sizeof(int),0);
			
			free(buffer);
			

		}
		else
		{
			//printf("\n\nRedovno slanje sa punlishera\n\n");
			iResult = send(socket, messageToSend + bytesSent, size - bytesSent, 0);
		}
		
		
		
		if (iResult == 0)
		{
			// connection was closed gracefully
			iResult = shutdown(socket, SD_SEND);
			if (iResult == SOCKET_ERROR)
			{
				printf("shutdown failed with error: %d\n", WSAGetLastError());
				closesocket(socket);
				WSACleanup();

			}


			printf("\n\nConnection with server closed.\n\n");
			closesocket(socket);
			WSACleanup();
			//break;

			return true;
		}
		else if (iResult < 0)
		{
			int ierr = WSAGetLastError();
			if (ierr == WSAEWOULDBLOCK) {  // currently no data available
				Sleep(100);  // wait and try again
				//printf("OCEKIVANO PONASANJE!");
				continue;
			}
			else
			{
				// there was an error during recv
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(socket);
				// break;

				return true;
			}

		}

		bytesSent += iResult;

	} while (bytesSent < size);


	printf("\n\nMessage sucessfully sent!\n\Bytes Sent: %ld\n", iResult);

	return false;
}

SOCKET AcceptNewSocket(SOCKET listenSocket)
{

	SOCKET acceptedSocket = accept(listenSocket, NULL, NULL);

	if (acceptedSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	return acceptedSocket;
}

bool PutSocketInNonblockingMode(SOCKET* listenSocket) {

	unsigned long mode = 1;

	if (ioctlsocket(*listenSocket, FIONBIO, &mode) == SOCKET_ERROR) {
		printf("ioctl failed with error: %d\n", WSAGetLastError());
		closesocket(*listenSocket);
		WSACleanup();
		return false;
	}

	printf("Socket is now in nonblocking mode.\n");
	return true;
}

char* SerializePublisherData(PublisherNode* publisherNode)
{
	int messageLength = strlen(publisherNode->message);

	int size = 2 * sizeof(int) + messageLength;
	char *buffer = (char*)malloc(sizeof(char) * (size + 1));

	
	*(int*)buffer = publisherNode->topicId;
	*(int*)(buffer + sizeof(int)) = messageLength;
	memcpy(buffer + sizeof(int) * 2, publisherNode->message, messageLength);
	buffer[size] = '\0';

	return buffer;
}

void DeserializePublisherNode(PublisherNode* publisherNode, char *buffer)
{

	
	publisherNode->topicId = *(buffer);

	memcpy(&(publisherNode->messageLength), buffer + sizeof(int), sizeof(int));


}


