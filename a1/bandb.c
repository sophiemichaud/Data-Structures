#include <stdio.h>
#include <string.h>
#include "bandb.h"

void getbytes(unsigned char dest[], int bytes, void *src, int reverse){

   //pointer to the source
   unsigned char *ptr = src;

   //if reverse is false, copies the bytes directly from source into dest
   if(reverse == 0){
      for(int i = 0; i < bytes; i++){
         dest[i] = ptr[i];
      }
   }

   //if reverse is true, copies the bytes in reverse order from source into dest
   else if(reverse == 1){
      for(int i = bytes - 1; i >= 0; i--){
	dest[bytes-i -1] = ptr[i];
      }
   }
}

void getbits(char dest[], int bytes, void *src, int start, int end){

   char *ptr = src;
   int remainder = 0;
   int quotient = 0;
   int byteIndex = 0;

   int count = 0;

   //parses through given start and end values of given bytes
   for(int i = start; i >= end + 1; i--){

      //finds the remainder, quotient, and byte index
      remainder = i%8;
      quotient = i/8;
      byteIndex = bytes - quotient - 1;

      //calculates the bit value using bitwise shift/and operations
      int bit = (ptr[byteIndex]) & (1 << (7-remainder));

      //sets the character value to 1 if calculated bit value is not 0
      if(bit != 0){

         dest[count] = '1';
      }
      else if(bit == 0){

         dest[count] = '0';
      }
      count++;
   }
   dest[start-end] = '\0';
}

unsigned long long bits2ull(char * bits){

   //keeps track of the total integer value of the bit string
   unsigned long long bitsProcessed = 0;

   //parses through a string of 0 and 1's
   for(int i = 0; i < strlen(bits); i++){

      //if the bits is equal to 1 the value is shifted by its index value -1
      if(bits[i] == '1'){
         bitsProcessed = bitsProcessed + ((unsigned long long)1 << ((int)strlen(bits) - i - 1));
      }
   }
   return bitsProcessed;
}

long long bits2ll(char *bits){

   //keeps track of the total integer value of the bit string
   long long bitsProcessed = 0;

   //parses through a string of 0 and 1's
   for(int i = 0; i < strlen(bits); i++){

      //if the bit is equal to 1 value is shifted by its index
      if(bits[i] == '1'){

         //for two's complement representation the int value is negative for the first bit
         if(i == 0){

            bitsProcessed = bitsProcessed - ((unsigned long long)1 << ((int)strlen(bits) - i - 1));
         }

         //any other bit will be shifted and given a positive int value
         else{

            bitsProcessed = bitsProcessed + ((unsigned long long)1 << ((int)strlen(bits) - i - 1));
         }
      }
   }

   return bitsProcessed;
}

void spff(char *sign, char *exponent, char *significand, float *src){

   //gets the full string of bits using the getBits function
   char allBits[33];
   getbits(allBits, 4, src, 31, -1);

   //sign string gets the first bit
   sign[0] = allBits[31];
   sign[1] = '\0';

   int counter1 = 30;

   //exponent string gets the next eight bits
   for(int i = 0; i < 8; i++){
      exponent[i] = allBits[counter1];
      counter1--;
   }
   exponent[8] = '\0';

   int counter2 = 22;

   //significand string gets the last 23 bits
   for(int i = 0; i < 23; i++){
      significand[i] = allBits[counter2];
      counter2--;
   }
   significand[23] = '\0';
}

void dpff(char *sign, char *exponent, char *significand, double *src){

   //gets the full string of bits using the getBits function
   char allBits[65];
   getbits(allBits, 8, src, 63, -1);

   //adds the first bit into the sign string
   sign[0] = allBits[63];
   sign[1] = '\0';

   int counter1 = 62;

   //adds the next 11 bits to the exponent string
   for(int i = 0; i < 11; i++){
      exponent[i] = allBits[counter1];
      counter1--;
   }
   exponent[11] = '\0';

   int counter2 = 51;

   //adds the last 52 bits to the significand string
   for(int i = 0; i < 52; i++){
      significand[i] = allBits[counter2];
      counter2--;
   }
   significand[52] = '\0';
}
