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
#include <math.h>

#include "bitmap.h"
#include "fsInit.h"
#include "mfs.h"
#include "fsLow.h"