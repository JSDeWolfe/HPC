#include <iostream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

#define DIM(a) (int) ( sizeof(a) / sizeof(*a) )
 
using namespace std;
 
void quick_sort(int[],int,int);
int partition(int[],int,int);
 
int main()
{
    int n = 10, i;
    int a[11] = { 1, 11, 9, 8, 14, 2, 13, 12, 19, 7, 0 };
        
    quick_sort(a,0,n-1);
    cout<<"\nArray after sorting:";
    for(i=0;i<n;i++) {
        cout<<a[i]<<" ";
    }
    cout<<"\n";
    cout<<"forktest\n";
    return 0;        
}
 
void quick_sort(int a[],int l,int u)
{
    int j;
    if(l<u)
    {
        j = partition(a,l,u);
        //handles all the swaps and tells us which quick sorts to do next
        //want each of these on own process
        //need some shmem to save what is going on

  pid_t child_pid = fork();

  switch(child_pid) {
    case 0:
      //child(result);
      cout << "case 0";
      break;
    case -1:
      cout << "case -1";
      //lg("fork", -1);
      break;
    default:
      cout<< "default";
      //  parent(result);
}
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
