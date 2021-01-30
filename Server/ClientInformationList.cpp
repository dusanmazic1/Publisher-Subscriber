#include "ClientInformationList.h"



void InsertFirstClientInformation(Client_information** head, int clientID, SOCKET clientSocket, DWORD lpThreadId, HANDLE handleClientThread) {

	Client_information* newElement = (Client_information*)malloc(sizeof(Client_information));

	if (*head == NULL)
	{
		newElement->clientID = clientID;
		newElement->clientSocket = clientSocket;
		newElement->lpThreadId = lpThreadId;
		newElement->handleClientThread = handleClientThread;
		newElement->next = NULL;

		*head = newElement;
	}
	else
	{
		newElement->clientID = clientID;
		newElement->clientSocket = clientSocket;
		newElement->lpThreadId = lpThreadId;
		newElement->handleClientThread = handleClientThread;
		newElement->next = *head;

		*head = newElement;
	}


}

void InsertEndClientInformation(Client_information** head, int clientID, SOCKET clientSocket, DWORD lpThreadId, HANDLE handleClientThread)
{
	Client_information* current = *head;
	Client_information* newElement = (Client_information*)malloc(sizeof(Client_information));

	if (*head == NULL)
	{
		newElement->clientID = clientID;
		newElement->clientSocket = clientSocket;
		newElement->lpThreadId = lpThreadId;
		newElement->handleClientThread = handleClientThread;
		newElement->next = NULL;

		*head = newElement;

		return;
	}

	while (current->next != NULL)
	{
		current = current->next;
	}

	newElement->clientID = clientID;
	newElement->clientSocket = clientSocket;
	newElement->lpThreadId = lpThreadId;
	newElement->handleClientThread = handleClientThread;
	newElement->next = NULL;

	current->next = newElement;

}


bool IsEmptyClientInformation(Client_information** head)
{
	return *head == NULL ? true : false;
}


struct Client_information* FindElementClientInformation(Client_information** head, int clientID)
{
	Client_information* current = *head;
	Client_information* newElement = (Client_information*)malloc(sizeof(Client_information));

	if (current == NULL)
		return NULL;

	while (current != NULL)
	{
		if (current->clientID == clientID)
			return current;

		current = current->next;
	}

	return NULL;

}


void PrintClientInformationList(Client_information** head) {

	Client_information* current = *head;

	if (current == NULL)
	{
		printf("List is empty");
		return;
	}

	while (current != NULL)
	{

		printf("\n\nClientInfomrationElement\n");
		printf("ClientID: %d\n", current->clientID);
		printf("lpThreadId: %d\n", current->lpThreadId);
		
		if(current->handleClientThread != NULL)
			printf("handleClientThread have assigned value. Thread succesfully created!");
		else
			printf("handleClientThread does not have assigned value. Thread unsuccesfully created!");

		current = current->next;
	}

}

void DeleteElementClientInformation(Client_information **head, int clientId)
{
	
	struct Client_information* temp = *head, *previous;

	if (temp != NULL && temp->clientID == clientId)
	{
		*head = temp->next;   
		free(temp);               
		return;
	}

	
	while (temp != NULL && temp->clientID != clientId)
	{
		previous = temp;
		temp = temp->next;
	}

	
	if (temp == NULL) return;

	
	previous->next = temp->next;

	free(temp);  
}

void FreeClientInformationList(Client_information **head)
{
	Client_information* current = *head;

	while (current != NULL)
	{
		Client_information* temp = current;
		current = current->next;

		free(temp);

	}

	*head = NULL;
}

