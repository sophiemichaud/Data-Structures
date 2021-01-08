#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* creates a performance structure */
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

/* adds an item to the head of the list */
void push(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width){

   //mallocs a new struct Node structure
   struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));

   //prints error message to standard error and exits if malloc fails
   if(newNode == NULL){
      fprintf(stderr, "Memory allocation failed.");
      exit(0);
   }
   else{
      //mallocs width bytes of data and saves the address in data
      newNode -> data = malloc(width);

      //copies width bytes of data from src to data
      unsigned char *dataPtr = newNode -> data;
      unsigned char *srcPtr = src;

      for(int i = 0; i < width; i++){
        dataPtr[i] = srcPtr[i];
      }
      newNode -> data = dataPtr;

      //sets the next pointer of the structure to list_ptr
      newNode -> next = *list_ptr;

      //stores the address of the structure in the pointer pointed to list_ptr
      *list_ptr = newNode;

      //mallocs and writes of performance are incremented by 1
      performance -> mallocs = (performance -> mallocs) + 1;
      performance -> writes = (performance -> writes) + 1;
   }
}

/* copies data from the head of the list into dest */
void readHead(struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width){

   //if the list is empty an error message is printed to standard error stream and exits
   if(*list_ptr == NULL){
      fprintf(stderr, "Error: List is empty.");
      exit(0);
   }
   else{
      //copies width bytes from the data pointer into dest
      unsigned char *destPtr = dest;
      struct Node *tempNode = *list_ptr;
      unsigned char *dataPtr = tempNode -> data;

      for(int i = 0; i < width; i++){
         destPtr[i] = dataPtr[i];
      }
      dest = destPtr;

      //reads are incremented by 1
      performance -> reads = (performance -> reads) + 1;
   }
}

/* removes an item from the head of the list */
void pop(struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width){

   //if the list is empty an error message is printed to standard error stream and exits
   if(*list_ptr == NULL){
      fprintf(stderr, "Error: List is empty.");
      exit(0);
   }
   else{
      //copies width bytes of data from data to dest
      unsigned char *destPtr = dest;
      struct Node *tempNode = *list_ptr;
      unsigned char *dataPtr = tempNode -> data;

      for(int i = 0; i < width; i++){
         destPtr[i] = dataPtr[i];
      }
      dest = destPtr;

      //updates the list ptr to the next node in the list
      *list_ptr = tempNode -> next;

      //frees the node structure that used to be first
      free(tempNode -> data);
      free(tempNode);

      //frees and reads are incremented by 1
      performance -> frees = (performance -> frees) + 1;
      performance -> reads = (performance -> reads) + 1;
   } 
}

/* returns pointer to the pointer to the second item in a list */
struct Node **next(struct Performance *performance, struct Node **list_ptr){

   struct Node **ptr = NULL;
   //if the list is empty an error message is printed to standard error stream and exits
   if(*list_ptr == NULL){
      fprintf(stderr, "Error: List is empty.");
      exit(0);
   }
   else{
      //gets address of the next pointer from the node pointed to by list pointer
      struct Node *firstNode = *list_ptr;
      ptr = &(firstNode -> next);

      //increments reads by 1
      performance -> reads = (performance -> reads) + 1;
   }
   return ptr;
}

/* determines if a list is empty */
int isEmpty(struct Performance *performance, struct Node **list_ptr){

   //if the list is empty return 1, otherwise return 0
   if(*list_ptr == NULL){
      return 1;
   }
   else{
      return 0;
   }
}

/* pops items off the list until the list is empty */
void freeList(struct Performance *performance, struct Node **list_ptr){

   struct Node *tempNode = *list_ptr;
   void *dest = NULL;
   int width = sizeof(tempNode -> data);

   while(isEmpty(performance, list_ptr) != 1){
      pop(performance, list_ptr, &dest, width);
   }

}

/* finds a node from a given index */
void readItem(struct Performance *performance, struct Node **list_ptr, unsigned int index, void *dest, unsigned int width){

   struct Node **currentNode = list_ptr;

   //uses next and readHead functions to read item at given index
   for(int i = 0; i < index; i++){
      currentNode = next(performance, currentNode);
   }
   readHead(performance, currentNode, dest, width);
}

/* adds an element to the end of the list */
void appendItem(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width){

   struct Node **currentNode = list_ptr;

   //uses next and push functions to add the item to the end of the list
   while(isEmpty(performance, currentNode) != 1){
      currentNode = next(performance, currentNode);
   }
   push(performance,currentNode, src, width);
}

/* inserts an element at a given index of the list */
void insertItem(struct Performance *performance, struct Node **list_ptr, unsigned int index, void *src, unsigned int width){

   struct Node **currentNode = list_ptr;

   //uses next and push functions to insert an item at given index
   for(int i = 0; i < index; i++){
      currentNode = next(performance, currentNode);
   }
   push(performance,currentNode,src,width);
}

void prependItem(struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width){

   //uses insertItem to insert data at position 0
   insertItem(performance, list_ptr, 0, src, width);
}

void deleteItem(struct Performance *performance, struct Node **list_ptr, unsigned int index){

   struct Node **currentNode = list_ptr;
   struct Node *tempNode = *list_ptr;
   void *dest = NULL;
   int width = sizeof(tempNode -> data);

   //uses next and pop functions to delete an item at given index
   for(int i = 0; i < index; i++){
      currentNode = next(performance, currentNode);
   }
   pop(performance,currentNode,&dest, width);
}

int findItem(struct Performance *performance, struct Node **list_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width){

   struct Node **currentNode = list_ptr;
   unsigned char *temp = malloc(width);
   int counter = 0;

   //loops through the list while the list is not empty
   while(isEmpty(performance, currentNode) != 1){

      //uses readHead to retreive an element from the list
      readHead(performance, currentNode, temp, width);

      //uses the compar function to determine if matching elements
      int compar_result = compar(target, temp);

      //if the elements match, pointer is free'd and index of element
      if(compar_result == 0){
         free(temp);
         return counter;
      }

      //otherwise, the current node and counter are updated and loops again
      currentNode = next(performance, currentNode);
      counter++;
   }

   //if the list has no matching elements temp is free'd and the function returns -1
   free(temp);
   return -1;
}

