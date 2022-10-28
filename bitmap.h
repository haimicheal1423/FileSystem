/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Sajan Gurung, Michael Hua, Deep Dhorajiya,Ron Elijah Rivera
* Student IDs: 921149577, 918729206, 920842114
* GitHub Name: michealhuaa
* Group Name: Team Buffer
* Project: Basic File System
*
* File: bitmap.h
*
* Description: Bitmap structure header file
*
**************************************************************/

#ifndef _bitmap_h
#define _bitmap_h

#include "vcb.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

int allocate(int numBlocks, VCB *buffer, char*bitmap);
void setBit(char*bitmap, int position);
int isUsed(char*bitmap, int position);
int resetBits(char*bitmap, int position, int numBlocks);

#endif