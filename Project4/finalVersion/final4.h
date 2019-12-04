/*
 * final4.h
 *
 *  Last Edit: Dec 4, 2019
 *      Author: Junxiang Wen
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

// #define EOF -1
#define MAX_BUFFER 1024 // The max length of a command in one line is 1024 char
#define MAX_ARGS 64		// The max length of a command in one line is 64 commands
#define HALF_BLOCK 8192	// the size of stuct block
#define MAX_FILES 256	// the max files in disk
#define MAX_INODE 256	// the max inodes in disk
#define SUPER_BLOCK_SIZE 2092	//the size of super block
#define DOUBLE_BLOCK_SIZE 8192	
#define ENTRY_SIZE 4868		// size of stuct dir entry
#define DIR_SIZE 4900		// size of stuct dir
#define INODE_SIZE 16416		// size of stuct Inode
#define DIRECTORY_BEGIN_INDEX 5	
#define DIRECTORY_END_INDEX 516
#define DIRECTORY_ENTRY_BEGIN_INDEX 600
#define DIRECTORY_ENTRY_END_INDEX 1111
#define INODE_BEGIN_INDEX 6912
#define INODE_END_INDEX 8191
#define MAX_DIR_CHAR 15	
#define MAX_RW_BUFF 33554432

 char *diskName;
 int isMout;   // is a disk mounted
 int isOpen;	//are some file opened
 int isRead;	// is a file read

// Struct for iNode
typedef struct INode
{
	 
	short blockNum[HALF_BLOCK];	// number of block
	int offset;	//offset of file
	//inode creating time
	int year;
	int month;
	int day; 
	int hour; 
	int min;
	int sec;	
	int size;
} iNode;

//  Struct for block
typedef struct Block
{
	bool used[HALF_BLOCK];	// is the index of block used
} block;
//  Struct for Directory Entry
typedef struct DirectoryEntry
{
	int index;	// the index of entry
	int INode_id[256];		// the id of file
	char fileName[256][MAX_DIR_CHAR]; // the name of the file
} directory_entry;

//  Struct for Directory 
typedef struct Directory
{
	int index; // the index of directory
	int parent;				// the parent of current directory
	int dirEntry;			// file  in current directory
	int childList[256];   //  the index of directory in current directory
	char dirName[256][MAX_DIR_CHAR]; // the name of dir in current directory
	//Directory creating time
	int year;
	int month;
	int day; 
	int hour; 
	int min;
	int sec;	

} list_directory;
//  Struct for SuperBlock 
typedef struct SuperBlock
{
	int rootDir;	// the index of root Directory
	char disk_name[MAX_BUFFER]; // name of disk
	char pwd[MAX_BUFFER];	// roow directory
	int iNodeNum;	// the max number of iNode in disk
	int blockNum;	//the max number of block in disk
	int iNodeUsedNum;	// the number iNode in used
	int blockUsedNum;	// the number block in used
	int fileNum;	// the file num in disk
	int totalSize;	//	the size of disk in used 
	int fileOpen;	// the number of file is opened
} super_block;



struct INode inode[64]; // storing iNode for openning file 
int fd[64];	// index of open file
char pwd[1024];	// current directory
char rBuf[MAX_RW_BUFF];	// read buffer for reading 
char wBuf[MAX_RW_BUFF];	//// write buffer for writing 
struct Block nodeBlock;	// block for node 
struct Block dataBlock;	//block for storing data
// struct Directory rootDirList;
struct Directory currectDirList;	// current dir
struct DirectoryEntry currectEntry;	// current dir entry
struct SuperBlock superBlock;	// superblock


int initBlock(); //  initialize  struct block
int runOneCommand(char *command[]);	// run a command from user
int initSuperBlock(struct SuperBlock sb);//  initialize superblock
int initEntry(int index); // initialize dir entry
int initlistD(int parentIndex);	// initialize dir
int make_fs(char *disk_name);	// creat a file system
void splitInput(char *command[], char inputD[]);	// split command from user
int mount_fs(char *disk_name); // mount a file system
int  umount_fs(char* disk_name); // umoun a file system
int wirteBacklistD();	// storing the dir into disk
int wirteBackBlock();	// storing block into disk
int wirteBackSuperBlock(); // storing superblock into disk
int wirteDirEntry();	// storing the dir entry into disk
int fs_create(char *name);	// creat a file
int initINode();	// inilist a inode
char* copy(char* des, char* src, int index,int size);	//combined a struct data in disk
char* writeCopy(char* des, char* src, int index, int size);//store struct data in disk separately
int fs_mkdir(char *name);	// creat a dir
int call_ls();	// print file neme in current dir
int fs_open(char* name); // open a file
int fs_close(int fildes); // close a file
int closeFD();	// close all files that are opened
int fs_get_filesize(int fildes) ; // get size of a file
int fs_read(int fildes, void *buf, size_t nbyte); // read a file
int fs_write(int fildes, void *buf, size_t nbyte); // write a file
char *copyFile( char *des, short blockIndex, int index, int size); // get data of file from disk
int cutData(char* buf, int offset, size_t nbyte,int size);	// cut out the required contextx
int writeFile(int fildes, char* buf, int size);// store data for a file
int fs_delete(char *name);	// delete a file
int fs_lseek(int fildes, off_t offset);	// set a offset for a file
int fs_truncate(int fildes, off_t length);	// truncate a file
void showInstr();


//check it can work or not