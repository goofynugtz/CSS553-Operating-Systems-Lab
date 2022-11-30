#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <pthread.h>
#define N (int)1e6

sem_t nrFull, nrEmpty, mutexPd, mutexCn;
int buffer, in, out;
pthread_t *p_id;
pthread_t *c_id;

void* producer(){
  while(1){
    sem_wait(&mutexPd);
    sem_wait(&nrEmpty); 
    printf(">> Produced by %lu. Buffer @ %d\n", pthread_self(), in);
    in = (in+1) % N;
    buffer++;
    sleep(1);
    sem_post(&nrFull);
    sem_post(&mutexPd);
    sleep(1);
  }
}

void *generateProducer(void *args){
  for (int i = 0; i < (*(int*)args); i++){
    pthread_create(&p_id[i], NULL, producer, NULL);
    printf("P Thread %d @id %ld\n", i, p_id[i]);
  }
}

void* consumer(){
  while(1){
    sem_wait(&mutexCn);
    sem_wait(&nrFull);
    printf("> Consumed  by %lu. Buffer @ %d\n", pthread_self(), out);
    out = (out+1) % N;
    buffer--;
    sleep(1);
    sem_post(&nrEmpty);
    sem_post(&mutexCn);
    sleep(1);
  }
}

void *generateConsumers(void *args){
  for (int i = 0; i < (*(int*)args); i++){
    pthread_create(&c_id[i], NULL, consumer, NULL);
    printf("C Thread %d @id %ld\n", i, c_id[i]);
  }
}

int main(void){

  int p_count = 5, c_count = 5;
  // printf("Enter #Producer #Consumer: ");
  // scanf("%d %d", &p_count, &c_count);

  sem_init(&nrFull, 0, 0);
  sem_init(&nrEmpty, 0, N);
  sem_init(&mutexPd, 0, 1);
  sem_init(&mutexCn, 0, 1);

  p_id = (pthread_t*) malloc(sizeof(pthread_t)* p_count);
  c_id = (pthread_t*) malloc(sizeof(pthread_t)* c_count);
  
  pthread_t gen_p;
  pthread_create(&gen_p, NULL, generateProducer, &p_count);
  pthread_t gen_c;
  pthread_create(&gen_c, NULL, generateConsumers, &c_count);
  
  while(1);
  return 0;
}