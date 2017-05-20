#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include <mpi.h>
#include <math.h>
#include <sys/time.h> 

int main(int argc, char *argv[])  {

    
    Serial_Qsort();   
    UserChoice();
    exit(0);

}

int UserChoice(void){
    int user_choice;
    printf( "Press any key to exit\n");

    scanf("%d",&user_choice);
    fflush(stdin);
    return user_choice;
}