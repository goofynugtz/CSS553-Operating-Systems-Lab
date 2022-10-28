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

Q4: Find ONE Prime number in range [x,y].
*/

pid_t c_pids[CC];

void kill_child_process(){
  for (int i = 0; i < CC; i++)
    if (c_pids[i] != 0){
      printf("[!] Killing: %d\n", c_pids[i]);
      kill(c_pids[i], SIGINT);
    }
}

bool isPrime(int n){
  if (n <= 1) return false;
  for (int i = 2; i*i <= n; i++)
    if (n % i == 0) return false;
  return true;
}

bool get_first_prime(int x, int y, int * ans){
  printf("Start: %d | End: %d by PID: %d\n", x, y, getpid());
  for (int i = x; i <= (int)y; i++)
    if (isPrime(i)){
      (*ans) = i;
      return true;
    }
  return false;
}

int main(void){
  signal(SIGUSR1, kill_child_process);
  int x,y;
  printf("\nEnter #x and #y: ");
  scanf("%d", &x);
  scanf("%d", &y);

  int ans, start, end, cc = CC;
  bool found = 0;

  for (int i = 0; i < cc; i++){
    start = i*(y-x)/cc + x+1;
    end = (i+1)*(y-x)/cc + x;
    c_pids[i] = fork();
    if (c_pids[i] == 0){
      found = get_first_prime(start, end, &ans);
      if (found){
        printf(">> Got %d.\n\n", ans);
        kill(getppid(), SIGUSR1);
        exit(EXIT_SUCCESS);
      } else exit(EXIT_FAILURE);
    }
    sleep(0.0001);
  }
  return 0;
}