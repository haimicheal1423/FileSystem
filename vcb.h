/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Sajan Gurung, Michael Hua, Deep Dhorajiya,Ron Elijah Rivera
* Student IDs: 921149577, 918729206, 920842114
* GitHub Name: michealhuaa
* Group Name: Team Buffer
* Project: Basic File System
*
* File: vcb.h
*
* Description: Volume Control Block Header file
**************************************************************/

#ifndef _VCB_H
#define _VCB_H

typedef struct VolumeBlock 
{
    int freeSpace; //location of freespace
    int numBlocks; //number of blocks in memory
    int bSize; //size of each block in memory
    int rootDir; //location of root directory
    int magicNum; //signature
}VolumeBlock; 

#endif
