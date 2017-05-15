#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include <mpi.h> 
#include <time.h> 
#include <unistd.h>
#include <math.h>

int *merge(int *v1, int n1, int *v2, int n2);

int main(int argc, char *argv[])  {


    int rank, nproc,array_size,i,local_array_size;
    int *Array,*local_array,*received_array;
    double start_time,total;   
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    
    MPI_Status status;

    
    
    
    if (rank == 0){

        printf( "Give array size\n");
        scanf("%d",&array_size);
        fflush(stdin);

        if(array_size%nproc!=0){
            printf("Size is not divisible by nproc. aborting");
            MPI_Abort(MPI_COMM_WORLD,1);
            exit(0);}

        //largest in supported without overflow is 2,147,483,647
        
        start_time = MPI_Wtime();
        Array = (int*)calloc(array_size , sizeof(int));

        srand48((unsigned int)time(NULL));
        // Initialize the array with random values
        for (i=0;i<array_size;i++){
            Array[i] = drand48() * 100000000;


        }
    }    

    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&array_size, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    local_array_size = array_size/nproc;


    local_array=(int*)calloc(local_array_size , sizeof(int));
    MPI_Scatter(Array, local_array_size, MPI_INT, local_array, local_array_size, MPI_INT, 0, MPI_COMM_WORLD);
    


    int steps=(int)log2((int)nproc);

    //steps=2;
    int dest=1;
    for(i=1;i<=steps;i++){
        
        
    }
       
    
    MPI_Barrier(MPI_COMM_WORLD);
    

    if (rank==0){
        total = MPI_Wtime() - start_time;
        printf("\nTotal time taken: %f seconds",total);
        int Result=isSorted(local_array,array_size);
        if (Result==YES)printf("Array Sorted! \n");
        free(local_array);
        UserChoice();
    }
    else{
        if (local_array!=NULL)free(local_array);
    }

    MPI_Finalize();
    
    exit(0);

}




int* merge_arrays(int *array1, int *array2, int array_size){

    int* new_array=(int*)calloc(array_size*2 , sizeof(int));

   
    
    int start_of_array1=array1[0];
    int start_of_array2=array2[0];
    int i;
    int index_1=0;
    int index_2=0;
   

    if(start_of_array2<start_of_array1){
        int* temp=array2;
        array2=array1;
        array1=temp;
    }
   
    
   for(i=0;i<array_size*2;i++){

        if(index_1>=array_size)
        {
            new_array[i]=array2[index_2];
            index_2++;
        }
        else if (index_2>=array_size)
        {
            new_array[i]=array1[index_1];
            index_1++;
        }

        else if(array1[index_1]<=array2[index_2] && index_1<array_size && index_2<array_size)
        {
            new_array[i]=array1[index_1];
            index_1++;
        }

        else{
            new_array[i]=array2[index_2];
            index_2++;
        }
   


   }        
   
    return new_array;

}




int UserChoice(void){
    int user_choice;
    printf( "Press any key and enter to exit\n");

    scanf("%d",&user_choice);
    fflush(stdin);
    return user_choice;
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
