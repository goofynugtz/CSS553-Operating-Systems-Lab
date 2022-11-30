#include <linux/sched.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#define SHM_KEY 0x1236
#define TC      5
#define SIZE    10

pid_t c_pid, wait_p;

bool isOdd(int i){
  return (i%2 != 0)? 1: 0; 
}

int main(void){
  int n = SIZE;
  int shmid = shmget(SHM_KEY, n*sizeof(int), 0666 | IPC_CREAT);
  if (shmid == -1){
    perror("Shared Memory");
    return 1;
  }
  void *memory = shmat(shmid, NULL, 0);
  int *X = (int*)memory;
  for (int i = 0; i < n; i++) X[i] = 0;
  int status;
  double sum, average;
  for (int i = 0; i < n; i++)
    if (isOdd(i))
      X[i] = rand()%100 + 1;
  printf("\nRandomly Genarated Array: ");
  for (int i = 0; i < n; i++)
    printf("%d ", X[i]);
  printf("\n");

  c_pid = fork();
  if (c_pid == 0){
    void *c_mem = shmat(shmid, NULL, 0);
    int *C = (int*)c_mem;
    for (int i = 0; i+1 < n; i++)
      if (isOdd(i))
        if (C[i] != 0)
          C[i+1] = C[i]+2;
    for (int i = 0; i < n; i++)
      if (isOdd(i))
        sum += C[i];
    average = sum/n;
    printf("Array in child process:   ");
    for (int i = 0; i < n; i++)
    printf("%d ", C[i]);
    printf("\n");
    printf("\nChild >> Sum: %.2lf\nChild >> Average: %.2lf\n", sum, average);
    exit(EXIT_SUCCESS);
  }
  while((wait_p = wait(&status)) > 0);
  for (int i = 0; i < n; i++)
    if (!isOdd(i))
      sum += X[i];
  average = sum/n;
  printf("\nParent >> Sum: %.2lf\nParent >> Average: %.2lf\n", sum, average);
  return 0;
}