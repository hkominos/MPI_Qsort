#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#define MASTER                  0

#define YES                     1
#define NO                      0



typedef struct LOCAL_ARRAY{

    int struct_array_size;
    int* struct_array;

} LOCAL_ARRAY_STRUCT;



    
int UserChoice(void);
void Serial_Qsort(void);
int isSorted(int *Array, int size);
void myqsort(int low, int high,int *Array);
int partition(int low, int high, int* Array);
int *merge_arrays(int* array1, int *array2, int array_size);
//void* merge_arrays(int *array1, int *array2, int array_size,int* new_array_ptr);
void partition_with_pivot(int low, int high, int* Array,int pivot,int** low_array, int** high_array, int* low_array_size, int* high_array_size,int current_rank);
int _getelement();
void swap(int num1, int num2,int *Array);



#endif /* FUNCTIONS_H_ */