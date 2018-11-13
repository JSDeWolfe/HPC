#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#define DIM(a) (int) ( sizeof(a) / sizeof(*a) )

//struct comparesplit {
//   int smallresults[];
//   int bigresults[];
//} comparesplit;

void sort(int *a, int n) {
   if(n<3) {
   printf("<3 block \n");
   return;
   }
   printf("A[0]: %d \n", *a);
   printf("size n: %d \n", n);
   int pivot = a[n-2];
   int smallarraycnt = 0;
   for(int i = 0; i < n; i++) {
      if(a[i] <= pivot) {
      	printf("less\n");
         smallarraycnt++;
      } 
   }
   //int leftarray[smallarraycnt];
   //int rightarray[n - smallarraycnt];
   //maybe use stack and keep track of top for array for each, after every add just have a tally
   //https://www.tutorialspoint.com/data_structures_algorithms/stack_program_in_c.htm
   //https://www.cprogramming.com/tutorial/c/lesson15.html
   //https://en.wikipedia.org/wiki/SHMEM
   //http://www-cdr.stanford.edu/touch/tele_projects/qnxhelp/src_help/DML_shmem.html
   //https://en.wikipedia.org/wiki/Mmap
   //https://sklise.com/2012/09/22/introduction-to-git/
   //https://stackoverflow.com/questions/8775850/how-do-i-add-files-and-folders-into-github-repos
   for(int j = 0; j < n;j ++) {
      if(a[j] < pivot) {
      
      }
   }
   return;
}



int main() {
   int a[11] = { 1, 11, 9, 8, 14, 2, 13, 12, 19, 7, 0 };
   int asize = DIM(a);
   //int *ptr = a;
   //int b = a;
   printf("StarA: %d \n", *a);
   printf("StarAplus1: %d \n", *a + 1);
   printf("AmpersandA: %p \n", &a);
   //printf("AmpersandAplus1: %x \n", &a[0]);
   printf("Array size: %d \n", (int) DIM(a));
   int i;
   printf("List before sorting\n");
   
   for(i = 0; i <= max; i++) {
      printf("%d ", a[i]);
  }
  printf("\n");
  sort(a, DIM(a));
}
