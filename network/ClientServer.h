//
//  ClientServer.h
//  

//

#ifndef ClientServer_h
#define ClientServer_h

#include <stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>

#endif /* ClientServer_h */

typedef struct {
    int port;
    double dataSize;
    double blockSize;
    int threadCount;
    char writeFile[100];
    char address[100];
} Data;
char *src;
double dataSize;
char *dest;
double blockSize;
int tcp;
//int threadCount;
char *writeFile;
void *connection_handler_client_tcp(void *arg);
void *connection_handler_server_tcp(void *socketID);
void *connection_handler_client_udp(void *arg);
int startClient(int noofThreads,int port,double blockSizeParam, int protocol, char *outFile, char * IPAddress);
void writeOutputFile(char * writeFile, double executionTime, double blockSize, char * operationType,int threadCount);
int startServer(int port, int blockSizeParam, int protocol, char * IPAddress);
