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

int writerandstofile(int sortsize);
int maprandstomem();
int readmap();
int randfiletoarray(int arrsize);
int shclienttest();

int writerandstofile(int sortsize) {
   srand(234);
   int num;
   FILE *fptr;
   fptr = fopen("rutf8.txt","w");

   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }

   for(int j=0;j<sortsize;j++) {
   int i = rand()%1000;
   fprintf(fptr,"%d\n", i);
   }
   fclose(fptr);

   return 0;
}

int randfiletoarray(int arrsize) {
   int sortarray[arrsize], c, j = 0;
   int shmid;
   key_t key;
   char *shm;
   char *s;
   key = 9876;
   FILE *fptr;
   fptr = fopen("rutf8.txt","r");
   while(!feof(fptr) && j<arrsize) {
   fscanf (fptr, "%d", &sortarray[j]);
   j++;
   }
   fclose(fptr);  

   shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);

   if(shmid<0)
   {perror("shmget");exit(1);}

   shm = shmat(shmid, NULL, 0);
   
   if(shm == (char *) -1)
   {perror("shmat");exit(1);}

   memcpy(shm, "Hellllllloo", 11);

   s = shm;
   s += 11;

   *s = 0;

   return 0;
}

int maprandstomem() {
    int i;
    int fd;
    int result;
    int *map;  /* mmapped array of ints */
    fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    
    if (fd == -1) {
    perror("Error opening file for writing");
    exit(EXIT_FAILURE);
    }
    
    if (result == -1) {
    close(fd);
    perror("Error calling lseek() to 'stretch' the file");
    exit(EXIT_FAILURE);
    }

    result = write(fd, "", 1);
    
    if (result != 1) {
    close(fd);
    perror("Error writing last byte of the file");
    exit(EXIT_FAILURE);
    }

    map = (int*)mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (map == MAP_FAILED) {
    close(fd);
    perror("Error mmapping the file");
    exit(EXIT_FAILURE);
    }
    
    for (i = 1; i <=NUMINTS; ++i) {
    map[i] = 2 * i; 
    }

    if (munmap(map, FILESIZE) == -1) {
    perror("Error un-mmapping the file");
    /* Decide here whether to close(fd) and exit() or not. Depends... */
    }

    close(fd);
    return 0;
}

int readmap() {
    int i;
    int fd;
    int *map;  /* mmapped array of int's */

    fd = open(FILEPATH, O_RDONLY);
    if (fd == -1) {
    perror("Error opening file for reading");
    exit(EXIT_FAILURE);
    }

    map = (int*)mmap(0, FILESIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
    close(fd);
    perror("Error mmapping the file");
    exit(EXIT_FAILURE);
    }
    
    for (i = 1; i <=NUMINTS; ++i) {
    printf("%d: %d\n", i, map[i]);
    }

    if (munmap(map, FILESIZE) == -1) {
    perror("Error un-mmapping the file");
    }
    close(fd);
    return 0;
}

int shclienttest() {
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
    printf("client reached \n");
    return 0;
}



int main(int argc, char * argv[]) 
{ 
    int sortsize = 7;
    writerandstofile(sortsize);
    randfiletoarray(sortsize);
    shclienttest();
    //maprandstomem();
    //readmap();
    return 0;
}
