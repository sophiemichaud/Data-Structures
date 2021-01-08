#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

struct Performance *newPerformance(){

   //allocates sufficient memory for a Performance structure
   struct Performance *newPerformance = (struct Performance*)malloc(sizeof(struct Performance));

   //prints error message to standard error and exits if malloc fails
   if(newPerformance == NULL){
     fprintf(stderr, "Memory allocation failed.");
     exit(0);
   }
   else{
      //sets reads, writes, mallocs, and frees to zero
      newPerformance -> reads = 0;
      newPerformance -> writes = 0;
      newPerformance -> mallocs = 0;
      newPerformance -> frees = 0;
   }
   //returns the address of the structure
   return newPerformance;
}

struct Array *newArray(struct Performance *performance, unsigned int width, unsigned int capacity){

   //allocates sufficient memory for an array structure
   struct Array *theArray = (struct Array*)malloc(sizeof(struct Array));

   if(theArray == NULL){
      fprintf(stderr, "Memory allocation failed.");
      exit(0);
   }
   else{
      //sets the width and capacity to the values provided
      theArray -> width = width;
      theArray -> capacity = capacity;

      //sets the nel attribute to zero
      theArray -> nel = 0;

      //allocates width*capacity bytes of memory storing the address in data
      theArray -> data = (void *)malloc(theArray -> width * theArray -> capacity);

      //increments performance mallocs by one
      performance -> mallocs = (performance -> mallocs) + 1;
   }
   return theArray;
}

void readItem(struct Performance *performance, struct Array *array, unsigned int index, void *dest){

   //if index is greater than or equal to number of elements error and exit
   if(index >= (array -> nel)){
      fprintf(stderr, "Index greater than number of elements.");
      exit(0);
   }
   else{

      //copy array width of bytes from data offset by index to dest
      unsigned char *ptr = array -> data;
      unsigned char *ptr1 = dest;

      for(int i = 0; i < array -> width; i++){
         ptr1[i] = ptr[(index*(array -> width)) + i];
      }
      dest = ptr1;

      //adds one to performance reads
      performance -> reads = (performance -> reads) + 1;
   }
}

void writeItem(struct Performance *performance, struct Array *array, unsigned int index, void *src){

   unsigned char *ptr = src;

   //if index exceeds number of elements or exceeds or equals capacity exit
   if(index > array -> nel || index >= array -> capacity){
      fprintf(stderr, "Index exceeds number of elements or capacity.");
      exit(0);
   }
   else{

      //unsigned char pointer used for arithmetic inside for loop
      unsigned char *ptr2 = array -> data;

      //copies bytes from source to address data offset by index
      for(int i = 0; i < array -> width; i++){
         ptr2[(index *(array -> width)) + i] = ptr[i];
      }

      //sets the data pointer equal to the unsigned char pointer
      array -> data = ptr2;

      //increments nel and writes if index exactly equals nel
      if(index == array -> nel){
         array -> nel = (array -> nel) + 1;
         performance -> writes = (performance -> writes) + 1;
      }
   }
}

void contract(struct Performance *performance, struct Array *array){

   //if no elements in the array prints an error message and exits
   if(array -> nel == 0){
      fprintf(stderr, "Number of elements in the array is zero.");
      exit(0);
   }

   //otherwise decrement the number of elements in the array by 1
   else{
      array -> nel = (array -> nel) - 1;
   }
}

void freeArray(struct Performance *performance, struct Array *array){

    //frees array data and the array structure, increments performance frees
    free(array -> data);
    free(array);
    performance -> frees = (performance -> frees) + 1;
}

void appendItem(struct Performance *performance, struct Array *array, void *src){

   //uses the writeItem function to append item to the end of the array
   writeItem(performance, array,array -> nel, src);
}

void insertItem(struct Performance *performance, struct Array *array, unsigned int index, void *src){

   //void pointer to temporarily store items that need to be moved
   void *itemRead;

   //moves items in the array one position over until index is reached
   for(int i = array -> nel; i > index; i--){
      readItem(performance, array, i - 1, &itemRead);
      writeItem(performance, array, i, &itemRead);
   }

   //writes the new data to the index
   writeItem(performance, array, index,src);
}

void prependItem(struct Performance *performance, struct Array *array, void *src){

   //uses insertItem to add new data to the first element in the array
   insertItem(performance, array, 0, src);
}

void deleteItem(struct Performance *performance, struct Array *array, unsigned int index){

   void *itemRead;

   //loops through array elements past the index of the item to be removed
   for(int i=index + 1; i < array -> nel; i++){
      readItem(performance, array, i, &itemRead);
      writeItem(performance, array, i - 1, &itemRead);
   }

   //removes duplicate item at the end of the array
   contract(performance, array);
}

int findItem(struct Performance *performance, struct Array *array, int (*compar)(const void *, const void *), void *target){

   unsigned char *temp = malloc(array -> width);

   //linearly searches through the entire array until a matching element is found
   for(int i = 0; i < array -> nel; i++){

      readItem(performance,array,i, temp);

      int compar_result = compar(target, temp);

      //if the element matches, the index is returned
      if(compar_result == 0){
         free(temp);
         return i;
      }
   }
   free(temp);
   return -1;
}

int searchItem(struct Performance *performance, struct Array *array, int (*compar)(const void *, const void *), void *target){

   unsigned char *itemRead = malloc(sizeof(array -> width));
   int comparResult = 0;
   int arrayLength = array -> nel;
   int arrayStart = 0;

   //binary search algorithm to find the matching element in the array
   while(arrayStart <= arrayLength){

     int middleIndex = arrayStart +(arrayLength - arrayStart)/2;
     readItem(performance, array, middleIndex, itemRead);
     comparResult = compar(target, itemRead);

     //returns the index if the item matches
     if(comparResult == 0){
        free(itemRead);
        return middleIndex;
     }

     //looks for element in the second half of the array
     else if(comparResult < 0){
        arrayLength = middleIndex - 1;
     }

     //looks for element in the first half of the array
     else if(comparResult > 0){
        arrayStart = middleIndex + 1;
     }
   }
   return -1;
}


