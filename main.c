#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include <mpi.h>
#include <math.h>
#include <sys/time.h> 

int main(int argc, char *argv[])  {


    int rank, nproc;   
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if (rank==MASTER){

        printf("\nParallel Quicksort\n");
        printf("\nwhat would you like to do");
    
        while(1)
        {
            switch(UserChoice()){
                case 1: {          
                        Serial_Qsort();                 
                        printf("Solve serial \n" );
                        break;}
                case 2: {                
                        printf("Solve naive \n" );  
                        break;}
                case 3: {
                        printf("Solve non naive \n" );  
                        break;}
                case 4:{exit(0);}           
                default: {                     
                        printf("Possible error in input. Exiting");
                        MPI_Finalize();
                        exit(0);}
            }
        }
    }else{
        MPI_Finalize();
    }

    return 0;
    
}

int UserChoice(void){
    int user_choice;
    printf( "1)----   Solve serial   ----\n"
            "2)----   Solve naive    ----\n"
            "3)----   Solve non naive   ----\n"            
            "4)----   Exit    ----\n");

    scanf("%d",&user_choice);
    fflush(stdin);
    return user_choice;
}