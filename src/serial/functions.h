#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#define MASTER                  0

#define YES                     1
#define NO                      0

    
int UserChoice(void);
void Serial_Qsort(void);
int isSorted(int *Array, int size);
void myqsort(int low, int high,int *Array);
int partition(int low, int high, int* Array);

#endif /* FUNCTIONS_H_ */