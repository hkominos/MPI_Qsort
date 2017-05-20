#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include <mpi.h> 
#include <time.h> 
#include <unistd.h>
#include <math.h>
#include <string.h>

//int *merge(int *v1, int n1, int *v2, int n2);

int main(int argc, char *argv[])  {


    int rank, nproc,array_size,i,chunk_size,global_pivot,current_rank,current_nproc;
    int *Array,*local_array,*received_array;
    double start_time,total;   
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    
    MPI_Status status;
    MPI_Comm current_comm,new_comm;    
    
    
    if (rank == 0){

        printf( "Give array size\n");
        scanf("%d",&array_size);
        //array_size=16;
        fflush(stdin);       

        if(array_size%nproc!=0){
            printf("Size is not divisible by nproc. aborting");
            MPI_Abort(MPI_COMM_WORLD,1);
            exit(0);}

        //largest in supported without overflow is 2,147,483,647
        
        start_time = MPI_Wtime();
        Array = (int*)malloc(array_size *sizeof(int));


        srand48((unsigned int)time(NULL));
        // Initialize the array with random values
        for (i=0;i<array_size;i++){
            //Array[i] = drand48() * 100000000;
            Array[i] = drand48() * 100;

        }
    }    


    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&array_size, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    chunk_size = array_size/nproc;
    local_array=(int*)malloc(chunk_size * sizeof(int));
    MPI_Scatter(Array, chunk_size, MPI_INT, local_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    
    
    const int MAX_NUMBERS = chunk_size*nproc;
    received_array=(int*)malloc(MAX_NUMBERS * sizeof(int));
    int steps=(int)log2((int)nproc);
 



     
    
    MPI_Comm_dup( MPI_COMM_WORLD, &current_comm );
    MPI_Comm_rank(current_comm, &current_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &current_nproc);

    int current_array_size=chunk_size;
    int* low_array=NULL;
    int* high_array=NULL;
    int low_array_size=0;
    int high_array_size=0;
    int MPI_LOW_TAG=111;
    int MPI_HIGH_TAG=222;
    int LOW_SET=1;
    int HIGH_SET=10;
    int received_array_size=0;
      
    for(i=1;i<=steps;i++){
        if(current_rank==0){global_pivot=local_array[2];}
        MPI_Bcast(&global_pivot, 1, MPI_INT, 0, current_comm);
        
        partition_with_pivot(0,current_array_size,local_array,global_pivot,&low_array,&high_array,&low_array_size,&high_array_size,current_rank);
        if (local_array!=NULL)free(local_array);

        float middle=current_nproc/2;

        if (current_rank<middle){
            //printf("SEND at %lf\n",MPI_Wtime());
            MPI_Send(high_array, high_array_size, MPI_INT, current_rank+middle, MPI_HIGH_TAG, current_comm);       
            MPI_Recv(received_array, MAX_NUMBERS, MPI_INT, current_rank+middle, MPI_LOW_TAG, current_comm, &status); 
            MPI_Get_count(&status, MPI_INT, &received_array_size);
            //printf("RCV at %lf\n with size %d",MPI_Wtime(),received_array_size);       
            local_array=merge_arrays(low_array,low_array_size,received_array,received_array_size);
            current_array_size=received_array_size+low_array_size;
            printf("Merge at %lf with array size %d \n",MPI_Wtime(),current_array_size);
            myqsort(0, current_array_size-1,local_array);

            for(i=0;i<=current_array_size-1;i++){            
            printf("%d\n",local_array[i]);
            }


            
            printf("Sorted at %lf\n",MPI_Wtime());




            
                
        }else
        {       
            //printf("RCV at %lf\n with size %d",MPI_Wtime(),received_array_size);     
            MPI_Recv(received_array, MAX_NUMBERS, MPI_INT, current_rank-middle, MPI_HIGH_TAG, current_comm, &status);           
            MPI_Get_count(&status, MPI_INT, &received_array_size);
            MPI_Send(low_array, low_array_size, MPI_INT, current_rank-middle, MPI_LOW_TAG, current_comm); 
            //printf("SEND at %lf\n",MPI_Wtime());           
            local_array=merge_arrays(high_array,high_array_size,received_array,received_array_size);
            current_array_size=received_array_size+high_array_size;
            printf("Merge at %lf with array size %d \n",MPI_Wtime(),current_array_size);
            //myqsort(0, current_array_size-1,local_array);
            printf("un Sorted at %lf\n",MPI_Wtime());

                
        }


        int color;
        if (current_rank<middle){color=LOW_SET;}
            else{color=HIGH_SET;}

        MPI_Comm_split(current_comm, color, current_rank, &new_comm);

        MPI_Comm_dup( new_comm, &current_comm );
        MPI_Comm_rank(current_comm, &current_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &current_nproc);

        
         /*
        for(i=0;i<=current_array_size-1;i++){            
            printf("%d\n",low_array[i]);
            }




            

        printf("Sizes received are high %d and low %d\n",high_array_size,low_array_size );
         printf("low array is \n");
        for(i=0;i<=low_array_size-1;i++){            
            printf("%d\n",low_array[i]);
            }
            printf("hign array is \n");
        for(i=0;i<=high_array_size-1;i++){
            printf("%d\n",high_array[i]);
                        }      
        */
        



    }

    //printf("time is %d\n",current_array_size );
    //myqsort(0, current_array_size,local_array);

    //printf("Processor with rank %d reports %d\n",rank,isSorted(local_array,current_array_size-1) );
       
    
    MPI_Barrier(MPI_COMM_WORLD);
    

    if (rank==0){
        total = MPI_Wtime() - start_time;
        printf("\nTotal time taken: %f seconds",total);
        //int Result=isSorted(local_array,array_size);
        //if (Result==YES)printf("Array Sorted! \n");
        //free(local_array);
        UserChoice();
    }
    else{
        //if (local_array!=NULL)free(local_array);
    }

    MPI_Finalize();
    
    exit(0);

}




void partition_with_pivot(int low, int high, int* Array,int pivot,int** low_array, int** high_array, int* low_array_size, int* high_array_size,int current_rank){
    int i,temp_value;   


    if(current_rank==0){
        for(i=low;i<=high-1;i++){
            if(Array[i]==pivot){
                swap(i,high-1,Array);
                break;
            }

        }
    }else{
    
    temp_value=Array[high-1];
    Array[high-1]=pivot;

    }

    int index=low;
    int steps=0;

    
    
    for(i=low;i<=high-2;i++){

        if(Array[i]<=pivot){
            swap(i,index,Array);
            index++;
        }
        steps++;        
    }

    if (index!=low)
    swap(index,high-1,Array);
    
    else if(index==low && steps!=0)
        swap(index,high-1,Array);


    if(current_rank!=0){
        if (temp_value<=pivot){
            
            Array[index]=temp_value;
            *low_array_size=index+1;
            *high_array_size=high-*low_array_size;
        }else{
            
            Array[index]=temp_value;
            *low_array_size=index;
            *high_array_size=high-*low_array_size;
        }
    }
    else{

        *low_array_size=index+1;
        *high_array_size=high-*low_array_size;
    }
   

    int lowsize=*low_array_size;
    int highsize=*high_array_size;
    

    if(lowsize!=0 && high_array_size!=0){
    *low_array=(int *)malloc(sizeof(int)*lowsize);
    memcpy(*low_array, Array, sizeof(int)*lowsize);
    *high_array=(int *)malloc(sizeof(int)*highsize);
    memcpy(*high_array, &Array[lowsize], sizeof(int)*highsize);
   
    
    }else if(lowsize==0 && highsize!=0){

        *high_array=(int *)malloc(sizeof(int)*highsize);
        memcpy(*high_array, Array, sizeof(int)*highsize);       


    }else {

    *low_array=(int *)malloc(sizeof(int)*lowsize);
    memcpy(*low_array, Array, sizeof(int)*lowsize);


    }




}


int _getelement(){
    return 2;
}





int* merge_arrays(int *array1, int array1_size,int *array2, int array2_size){

    int* new_array=(int*)malloc((array1_size+array2_size)* sizeof(int));
    int i,temp;

    for(i=0;i<array1_size;i++){        
        new_array[i]=array1[i];
        //temp=i;
    }
    temp=array1_size;
    for(i=0;i<array2_size;i++){        
        new_array[temp+i]=array2[i];
    }

    free(array1);
    free(array2);  

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
