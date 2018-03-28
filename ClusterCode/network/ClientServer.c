//
//  ClientServer.c
//

#include "ClientServer.h"

void writeOutputFile(char * writeFile, double executionTime, double blockSize, char * operationType,int threadCount){
    FILE *outFile;
    outFile=fopen(writeFile, "a");
    if(NULL==outFile){
        exit(-1);
    }else{
        if(blockSize == 1){
            double latency=(double)(executionTime/1000000) * 1000;
            fprintf(outFile, "%s\t%d\t%f\t%f\t%f\t%d\n",operationType, threadCount,blockSize, latency, 0.0, 100);
        } else {
            double throughput=(double)(100/executionTime)*8;
            fprintf(outFile, "%s\t%d\t%f\t%f\t%f\t%d\n",operationType, threadCount,blockSize, throughput, 0.0, 100);
        }
    }
    fclose(outFile);
}
void *connection_handler_server_tcp(void *socketID)
{
    
    int sock = *(int*)socketID;
    int n;
    
    char block[(int)blockSize+10];
    
    while(1)
    {
        n=recv(sock,block,blockSize,0);
        if(strstr(block,"Done") != NULL){
            close(sock);
            break;
        }
        /*strcat(dest,block);
        if(strlen(dest) >= dataSize){
            //printf("\n\n**********Reached limit so free the size : %d\n\n",i);
            dest[0]='\0';
            //dest = (char *)malloc(dataSize+10);
        }*/
    }
    printf("\n\nTerminated");
    printf("\nListening for connections....");
    return 0;
}

void *connection_handler_client_tcp(void *arg){
    Data* data = (Data*) arg;
    int port = data->port;
    int blockSize = data->blockSize;
    int socketReturn = -1;
    struct sockaddr_in server;
    int threadCount = data->threadCount;
    char writeFile[100];
    strncpy(writeFile,data->writeFile,strlen(data->writeFile));
    char clientAddress[100];
    strncpy(clientAddress,data->address,strlen(data->address));
    //printf("\n%s\n",writeFile);
    char protocol[4]="TCP";
    socketReturn = socket(AF_INET , SOCK_STREAM , 0);
    if (socketReturn == -1)
    {
        printf("Could not create socket");
    }
    printf("\n%s\n",clientAddress);
    struct hostent *host;
    if((host = gethostbyname(clientAddress)) == NULL){
        exit(1);
    }
    char *IPbuffer;
    IPbuffer = inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));
    printf("\n%s\n",IPbuffer);
    server.sin_addr.s_addr = inet_addr(IPbuffer);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
    if (connect(socketReturn , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    printf("\nConnected to server");
    clock_t start, end;
    int numOFTimes = 1;
    if(blockSize > 1){
     //   start=clock();
        numOFTimes = 100;
    }
    int numOfBlocks = data->dataSize/blockSize;
        int count;
        //printf("\n%d",numOFTimes);
        for(count=1;count<=numOFTimes;count++){
           // printf("\nCount : %d\n",count);
            //char *block = (char *)malloc(blockSize + 1);
            char block[(long long int)blockSize+10];
            for (int i = 0; i< numOfBlocks; i++)
            {
                strncpy(block,src,blockSize);
                    if( send(socketReturn , block , strlen(block) , 0) < 0)
                    {
                        break;
                    }
            }
        }
   /* if(blockSize > 1){
        end=clock();
        double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
        // printf("\nTime taken for execution:%f",executionTime);
        printf("\nThrough put: %f\n",(double)(100/executionTime)*8);
        writeOutputFile(writeFile, executionTime, blockSize, protocol,threadCount);
    }*/
            if( send(socketReturn , "Done" , strlen("Done") , 0) < 0)
            {
                return 0;
            }
            close(socketReturn);
    printf("\nConnection terminated\n\n");
}


void *connection_handler_client_udp(void *arg){
    Data* data = (Data*) arg;
    int port = data->port;
    int blockSize = data->blockSize;
    int socketReturn = -1;
    struct sockaddr_in server;
    int threadCount = data->threadCount;
    char protocol[4]="UDP";
//    strncpy(writeFile,data->writeFile,strlen(data->writeFile));
  //  char clientAddress[100];
  //  strncpy(clientAddress,data->address,strlen(data->address));
    //printf("\n%s\n",writeFile);
    printf("\nConnecting to handler\n");
    socketReturn = socket(AF_INET , SOCK_DGRAM , 0);
    if (socketReturn == -1)
    {
        printf("Could not create socket");
    }
//	printf("\nClient address : %s",clientAddress);
    char writeFile[100];
    strncpy(writeFile,data->writeFile,strlen(data->writeFile));
    char clientAddress[100];
    strncpy(clientAddress,data->address,strlen(data->address));
struct hostent *host;
    if((host = gethostbyname(clientAddress)) == NULL){
        exit(1);
    }
    char *IPbuffer;
IPbuffer = inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));
    printf("\n%s\n",IPbuffer);
    server.sin_addr.s_addr = inet_addr(IPbuffer);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
    clock_t start, end;
    int numOFTimes = 1;
    if(blockSize > 1){
       // start=clock();
        numOFTimes = 100;
    }
    int numOfBlocks = data->dataSize/blockSize;
    int count;
    //printf("\n%d",numOFTimes);
    for(count=1;count<=numOFTimes;count++){
        // printf("\nCount : %d\n",count);
        //char *block = (char *)malloc(blockSize + 1);
        char block[(long long int)blockSize+10];
        for (int i = 0; i< numOfBlocks; i++)
        {
            strncpy(block,src,blockSize);
            if (sendto( socketReturn, block , strlen(block), 0, (struct sockaddr *)&server, sizeof(server)) < 0 ) {
                perror( "sendto failed" );
                break;
            }
        }
    }
   /* if(blockSize > 1){
        end=clock();
        double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
        // printf("\nTime taken for execution:%f",executionTime);
        printf("\nThrough put: %f\n",(double)(100/executionTime)*8);
        writeOutputFile(writeFile, executionTime, blockSize, protocol,threadCount);
    }*/
    if( sendto(socketReturn , "Done" , strlen("Done") , 0,(struct sockaddr *)&server, sizeof(server)) < 0)
    {
        return 0;
    }
    close(socketReturn);
}


int startClient(int noofThreads,int port,double blockSizeParam,int protocol, char * outFile, char * IPAddress){
    tcp = protocol;
    char prot[4];
    blockSize = blockSizeParam;
    if(blockSize == 1){
        dataSize = (double)1024 * 1024;
    } else {
        dataSize = (double)1024 * 1024 * 1024;
    }

    src = (char *)malloc( sizeof(char) * (dataSize));
    memset(src, 0, dataSize);
    printf("\n%s\n",IPAddress);
    char writeFile[100];
    strncpy(writeFile,outFile,strlen(outFile));
    writeFile[strlen(outFile)]='\0';
    char clientAddress[100];
    strncpy(clientAddress,IPAddress,strlen(IPAddress));
    clientAddress[strlen(IPAddress)]='\0';
    printf("\n%s\n",clientAddress);
    //printf("\n%s\n",writeFile);
    if(tcp == 1){ // TCP program
        strncpy(prot,"TCP",3);
        //printf("\nTCP Client\n");
        //printf("\n\nDone Creating 1 gb of memory");
        pthread_t *pid = malloc(sizeof(*pid) * noofThreads);
        Data* data = (Data*)malloc(sizeof(Data));
        clock_t start, end;
            start=clock();
        for (int k = 0; k < noofThreads; ++k) {
            data[k].dataSize = dataSize / noofThreads;
            data[k].port = port;
            data[k].blockSize = blockSize;
            data[k].threadCount = noofThreads;
            strncpy(data[k].writeFile,writeFile,strlen(writeFile));
            strncpy(data[k].address, clientAddress, strlen(clientAddress));
            pthread_create(&pid[k], NULL, connection_handler_client_tcp, &data[k]);
        }
        
        for (int l = 0; l < noofThreads; ++l) {
            pthread_join(pid[l], NULL);
        }
        fflush(stdout);
	end=clock();
	double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
	printf("\nTime taken for execution: %f",executionTime);
        if(blockSize == 1){
//            printf("\nTime taken for execution:%f",executionTime);
            printf("\nLatency: %f\n",(double)(executionTime/1000000) * 1000);
//            writeOutputFile(writeFile, executionTime, blockSize, prot,noofThreads);
        } else {
		printf("\nThroughtput: %f\n",(double)(100/executionTime)*8);		
	}
	writeOutputFile(writeFile, executionTime, blockSize, prot, noofThreads);
    } else { // UDP program
        strncpy(prot,"UDP",3);
        printf("\nUDP Client\n");
        printf("\n\nDone Creating 1 gb of memory");
        pthread_t *pid = malloc(sizeof(*pid) * noofThreads);
        Data* data = (Data*)malloc(sizeof(Data));
        clock_t start, end;
            start=clock();
        for (int k = 0; k < noofThreads; ++k) {
            data[k].dataSize = dataSize / noofThreads;
            data[k].port = port;
            data[k].blockSize = blockSize;
data[k].threadCount = noofThreads;
strncpy(data[k].writeFile,writeFile,strlen(writeFile));
            strncpy(data[k].address, clientAddress, strlen(clientAddress));
            pthread_create(&pid[k], NULL, connection_handler_client_udp, &data[k]);
        }
        
        for (int l = 0; l < noofThreads; ++l) {
            pthread_join(pid[l], NULL);
        }
        fflush(stdout);
       /* if(blockSize == 1){
            end=clock();
            double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
            printf("\nTime taken for execution:%f",executionTime);
            printf("\nLatency: %f\n",(double)(executionTime/1000000) * 1000);
            writeOutputFile(writeFile, executionTime, blockSize, prot,noofThreads);
        }*/
end=clock();
double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
        printf("\nTime taken for execution: %f",executionTime);
        if(blockSize == 1){
//            printf("\nTime taken for execution:%f",executionTime);
            printf("\nLatency: %f\n",(double)(executionTime/1000000) * 1000);
//            writeOutputFile(writeFile, executionTime, blockSize, prot,noofThreads);
        } else {
                printf("\nThroughtput: %f\n",(double)(100/executionTime)*8);
        }
        writeOutputFile(writeFile, executionTime, blockSize, prot, noofThreads);
    }
    return 0;
}

int startServer(int port, int blockSizeParam, int protocol,  char * IPAddress){
    tcp = protocol;
    blockSize = blockSizeParam;
    if(blockSize == 1){
        dataSize = (double)1024 * 1024;
    } else {
        dataSize = (double)1024 * 1024 * 1024;
    }
    if(tcp == 1){ //TCP program
        printf("\nTCP Server\n");
        int socket_desc , client_sock , c , *new_sock;
        struct sockaddr_in server , client;
        //printf("\nServer port: %d",port);
        //Create socket
        socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        if (socket_desc == -1)
        {
            printf("Could not create socket");
        }
        //printf("\nSocket created");
        
        //Prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons( port );
        
        //Bind
        if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
        {
            //print the error message
            perror("bind failed. Error");
            return 1;
        }
        //printf("\nbind done");
        
        //Listen
        listen(socket_desc , 3);
        
        //Accept and incoming connection
        printf("\nWaiting for incoming connections...");
        c = sizeof(struct sockaddr_in);
        
        c=sizeof(struct sockaddr_in);
        while(client_sock=accept(socket_desc,(struct sockaddr*)&client,(socklen_t*)&c))
        {
            puts("Connection accepted");
            
            pthread_t sniffer_thread;
            new_sock = new_sock = malloc(sizeof *new_sock);
            *new_sock = client_sock;
            
            if( pthread_create( &sniffer_thread , NULL ,  connection_handler_server_tcp , (void*) new_sock) < 0)
            {
                perror("could not create thread");
                return 1;
            }
            
            puts("Handler assigned");
        }
        
        if (client_sock < 0)
        {
            perror("accept failed");
            return 1;
        }
    } else { // UDP program
        printf("\nUDP Server\n");
        
        int socket_desc , client_sock , c , *new_sock;
        struct sockaddr_in server , client;
        //printf("\nServer port: %d",port);
        //Create socket
        socket_desc = socket(AF_INET , SOCK_DGRAM , 0);
        if (socket_desc == -1)
        {
            printf("Could not create socket");
        }
        //printf("\nSocket created");
        
        //Prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons( port );
        
        //Bind
        if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
        {
            //print the error message
            perror("bind failed. Error");
            return 1;
        }
        int n;
        char block[(int)blockSize+10];
        while(1)
        {
            n=recvfrom(socket_desc,block,blockSize,0,NULL,0);
            if(strstr(block,"Done") != NULL){
                close(socket_desc);
                break;
            }
            /*strcat(dest,block);
             if(strlen(dest) >= dataSize){
             //printf("\n\n**********Reached limit so free the size : %d\n\n",i);
             dest[0]='\0';
             //dest = (char *)malloc(dataSize+10);
             }*/
        }
        printf("\n\nTerminated");
    }
    return 0;

}
