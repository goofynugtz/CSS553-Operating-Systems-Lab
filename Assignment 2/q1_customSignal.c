#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/* 
Name: Rahul Ranjan
Roll No: 20CS8016

Q1: Child spawns another child process.
Crtl + C kills all child process but not the parent.
*/

void ignore_signal(int x){
  printf("\t[Alert!] Recieved SIG:%d. Signal Ignored by PID: %d\n", x, getpid());
  return;
}

void terminate_process(int x){
  printf("\t[Alert!] Recieved SIG:%d. Terminating PID: %d\n", x, getpid());
  signal(SIGINT, SIG_DFL);
  kill(getpid(), SIGINT);
}

void main(void){
	pid_t c_pid, wait_p;
  int status = 0;

  int n = 5;
  printf(">> PID: %d\n", getpid());
  signal(SIGINT, ignore_signal);
  
  for (int i = 0; i < n; i++){
    c_pid = fork();
    if (c_pid == 0){
      signal(SIGINT, terminate_process);
      printf(">> PID: %d PPID: %d\n", getpid(), getppid());
    } else break;
  }
  while((wait_p = wait(&status)) > 0);
  while(1);
}
