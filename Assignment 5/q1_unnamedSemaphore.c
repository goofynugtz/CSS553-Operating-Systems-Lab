#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define TC 10

pthread_t *c_thread;
int count = 0;
sem_t semaphore;

typedef struct c_args {
  int x, y;
} c_args;

bool isPrime(int n){
  if (n <= 1)
    return false;
  for (int i = 2; i*i <= n; i++)
    if (n % i == 0) return false;
  return true;
}

void * getPrimes(void * a){
  c_args * args = (c_args*)a;
  int start = (int)((c_args*)args)->x;
  int end = (int)((c_args*)args)->y;
  for (int i = start; i <= end; i++)
    if(isPrime(i)){
      sem_wait(&semaphore);
      count++;
      sem_post(&semaphore);
    }
  free(a);
  return 0;
}

int main(void){

  sem_init(&semaphore, 0, 1);
  printf("\nEnter #x and #y: ");
  int x,y;
  scanf("%d%d", &x, &y);
  printf("\n");
  int cc = TC;
  c_thread = (pthread_t*) malloc(sizeof(pthread_t)*cc);
  c_args **args = (c_args**) malloc(sizeof(c_args*)*cc);
  for (int i = 0; i < cc; i++){
    args[i] = (c_args*) malloc(sizeof(c_args));
    args[i]->x = i*(y-x)/cc + x+1;
    args[i]->y = (i+1)*(y-x)/cc + x;
    pthread_create(&c_thread[i], NULL, getPrimes, (void*)(args[i]));
  }
  free(args);
  int c;
  for (int i = 0; i < TC; i++)
    c = pthread_join(c_thread[i], NULL);
  sem_destroy(&semaphore);
  printf("Prime count: %d\n\n", count);

  return 0;
}