#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sched.h>
#include <pthread.h>

#define FINAL_VALUE 20
#define N_WRITER 1
#define N_READER 1


void *writring(void *arg);
void *reading(void *arg);
unsigned globle_count;
pthread_mutex_t mutex1;
pthread_cond_t read,write;



void main(int argc, char * argv[]) {

  pthread_t tid_W[N_WRITER];
  pthread_t tid_R[N_READER];


  int j = 0;
  for (j=0; j<N_WRITER; j++){
    pthread_create(&tid_W[j], NULL, writring, NULL);
  }
  for (j=0; j<N_READER; j++){
    pthread_create(&tid_R[j], NULL, reading, NULL);
    
  }
  pthread_join(tid_W[0], NULL);
  pthread_join(tid_R[0], NULL);


}



void *writring(void *arg){
  int temp =-1;
  int i =0;
  while(1){
    if (i == FINAL_VALUE+1){
      printf("the writring thread is exiting\n" );
      break;
    }

      pthread_mutex_lock (& mutex1 );
      if (temp == globle_count){
      pthread_cond_wait(&write,& mutex1);
    }
       printf("%d ->\n", i);
       globle_count = i;
       pthread_cond_signal(&read);
       pthread_mutex_unlock(& mutex1 );
       i++;
       temp = i;
     
     sleep(random()%5);
   }
   
   pthread_exit(NULL);
 }




void *reading(void *arg){
  int temp =-1;
  while(1){
    if (globle_count == FINAL_VALUE) {
      printf("the reading thread is exiting\n" );
      break;       
    }
      pthread_mutex_lock (& mutex1 );
      if (temp == globle_count){
        pthread_cond_wait(&read,& mutex1);
      }

      printf("%d <-\n", globle_count);
      temp = globle_count;
      pthread_cond_signal(&write);
      pthread_mutex_unlock(& mutex1 );
  sleep(random()%5);
 }
 
 pthread_exit(NULL);
}
		
	