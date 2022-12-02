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

#ifndef _B_IO_H
#define _B_IO_H
#include <fcntl.h>

typedef int b_io_fd;

b_io_fd b_open (char * filename, int flags);
int b_read (b_io_fd fd, char * buffer, int count);
int b_write (b_io_fd fd, char * buffer, int count);
int b_seek (b_io_fd fd, off_t offset, int whence);
int b_close (b_io_fd fd);

#endif

