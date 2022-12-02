/**************************************************************
* Class:  CSC-415-02 Fall 2022
* Names: Sajan Gurung, Michael Hua, Deep Dhorajiya, Ron Elijah Rivera
* Student IDs: 921149577, 918729206, 920842114, 921656746
* GitHub Name: michealhuaa
* Group Name: Team Buffer
* Project: Basic File System
*
* File: mfs.c
*
* Description: Makes functions that interacts with the file system
*
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



int allocate(int numBlocks, VolumeBlock *buffer, char*bitmap){
    int returnSpace = buffer->freeSpace; //Save location of freespace
    //printf("Allocating %d blocks ----\n", numBlocks);
    for(int i = 0; i < numBlocks; i++){
        setBit(bitmap, buffer->freeSpace); //set bits from 0 to 1
        buffer->freeSpace++; 
    }

    LBAwrite(bitmap, 5, 1);
    LBAwrite(buffer, 1, 0);
    return returnSpace; //return starting point for LBAwrite
}
//http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
//Reference

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

int resetBits(char*bitmap, int position, int numBlocks)
{
    for(int k = position; k < position+numBlocks; k++){ //iterate over each position 
        int i = k/8; //gives the index inside of array.
        int pos = k%8; //gets the position inside of the index
        
        unsigned int clearBit = 1;

        clearBit = clearBit << pos; //shift the bit
        clearBit = ~clearBit; //~ flips the bit from 1 to 0

        bitmap[i] = bitmap[i] & clearBit; //change to zero
    }
    LBAwrite(bitmap, 5, 1); //write to memory
}

