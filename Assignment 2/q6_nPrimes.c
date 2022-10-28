#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#define CC 6

/* 
Name: Rahul Ranjan
Roll No: 20CS8016

Q6: Find 'p' Prime numbers in range [x,y].
*/

pid_t c_pids[CC];
int prime_count;
char FILE_NAME[] = "prime.txt";
int SIG = 35;

void kill_child_process(){
  for (int i = 0; i < CC; i++)
    if (c_pids[i] != 0){
      kill(c_pids[i], SIGKILL);
    }
}

void updateCount(int x){
  if (prime_count <= 0){
    kill_child_process();
  } else prime_count--;
}

bool isPrime(int n){
  if (n <= 1)
    return false;
  for (int i = 2; i*i <= n; i++)
    if (n % i == 0) return false;
  return true;
}

void getPrimes(int x, int y){
  printf("Fetching from %d | to: %d by PID: %d\n",x,y, getpid());
  for (int i = x; i <= (int)y; i++)
    if(isPrime(i)){
      FILE * log;
      log = fopen(FILE_NAME, "a");
      kill(getppid(), SIG);
      fprintf(log, "%d\n", i);
      fflush(log);
      fclose(log);
    }
}

// Parent Processes for 'p' primes and discards other values.
void cleanup(int p){
  int primes[p];
  FILE * reader = fopen(FILE_NAME, "r+");
  for (int i = 0; i < p; i++) fscanf(reader, "%d\n", &primes[i]);
  fclose(reader);
  FILE * writer = fopen(FILE_NAME, "w");
  for (int i = 0; i < p; i++) fprintf(writer, "%d\n", primes[i]);
}


int main(void){
  signal(SIG, updateCount);

  int x,y,p;
  printf("\nEnter #x #y #p: ");
  scanf("%d", &x);
  scanf("%d", &y);
  scanf("%d", &p);
  printf("\n");
  prime_count = p;
  FILE * log;
  log = fopen(FILE_NAME, "w");
  fclose(log);

  pid_t wait_c; int status = 0;
  int ans, start, end, cc = CC;

  for (int i = 0; i < cc; i++){
    start = i*(y-x)/cc + x+1;
    end = (i+1)*(y-x)/cc + x;
    c_pids[i] = fork();
    if (c_pids[i] == 0){
      getPrimes(start, end);
      exit(EXIT_SUCCESS);
    }
  }
  while((wait_c = wait(&status)) > 0);
  cleanup(p);
  return 0;
}