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
#define INFILE   "rutf8.txt"
#define NUMINTS                      100
#define SHSIZE   ((NUMINTS)*sizeof(int))
#define KEY                         9876

// Create random-inputs file.
static void write_rands_to_file( const int sortsize ) {

  // Open file.
  FILE *fptr = fopen( INFILE, "w" );
  if ( !fptr ) {
    perror( INFILE );
    exit( 1 );
  }
  // Write random integers.
  srand( 234 );
  for ( int j=0; j<sortsize; j++ ) {
    int i = rand() % 1000;
    fprintf( fptr, "%d\n", i );
  }
  // Close file.
  fclose( fptr );
}

// Create and populate shared memory.
static void rand_file_to_array( const int arrsize ) {

  // Delete pre-existing shared memory, if any.
  int shmid = shmget( KEY, 0, IPC_CREAT | 0666 );
  shmctl( shmid, IPC_RMID, NULL );

  // Create shared memory.
  shmid = shmget( KEY, SHSIZE, IPC_CREAT | 0666 );
  if ( shmid < 0 ) {
    perror( "create" );
    exit( 1 );
  }
  int *shm = (int *)shmat( shmid, NULL, 0 );
  if ( shm == (int *) -1 ) {
    perror( "shmat" );
    exit( 1 );
  }
  // Scan inputs to shared memory.
  FILE *fptr = fopen( INFILE, "r" );
  for ( int j=0; !feof(fptr) && j<arrsize && (j+1)*sizeof(*shm)<SHSIZE; j++ )
    fscanf( fptr, "%d", shm + j );
  fclose( fptr );

  shmdt( shm );
}

// Anything with so much recursion should never be called "quick".
static void quick_sort( int *a, int n ) {
  if ( n > 1 ) {

    // Partition.
    int j=n;
    for ( int i=0; i || j==n; ) {

      // Find the next that's higher than the first.
      while ( a[ ++i ] < *a && i < n ) { }

      // Find the last that's lower than the first.
      while ( a[ --j ] > *a ) { }

      // When all are traversed, swap the first instead of next higher.
      if ( i >= j )
        i = 0;

      // Swap next higher (if any) with last lower.
      if ( j ) {
        int  v = a[ i ];
        a[ i ] = a[ j ];
        a[ j ] = v;
      }
    }
    // Recurse over two sub-ranges, separated by original first value.
    quick_sort( a, j );          // all lower than original first
    quick_sort( a+j+1, n-j-1 );  // all higher than original first
  }
}

// Sort shared memory.
static void client( int sortsize ) {

  // Open shared memory.
  int shmid = shmget( KEY, SHSIZE, 0666 );
  if ( shmid < 0 ) {
    perror( "find" );
    exit( 1 );
  }
  int *shm = (int *)shmat( shmid, NULL, 0 );
  if ( shm == (int *)-1 ) {
    perror( "shmat" );
    exit( 1 );
  }
  // Sort.
  quick_sort( shm, sortsize );

  // Dump results to stdout.
  for ( int i=0; i<sortsize; i++ )
    printf( "  %3d\n", shm[ i ] );

  // Detach and delete shared memory.
  shmdt( shm );
  shmctl( shmid, IPC_RMID, NULL );
}

// Sort in shared memory.
int main( int argc, char **argv )
{
  // Create random input file.
  int sortsize = 7;
  write_rands_to_file( sortsize );

  // Scan input file to shared memory.
  rand_file_to_array( sortsize );

  // Sort and dump shared memory.
  client( sortsize );

  // Done.
  return 0;
}
