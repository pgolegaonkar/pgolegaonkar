#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <immintrin.h>
#define MAXCHAR 12
typedef struct {
    long i;

} Data;
struct Operations {
	char precisionOp[2];
	char numberOfThreads[1];
} op;

void *quarterPrecision(void *arg){
    Data* data = (Data*) arg;
    __m256i small = _mm256_set_epi8('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h','a', 'b', 'c', 'd', 'e', 'f', 'g', 'h','a', 'b', 'c', 'd', 'e', 'f', 'g', 'h','a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
    __m256i capital = _mm256_set_epi8('H', 'G', 'F', 'E', 'D', 'C', 'B', 'A','H', 'G', 'F', 'E', 'D', 'C', 'B', 'A','H', 'G', 'F', 'E', 'D', 'C', 'B', 'A','H', 'G', 'F', 'E', 'D', 'C', 'B', 'A');
    for(long i=0;i<data->i/128;i++)
    {
        __m256i result = _mm256_add_epi8(small, capital);
    }
    return NULL;
}
void *doublePrecision(void *arg){
    Data* data = (Data*) arg;
    __m256d evens = _mm256_set_pd(2.0, 4.0, 6.0, 8.0);
    __m256d odds = _mm256_set_pd(1.0, 3.0, 5.0, 7.0);
    for(long i=0;i<data->i/32;i++)
    {
        __m256d result = _mm256_sub_pd(evens, odds);
    }
    return NULL;
}
void *halfPrecision(void *arg){
    Data* data = (Data*) arg;
    __m256i evens = _mm256_set_epi16(2, 4, 6, 8, 10, 12, 14, 16,2, 4, 6, 8, 10, 12, 14, 16);
    __m256i odds = _mm256_set_epi16(1, 3, 5, 7, 9, 11, 13, 15,1, 3, 5, 7, 9, 11, 13, 15);
    for(long i=0;i<data->i/32;i++)
    {
        __m256i result = _mm256_sub_epi16(evens, odds);
    }
    return NULL;
}
void *singlePrecision(void *arg){
    Data* data = (Data*) arg;
    for(long i=0;i<data->i/32;i++)
    {
        __m256 evens = _mm256_set_ps(2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0);
        __m256 odds = _mm256_set_ps(1.0, 3.0, 5.0, 7.0, 9.0, 11.0, 13.0, 15.0);
        __m256 result = _mm256_sub_ps(evens, odds);
    }
    return NULL;
}
void writeOutputFile(char * writeFile, double executionTime, char * precision, int threadCount,double opIPS){

    FILE *outFile;
    outFile=fopen(writeFile, "a");
    if(NULL==outFile){
        exit(-1);
    }else{
        precision[2]='\0';
        double gigaOps=(double)((1000000)/executionTime)/1000000000;
	double theoreticalValue=(double)(2.3*1*16*opIPS);
	double efficiency=(double)(gigaOps/theoreticalValue)*100;
        fprintf(outFile, "%s\t%d\t%f\t%f\t%f\n", precision, threadCount,
                gigaOps, theoreticalValue, efficiency);
        printf("\nDouble Precision execution time: %f",executionTime);
        printf("\nNumber of operations per second: %f",(double)(1000000000)/executionTime);
        printf("\nNumber of GigaOPS per second: %f\n\n",(double)((1000000000)/executionTime)/1000000000);
    }
    fclose(outFile);
}
void compute(char* precisionOp, char* noOfThreads, char * writeFile){
clock_t start, end;
int threadCount=atoi(noOfThreads);
pthread_t *tid = (pthread_t *)malloc(threadCount * sizeof(pthread_t));
Data* data = (Data*)malloc(sizeof(Data));
data->i = (long)1000000000/threadCount;
//printf("\n in Compute, %c and %s",precisionOp[0], noOfThreads);
char *precision=precisionOp;
if(precision[0]=='S'){
	start=clock();
    for(int i=0;i<threadCount;i++){
        pthread_create(&tid[i], NULL, singlePrecision, data );
    }
    for(int i=0;i<threadCount;i++){
        pthread_join(tid[i],NULL);
    }
    end=clock();
    double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
    writeOutputFile(writeFile,executionTime,precision,threadCount,16);

}else if(precision[0]=='Q'){
start=clock();
    for(int i=0;i<threadCount;i++){
    pthread_create(&tid[i], NULL, quarterPrecision, data );
    }
    for(int i=0;i<threadCount;i++){
        pthread_join(tid[i],NULL);
    }
    end=clock();
    double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
    writeOutputFile(writeFile,executionTime,precision,threadCount,64);
}else if(precision[0]=='D'){
start=clock();
    for(int i=0;i<threadCount;i++){
        pthread_create(&tid[i], NULL, doublePrecision, data );
    }
    for(int i=0;i<threadCount;i++){
        pthread_join(tid[i],NULL);
    }
    end=clock();
    double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
    writeOutputFile(writeFile,executionTime,precision,threadCount,8);
}else if(precision[0]=='H'){
start=clock();
    for(int i=0;i<threadCount;i++){
        pthread_create(&tid[i], NULL, halfPrecision, data );
    }
    for(int i=0;i<threadCount;i++){
        pthread_join(tid[i],NULL);
    }
    end=clock();
    double executionTime = (double)(end-start)/CLOCKS_PER_SEC;
    writeOutputFile(writeFile,executionTime,precision,threadCount,32);
}
}

int main(int argc, char **argv){

char * readFile =argv[1];
char * writeFile=argv[2];
readFile[strlen(readFile)-10]='\0';
char *token;
const char s[2]="_";
token = strtok(readFile,s);
int i=0;
while(token!=NULL){
if(i==1){
strncpy(op.precisionOp,token, strlen(token));
} else if(i==2){
strncpy(op.numberOfThreads, token, strlen(token));
}
i=i+1;
token=strtok(NULL, s);
}
//printf("\n%s,%s\n",op.precisionOp, op.numberOfThreads);
compute(op.precisionOp,op.numberOfThreads, writeFile);
}
