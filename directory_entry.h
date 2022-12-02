/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Sajan Gurung, Michael Hua, Deep Dhorajiya,Ron Elijah Rivera
* Student IDs: 921149577, 918729206, 920842114
* GitHub Name: michealhuaa
* Group Name: Team Buffer
* Project: Basic File System
*
* File: directory_entry.h
*
* Description: Directory entry for our file system
*
**************************************************************/
#ifndef _directoryentry_H
#define _directoryentry_H

#include <stdlib.h>


typedef struct directoryEntry
{
	int fileLocation; //Location of file
    int fileSize; //Size of individual file
    char fileName[36]; //Name of file
    int created; //Date file was created
    int modified; //Date file was modified
    int accessed; //Last date of access
    int isDir; //Determine if it's a directory or not
}directoryEntry;

#endif