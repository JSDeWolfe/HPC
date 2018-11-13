#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;
#define DIM(a) (int) ( sizeof(a) / sizeof(*a) )
int main() {
pid_t pid = fork();
int a[11] = { 1, 11, 9, 8, 14, 2, 13, 12, 19, 7, 0 };
int asize = DIM(a);
//int *ptr = a;
//int b = a;
printf("StarA: %d \n", *a);
cout << "Don’t you just feel like a louse";
cout << endl;
cout << "To learn that your \"new\" theorem was proved by Gauss?";
cout << endl;
return 0;
}