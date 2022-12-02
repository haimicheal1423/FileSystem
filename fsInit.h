/**************************************************************
* Class:  CSC-415-02 Fall 2022
* Names: Sajan Gurung, Michael Hua, Deep Dhorajiya, Ron Elijah Rivera
* Student IDs: 921149577, 918729206, 920842114, 921656746
* GitHub Name: michealhuaa
* Group Name: Team Buffer
* Project: Basic File System
*
* File: fsInit.h
*
* Description: Header for File System Init
*
*
**************************************************************/

#ifndef _FSINIT_H
#define _FSINIT_H
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "vcb.h"
#include "directory_entry.h"

#include "b_io.h"

#include <dirent.h>
#define FT_REGFILE	DT_REG
#define FT_DIRECTORY DT_DIR
#define FT_LINK	DT_LNK

#ifndef uint64_t
typedef u_int64_t uint64_t;
#endif
#ifndef uint32_t
typedef u_int32_t uint32_t;
#endif

int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize);
VolumeBlock*getVCB();
char*getBitmap();
void exitFileSystem();

#endif