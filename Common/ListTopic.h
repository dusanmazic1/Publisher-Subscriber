#ifndef LISTTOPIC_H_
#define LISTTOPIC_H_


#define MAX_TOPIC_LENGTH 256

typedef struct Topic_node {

	char topic[MAX_TOPIC_LENGTH];
	struct Topic_node* next;
}Topic_node;


void InsertFirstTopicList(Topic_node** head, char* newTopic);
void InsertEndTopicList(Topic_node** head, char* newTopic);
bool IsEmptyTopicList(Topic_node**);
struct Topic_node* FindElementTopicList(Topic_node** head, char* newTopic);
Topic_node* FindElementByIdTopicList(Topic_node** head, int id);
char* FindTopicByIdTopicList(Topic_node** head, int id); 
void PrintTopicList(Topic_node** head);
void PrintTopicListWithCounter(Topic_node** head);
void DeleteElementTopicList(Topic_node **head, char* newTopic);
void FreeTopicList(Topic_node **head);
void InitializeTopicListCriticalSection();



#endif