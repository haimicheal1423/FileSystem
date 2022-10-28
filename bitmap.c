/**************************************************************
* Class:  CSC-415-0# Fall 2021
* Names: 
* Student IDs:
* GitHub Name:
* Group Name:
* Project: Basic File System
*
* File: bitmap.c
*
* Description: 
*
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include <math.h>
#include "bitmap.h"

int allocate(int numOfBlocks, VCB *buffer, char*bitmap){
    int returnSpace = buffer->freeSpacePointer; //Save location of freespace

    for(int i = 0; i < numOfBlocks; i++){
        setBit(bitmap, buffer->freeSpacePointer); //set bits from 0 to 1
        buffer->freeSpacePointer++; 
    }

    LBAwrite(bitmap, 5, 1);
    LBAwrite(buffer, 1, 0);
    return returnSpace; //return starting point for LBAwrite
}


void setBit(char*bitmap, int position){
    //Each char is 8 bits, we can store 8 0s or 1s in 1 char
    int i = position/8; //gives the index inside of array.
    int pos = position%8; //gets the position inside of the index

     //<< will shift the position of the 1 to the position within index
    bitmap[i] |= (1 << pos); //now set the position inside the bitmap
}

int isUsed(char*bitmap, int position)
{
    int i = position/8; //gives the index inside of array.
    int pos = position%8; //gets the position inside of the index
    
    if(bitmap[i] & (1 << (pos))){ //check if space is used
        return 1; //true
    }
    else{
        return 0; //false
    }     
    return -1;
}

int resetBits(char*bitmap, int position, int numOfBlocks)
{
    for(int k = position; k < position+numOfBlocks; k++){ //iterate over each position 
        int i = k/8; //gives the index inside of array.
        int pos = k%8; //gets the position inside of the index
        
        unsigned int clearBit = 1;

        clearBit = clearBit << pos; //shift the bit
        clearBit = ~clearBit; //~ flips the bit from 1 to 0

        bitmap[i] = bitmap[i] & clearBit; //change to zero
    }
    LBAwrite(bitmap, 5, 1); //write to memory
}