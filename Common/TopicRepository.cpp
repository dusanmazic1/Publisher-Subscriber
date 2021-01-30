#include "pch.h"
#include "TopicRepository.h"




FILE *SafeOpen(char filename[], char mode[]) {



	FILE *fp = fopen(filename, mode);

	if (fp == NULL) {

		printf("Can't open '%s'!", filename);
		char c = getchar();
		exit(-1);
	}
	return fp;
}

void ReadAllTopics(FILE *in, Topic_node** head, int* elementCounter)
{

	*elementCounter = 0;

	
	
	char newTopic[MAX_TOPIC_LENGTH];

	while (fscanf(in, "%s", newTopic) != EOF) {


		
		InsertEndTopicList(head, newTopic);
		
		(*elementCounter)++;
	}
}
