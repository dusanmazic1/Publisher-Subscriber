
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

void InsertFirstClientList(Node** head, int clientID) {

    Node* newElement = ( Node*)malloc(sizeof(Node));

	if (*head == NULL)
	{
		newElement->clientID = clientID;
		newElement->next = NULL;
		*head = newElement;
	}
	else
	{
		newElement->clientID = clientID;
		newElement->next = *head;
		*head = newElement;
	}


}

void InsertEndClientList( Node** head, int clientID)
{
	Node* current = *head;
	Node* newElement = (Node*)malloc(sizeof( Node));

	while (current->next != NULL)
	{
		current = current->next;
	}

	newElement->clientID = clientID;
	newElement->next = NULL;
	current->next = newElement;

}


bool IsEmptyClientList( Node** head)
{
	return *head == NULL ? true : false;
}


struct Node* FindElementClientList(struct Node** head, int clientID)
{
	Node* current = *head;
	Node* newElement = ( Node*)malloc(sizeof( Node));

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


void PrintClientList(Node** head) {

	Node* current = *head;

	if (current == NULL)
	{
		printf("List is empty");
		return;
	}

	while (current != NULL)
	{

		printf("ClientID: %d\n", current->clientID);

		current = current->next;
	}

}

void DeleteElementClientList(Node **head, int clientId)
{
	// Store head node 
	struct Node* temp = *head, *previous;

	// If head node itself holds the key to be deleted 
	if (temp != NULL && temp->clientID == clientId)
	{
		*head = temp->next;   // Changed head 
		free(temp);               // free old head 
		return;
	}

	// Search for the key to be deleted, keep track of the 
	// previous node as we need to change 'prev->next' 
	while (temp != NULL && temp->clientID != clientId)
	{
		previous = temp;
		temp = temp->next;
	}

	// If key was not present in linked list 
	if (temp == NULL) return;

	// Unlink the node from linked list 
	previous->next = temp->next;

	free(temp);  // Free memory 
}

void FreeClientList(Node **head)
{
	Node* current = *head;

	while (current != NULL)
	{
		Node* temp = current;
		current = current->next;

		free(temp);

	}

	*head = NULL;
}

