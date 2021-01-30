#ifndef QUEUE_H_
#define QUEUE_H_

#define QUEUE_MAX_SIZE 100

typedef struct Queue_node
{
	char* message;
	Queue_node* next;
}Queue_node;


typedef struct Queue
{
	int count;
	Queue_node* front;
	Queue_node* rear;

}Queue;


void InitializeQueue(Queue** queue);

int IsEmptyQueue(Queue* queue);

void Enqueue(Queue** queue, char* message);

char* Dequeue(Queue** queue);

#endif