#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/wait.h>  /* Needed for the wait function */
#include <unistd.h>    /* needed for the fork function */
#include <string.h>    /* needed for the strcat function */
#define SHMSIZE 27
int main() {
   int shmid;
   char *shm;

   if(fork() == 0) {
      shmid = shmget(2009, SHMSIZE, 0);
      shm = shmat(shmid, 0, 0);
      char *s = (char *) shm;
      *s = '\0';  /* Set first location to string terminator, for later append */
      int i;
      for(i=0; i<5; i++) {
         int n;  /* Variable to get the number into */
         printf("Enter number<%i>: ", i);
         scanf("%d", &n);
         sprintf(s, "%s%d", s, n);  /* Append number to string */
      }
      strcat(s, "\n");  /* Append newline */
      printf ("Child wrote <%s>\n",shm);
      shmdt(shm);
   }
   else {
      /* Variable s removed, it wasn't used */
      /* Removed first call to wait as it held up parent process */
      shmid = shmget(2009, SHMSIZE, 0666 | IPC_CREAT);
      shm = shmat(shmid, 0, 0);
      wait(NULL);
      printf ("Parent reads <%s>\n",shm) ;
      shmdt(shm);
      shmctl(shmid, IPC_RMID, NULL);
   }
   return 0;
}
