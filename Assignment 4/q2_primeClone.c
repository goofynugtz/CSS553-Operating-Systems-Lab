#define _GNU_SOURCE
#include <sched.h>
#include <sys/syscall.h>
#include <linux/sched.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define STACK_SIZE 32784
#define CC 5

typedef struct c_args {
  int x, y, t_i;
  void *stack;
} c_args;

bool isPrime(int n){
  if (n <= 1)
    return false;
  for (int i = 2; i*i <= n; i++)
    if (n % i == 0) return false;
  return true;
}

int getPrimes(void * a){
  c_args * args = (c_args*)a;
  int start=(int)((c_args*)args)->x;
  int end=(int)((c_args*)args)->y;

  for (int i = start; i <= end; i++){
    if(isPrime(i)){
      FILE *log = fopen("primes.txt", "a+");
      fprintf(log, "%d ", i);
      fclose(log);
    }
  }

  return 0;
}

int main(void){
  
  printf("\nEnter #x and #y: ");
  int x,y;
  scanf("%d", &x);
  scanf("%d", &y);
  printf("\n");
  int start, end, cc = CC;
  // Resetting the logfile
  FILE * log;
  log = fopen("primes.txt", "w");
  fclose(log);

  c_args **args = (c_args**) malloc(sizeof(c_args*));
  for (int i = 0; i < CC; i++){
    args[i] = (c_args*) malloc(sizeof(c_args));
    args[i]->x = i*(y-x)/cc + x+1;
    args[i]->y = (i+1)*(y-x)/cc + x;
    args[i]->stack = malloc(STACK_SIZE);
    args[i]->t_i = i;
    clone(&getPrimes, (args[i]->stack)+STACK_SIZE, CLONE_VM, args[i]);
  }
  
  return 0;
}