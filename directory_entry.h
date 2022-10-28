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

typedef struct ValidCStruct{
    char directoryName[255]; // name of the directory
    int location; // location of the directory
    int size; // size of the directory
    int lastModifiedDate; // time last modified
    int createdDate; // when the directory was created
    int accessedTime; // last accessed time
    int isDir; // Determine if it's dir or not

} directory_entry;
