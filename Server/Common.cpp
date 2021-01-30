#include "Common.h"

#include <stdio.h>
#include <stdlib.h>


bool InitializeWindowsSockets()
{
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

void SelectFunction(SOCKET socket, SelectOption option)
{
	while (1)
	{
		// Initialize select parameters
		FD_SET set;
		timeval timeVal;

		FD_ZERO(&set);
		// Add socket we will wait to read from
		FD_SET(socket, &set);
		// Set timeouts to zero since we want select to return
		// instantaneously

		timeVal.tv_sec = 0;
		timeVal.tv_usec = 0;

		int iResult;

		if (option == READ)
			iResult = select(0 /* ignored */, &set, NULL, NULL, &timeVal);
		else if (option == WRITE)
			iResult = select(0 /* ignored */, NULL, &set, NULL, &timeVal);
		else
		{
			printf("\nUnknown SelectOption!\n");
			return;
		}


		if (iResult == 0) {
			// vreme za cekanje je isteklo
			Sleep(100);
			continue;
		}
		else if (iResult == SOCKET_ERROR) {
			//desila se greska prilikom poziva funkcije
			printf("\nError occured in select function..\n");
		}
		else {
			// rezultat je jednak broju soketa koji su zadovoljili uslov
			return;
		}
	}


	/*if (iResult < 0) {
		printf("Select failed with error: %s", WSAGetLastError());
	}
	else if (iResult == 0) {
		Sleep(100);
		continue;
	}*/
}

int RecieveMessage(SOCKET acceptedSocket, char* recieveBuffer, int DEFAULT_BUFLEN)
{
	int iResult = recv(acceptedSocket, recieveBuffer, DEFAULT_BUFLEN, 0);

	if (iResult > 0)
	{
		printf("Message received from client: %s.\n", recieveBuffer);
	}
	else if (iResult == 0)
	{
		// connection was closed gracefully
		printf("Connection with client closed.\n");
		closesocket(acceptedSocket);
	}
	else
	{
		// there was an error during recv
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(acceptedSocket);
	}

	return iResult;


}

SOCKET AcceptNewSocket(SOCKET listenSocket)
{

	SOCKET acceptedSocket = accept(listenSocket, NULL, NULL);

	if (acceptedSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	return acceptedSocket;
}

bool PutSocketInNonblockingMode(SOCKET listenSocket) {

	unsigned long mode = 1;

	if (ioctlsocket(listenSocket, FIONBIO, &mode) == SOCKET_ERROR) {
		printf("ioctl failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return false;
	}

	printf("Socket is now in nonblocking mode.");
	return true;
}






