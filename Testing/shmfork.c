#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <string.h>
#include <stdint.h>

#define FILESIZE (NUMINTS * sizeof(int))
#define SHSIZE 100

void* create_shared_memory(size_t size);
int * sortinput();

void* create_shared_memory(size_t size) {
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_ANONYMOUS | MAP_SHARED;
  return mmap(NULL, size, protection, visibility, 0, 0);
}

int * sortinput() {
	srand(234);
    static int arr[100];
    for(int i=0;i<100;i++) {
    	arr[i] = rand()%1000;
    }
    return arr;
}

int main(int argc, char * argv[]) 
{ 
  char* parent_message = "hello";
  char* child_message = "goodbye";
  int* sortarrayptr = sortinput();
  void* shmem = create_shared_memory(128);
  int *s;

  memcpy((int*)shmem, sortarrayptr, sizeof(sortarrayptr));

  int pid = fork();

  if (pid == 0) {
    //printf("Child read: %d\n", (int *)shmem);
    //memcpy(shmem, child_message, sizeof(child_message));
    //printf("Child wrote: %s\n", (char *)shmem);
    printf("shmem val %d \n", (int)(uintptr_t)&shmem);
    //for(s = (int*)shmem; *s != 0;s++)
    //{
    //	printf("Got to loop \n");
    //	printf("%d \n",(int)(uintptr_t));
        //printf("%d \n", *s);
    //}

  } else {
    //printf("Parent read: %d\n", (int *)shmem);
    //sleep(1);
    //printf("After 1s, parent read: %s\n", (char *)shmem);
  }
}