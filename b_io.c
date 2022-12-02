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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>			// for malloc
#include <string.h>			// for memcpy
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "b_io.h"
#include "fsLow.h"

#define MAXFCBS 20
#define B_CHUNK_SIZE 512



typedef struct b_fcb
	{
	/** TODO add al the information you need in the file control block **/
	//char * buf;		//holds the open file buffer
	//int index;		//holds the current position in the buffer
	//int buflen;		//holds how many valid bytes are in the buffer
	fileInfo *fi;		
	char *buf;
	int index;			// position in the buffer
	int bufferFilled;	// to loads valid bytes in the buffer
	int blockFileIndex; // indicates position of block in the file
	int endOfFile;		// check if eof is reached
	int givenBytes;		// to check if the filesize is equal to givenBytes
	} b_fcb;
	
b_fcb fcbArray[MAXFCBS];

int startup = 0;	//Indicates that this has not been initialized

//Method to initialize our file system
void b_init ()
	{
	//init fcbArray to all free
	for (int i = 0; i < MAXFCBS; i++)
		{
		fcbArray[i].buf = NULL; //indicates a free fcbArray
		}
		
	startup = 1;
	}

//Method to get a free FCB element
b_io_fd b_getFCB ()
	{
	for (int i = 0; i < MAXFCBS; i++)
		{
		if (fcbArray[i].buf == NULL)
			{
			return i;		//Not thread safe (But do not worry about it for this assignment)
			}
		}
	return (-1);  //all in use
	}
	
// Interface to open a buffered file
// Modification of interface for this assignment, flags match the Linux flags for open
// O_RDONLY, O_WRONLY, or O_RDWR
b_io_fd b_open (char * filename, int flags)
	{
	b_io_fd returnFd;

	//*** TODO ***:  Modify to save or set any information needed
	//
	//
		
	if (startup == 0) b_init();  //Initialize our system
	
	returnFd = b_getFCB();				// get our own file descriptor
										// check for error - all used FCB's

	if (returnFd < 0)
	{
		printf("FCB not avaiable.");
		return (-1);
	}
	else
	{
		//fcbArray[returnFd].fi = GetFileInfo(filename);	   // GetFileInfo gives the starting block number of the file | return NULL if can't find file
		fcbArray[returnFd].buf = malloc(B_CHUNK_SIZE); // allocating our buffer
		fcbArray[returnFd].blockFileIndex = fcbArray[returnFd].fi->fileLocation;

		// initializing
		fcbArray[returnFd].index = 0;
		fcbArray[returnFd].bufferFilled = 0;
		fcbArray[returnFd].endOfFile = 0;
	}

	
	return (returnFd);						// all set
	}


// Interface to seek function	
int b_seek (b_io_fd fd, off_t offset, int whence)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		
		
	return (0); //Change this
	}



// Interface to write function	
int b_write (b_io_fd fd, char * buffer, int count)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		
		
	return (0); //Change this
	}



// Interface to read a buffer

// Filling the callers request is broken into three parts
// Part 1 is what can be filled from the current buffer, which may or may not be enough
// Part 2 is after using what was left in our buffer there is still 1 or more block
//        size chunks needed to fill the callers request.  This represents the number of
//        bytes in multiples of the blocksize.
// Part 3 is a value less than blocksize which is what remains to copy to the callers buffer
//        after fulfilling part 1 and part 2.  This would always be filled from a refill 
//        of our buffer.
//  +-------------+------------------------------------------------+--------+
//  |             |                                                |        |
//  | filled from |  filled direct in multiples of the block size  | filled |
//  | existing    |                                                | from   |
//  | buffer      |                                                |refilled|
//  |             |                                                | buffer |
//  |             |                                                |        |
//  | Part1       |  Part 2                                        | Part3  |
//  +-------------+------------------------------------------------+--------+
int b_read (b_io_fd fd, char * buffer, int count)
	{

	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}

	fcbArray[fd].givenBytes += count;

	// number of bytes available to copy from buffer
	int remainingBytes = fcbArray[fd].bufferFilled - fcbArray[fd].index;

	// buffer has data already

	if (remainingBytes > count) // part1
	{
		memcpy(buffer, fcbArray[fd].buf + fcbArray[fd].index, count); // copying memory area buffer to fcbArray

		fcbArray[fd].index += count; // increasing the index of buffer by the number of bytes requested by caller

		return count;
	}
	else
	{
		// transfering what we have and copying to remaining needed bytes

		memcpy(buffer, fcbArray[fd].buf + fcbArray[fd].index, remainingBytes);

		if (fcbArray[fd].endOfFile)

		{
			fcbArray[fd].index += remainingBytes; // how may bytes are actually in buffer

			return remainingBytes; // returning the remaining bytes
		}

		int block = fcbArray[fd].blockFileIndex++; // getting the 512 bytes block size

		LBAread(fcbArray[fd].buf, 1, block); /* trying to read size of buffer bytes into our buffer
																LBAread is a position to the entire volume */

		// resetting the buffer to zero
		fcbArray[fd].index = 0;

		int nextBuffer = fcbArray[fd].bufferFilled = strlen(fcbArray[fd].buf); // 512 or eof (0 or 1)

		// calculates the number of bytes
		int remainder = count - remainingBytes; // remainder is now part2-part3

		int next;

		// if the buffer is not filled even if we requested another 512 block eof is reached
		if (nextBuffer < B_CHUNK_SIZE)

		{
			fcbArray[fd].endOfFile = 1;
		}

		// if the read is greater than 512 
		if (fcbArray[fd].bufferFilled > remainder)

		{

			next = remainder;
		}

		else
		{

			next = nextBuffer;
		}

		memcpy(buffer + remainingBytes, fcbArray[fd].buf + fcbArray[fd].index, next); // memcpy nextBuffer which is needed

		fcbArray[fd].index += next;

		return remainingBytes + remainder; // adding reaming bytes and the remainder
	}
		
	}
	
// Interface to Close the file	
int b_close (b_io_fd fd)
	{
	fcbArray[fd].fi = NULL; // close the file
	fcbArray[fd].index = 0;
	fcbArray[fd].bufferFilled = 0;
	fcbArray[fd].endOfFile = 0;
	free(fcbArray[fd].buf);
	fcbArray->buf = NULL;
	}
