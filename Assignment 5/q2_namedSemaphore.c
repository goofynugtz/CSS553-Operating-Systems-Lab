#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SEM_NAME "/mutex"
#define SHM_KEY 0x1234
#define PC 10

pid_t *c_pid, wait_p;
int shmid, *count;
void *memory;

bool isPrime(int n){
  if (n <= 1)
    return false;
  for (int i = 2; i*i <= n; i++)
    if (n % i == 0) return false;
  return true;
}

void getPrimes(int x, int y, sem_t *b_sem){
  for (int i = x; i <= y; i++)
    if(isPrime(i)){
      sem_wait(b_sem);
      void *c_m = shmat(shmid, NULL, 0);
      int *_count = (int*)c_m;
      (*_count) += 1;
      shmdt(c_m);
      sem_post(b_sem);
    }
  exit(EXIT_SUCCESS);
}

int main(void){
  shmid = shmget(SHM_KEY, sizeof(int), 0666|IPC_CREAT);
  memory = shmat(shmid, NULL, 0);
  count = (int*)memory;
  (*count) = 0;
  int x,y;
  printf("\nEnter #x and #y: ");
  scanf("%d", &x);
  scanf("%d", &y);
  printf("\n");
  int pc = PC, status;
  c_pid = (pid_t*) malloc(sizeof(pid_t)*pc);
  sem_t *binary_sem = sem_open(SEM_NAME, O_CREAT, 0660, 1);
  int start, end;

  for (int i = 0; i < pc; i++){
    start = i*(y-x)/pc + x+1;
    end = (i+1)*(y-x)/pc + x;
    c_pid[i] = fork();
    if (c_pid[i] == 0)
      getPrimes(start, end, binary_sem);
  }
  while ((wait_p = wait(&status)) > 0);
  printf("Prime count: %d\n\n", *count);

  shmdt(memory);
  shmctl(shmid, IPC_RMID, 0);
}