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

  FILE *fptr = fopen( INFILE, "w" );
  if ( !fptr ) {
    perror( INFILE );
    exit( 1 );
  }

  srand( 234 );
  for ( int j=0; j<sortsize; j++ ) {
    int i = rand() % 1000;
    fprintf( fptr, "%d\n", i );
  }

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
  const int n = arrsize < SHSIZE/sizeof(*shm) ? arrsize : SHSIZE/sizeof(*shm);
  FILE *fptr = fopen( INFILE, "r" );
  for ( int j=0; !feof(fptr) && j<n; j++ )
    fscanf( fptr, "%d", shm + j );
  fclose( fptr );

  shmdt( shm );
}

static void quick_sort( int *array, int n ) {

  // Stack length is log2(n)+1.
  int nStack = 1;
  for ( unsigned m=n; m; ++nStack, m/=2 ) { }

  // Define stack, initialized with single entry describing the entire array.
  struct stackS {
    int i; // sub-array starting index
    int n; // sub-array length
  } stack[ nStack ];

  // Single entry describes the entire array.
  stack[ 0 ].i = 0;
  stack[ 0 ].n = n;

  // While stack not empty:
  for ( struct stackS *p=stack; p>=stack; ) {

    // On trivial subrange, pop and go.
    if ( p->n < 2 ) {
      --p;
      continue;
    }
    // Partition.
    int *a=array+p->i, j=p->n;
    for ( int i=0; i || j==p->n; ) {

      // Find the next item that's higher than the first.
      while ( a[ ++i ] < *a && i < p->n ) { }

      // Find the last item that's lower than the first.
      while ( a[ --j ] > *a ) { }

      // When all are items traversed, swap the first instead of next higher.
      if ( i >= j )
        i = 0;

      // Swap next higher item (if any) with last lower item.
      if ( j ) {
        int  v = a[ i ];
        a[ i ] = a[ j ];
        a[ j ] = v;
      }
    }
    // Replace current node with larger subrange.  Push smaller on top of it.
    struct stackS leading = { p[0].i,          j     }, // 0  ..  j-1
                 trailing = { p[0].i+j+1, p[0].n-j-1 }; // j+1 .. n-1
    int iLeading = j < p->n / 2; // 0 means large leading, 1 means small leading
    p[ iLeading ] =  leading;
    p[!iLeading ] = trailing;
    ++p;
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
