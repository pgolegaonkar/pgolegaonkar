//
//  myRAMBenchmark
//
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 10737418240
typedef struct {
    double size;
    double workload;
    double startPosition;
} Data;

struct DiskOperations {
    double blockSize;
    char operationType[3];
    int numberOfThreads;
} op;

char* src;

void *sequentialWrite(void *arg){

    Data* data = (Data*) arg;
    int count = (data->workload / data->size);
    FILE *file = fopen("/tmp/write_output.txt","w+");

    for(int i = 0;i<count;i++)
    {
        fwrite("a",1,data->size,file);
    }
    //remove("write_output.txt");
    fclose(file);
    return NULL;
}

void *randomWrite(void *arg){
    Data* data = (Data*) arg;
    int count = (data->workload / data->size);
    FILE *file = fopen("/tmp/write_output.txt","w+");

    for(int i = 0;i<count;i++)
    {
        double random = (double)(rand()/RAND_MAX) * (double)((double)(10 * 1000 * 1000 * 1000)/data->size);
        fseek(file,random,SEEK_SET);
        fwrite("a",1,data->size,file);
    }
    fclose(file);
    return NULL;
}

void *sequentialRead(void *arg){
    Data* data = (Data*) arg;
    int count = (data->workload / data->size);
    FILE *file = fopen("/tmp/read_output.txt","r+");
    for(int i = 0;i<count;i++)
    {
        fread(src,1,data->size,file);
    }
    fclose(file);
    return NULL;
}
void *randomRead(void *arg){
    Data* data = (Data*) arg;
    int count = (data->workload / data->size);
    FILE *file = fopen("/tmp/read_output.txt","r+");
    for(int i = 0;i<count;i++)
    {
        double random = (double)(rand()/RAND_MAX) * (double)((double)(10 * 1000 * 1000 * 1000)/data->size);
        fseek(file,random,SEEK_SET);
        fread(src,1,data->size,file);
    }
    fclose(file);
    return NULL;
}

void writeOutputFile(char * writeFile, double executionTime, double memorySize, char * operationType,int threadCount,
                     long fileSize){
    FILE *outFile;
    outFile=fopen(writeFile, "a");
    if(NULL==outFile){
        exit(-1);
    }else{
        double throughput=(double)(fileSize)/executionTime;
        fprintf(outFile, "%s\t%d\t%f\t%f\t%f\t%d\n",operationType, threadCount,memorySize, throughput, 0.0, 100);
    }
    fclose(outFile);
}

void compute(double blockSize,char* operType, int numOfThreads, char * writeFile){
    clock_t start, end;
    pthread_t *tid = (pthread_t *)malloc(numOfThreads * sizeof(pthread_t));
    double fileSize;
    if(blockSize==1024){
      fileSize = (double)10 * 1000 * 1000;
    }else{
       fileSize = (double)10 * 1000 * 1000 * 1000;
    }

    Data* data = (Data*)malloc(sizeof(Data));
    data->size = (double)(blockSize);
    data->workload = (double)( fileSize /numOfThreads);
    char *operationType=operType;
    src = (char *)malloc(blockSize);
    if(strstr(operationType,"WR")!=NULL){
        start=clock();
        for(int i=0;i<numOfThreads;i++){
            data->startPosition = i * (data->workload);
            pthread_create(&tid[i], NULL, randomWrite, data );
        }
        for(int i=0;i<numOfThreads;i++){
            pthread_join(tid[i],NULL);
        }
        end=clock();
        double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
        if(blockSize == 1024){
          //executionTime is in seconds, we  need in milliseconds. so latency = executionTime * 1000 / (number of operations). Here number of operations will be 1000000. Therefore, latency is given as below.
          double latency = (double)executionTime / 1000;
        }
        printf("\nRandom write execution time: %f",(double)(end-start)/CLOCKS_PER_SEC);
        printf("\nThrough put(bytes per second): %f\n",(double)(fileSize)/((end-start)/CLOCKS_PER_SEC));
        writeOutputFile(writeFile,executionTime,blockSize,operationType,numOfThreads,fileSize);
    }else if(strstr(operationType,"WS")!=NULL){
        start=clock();
        for(int i=0;i<numOfThreads;i++){
            data->startPosition = i * (data->workload);
            pthread_create(&tid[i], NULL, sequentialWrite, data );
        }
        for(int i=0;i<numOfThreads;i++){
            pthread_join(tid[i],NULL);
        }
        end=clock();
        double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
        printf("\nSequential write execution time: %f",(double)(end-start)/CLOCKS_PER_SEC);
        printf("\nThrough put(bytes per second): %f\n",(double)(fileSize)/((end-start)/CLOCKS_PER_SEC));
        writeOutputFile(writeFile,executionTime,blockSize,operationType,numOfThreads,fileSize);
    }else if(strstr(operationType,"RS")!=NULL){
        start=clock();
        for(int i=0;i<numOfThreads;i++){
            data->startPosition = i * (data->workload);
            pthread_create(&tid[i], NULL, sequentialRead, data );
        }
        for(int i=0;i<numOfThreads;i++){
            pthread_join(tid[i],NULL);
        }
        end=clock();
        double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
        printf("\nSequential read execution time: %f",(double)(end-start)/CLOCKS_PER_SEC);
        printf("\nThrough put(bytes per second): %f\n",(double)(fileSize)/((end-start)/CLOCKS_PER_SEC));
        writeOutputFile(writeFile,executionTime,blockSize,operationType,numOfThreads,fileSize);
    }else if(strstr(operationType,"RR")!=NULL){
        start=clock();
        for(int i=0;i<numOfThreads;i++){
            data->startPosition = i * (data->workload);
            pthread_create(&tid[i], NULL, randomRead, data );
        }
        for(int i=0;i<numOfThreads;i++){
            pthread_join(tid[i],NULL);
        }
        end=clock();
        double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
        if(blockSize == 1024){
          //executionTime is in seconds, we  need in milliseconds. so latency = executionTime * 1000 / (number of operations). Here number of operations will be 1000000. Therefore, latency is given as below.
          double latency = (double)executionTime / 1000;
        }
        printf("\nRandom read execution time: %f",(double)(end-start)/CLOCKS_PER_SEC);
        printf("\nThrough put(bytes per second): %f\n",(double)(fileSize)/((end-start)/CLOCKS_PER_SEC));
        writeOutputFile(writeFile,executionTime,blockSize,operationType,numOfThreads,fileSize);
    }

}


int main(int argc, char *argv[]){
    char * readFile =argv[1];
    char * writeFile=argv[2];
    readFile[strlen(readFile)-10]='\0';
    char *token;
    const char s[2] = "-";
    token = strtok(readFile, s);
    int i=0;
    while( token != NULL ) {
        if(i == 1){
            strncpy(op.operationType,token, strlen(token));
        } else if(i==2){
            op.blockSize = atof(token);
            op.blockSize = op.blockSize * 1024.0;
        } else if(i==3){
            op.numberOfThreads = atoi(token);
        }
        i=i+1;
        token = strtok(NULL, s);
    }
    printf("\n%s,%f,%d\n",op.operationType,op.blockSize,op.numberOfThreads);
    if(strstr(op.operationType,"RS")!=NULL || strstr(op.operationType,"RR")!=NULL){
        long long int fileSize = BUFFERSIZE;
        char block[] = { "abcdefghijklmnopqrstuvwxyz" };
        FILE *fl = fopen("/tmp/read_output.txt","w+");
        for (long long int i = 0; i < (fileSize / 26); i++) {
            fprintf(fl, "%s", block);
        }
        fclose(fl);
    }
    compute(op.blockSize,op.operationType,op.numberOfThreads, writeFile);
    /*
	if(strstr(op.operationType,"WS")!=NULL || strstr(op.operationType,"WR")!=NULL){
        remove("write_output.txt");
    }
	*/
    //remove("read_output.txt");
}
