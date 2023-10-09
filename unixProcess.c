#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void createFile(void){
    FILE *results;
    results = fopen("results.txt", "w");
    if (results == NULL) {
      perror("Could not open file.\n");
    }
    fclose(results);
    printf("Results file created successfully.\n\n");
}

int sumOfRow(int row) {

    row = row - 1;

    FILE *matrix = fopen("matrix.txt", "r");
    if (matrix == NULL) {
        perror("Could not open file.\n");
    }

    int ch;
    for (int i = 0; i < row; i++) {
        while ((ch = fgetc(matrix)) != EOF && ch != '\n');
    }

    int numbers[9];
    for (int i = 0; i < 9; i++) {
        if (fscanf(matrix, "%d", &numbers[i]) != 1) {
            printf("Error to read the number [%d] of the row [%d]\n", i + 1, row);
            break;
        }
    }
    fclose(matrix);

    int sum=0;
    printf("Numbers of the row %d:\n", (row+1));
    for (int i = 0; i < 9; i++) {
        sum+=numbers[i];
        printf("%d ", numbers[i]);
    }
    printf("\n");
    printf("Sum of first row: %d\n\n", sum);
    return sum;
}

void writeInFile(int row, int sum){
    FILE *results = fopen("results.txt", "a");
    if(results==NULL){
        perror("Could not open file.\n");    
    }
    
    fprintf(results, "The sum of row[%d] is %d\n", row, sum);

    fclose(results);
}

void parentProcess(const char *process){
    int status;
    wait(&status);
    printf("I'm the parent process (%sIF) [%d]\n-----------------------------------------------------------------------------------------------------------\n\n", process, getpid());
    sleep(5);
}

void childProcess(const char *process, int processNumber){
    printf("-----------------------------------------------------------------------------------------------------------");
    printf("I'm the %s child process (%sIF) [%d] and my parent process is [%d]\n", process, process, getpid(), getppid());
    int sum = sumOfRow(processNumber);
    writeInFile(processNumber, sum);
    sleep(5);
    exit(0);
}

int main(){

   pid_t pid;
   pid = fork();

   if(pid!=0){
      createFile();
      parentProcess("first");

      pid_t pid_;
      pid_ = fork();

      if(pid_!=0){

        parentProcess("second");

        pid_t pid_s;
        pid_s = fork();

        if(pid_s!=0){
            parentProcess("third");
        }

        if(pid_s==0){
            childProcess("third",3);
        }

      }

      if(pid_==0){
        childProcess("second",2);
      }


   }

   if(pid==0){
       childProcess("first",1);
   }
   
   return 0;
}