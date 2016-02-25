#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void* Prime(void* vars);

// Struct that is passed in to the creation of each thread
typedef struct threadVars{
  int primeRange;
  int threadNum;
}threadVars;

// Initializes the threads 
// Creates a timer at the creation of the first thread and end at the last thread finish
int main (int argc, char* argv[]){
	struct timeval t1, t2;
	double elapsedTime;
	int totalThreads, totalPrime, i, j;
	pthread_t thread[atoi(argv[1])];
	threadVars *threadVars;

	//command line argument verification
	if(argc != 3){
	  printf("Invalid Arguments");
	  return 0;
	}

	totalThreads = atoi(argv[1]);
	totalPrime = atoi(argv[2]);

	//write elapsed time to file
	FILE *TimeLog = fopen("ThreadTimes.txt", "a+");
	if (TimeLog == NULL){
	  printf("Error opening file!\n");
	  exit(1);
	}

	//timer started 
	gettimeofday(&t1, NULL);

	//Create threads
	for(i = 0; i < totalThreads; i++){
		threadVars = malloc(sizeof (struct threadVars));
		threadVars->primeRange = totalPrime;
		threadVars->threadNum = i;
		pthread_create (&thread[i], NULL, Prime, threadVars);
	}

	//wait for all threads to end before continuing 
	for(j = 0; j < totalThreads; j++){
		pthread_join (thread[j], NULL);
	}

	//end timer
	gettimeofday(&t2, NULL);

	// calculate the elapsed time and print it to the log file and stdout
	elapsedTime = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec)/1000000.0);
	printf("%lf\n", elapsedTime);
	fprintf(TimeLog, "TOTALTHREADS:%d || totalPrime:%d || ELAPSEDTIME:%lf\n", totalThreads, totalPrime, elapsedTime);
	fclose(TimeLog);

	return 0;
}

// Write all prime numbers to file
void* Prime(void* vars){
    int count, c, halfCount;
    char fileName[50];
    threadVars *threadVars = vars;
	
	struct timeval tv;
	struct tm* ptm;
	char time_string[40];
	long milliseconds;

    sprintf(fileName, "%d", threadVars->threadNum);
    strcat(fileName, "-Thread_Log.txt");

    //Create log file 
    FILE *LogFile = fopen(fileName, "w");

    if (LogFile == NULL){
		printf("Error opening file!\n");
		exit(1);
    }

    //Find all prime numbers
    for ( count = 3 ; count <= threadVars->primeRange; count++){
        halfCount = count / 2;

        for ( c = 2 ; c < halfCount ; c++ ){
            if ( count%c == 0 )
                break;
        }
        if ( c >= halfCount ){
			gettimeofday (&tv, NULL);
			ptm = localtime (&tv.tv_sec);
			/* Format the date and time, down to a single second. */
			strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
			/* Compute milliseconds from microseconds. */
			milliseconds = tv.tv_usec / 1000;
            fprintf(LogFile, "%s.%03ld - %d\n",time_string, milliseconds,count);
        }
    }
    fclose(LogFile);
    printf("FINISHED THREAD NUMBER: %d\n", threadVars->threadNum);
	free(threadVars);

	return 0;
}
