#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

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

/* Adds a node to a pointer */
void attachNode( struct Performance *performance, struct Node **node_ptr, void *src, unsigned int width ){

   //mallocs a new struct Node structure
   *node_ptr = malloc(sizeof(struct Node));

   //prints error message to standard error and exits if malloc fails
   if(*node_ptr == NULL){
      fprintf(stderr, "Memory allocation failed.");
      exit(0);
   }
   else{

      //mallocs width bytes of data and saves the address in data
      (*node_ptr) -> data = malloc(width);

      //copies width bytes of data from src to data
      unsigned char *dataPtr = (*node_ptr) -> data;
      unsigned char *srcPtr = src;

      for(int i = 0; i < width; i++){
        dataPtr[i] = srcPtr[i];
      }
      (*node_ptr) -> data = dataPtr;

      //sets the lower and higher pointers in the structure to NULL
      (*node_ptr) -> lt = NULL;
      (*node_ptr) -> gte = NULL;

      //mallocs and writes of performance are incremented by 1
      performance -> mallocs = (performance -> mallocs) + 1;
      performance -> writes = (performance -> writes) + 1;
   }
}

int comparNode( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target ){

   //uses the compar function to determine if matching elements
   int comparResult = compar(target, (*node_ptr) -> data);

   //reads in the performance structure are incremented by 1
   performance -> reads = (performance -> reads) + 1;

   return comparResult;
}

/* determines the next node in the tree to visit */
struct Node **next( struct Performance *performance, struct Node **node_ptr, int direction ){

   struct Node **nextNode = NULL;

   //prints error message to standard error and exits if tree is empty
   if(*node_ptr == NULL){

      fprintf(stderr, "The tree is empty.");
      exit(0);
   }
   else{

      //returns the lt node or the gte node depending on the value of direction
      if(direction < 0){

         nextNode = &((*node_ptr) -> lt);
      }
      else if(direction >= 0){

         nextNode = &((*node_ptr) -> gte);
      }

      //reads in the performance structure are incremented by 1
      performance -> reads = (performance -> reads) + 1;
   }
   return nextNode;
}

/* copies data from an node in the tree into dest */
void readNode( struct Performance *performance, struct Node **node_ptr, void *dest, unsigned int width ){

   //prints error message to standard error and exits if tree is empty
   if(*node_ptr == NULL){

      fprintf(stderr, "The tree is empty.");
      exit(0);
   }
   else{

      //copies width bytes of data from the data pointer into dest
      unsigned char *dataPtr = (*node_ptr) -> data;
      unsigned char *destPtr = dest;

      for(int i = 0; i < width; i++){
        dataPtr[i] = destPtr[i];
      }
      dest = destPtr;

      //reads in the performance structure are incremented by 1
      performance -> reads = (performance -> reads) + 1;
   }
}

/* Removes an item from a tree consisting of only one node */
void detachNode( struct Performance *performance, struct Node **node_ptr ){

   //prints error message to standard error and exits if tree is empty
   if(*node_ptr == NULL){

      fprintf(stderr, "The tree is empty.");
      exit(0);
   }
   else{

      //frees the node structure that used to be in the tree
      free((*node_ptr) -> data);
      free(*node_ptr);

      //frees in the performance structure are incremented by 1
      performance -> frees = (performance -> frees) + 1;
   }
}

/* checks if node pointer is NULL */
int isEmpty( struct Performance *performance, struct Node **node_ptr ){

   //if the pointer pointed to by node_ptr is empty return 1, otherwise return 0
   if(*node_ptr == NULL){
      return 1;
   }
   else{
      return 0;
   }
}

/* Adds an item to a tree in the appropriate spot */
void addItem( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *src, unsigned int width ){

   //loops through the nodes of the tree using the compar function and the next function
   struct Node **currentNode = node_ptr;

   while(isEmpty(performance, currentNode) != 1){

      int result = comparNode(performance, currentNode, compar, src);
      currentNode = next(performance, currentNode, result);
   }
   attachNode(performance, currentNode, src, width);
}

/* Removes all the items from the tree */
void freeTree( struct Performance *performance, struct Node **node_ptr ){

   if(*node_ptr){
      freeTree(performance, next(performance, node_ptr, -1));
      freeTree(performance, next(performance, node_ptr, 1));
      detachNode(performance, node_ptr);
   }
}

int searchItem( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width ){

   int result = 1;
   struct Node **currentNode = node_ptr;
   while(*currentNode != NULL || result == 0){

      result = comparNode(performance, currentNode, compar, target);
      if(result == 0){
         
         readNode(performance, node_ptr, target, width);
         return 1;
      }
      else{
         if(result < 0){
            currentNode = next(performance, currentNode, -1);
         }
         else{
            currentNode = next(performance, currentNode, 1);
         }
      }
   }
   return 0;
}