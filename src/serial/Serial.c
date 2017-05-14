
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "functions.h"
#include <mpi.h>




void Serial_Qsort(void){
    int i,array_size;
    printf( "Give array size\n");
    scanf("%d",&array_size);
    fflush(stdin);

    double start_time,total;
    start_time = MPI_Wtime();  

    int *Array = (int*)calloc(array_size , sizeof(double));
        // Initialize the array with random values
        for (i=0;i<array_size;i++){
            Array[i] = drand48() * 1000000;
        }
        myqsort(0, array_size-1,Array);
        if (isSorted(Array,array_size))printf("table sorted! ");
        free(Array);

        total = MPI_Wtime() - start_time;
        printf("\nTotal time taken: %f seconds",total);
    


}



int isSorted(int *Array, int size)
{
    int i;
    for (i = 1;i < size;i ++){
        if (Array[i] < Array[i-1]){
            printf("Values are wrong in Array %d and %d,\n",Array[i],Array[i-1]);
            return NO;
        }
    }
    return YES;
}


void swap(int num1, int num2,int *Array) {
   int temp = Array[num1];
   Array[num1] = Array[num2];
   Array[num2] = temp;
}


void myqsort(int low, int high,int *Array){    
    if(high-low <= 0) {
      return;
      } else {      
      int partitionPoint = partition(low, high, Array);
      myqsort(low,partitionPoint-1,Array);
      myqsort(partitionPoint+1,high,Array);
   }       

}




int partition(int low, int high, int* Array) { 
    int pivot = Array[high];
    int index=low;
    int i;
    for(i=low;i<=high-1;i++){
        if(Array[i]<=pivot){
            swap(i,index,Array);
            index++;
        }
    }
    swap(index,high,Array);
    return index;
}