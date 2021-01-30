#ifndef PUBLISHERHELPER_H_
#define PUBLISHERHELPER_H_

#include "../Common/ListTopic.h"
#include "../Common/TopicRepository.h"
#include "../Common/Common.h"



void PrintPublisherMenu(Topic_node** head);
int SelectSpecificTopic(int topicCounter);
bool ValidatePublisherInput(int selectedOption, int topicCounter);
//char* EnterMessage(char* newMessage);
void EnterMessage(char* newMessage);



#endif
