//https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c
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

int writerandstofile(int sortsize);
int maprandstomem();
int readmap();
int randfiletoarray(int arrsize);
int shclienttest();
void* create_shared_memory(size_t size);
int partition(int a[],int l,int u);

void* create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_ANONYMOUS | MAP_SHARED;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, 0, 0);
}

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
   int* sortarrayptr;
   int shmid;
   key_t key;
   int *shm;
   int *s;
   key = 9876;
   FILE *fptr;
   fptr = fopen("rutf8.txt","r");

   while(!feof(fptr) && j<arrsize) {
   fscanf (fptr, "%d", &sortarray[j]);
   j++;
   }
   fclose(fptr);  
   sortarrayptr = sortarray;
   shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);

   if(shmid<0)
   {perror("shmget");exit(1);}

   shm = shmat(shmid, NULL, 0);
   
   if(shm == (int *) -1)
   {perror("shmat");exit(1);}

   memcpy(shm, sortarrayptr, sizeof(sortarray));

   s = shm;
   s += sizeof(sortarray);

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

int partition(int a[],int l,int u)
{
    int v,i,j,temp;
    v=a[l];
    i=l;
    j=u+1;
    
    do
    {
           do
               i++;
            
           while((a[i] < v) && (i <= u));
        
           do
               j--;
           while(v < a[j]);
        
           if(i < j)
           {
               temp=a[i];
               a[i]=a[j];
               a[j]=temp;
           }
    }while(i < j);
    
    a[l]=a[j];
    a[j]=v;
    
    return(j);
}

int shclienttest() {
    key_t key;
    int shmid;
    int *shm;
    int *s;
    key = 9876;
    shmid = shmget(key, SHSIZE, 0666);
    if(shmid<0)
    {perror("shmget");exit(1);}

    shm = shmat(shmid, NULL, 0);
    if(shm == (int *) -1)
    {perror("shmat");exit(1);}

    for(s = shm; *s != 0;s++)
    {
        printf("%d \n", *s);
    }

    printf("\nclient reached \n");
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
