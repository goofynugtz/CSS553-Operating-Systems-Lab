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
#define TC 5

pthread_t c_thread[TC];

typedef struct c_args {
  int x, y, t_i;
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
  printf(">> @index: %d >> ", (int)((c_args*)args)->t_i);
  for (int i = start; i <= end; i++)
    if(isPrime(i))
      printf("%d ", i);
  
  printf("\n");
  return 0;
}

int main(void){
  
  printf("\nEnter #x and #y: ");
  int x,y;
  scanf("%d", &x);
  scanf("%d", &y);
  printf("\n");
  int start, end, cc = TC;

  c_args **args = (c_args**) malloc(sizeof(c_args*)*TC);
  for (int i = 0; i < TC; i++){
    args[i] = (c_args*) malloc(sizeof(c_args));
    args[i]->x = i*(y-x)/cc + x+1;
    args[i]->y = (i+1)*(y-x)/cc + x;
    args[i]->t_i = i;
    pthread_create(&c_thread[i], NULL, getPrimes, (void*)(args[i]));
  }
  int c;
  for (int i = 0; i < TC; i++)
    c = pthread_join(c_thread[i], NULL);
  printf("\n");
  
  return 0;
}