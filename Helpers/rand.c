//rand
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILEPATH "/tmp/mmapped.bin"
#define NUMINTS  (100)
#define FILESIZE (NUMINTS * sizeof(int))

using namespace std;

void writerandstofile();
int maprandstomem();
int readmap();

//https://stackoverflow.com/questions/25833541/reading-numbers-from-file-c

void writerandstofile() {
    ofstream myfile;
    srand(244); 
    int i;
    int p;
    myfile.open ("example.txt");
    for(i=0;i<100;i++) {
    p = rand()%1000000; 
    myfile << p << "\n";
    }
    myfile.close();
}

int maprandstomem() {
    //std::ifstream readfile("example.txt", std::ios_base::in);
    //int a;
    //while (readfile >> a)
    //{
        //printf("%d ", a);
    //    cout << a << "\n";
    //}
    //getchar();
    int i;
    int fd;
    int result;
    int *map;  /* mmapped array of int's */
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
    
    /* Read the file int-by-int from the mmap
     */
    for (i = 1; i <=NUMINTS; ++i) {
    printf("%d: %d\n", i, map[i]);
    }

    if (munmap(map, FILESIZE) == -1) {
    perror("Error un-mmapping the file");
    }
    close(fd);
    return 0;
}



int main(int argc, char * argv[]) 
{ 
    writerandstofile();
    maprandstomem();
    readmap();
    return 0;
}
