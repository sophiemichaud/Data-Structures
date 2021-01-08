#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

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

/* Allocate the memory for a HashTable and initialize the parameters */
struct HashTable *createTable( struct Performance *performance, unsigned int capacity, int (*hash)( void *, int ), int (*compar)(const void *, const void *) ){

    //allocates sufficient memory for a HashTable structure
    struct HashTable *newTable = malloc(sizeof(struct HashTable));

    //prints error message to standard error and exits if malloc fails
    if(newTable == NULL){
        fprintf(stderr, "Memory allocation failed at table.");
        exit(0);
    }
    else{
        //copies over values of capacity, hash, and compar
        newTable -> capacity = capacity;
        newTable -> hash = hash;
        newTable -> compar = compar;

        //sets nel to zero
        newTable -> nel = 0;

        //sets data to a newly allocated block of memory sufficiently large enough to store capacity many pointers
        newTable -> data = malloc(capacity * sizeof(void*));

        //prints error message to standard error and exits if malloc fails
        if(newTable -> data == NULL){
            fprintf(stderr, "Memory allocation failed at data.");
            exit(0);
        }
        else{

            //sets each pointer in the data array equal to NULL
            for(int i = 0; i < newTable -> capacity; i++){
                (newTable -> data)[i] = NULL;
            }

            //increments the performance structure reads by capacity
            performance -> reads = (performance -> reads) + capacity;
        }

        performance -> mallocs = (performance -> mallocs) + 1;
    }
    return newTable;
}

/* Adds an element to the HashTable with linear probing when a collision occurs */
void addElement( struct Performance *performance, struct HashTable *table, void *src ){

    //If number of elements is equal to the capacity, prints error message then exits
    if(table -> nel >= table -> capacity){
        fprintf(stderr, "Number of elements in table equals capacity.");
        exit(0);
    }
    else{

        /*pass the value of src and the value of capacity in the structure pointed to by table
           use the function hash in the structure pointed to by table to calculate an index in the pointer array data */
        int index = (table -> hash(src, table -> capacity));

        //beginning at index, increments until null pointer is found
        while((table -> data)[index] != NULL){
            //index is incremented
            index++;

            //reads are incremented by 1 each time the index is incremented
            performance -> reads = (performance -> reads) + 1;

            //if index reaches capacity, sets it to zero and continue
            if(index >= table -> capacity){
                index = 0;
            }
        }

        //copies the src to the index in the array
        (table -> data)[index] = src;

        //increments nel by 1
        table -> nel = (table -> nel) + 1;

        //writes in the performance structure are incremented by 1
        performance -> writes = (performance -> writes) + 1;
    }
}

/* Find an element in the HashTable and return its index */
int getIdx( struct Performance *performance, struct HashTable *table, void *src ){

    /*pass the value of src and the value of capacity in the structure pointed to by table
      use the function hash in the structure pointed to by table to calculate an index in the pointer array data */
    int index = table -> hash(src, table -> capacity);

    //beginning at the index, compar is used to determine if src pointer = data pointer
    for(int i = index; i < table -> capacity; i++){

        if((table -> data)[i] != NULL){
            int compar_result = (table -> compar(src, (table -> data)[i]));

            //reads are incremented by 1 each time the index is incremented
            performance -> reads = (performance -> reads) + 1;

            //if there is a match (compar returns 0), returns the index
            if(compar_result == 0){
                return i;
            }
            
            //if index reaches capacity, sets it to zero and continue
            else if(i >= table -> capacity){
                i = 0;
            }

            else if(i == (index - 1)){
                return -1;
            }
        }
    }
    return 0;
}

/* Frees the HashTable */
void freeTable( struct Performance *performance, struct HashTable *table ){

    free(table -> data);

    //frees the table itself
    free(table);

    //increments frees in performance structure by 1
    performance -> frees = (performance -> frees) + 1;
}

/* Finds an element in the HashTable and returns its pointer */
void *getElement( struct Performance *performance, struct HashTable *table, void *src ){

    int index = getIdx(performance, table, src);

    if(index == -1){
        return NULL;
    }
    else{
        return ((table -> data)[index]);
    }
    return NULL;
}

/* Removes an element from the HashTable */
void removeElement( struct Performance *performance, struct HashTable *table, void *target ){

    int index = getIdx(performance, table, target);

    (table -> data)[index] = NULL;

    //increments writes in performance structure by 1
    performance -> writes = (performance -> writes) + 1;

    //decrements nel in table structure by 1
    table -> nel = (table -> nel) - 1;
}

/* Computes the hash accuracy of the contents of the HashTable */
int hashAccuracy( struct HashTable *table ){

    //calculates the difference between every entry in the HashTable's index and the value computed by the hash function
    int index  = 0;
    int counter = 0;
    int hashIndex = 0;
    
    for(int i = index; i < table -> capacity; i++){

        //if the index is less than the hash functions value, add the index to the difference between the hash function's value and capacity
        if((table -> data)[i] != NULL){
            hashIndex = table -> hash((table -> data)[i], table -> capacity);

            if(i < hashIndex){
                counter = counter + (hashIndex - table -> capacity) + i;
            }
            else if(i >= hashIndex){
                counter = counter + (i - hashIndex);
            }
        }
    }
    return counter;
}

/* Moves the pointers in the HashTable to get a better hashAccuracy */
void rehash( struct HashTable *table ){
    
    //calculates the difference between every entry in the HashTable's index and the value computed by the hash function
    int index  = 0;
    int hashIndex = 0;
    
    for(int i = index; i < table -> capacity; i++){

        //if the index is less than the hash functions value, add the index to the difference between the hash function's value and capacity
        if((table -> data)[i] != NULL){
            hashIndex = table -> hash((table -> data)[i], table -> capacity);

            if((table -> data)[hashIndex] == NULL){
                (table -> data)[hashIndex] = (table -> data)[i];
            }
            else{
                for(int j = 0; j < table -> capacity; j++){

                    int newHash = table -> hash((table -> data)[hashIndex + (j * j)], table -> capacity);

                    if((table -> data)[newHash] == NULL){
                        (table -> data)[newHash] = (table -> data)[i];
                        break;
                    }
                }
            }
        }
    }
}