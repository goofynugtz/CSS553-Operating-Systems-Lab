#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#define CC 2

/* 
Name: Rahul Ranjan
Roll No: 20CS8016

Q2: Searching a value in a large unsorted array with '2' child process.
*/

int r;

pid_t c_pids[CC];

void kill_child_process(){
  for (int i = 0; i < CC; i++)
    if (c_pids != 0){
      printf("[!] Killing: %d\n", getpid());
      kill(c_pids[i], SIGINT);
    }
}

void elementFoundHandler(int x){
  kill_child_process();
}

void elementNotFoundHandler(int x){}

bool linearSearch(int * arr, int n, int i, int key){
  int start = (i*n/2);
  int end = ((i+1)*n/2)-1;
  printf("Searching in arr[ %d ] .. arr[ %d ] by PID: %d\n", start, end, getpid());
  for (int j = start; j <= end; j++)
    if (arr[j] == key) return 1;
  return 0;
}

int main(void){
  signal(35, elementFoundHandler);
  signal(36, elementNotFoundHandler);
  
  int n = 10000;
  int arr[n];
  srand(22);
  for (int i = 0; i < n; i++) arr[i] = rand() % 10000;
  
  FILE * fp;
  fp = fopen("array.txt", "w");
  for (int i = 0; i < n; i += 10){
    for (int j = 0; j < 10; j++)
      fprintf(fp, "%4d ", arr[i+j]);
    fprintf(fp, "\n");
  }
  fclose(fp);

  int key; // 1137 9396
  
  printf("\nEnter #value to search: ");
  scanf("%d", &key);
  for (int i = 0; i < 2; i++){
    c_pids[i] = fork();
    if (c_pids[i] == 0){
      if (linearSearch(arr, n, i, key)){
        printf(">> Key Found\n\n");
        kill(getppid(), 35);
        exit(EXIT_SUCCESS);
      }
      else {
        printf(">> Key NOT found\n\n");
        kill(getppid(), 36);
        exit(EXIT_FAILURE);
      }
    }
    if (c_pids[i] != 0){
      r = i;
      printf(">> [r] is set to %d\n", r);
    }
  }
  return 0;
}