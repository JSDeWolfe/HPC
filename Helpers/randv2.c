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
int randfiletoarray(int arrsize);
int client(int sortsize);
int partition(int a[],int l,int u);

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

void quick_sort(int a[],int l,int u)
{
    int j;
    if(l<u)
    {
        j = partition(a,l,u);
        quick_sort(a,l,j-1);
        quick_sort(a,j+1,u);
    }
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

int client(int sortsize) {
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
    
    //quick_sort(s,0,sortsize-1);

    for(s = shm; *s != 0;s++) {
        printf("%d \n", *s);
    }

    return 0;
}


int main(int argc, char * argv[]) 
{ 
    int sortsize = 7;
    writerandstofile(sortsize);
    randfiletoarray(sortsize);
    client(sortsize);
    return 0;
}
