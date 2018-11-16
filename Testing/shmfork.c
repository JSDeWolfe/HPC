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

#define FILEPATH "/tmp/mmapped.bin"
#define NUMINTS  (100)
#define FILESIZE (NUMINTS * sizeof(int))
#define SHSIZE 100

void* create_shared_memory(size_t size);

void* create_shared_memory(size_t size) {
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_ANONYMOUS | MAP_SHARED;
  return mmap(NULL, size, protection, visibility, 0, 0);
}

int main(int argc, char * argv[]) 
{ 
  char* parent_message = "hello";
  char* child_message = "goodbye";

  void* shmem = create_shared_memory(128);

  memcpy(shmem, parent_message, sizeof(parent_message));

  int pid = fork();

  if (pid == 0) {
    printf("Child read: %s\n", (char *)shmem);
    memcpy(shmem, child_message, sizeof(child_message));
    printf("Child wrote: %s\n", (char *)shmem);

  } else {
    printf("Parent read: %s\n", (char *)shmem);
    sleep(1);
    printf("After 1s, parent read: %s\n", (char *)shmem);
  }
}