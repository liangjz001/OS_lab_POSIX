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

main(int argc, char * argv[]) {
  int        fd;
  unsigned  *addr;
  //char* name =;

  /* Create a new memory object */
  fd = shm_open("/mymem1", O_RDWR | O_CREAT, 0777);
  if (fd == -1) {
    perror("Open failed");
    exit(0);
  }

  /* Set the memory object's size */
  if (ftruncate(fd, sizeof(*addr)) == -1) {
    
    perror("ftruncate error");
    exit(0);
  }
  //printf("sizeof(*addr) =%d\n",sizeof(*addr));sizeof(*addr) =4

  /* Map the memory object */
  addr = (unsigned *)(mmap(0, sizeof(*addr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
  if (addr == MAP_FAILED) {
     perror("mmap failed");
     exit(0);
  }

  printf("Map addr is %6.6X\n", addr);

  // Remove the link to the memory
  close(fd);

  *addr = 0;

  pthread_t tid_W[N_WRITER];
  pthread_t tid_R[N_READER];


  int j = 0;
  for (j=0; j<N_WRITER; j++){
    pthread_create(&tid_W[j], NULL, writring, NULL);
    pthread_join(&tid_W[j], NULL);
  }
  for (j=0; j<N_READER; j++){
    pthread_create(&tid_R[j], NULL, reading, NULL);
    pthread_join(&tid_R[j], NULL);
  }

}



void *writring(void *arg){
  int i =0;
  while(1){
    if (i == FINAL_VALUE){
      printf("the writring thread is exiting\n", );
      pthread_exit();
    }
    if (*addr == 0) {
       printf("%d ->\n", i);
       *addr = i;
       i++;
     }
     sleep(random()%5);
   }
 }




void *reading(void *arg){
  while(1){
    if (*addr == FINAL_VALUE) {
      printf("the reading thread is exiting\n", );
      pthread_exit();        
    }
    if(*addr != 0) {
      printf("%d <-\n", *addr);
      *addr = 0;
  }
 }
 sleep(random()%5);
}
		
	