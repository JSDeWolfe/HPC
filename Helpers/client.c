#include <time.h>
//#include <shmem.h>
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

int main(int argc, char * argv[]) 
{ 
    key_t key;
    int shmid;
    char *shm;
    char *s;
    key = 9876;
    shmid = shmget(key, SHSIZE, 0666);
    if(shmid<0)
    {perror("shmget");exit(1);}
    shm = shmat(shmid, NULL, 0);
    if(shm == (char *) -1)
    {perror("shmat");exit(1);}
    for(s = shm; *s != 0;s++)
    {
        printf("%c", *s);
    }
    printf("\n");
    *shm = '*';
    return 0;
}
