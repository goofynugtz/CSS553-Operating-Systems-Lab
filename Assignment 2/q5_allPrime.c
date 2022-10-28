#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#define CC 2

/* 
Name: Rahul Ranjan
Roll No: 20CS8016

Q5: Find ALL Prime numbers in range [x,y].
*/

pid_t c_pids[CC];
int complete = 0;

void kill_child_process(){
  for (int i = 0; i < CC; i++)
    if (c_pids[i] != 0){
      printf("[!] Killing: %d\n", getpid());
      kill(c_pids[i], SIGINT);
    }
}

void increment_and_check(){
  complete++;
  if (complete == CC) kill_child_process();
}

bool isPrime(int n){
  if (n <= 1)
    return false;
  for (int i = 2; i*i <= n; i++)
    if (n % i == 0) return false;
  return true;
}

void getPrimes(int x, int y){
  for (int i = x; i <= (int)y; i++)
    if(isPrime(i)){
      FILE * log;
      log = fopen("primes.txt", "a");
      fprintf(log, "%d ", i);
      fclose(log);
    }
}

int main(void){
  signal(SIGUSR1, increment_and_check);
  signal(SIGUSR2, increment_and_check);
  int x,y;
  printf("\nEnter #x and #y: ");
  scanf("%d", &x);
  scanf("%d", &y);
  printf("\n");

  // Resetting the logfile
  FILE * log;
  log = fopen("primes.txt", "w");
  fclose(log);
  
  int ans, start, end, cc = CC;

  for (int i = 0; i < cc; i++){
    start = i*(y-x)/cc + x+1;
    end = (i+1)*(y-x)/cc + x;
    c_pids[i] = fork();
    if (c_pids[i] == 0){
      getPrimes(start, end);
      if (i == 0) kill(getppid(), SIGUSR1);
      else if (i == 1) kill(getppid(), SIGUSR2);
      exit(EXIT_SUCCESS);
    }
  }
  return 0;
}