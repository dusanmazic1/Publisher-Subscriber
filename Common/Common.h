#ifndef COMMON_H_
#define COMMON_H_

#define MAX_MESSAGE_LENGTH 512
#define DEFAULT_BUFLEN 512

#include "ListTopic.h"
#include "TopicRepository.h"
#include "Dictionary.h"
#include "ClientInformationList.h"

#include <ws2tcpip.h>

typedef struct PublisherNode {
	
	int topicId; // id topica za koji se salje nova poruka
	int messageLength; // duzina poruke, koristice se za proveru celosti poruke
	char message[MAX_MESSAGE_LENGTH]; // tekst poruka

}PublisherNode;

typedef enum SelectOption { READ, WRITE };  // enum used in SelectFunction to chech fdset

//bool InitializeWindowsSockets();

void SelectFunction(SOCKET socket, SelectOption option);

int SelectFunctionRW(SOCKET socket);

int SelectFunctionServer(SOCKET socket);


void RecievePublisherMessage(Client_information** client_info, Topic_node **head, SOCKET acceptedSocket, char* recieveBuffer, int clintId);

void RecieveSubsriberMessage(Client_information** client_info, Topic_node **head, SOCKET acceptedSocket, char * recieveBuffer, int clientId);

void RecieveServerMessage(Topic_node **head, SOCKET socket, char* recieveBuffer);

void SendMessageToSubscriber(Client_information** client_info, Topic_node** topic_head, int clientId);

bool SendPublisherMessageToServer(SOCKET socket, char* messageToSend, int size);

int RecieveInitialMessage(SOCKET socket, char* recieveBuffer);

SOCKET AcceptNewSocket(SOCKET listenSocket);

bool PutSocketInNonblockingMode(SOCKET* listenSocket);

char* SerializePublisherData(PublisherNode* publisherNode);  

void DeserializePublisherNode(PublisherNode *publisherNode, char *buffer); 




#endif
