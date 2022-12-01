/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Sajan Gurung, Michael Hua, Deep Dhorajiya,Ron Elijah Rivera
* Student IDs: 921149577, 918729206, 920842114
* GitHub Name: michealhuaa
* Group Name: Team Buffer
* Project: Basic File System
*
 * File: fsInit.c
 *
 * Description: Main driver for file system assignment.
 *
 * This file is where you will start and initialize your system
 *
 **************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "bitmap.h"
#include "bitmap.c"
#include "fsInit.h"
#include <time.h>
#include <math.h>
#include "directory_entry.h"

VCB *VCBbuffer;
char *bitmap;

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */
	int deSize = 60;
	int deNum = 51;
	int bitmapSize = ceil(((double)numberOfBlocks / 8) / (double)blockSize);
	VCBbuffer = malloc(blockSize);
	bitmap = malloc(bitmapSize * blockSize);

	LBAread(VCBbuffer, 1, 0);

	if (VCBbuffer->magicNum != 999)
	{ // check for magic number
		printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);

		// Initialize VCB
		VCBbuffer->numOfBlocks = numberOfBlocks;
		VCBbuffer->sizeOfBlock = blockSize;
		VCBbuffer->magicNum = 999;
		VCBbuffer->freeSpacePointer = 0;

		// Initialize bitmap
		for (int i = 0; i < bitmapSize + 1; i++)
		{
			setBit(bitmap, VCBbuffer->freeSpacePointer);
			VCBbuffer->freeSpacePointer++; // increment
		}
		LBAwrite(bitmap, 5, 1);

		// initializing the root
		directory_entry *root = malloc(deSize * 51);
		int deBlocks = ceil(((double)deSize * 51.0) / blockSize); // number of blocks needed by root
		
		for (int i = 0; i < deNum; i++)
		{
			root[i].location = 0; // if fileLocation is 0, we know it is free
		}
		int start = allocate(deBlocks, VCBbuffer, bitmap); // get starting location from allocate

		root[0].size = deSize * deNum;
		root[0].location = start;
		strcpy(root[0].directoryName, ".");
		root[0].isDir = 1; // root is a directory
		root[0].createdDate = (int)time(NULL);

		root[1].size = deSize * deNum; // root is its own parent
		root[1].location = start;
		strcpy(root[1].directoryName, "..");
		root[1].isDir = 1;
		root[1].createdDate = (int)time(NULL);

		VCBbuffer->rootDirectory = start; // Set location of Root directory

		LBAwrite(root, deBlocks, start);
		LBAwrite(VCBbuffer, 1, 0); // write buffer last, we just updated root location
		free(root);
		printf("Current working diretory: .\n");
	}

	LBAread(VCBbuffer, 1, 0);
	LBAread(bitmap, 5, 1);
	return 0;
}

VCB*getVCB()
{
	return VCBbuffer;
}

char*getBitmap()
{
	return bitmap;
}

void exitFileSystem()
{
	free(VCBbuffer);
	VCBbuffer = NULL;
	free(bitmap);
	bitmap = NULL;
	printf("System exiting\n");
}