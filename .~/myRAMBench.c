//
//  myRAMBenchmark.c
//
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define MAXCHAR 12
typedef struct {
    double size;
    double workload;
    double startPosition;
} Data;
struct RamOperations {
	double memorySize;
	char operationType[4];
	int numberOfThreads;
} op;

char *src, *dest;
void *readWrite(void *arg){
    Data* data = (Data*) arg;
    int count = (data->workload / data->size);
    for(int j=0;j<100;j++){
        for(int i = 0;i<count;i++)
        {
            long location = (i)*data->size + data->startPosition;
            memcpy(&dest[location],&src[location],data->size);
        }
    }
    return NULL;
}
void *sequentialWrite(void *arg){
    Data* data = (Data*) arg;
    int count = (data->workload / data->size);
    for(int j=0;j<100;j++){
        for(int i = 0;i<count;i++)
        {
            long location = (i)*data->size + data->startPosition;
            memcpy(&dest[location],&src[location],data->size);
        }
    }
    return NULL;
}
void *randomWrite(void *arg){
    Data* data = (Data*) arg;
    int count = (data->workload / data->size);
    for(int j=0;j<100;j++){
        for(int i = 0;i<count;i++)
        {
            long random = (double)(rand()/RAND_MAX) * ((1024 * 1024 * 1024)/data->size);
            memcpy(&dest[random],&src[random],data->size);
        }
    }
    return NULL;
}


void writeOutputFile(char * writeFile, double executionTime, double memorySize, char * operationType,int threadCount,
                     long fileSize){
    FILE *outFile;
    outFile=fopen(writeFile, "a");
    if(NULL==outFile){
        exit(-1);
    }else{
        double throughput=(double)(100 * fileSize)/executionTime;
        fprintf(outFile, "%s\t%d\t%f\t%f\t%f\t%d\n",operationType, threadCount,memorySize, throughput, 0.0, 100);
    }
    fclose(outFile);
}

void compute(double memorySize,char* operType, int numOfThreads, char * writeFile){
  clock_t start, end;
    int nuOfThreads=numOfThreads;
  pthread_t *tid = (pthread_t *)malloc(nuOfThreads * sizeof(pthread_t));
  double fileSize = (double)1024 * 1024 * 1024;
  dest = (char *)malloc(fileSize);
  src = (char *)malloc(fileSize);
  for(long j=0;j<(fileSize);j++){
      src[j] = 'a';
  }
  Data* data = (Data*)malloc(sizeof(Data));
  data->size = (double)memorySize;
  data->workload = (double)( fileSize )/nuOfThreads;
  char *operationType=operType;
    
  if(strstr(operationType,"RWR")!=NULL){
      
    start=clock();
    for(int i=0;i<nuOfThreads;i++){
        data->startPosition = i * (data->workload);
        pthread_create(&tid[i], NULL, randomWrite, data );
    }
    for(int i=0;i<nuOfThreads;i++){
        pthread_join(tid[i],NULL);
    }
    end=clock();
    double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf("\nRandom write execution time: %f",(double)(end-start)/CLOCKS_PER_SEC);
    printf("\nThrough put(bytes per second): %f\n",(double)(100 * fileSize)/((end-start)/CLOCKS_PER_SEC));
    writeOutputFile(writeFile,executionTime,memorySize,operationType,nuOfThreads,fileSize);
  }else if(strstr(operationType,"RWS")!=NULL){
      
    start=clock();
    for(int i=0;i<nuOfThreads;i++){
        data->startPosition = i * (data->workload);
        pthread_create(&tid[i], NULL, sequentialWrite, data );
    }
    for(int i=0;i<nuOfThreads;i++){
        pthread_join(tid[i],NULL);
    }
    end=clock();
    double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf("\nSequential write execution time: %f",(double)(end-start)/CLOCKS_PER_SEC);
    printf("\nThrough put(bytes per second): %f\n",(double)(100 * fileSize)/((end-start)/CLOCKS_PER_SEC));
    writeOutputFile(writeFile,executionTime,memorySize,operationType,nuOfThreads,fileSize);
  }
    
}

int main(int argc, char *argv[]){
    char * readFile =argv[1];
    char * writeFile=argv[2];
    FILE *file;
    file=fopen(readFile,"r");
    if(NULL!=file){
    	int i=0;
        char line[256];
        while (fgets(line, sizeof(line), file) && i < 3) {
            if(i==0){
                strncpy(op.operationType,line, strlen(line)-1);
            }
            else if(i==1){
                op.memorySize = atof(line);
            } else if(i==2){
                op.numberOfThreads = atoi(line);
            }
            i=i+1;
        }
        compute(op.memorySize,op.operationType,op.numberOfThreads, writeFile);
    }else{
        perror("Error in opening the file!");
    }
    fclose(file);
}
