#ifndef SUBSCRIBERHELPER_H_
#define SUBSCRIBERHELPER_H_

//#include "../Common/ListTopic.h"
//#include "../Common/TopicRepository.h"
#include "../Common/Common.h"



//#define DEFAULT_BUFLEN 256

typedef struct Important_data {

	Topic_node **head;
	SOCKET connectSocket;

}Important_data;



void PrintSubsriberMenu(Topic_node** head, int counter);  // print menu

int SubscribeToSpecificTopic(int topicCounter, int* subscribedTopics); // select topics to subsribe

bool ValidateSubscriberInput(int selectedOption, int topicCounter); // validate user input

bool SubscribedToAllTopics(int *subscribedTopics, int topicCounter); // check if user already subsribed to all topics

void PrintSubsribedTopics(Topic_node** head, int *subscribedTopics, int topicCounter); // print subsribed topics

//void ThreadRecieveFunction(Topic_node **head, SOCKET connectSocket, char* recvbuf);

DWORD WINAPI ThreadRecieveFunction(LPVOID lpvThreadParam);





#endif