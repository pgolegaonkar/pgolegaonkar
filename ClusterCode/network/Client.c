//
//  TCPClient.c
/
//

#include<stdio.h>


#include "ClientServer.h"


int main(int argc , char *argv[])
{
    char * readFile =argv[1];
    char * writeFile=argv[2];
    readFile[strlen(readFile)-10]='\0';
    char *token;
    const char s[2] = "-";
    token = strtok(readFile, s);
    int i=0;
    int protocol = -1;
    double blockSize;
    int port = atoi(argv[3]);
    int noofThreads;
    char * IPAddress = argv[4];
    while( token != NULL ) {
        if(i == 1){
            if(strstr(token,"UDP") != NULL){
                protocol = 0;
            } else {
                protocol = 1;
            }
        } else if(i==2){
            blockSize = atoi(token) * 1.0;
        } else if(i==3){
            noofThreads = atoi(token);
        }
        i=i+1;
        token = strtok(NULL, s);
    }

    startClient(noofThreads,port,blockSize,protocol,writeFile, IPAddress);
}
