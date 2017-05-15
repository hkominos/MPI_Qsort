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
    int *Array,*local_array,*rcv_array;
    double start_time,total;   
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    
    MPI_Status status;

    start_time = MPI_Wtime();
    
    
    if (rank == 0){

        printf( "Give array size\n");
        scanf("%d",&array_size);
        fflush(stdin);

        if(array_size%nproc!=0){
            printf("Size is not divisible by nproc. aborting");
            MPI_Abort(MPI_COMM_WORLD,1);
            exit(0);}

        //for testing
        //array_size =50000 

        Array = (int*)calloc(array_size , sizeof(int));

        srand48((unsigned int)time(NULL));
        // Initialize the array with random values
        for (i=0;i<array_size;i++){
            Array[i] = drand48() * 1000000;
        }
    }    

    for (i=0;i<array_size;i++){
            printf("%d\n",Array[i] );
        }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&array_size, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    local_array_size = array_size/nproc;
    if (array_size % nproc){
        local_array_size++;
    }

    local_array=(int*)calloc(local_array_size , sizeof(int));
    MPI_Scatter(Array, local_array_size, MPI_INT, local_array, local_array_size, MPI_INT, 0, MPI_COMM_WORLD);
    myqsort(0,local_array_size-1,local_array);

   

    int steps=(int)log2((int)nproc);

    //steps=2;
    int dest=1;
    for(i=1;i<=steps;i++){
        
        int new_array_size=local_array_size*i;
        
        if (rank % (2*i)){
            MPI_Send(local_array, new_array_size, MPI_INT, rank-dest, 10*i, MPI_COMM_WORLD); 
            break;
        }
    
        else {
            rcv_array=(int*)calloc(new_array_size , sizeof(int));
            MPI_Recv(rcv_array, new_array_size, MPI_INT, rank+dest, 10*i, MPI_COMM_WORLD, &status);
            sleep(2);
            printf("\n\n\n\n");

            //printf("new array size is %d and step is  %d  proc %d\n",new_array_size,i,rank )
            printf("Value send is %lf value point by is %d",&local_array,local_array[0] );;
            local_array=merge_arrays(&local_array,rcv_array,new_array_size);
            //local_array=merge(local_array,new_array_size,rcv_array,new_array_size);
            free(rcv_array);
      
        }


        


    dest=2*i;
        
    }



       
    //UserChoice();
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    free(local_array);
    if (rank==0){
        //free(Array);
printf("\n\n\n\n");
    for (i=0;i<array_size;i++){
            printf("%d\n",Array[i] );
        }

    }
    
    exit(0);

}




int *merge_arrays(void *headarray1, int *array2, int array_size){

    int *array1=(int*)headarray1;
    printf("Value received is %lf value point by is %d",headarray1,array1 );
    int start_of_array1=array1[0];
    int start_of_array2=array2[0];
    int i;
    int index_1=0;
    int index_2=0;

    printf("Array1 is\n");

    for (i=0;i<array_size;i++){
            printf("%d\n",array1[i] );
        }

        sleep(2);
            printf("\n\n\n\n");
            printf("Array2 is\n");

    for (i=0;i<array_size;i++){
            printf("%d\n",array2[i] );
        }

   

    if(start_of_array2<start_of_array1){
        int* temp=array2;
        array2=array1;
        array1=temp;
    }



 

    int* new_array=(int*)calloc(array_size*2 , sizeof(int));
    
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
   sleep(2);
   printf("\n\n\n\n");
            printf("final is\n");
   
   for (i=0;i<array_size*2;i++){
            printf("%d\n",new_array[i] );
        }

   
    return new_array;

}




int UserChoice(void){
    int user_choice;
    printf( "Press any key to exit\n");

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
