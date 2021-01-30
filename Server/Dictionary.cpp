

#include "Dictionary.h"
#include <stdio.h>
#include <stdlib.h>


unsigned HashFunction(char *key)
{
	unsigned hashValue;

	for (hashValue = 0; *key != '\0'; key++)
		hashValue = *key + 31 * hashValue;

	return hashValue % HASHSIZE;
}


// return clients subsrcibed to specific topic
Node* GetDictionaryValue(char *key)
{
	Key_value_pair* keyValuePair;
	keyValuePair = NULL;
	for (keyValuePair = hashTable[HashFunction(key)]; keyValuePair != NULL; keyValuePair = keyValuePair->next)
		if (strcmp(key, keyValuePair->topic) == 0)
			return keyValuePair->listAllClients;

	return NULL;
}



// return key value pair from dictionary
Key_value_pair* FindDictionaryElement(char *key)
{
	Key_value_pair* keyValuePair;

	for (keyValuePair = hashTable[HashFunction(key)]; keyValuePair != NULL; keyValuePair = keyValuePair->next)
		if (strcmp(key, keyValuePair->topic) == 0)
			return keyValuePair;

	return NULL;
}



// add element to dictionary 
Key_value_pair* AddDictionaryElement(char *topic, int clientId)
{
	Key_value_pair* keyValuePair;
	unsigned hashValue;

	if ((keyValuePair = FindDictionaryElement(topic)) == NULL)
	{
		keyValuePair = (Key_value_pair* )malloc(sizeof(*keyValuePair));

		/*
		if (keyValuePair == NULL || (keyValuePair->topic = strdup(topic)) == NULL)
			return NULL;
			*/
		if (keyValuePair == NULL || (keyValuePair->topic = topic) == NULL)
			return NULL;

		hashValue = HashFunction(topic);
		keyValuePair->next = hashTable[hashValue];
		keyValuePair->listAllClients = NULL;

		hashTable[hashValue] = keyValuePair;
	}

	if ((keyValuePair->listAllClients) == NULL)
		InsertFirstClientList(&(keyValuePair->listAllClients), clientId);
	else
		InsertEndClientList(&(keyValuePair->listAllClients), clientId);

	return keyValuePair;
}
