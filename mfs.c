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
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>

#include "fsLow.h"
#include "mfs.h"
#include <math.h>
#include "bitmap.h"
#include "fsInit.h"
#include <string.h>


char*savePtr = NULL;
char*token;
char cwd[140] = ".";
int blockSize = 512;
int currentSize;
int currentFiledir;
int mkDirLocation;
char lastWord[30];


struct fs_diriteminfo *fs_readdir(fdDir *dirp){ //WORKING currently
    //printf("readdir runs\n");
    struct fs_diriteminfo*info = malloc(300); //fix later
    static int counter = 0;
    //printf("COUNTER CURRENTLY IS AT %d\n", counter);
    directory_entry*newDir = malloc(60*52);
    if(dirp[counter].d_reclen == 0){
        counter = 0;
        //printf("RETURNING NULL\n");
        return NULL;
    }
    LBAread(newDir, 6, dirp[0].directoryStartLocation);
    int i = dirp[counter].dirEntryPosition;
    //printf("INDEX i CURRENTLY AT %d\n", i);
    if(i > 52){
        return NULL;
    }
    //printf("reading in file at %d name: %s\n", dirp[counter].directoryStartLocation, newDir[i].fileName);

    strcpy(info->d_name, newDir[i].directoryName);
    //printf("START LOCATION position %d\n", dirp[counter].directoryStartLocation);
    //printf("entry position %d\n", dirp[counter].dirEntryPosition);
    //printf("Value of fileName %s\n", newDir[0].fileName);
    info->d_reclen = dirp[counter].d_reclen;
    info->fileType = 1; //change later
    counter++;
    //printf("info->fileName %s\n", info->d_name);
    return info;
}
 

int fs_isDir(char * path){
    //printf("isdir runs\n");
    directory_entry*newDir = malloc(60*52);
    //printf("ROOT AT %d", getVCB()->rootDir);
    LBAread(newDir, 6, getVCB()->rootDirectory);

    char temp[140];
    strcpy(temp, path);
    int location = parsePath(temp, newDir);

    if(location == -1){
        return -1;
    }
    LBAread(newDir, 6, location); //fix size later
    if(newDir[0].isDir == 0){
        return 1;
    }
}

int fs_isFile(char * path){
    return !fs_isDir(path);
}

int fs_closedir(fdDir *dirp){
    dirp = NULL;
	free(dirp);
	return 1;
}


fdDir * fs_opendir(const char *name){
    //printf("opendir runs\n");
    directory_entry*newDir = NULL;
    newDir = malloc(60*52);

    LBAread(newDir, 6, getVCB()->rootDirectory);

    char temp[140];
    strcpy(temp, cwd);
    int location = parsePath(temp, newDir);

    if(location == -1){
        printf("location not found\n");
        return NULL;
    }

    LBAread(newDir, 6, location);
    fdDir*dirArray = malloc(sizeof(fdDir)); //fix later
    int counter = 0;
    unsigned short x = 5;

    for(int i = 0; i < 51; i++){
        if(newDir[i].location!=0){
            dirArray[counter].d_reclen=(unsigned short)newDir[i].size;
            //dirArray->d_reclen=5;
            dirArray[counter].directoryStartLocation = newDir[i].size;
            dirArray[counter].dirEntryPosition = i;
            //dirArray++;
            counter++;
            dirArray = (fdDir*)realloc(dirArray, (counter+1)*(sizeof(fdDir)));
            //dirArray++;
        }
    }

    return dirArray;
}



char * fs_getcwd(char *buf, size_t size){
    return cwd;
}

int fs_stat(const char *path, struct fs_stat *buf){
    //buf = malloc(sizeof(struct fs_stat));
    directory_entry*newDir = malloc(60*52);
    LBAread(newDir, 6, getVCB()->rootDirectory);

    char temp[140];
    strcpy(temp, path);
    int location = parsePath(temp, newDir);

    if(location == -1){
        printf("location not found\n");
        return -1;
    }
    LBAread(newDir, 6, location);
    buf->st_blksize = getVCB()->sizeOfBlock;
    buf->st_blocks = ceil(((double)60*51.0)/getVCB()->sizeOfBlock);
    buf->st_createtime = newDir->createdDate;
    buf->st_size = newDir->size;
    return 0;

}

int fs_setcwd(char *buf){

    //directoryEntry*currentDir;
    //int numBlocks = (3600/VCBbuffer->bSize);
    //printf("NUMBLOCKs %d\n", numBlocks);
    directory_entry*root;
    directory_entry*newDir = malloc(60*52);
    
    LBAread(newDir, 6, getVCB()->rootDirectory);
    //int location = parseDir(buf, VCBbuffer, root, newDir);

    //char*path = strdup("./one");
    //char pathname[] = "./one/two";
    char temp[140];
    strcpy(temp, buf);
    char relativePath[140];
    if(strcmp(buf, ".")==0){
        return 0;
    }

    int location = parsePath(temp, newDir);
    if(location!=-1){
        //printf("LOCATION IS AT %d\n", location);
        getPath(location);
        printf("Current working directory: %s\n", cwd);
        //strcpy(cwd, temp); //undo later
        //printf("--location of directory at %d\n", location);
        return 0;
    }
    free(newDir);
    return -1;
    
}

char* getFileName(directory_entry* newDir){
    directory_entry*parentDir = malloc(60*52);
    LBAread(parentDir, 6, newDir[1].location);
    for(int i = 0; i < 51; i++){
        if(parentDir[i].location == newDir[0].location){
            //printf("filename is %s", parentDir[i].fileName);
            return parentDir[i].directoryName;
        }
    }
}

int getPath(int location){ //FIX LATER, array is hardcoded
    char newPath[140] = "";
    directory_entry*newDir = malloc(60*52);
    //printf("????????????????????this runs\n");
    int num = 10;
    int size;
    /*char **pathArray = malloc(num * sizeof(*pathArray));
    for(int i =0 ; i < num; i++) {
        pathArray[i] = malloc(size * sizeof(**pathArray)); /* define this MACRO */
    char pathArray[10][40] = {""};
    //pathArray = (char*)realloc(pathArray, (10)*40);
    //printf("this runs\n");
    //printf("LOCATION IS AT %d\n", location);
    LBAread(newDir, 6, location);
    int count = 0;
    strcat(pathArray[count++], getFileName(newDir));

    while(newDir[0].location!=getVCB()->rootDirectory){
        LBAread(newDir, 6, newDir[1].location);
        strcat(pathArray[count], getFileName(newDir));
        count++;
        //pathArray = (char*)realloc(pathArray, (count+1)*40);
    }
    //strcat(pathArray[count], newDir[0].fileName);
    //count++;

    for(int i = count-1; i >= 0; i--){
        //printf("file path: %s\n", pathArray[i]);
        strcat(newPath, pathArray[i]);
        if(i!=0){
            strcat(newPath, "/");
        }
    }
    strcpy(cwd, newPath);
    //printf("NEW PATH: ----------------- %s\n", newPath);
}

int parsePath(char pathname[], directory_entry* newDir){
    char temp[140];
    char relativePath[140];

    //printf("pathname %s\n", pathname);
    //printf("pathname[0] %c\n", pathname[0]);
    if(strcmp(pathname, "..")==0){ //fix lazy code later
        //printf("Finding relative path\n");
        strcpy(relativePath, cwd);
        strcat(relativePath, "/");
        strcat(relativePath, pathname);
        //printf("new path: %s\n", relativePath);
    }
    else if((pathname[0] != '.')&&(pathname[1] != '/')){
        //printf("Finding relative path\n"); //maybe delete null
        strcpy(relativePath, cwd);
        strcat(relativePath, "/");
        strcat(relativePath, pathname);
        //printf("new path: %s\n", relativePath);
    }
    else{
        strcpy(relativePath, pathname);
    }
    strcpy(temp, relativePath);
    //printf("current tokenadawdawd \n");
    //char*savePtr = pathname;
    //char* counter = strtok_r(pathname, "/", &savePtr);
    /*for (int i = 0; pathname != NULL; i++) {
        pathname = strtok(NULL, " ");
    }*/
    //printf("new path: %c\n", pathname[0]);
    //printf("equal?: %d\n", strcmp(pathname, "."));
    token = strtok_r(temp, "/", &savePtr);
    //printf("current NULL token %s\n", token);
    //printf("current token %s\n", token);
    int ret = recursive(temp, newDir);
    return ret;
 /*
        else(root[i].fileName!=0 && (root[i].isDir==1));{
            LBAread(newDir, root[i].fileLocation, 6);
            parseDir(pathname, VCBbuffer, root, newDir);
        }*/
}
int recursive(char pathname[], directory_entry* newDir){
    for(int i = 0; i < 51; i++){ //skip first 2 iterations, irrelevant
        /*if (cancel){

        }*/
        //printf("comparing %s, %s\n", newDir[i].fileName, token);
        /*if(i == 2){
            printf("comparing1strun %s, %s\n", newDir[0].fileName, token);
        }*/
        /*if(strcmp(newDir[0].fileName, token) == 0){
            printf("this found the number at index 0, filename %s, returning location at %d\n", newDir[0].fileName, newDir[0].fileLocation);
            return newDir[0].fileLocation;
        }*/
        strcpy(lastWord, token);
        mkDirLocation = newDir[0].location;
        if(strcmp(newDir[i].directoryName, token) == 0){
            //newDir = &newDir[i];
            //LBAread(newDir, 6, newDir[i].fileLocation);
            token = strtok_r(NULL, "/", &savePtr);
            //printf("current token in loop %s\n", token);
            if(token==NULL){
                //printf("this found the number, filename %s, returning location %d\n", newDir[i].fileName, newDir[i].fileLocation);
                currentSize = ceil((double)newDir[i].size/blockSize);
                currentFiledir = newDir[i].isDir;
                //printf("current blocks = %d", currentSize);
                return newDir[i].location;
            }
            else{
                LBAread(newDir, 6, newDir[i].location);
                if(newDir->isDir==0){
                    return -1;
                }
                //printf("RECURSION\n");
                return recursive(pathname, newDir);
            }
        }
    }
    return -1;
}
int fs_delete(char* filename){
    //printf("this ran");
    return -1;
}

int fs_rmdir(const char *pathname){
    char temp[140];
    strcpy(temp, pathname);


    directory_entry*newDir = malloc(60*52);
    directory_entry*parentDir = malloc(60*52);
    LBAread(newDir, 6, getVCB()->rootDirectory);
    int location = parsePath(temp, newDir);
    
    if(location == -1){
        printf("location not found, could not delete\n");
        return -1;
    }   
    if(currentFiledir==0){
        printf("File is not a directory\n");
        return -1;
    }
    LBAread(newDir, currentSize, location);
    for(int i = 2; i < 51; i++){ //iterate starting after the . and ..
        if(newDir[i].location > 0){
            printf("File is not empty, delete directory/file at index %d.\n", i);
            return -1;
        }
    }
    resetBits(getBitmap(), location, currentSize);
    LBAread(parentDir, 6, newDir[1].location);
    for(int i = 2; i < 51; i++){
        if(parentDir[i].location == newDir[0].location){
            parentDir[i].location = 0;
            //printf("parentDir[i] is %s\n", parentDir[i].fileName);
            strcpy(parentDir[i].directoryName, "");
            LBAwrite(parentDir, 6, parentDir[0].location);
            break;
        }
    }


    

}
int fs_mvdir(const char *source, const char *destination){
    //printf("This runs\n");
    char temp[140];
    strcpy(temp, source);

    directory_entry*newDir = malloc(60*52);
    directory_entry*destDir = malloc(60*52);
    directory_entry*parentDir = malloc(60*52);

    LBAread(newDir, 6, getVCB()->rootDirectory);
    int location = parsePath(temp, newDir);

    if(location == -1){ //check if path is valid
        printf("Source location not found.\n");
        return -1;
    } 

    char getLastWord[140];
    char last[30];
    char file[30];

    strcpy(temp, destination);
    strcpy(getLastWord, source);

    char* tok = strtok(getLastWord, "/");
    for (int i = 0; tok != NULL; i++) {
        strcpy(last, tok);
        tok = strtok(NULL, "/");
    }
    LBAread(newDir, 6, getVCB()->rootDirectory);
    int location2 = parsePath(temp, newDir);

    if(location == -1){ //check if path is valid
        printf("Destination not found.\n");
        return -1;
    } 
    LBAread(newDir, 6, location);
    //printf("source file name %s\n", getFileName(newDir));
    LBAread(destDir, 6, location2);
    //printf("dest file name %s\n", getFileName(destDir));

    for(int i = 0; i < 51; i++){
        if(strcmp(destDir[i].directoryName, last) == 0){
            printf("File of same name already exists, cannot move to destination.\n");
            return -1;
        }
    }
    LBAread(parentDir, 6, newDir[1].location);
    for(int i = 0; i < 51; i++){
        if(parentDir[i].location == newDir[0].location){
            strcpy(file, parentDir[i].directoryName);
            strcpy(parentDir[i].directoryName, "");
            parentDir[i].location = 0;
            parentDir[i].size = 0;
        }
    }
    LBAwrite(parentDir, 6, parentDir[0].location);

    for(int i = 0; i < 51; i++){
        if(destDir[i].location == 0){
            //printf("target found at %d\n", i);
            strcpy(destDir[i].directoryName, file);
            destDir[i].location = newDir[0].location;
            newDir[1].location = destDir[0].location;
            destDir[i].isDir = 1;
            destDir[i].size = newDir[0].size;
            //printf("dest[i] filename is %s at %d write location at %d\n", destDir[i].fileName, destDir[0].fileLocation, location2);
            break;
        }
    }
    LBAwrite(destDir, 6, destDir[0].location);
    LBAwrite(newDir, 6, newDir[0].location);


}


int fs_mkdir(const char *pathname, mode_t mode){


    directory_entry*newDir = malloc(60*52);
    LBAread(newDir, 6, getVCB()->rootDirectory);

    char temp[140];
    char getLastWord[140];
    char last[30];

    strcpy(temp, pathname);
    strcpy(getLastWord, pathname);

    char* tok = strtok(getLastWord, "/");
    for (int i = 0; tok != NULL; i++) {
        strcpy(last, tok);
        tok = strtok(NULL, "/");
    }
    int location = parsePath(temp, newDir);
    //printf("lastfromrecursion %s\n", lastWord);
    //printf("lastfromfunction %s\n", last);
    if(location == -1 && (strcmp(lastWord, last)==0)){ //this is valid.
        int indexPlace;
        int start = allocate(6, getVCB(), getBitmap());
        LBAread(newDir, 6, mkDirLocation);
        //printf("mkdir location %d\n", mkDirLocation);
        directory_entry*createDir = malloc(60*51);
        for(int i = 0; i < 51; i++){
			if(newDir[i].location == 0){
                //printf("INDEX FOUND %d\n", i);
                indexPlace = i;
                break;
            } 
		}
        //printf("ADDING NEW DIR\n");
		strcpy(newDir[indexPlace].directoryName, last);
		newDir[indexPlace].size=newDir[0].size; //root is its own parent
		newDir[indexPlace].location=start;
		newDir[indexPlace].isDir=1;
		newDir[indexPlace].createdDate=newDir[0].createdDate; //set time later
		directory_entry*new2 = malloc(60*51); //replace constants later
		createDir[0] = newDir[indexPlace];
        strcpy(createDir[0].directoryName, ".");
		createDir[1] = newDir[0];
        strcpy(createDir[1].directoryName, "..");
        LBAwrite(createDir, 6, start);
        LBAwrite(newDir, 6, mkDirLocation);
        return 1;
    }
    else{
        printf("Could not add new directory\n");
        return -1;
    }
    return 0;
}