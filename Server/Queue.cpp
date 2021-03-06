#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void InitializeQueue(Queue** queue)
{
	*queue = (Queue*)malloc(sizeof(Queue));
	 (int)(*queue)->count = 0;
	(*queue)->front = NULL;
	(*queue)->rear = NULL;
}

int IsEmptyQueue(Queue* queue)
{
	return queue->count == 0;
}

void Enqueue(Queue** queue, char* message)
{
	if ((*queue)->count < QUEUE_MAX_SIZE)
	{
		Queue_node* temp;
		temp = (Queue_node*)malloc(sizeof(Queue_node));
		temp->message = message;
		//strcpy(temp->message, message);
		temp->next = NULL;

		if (!IsEmptyQueue(*queue))
		{
			(*queue)->rear->next = temp;
			(*queue)->rear = temp;
		}
		else
		{
			(*queue)->front = temp;
			(*queue)->rear = temp;
		}
		(*queue)->count++;
	}
	else
	{
		printf("Queue is full\n");
	}
}

char* Dequeue(Queue** queue)
{
	Queue_node *temp;
	char *message = (*queue)->front->message;
	temp = (*queue)->front;
	(*queue)->front = (*queue)->front->next;
	(*queue)->count--;

	free(temp);
	return message;
}

