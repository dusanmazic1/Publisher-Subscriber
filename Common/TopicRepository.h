#ifndef TOPICREPOSITORY_H_
#define TOPICREPOSITORY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ListTopic.h"

FILE *SafeOpen(char filename[], char mode[]);
void ReadAllTopics(FILE *in, Topic_node** head, int* elementCounter);


#endif
