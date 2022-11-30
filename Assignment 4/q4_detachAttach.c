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

void *thread_function(void *args){
  pthread_exit(0);
}

bool isEven(int i){
  return (i%2 == 0)? 1: 0; 
}

int main(void){
  
  printf("\nEnter #n: ");
  int n = TC;
  scanf("%d", &n);
  printf("\n");
  int cc = n;

  c_args **args = (c_args**) malloc(sizeof(c_args*)*TC);
  for (int i = 0; i < cc; i++){
    args[i] = (c_args*) malloc(sizeof(c_args));
    args[i]->t_i = i;
    pthread_create(&c_thread[i], NULL, thread_function, (void*)(args[i]));
    if (!isEven(i)){
      pthread_detach(c_thread[i]);
    }
  }
  int *c = (int*)malloc(sizeof(int)*n);
  for (int i = 0; i < n; i++) c[i] = -1;
  for (int i = 0; i < n; i++)
    printf("%d", c[i]);
  printf("\n");

  for (int i = 0; i < n; i++) 
    if (isEven(i))
      c[i] = pthread_join(c_thread[i], NULL);
  
  for (int i = 0; i < n; i++) 
    printf("%d", c[i]);
  printf("\n");
  
  return 0;
}