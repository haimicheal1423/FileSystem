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

typedef struct VolumeControlBlock{
    int magicNum; // a magic number to identify the disk
    // char volumeName[256]; //name of the volume

    // disk layout

    int numOfFreeBlocks; // free blocks
    int sizeOfBlock;    // 512 bytes
    int rootDirectory;    // reference to the root
    int freeSpacePointer; //free space pointer using bitmap

} VolumeBlock; 

#endif
