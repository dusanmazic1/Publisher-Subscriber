#include "Helper.h"

#include <conio.h>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 


int generateRandomTopic(int numberOfTopics)
{
	return (rand() % numberOfTopics );
}

int generateRandomMessage(int numberOfMessages)
{
	return (rand() % numberOfMessages);
}

