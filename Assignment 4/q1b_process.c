#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#define CPC 100000

pid_t *c_pid;

int main(void){
  int c_process_count = CPC;
  int i;

  pid_t wait_p; int status;
  clock_t p_start, p_end;
  p_start = clock();

  c_pid = (pid_t*)malloc(sizeof(pid_t)*c_process_count);
  for (i = 0; i < c_process_count; i++){
    c_pid[i] = fork();
    if (c_pid[i] == 0){
      exit(EXIT_SUCCESS);
    }
  }

  // while((wait_p = wait(&status)) > 0);
  p_end = clock();
  double p_time_used = ((double)(p_end - p_start))/CLOCKS_PER_SEC;
  printf("\nTime taken for %6d process: %lf\n\n", c_process_count, p_time_used);
  
  return 0;
}