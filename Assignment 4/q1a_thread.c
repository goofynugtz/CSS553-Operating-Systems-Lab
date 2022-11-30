#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#define CTC 20000

pthread_t *c_thread;

void * thread_process(void * args){
  pthread_exit(0);
}

int main(void){
  int c_thread_count = CTC;
  int i;
  
  clock_t t_start, t_end;
  t_start = clock();
  
  c_thread = (pthread_t*)malloc(sizeof(pthread_t)*c_thread_count);
  for (i = 0; i < c_thread_count; i++){
    pthread_create(&c_thread[i], NULL, thread_process, NULL);
    pthread_detach(c_thread[i]);
  }

  t_end = clock();
  double t_time_used = ((double)(t_end - t_start))/CLOCKS_PER_SEC;
  printf("\nTime taken for %6d threads: %lf\n\n", c_thread_count, t_time_used);
  return 0;
}