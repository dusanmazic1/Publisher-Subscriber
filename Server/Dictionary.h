#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"



// element of hash table
typedef struct Key_value_pair {

	char *topic;
	Node *listAllClients;
	struct Key_value_pair *next;

}Key_value_pair;




#define HASHSIZE 101
static Key_value_pair* hashTable[HASHSIZE];






unsigned HashFunction(char* key);      // create hash value
Node* GetDictionaryValue(char* key);   // return clients subsrcibed to specific topic
Key_value_pair* FindDictionaryElement(char* key);  // return key value pair from dictionary
Key_value_pair* AddDictionaryElement(char* topic, int clientId);   // add element to dictionary 
 















































#endif


