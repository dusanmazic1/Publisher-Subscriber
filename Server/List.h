#ifndef LIST_H_
#define LIST_H_

typedef struct Node {

	int clientID;
	struct  Node* next;
}Node;


void InsertFirstClientList( Node** head, int clientID);
void InsertEndClientList( Node** head, int clientID);
bool IsEmptyClientList(Node**);
struct Node* FindElementClientList( Node** head, int clientID);
void PrintClientList( Node** head);
void DeleteElementClientList(Node **head, int clientId);
void FreeClientList(Node **head);




#endif


