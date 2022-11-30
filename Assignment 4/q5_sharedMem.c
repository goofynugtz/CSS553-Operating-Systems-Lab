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
#define SHM_KEY 0x1234
#define TC      5

pid_t *c_pid, wait_p;

int main(void){

  int shmid = shmget(SHM_KEY, sizeof(int), 0666|IPC_CREAT);
  if (shmid == -1){
    perror("Shared Memory");
    return 1;
  }

  int n, m;
  scanf("%d %d", &n, &m);

  void *memory = shmat(shmid, NULL, 0);
  int *X = (int*)memory;
  *X = 1;
  int status;
  c_pid = (pid_t*) malloc(sizeof(pid_t)*n);
  for (int i = 0; i < n; i++){
    c_pid[i] = fork();
    if (c_pid[i] == 0){
      void *mem_c = shmat(shmid, NULL, 0);
      int *C = (int*)mem_c;
      (*C) += m;
      printf(">> Updated to: %4d @PID: %5d\n", (*C), getpid());
      exit(EXIT_SUCCESS);
    }
  }
  
  while((wait_p = wait(&status)) > 0);
  printf("\nFinal Value: %d\n\n", *X);

  return 0;
}