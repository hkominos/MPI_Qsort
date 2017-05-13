#include <stdlib.h>
#include <stdio.h>

#include "functions.h"


/*
int sudoku_size=0;
int *ptr_sudoku_size=NULL,*Board=NULL;
int *solved_board=NULL;
*/

int main(){
    

    printf("\nParallel Quicksort\n");
    printf("\nwhat would you like to do");
    
    while(1)
    {
        switch(UserChoice()){
            case 1: {                  
                    
                    printf("Solve serial \n" );  

                    break;}
            case 2: {                
                    printf("Solve naive \n" );  
                    break;}
            case 3: {
                    printf("Solve non naive \n" );  
                    break;}
            case 4:{
                    exit(0);
                    
                    }
            
            default: {
                     
                     printf("Possible error in input. Exiting");
                     exit(0);}
        }

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