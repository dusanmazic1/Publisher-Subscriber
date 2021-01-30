#ifndef COMMON_H_
#define COMMON_H_

#include <ws2tcpip.h>

typedef enum SelectOption { READ, WRITE };

bool InitializeWindowsSockets();

void SelectFunction(SOCKET socket, SelectOption option); 

int RecieveMessage(SOCKET acceptedSocket, char* recieveBuffer, int DEFAULT_BUFLEN);

SOCKET AcceptNewSocket(SOCKET listenSocket);

bool PutSocketInNonblockingMode(SOCKET listenSocket);







#endif
