#include <stdio.h>
#include <time.h>
#include <sys/wait.h>

#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>


void Prime(int PrimeRange, int processNum);

// Initializes the threads 
// Creates a timer at the creation of the first thread and end at the last thread finish
int main (int argc, char *argv[]) 
{
	pid_t childpid = 0; 
    struct timeval t1, t2;
    double elapsedTime;
    int totalProcesses, totalPrime, i;

    //command line argument verification
    if(argc != 3){
		printf("Invalid Arguments");
        return 0;
    }
  
    totalProcesses = atoi(argv[1]);
    totalPrime = atoi(argv[2]);

    //write elapsed time to file
    FILE *TimeLog = fopen("ProcessTimes.txt", "a+");

    if (TimeLog == NULL){
        printf("Error opening file!\n");
        exit(1);
    }
    
    //timer started 
    gettimeofday(&t1, NULL);

    //Fork new processes 
   	for (i = 1; i < totalProcesses; i++)
            //children break out of the loop
      		if ((childpid = fork()) <= 0)
         		break;
			
    Prime(totalPrime, i);
	
    // if the process is the parent
    if (childpid > 0){
        while (waitpid(-1, NULL, 0)){
            // if no children left
            if (errno == ECHILD){
                // timer end
                gettimeofday(&t2, NULL);
				// calculate the elapsed time
				elapsedTime = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec)/1000000.0);
				printf("%lf\n", elapsedTime);
				fprintf(TimeLog, "TOTALPROCESSES: %d|| totalPrime: %d|| ELAPSEDTIME:%lf\n", totalProcesses,totalPrime, elapsedTime);
				fclose(TimeLog);
                break;
            }
        }
    }

   	return 0; 
}

// Write all prime numbers to file
void Prime(int PrimeRange, int processNum){
    int count, c, halfCount;
    char fileName[50];
	
	struct timeval tv;
	struct tm* ptm;
	char time_string[40];
	long milliseconds;

    sprintf(fileName, "%d", processNum);
    strcat(fileName, "-Process_Log.txt");

    //Create log file 
    FILE *LogFile = fopen(fileName, "w");

    if (LogFile == NULL){
        printf("Error opening file!\n");
        exit(1);
    }

    //Find all prime numbers
    for ( count = 3 ; count <= (int)PrimeRange; count++){
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
    printf("FINISHED PROCESS NUMBER: %d\n", processNum);
}
