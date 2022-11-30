#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define CC 4

/* 
Name: Rahul Ranjan
Roll No: 20CS8016

Q3: Search a value in large unsorted array with 'n' child process.
*/

int r;

pid_t c_pids[CC];
int cc = CC; // Child Count

void elementFoundHandler(int x){
  for (int i = 0; i < cc; i++){
    if (c_pids[i] != 0){
      printf("[!] Killing: %d\n", c_pids[i]);
      kill(c_pids[i], SIGINT);
    }
  }
}

void elementNotFoundHandler(int x){}

bool linearSearch(int * arr, int n, int i, int cc, int key){
  int start = (i*n/cc);
  int end = ((i+1)*n/cc)-1;
  printf( "Searching from: %4d | to: %4d by PID: %d\n", start, end, getpid());
  for (int j = start; j <= end; j++)
    if (arr[j] == key) return true;
  return false;
}

int main(void){
  signal(SIGUSR2, elementNotFoundHandler);
  signal(SIGUSR1, elementFoundHandler);
  int n = 10000;
  int arr[n];
  srand(22);
  for (int i = 0; i < n; i++) arr[i] = rand() % 10000;
  
  FILE * fp;
  fp = fopen("array.txt", "w");
  for (int i = 0; i < n; i+=10){
    for (int j = 0; j < 10; j++)
      fprintf(fp, "%4d ", arr[i+j]);
    fprintf(fp, "\n");
  }
  fclose(fp);
  pid_t wait_p;
  
  printf("\nEnter Number of Child Process to spawn: ");
  scanf("%d", &cc);

  int status = 0;
  int key; // 1137 // 1410 present in 3/4 divs
  
  printf("\nEnter #value to search: ");
  scanf("%d", &key);

  clock_t start, end;
  start = clock();

  for (int i = 0; i < cc; i++){
    c_pids[i] = fork();
    if (c_pids[i] == 0){
      bool found = linearSearch(arr, n, i, cc, key);
      if (found){
        printf(">> Key Found\n\n");
        kill(getppid(), SIGUSR1);
        exit(EXIT_SUCCESS);
      } else {
        printf(">> Key NOT Found\n");
        kill(getppid(), SIGUSR2);
        exit(EXIT_FAILURE);
      }
    }
  }
  for (int i = 0; i < CC; i++)
    wait_p = waitpid(c_pids[i], &status, 0);
  
  end = clock();
  double time_used = ((double)(end - start))/CLOCKS_PER_SEC;
  printf("\nTime taken: %lf\n\n", time_used);
  return 0;
}