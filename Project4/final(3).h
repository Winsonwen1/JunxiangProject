#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_BUFFER 1024 // The max length of a command in one line is 1024 char
#define MAX_ARGS 64		// The max length of a command in one line is 64 commands
#define HALF_BLOCK 8192
#define MAX_FILES 256
#define MAX_INODE 256
#define SUPER_BLOCK_SIZE 2088
#define DOUBLE_BLOCK_SIZE 8129
#define ENTRY_SIZE 4856
#define DIR_SIZE 4864
#define INODE_SIZE 16412
#define DIRECTORY_BEGIN_INDEX 5
#define DIRECTORY_END_INDEX 516
#define DIRECTORY_ENTRY_BEGIN_INDEX 600
#define DIRECTORY_ENTRY_END_INDEX 1111
#define INODE_BEGIN_INDEX 6912
#define INODE_END_INDEX 8191
#define MAX_DIR_CHAR 15	

 char *diskName;
 int isMout;     /* file handle to virtual disk       */
 int isOpen;

typedef struct INode
{
	 // number of block
	short blockNum[8192];
	//inode creating time
	int year;
	int month;
	int day; 
	int hour; 
	int min;
	int sec;	
	int size;	  
} iNode;

typedef struct Block
{
	bool used[HALF_BLOCK];
} block;

typedef struct DirectoryEntry
{
	int index;
	int INode_id[256];		// the id of file
	char dirName[255][MAX_DIR_CHAR]; // the name of dir
} directory_entry;

typedef struct Directory
{
	int index;
	int parent;				// the parent of current directory
	int dirEntry;			// file  in current directory
	int childList[256];   //  the index of directory in current directory
	char dirName[255][MAX_DIR_CHAR]; // the name of dir
} list_directory;

typedef struct SuperBlock
{
	int rootDir;
	char disk_name[MAX_BUFFER];
	char pwd[MAX_BUFFER];
	int first_inode_index;
	int first_inode_idle_index;
	int first_inode_block_index;
	int blockIndex;
	int iNodeNum;
	int blockNum;
	int iNodeUsedNum;
	int blockUsedNum;
	int fileNum;
} super_block;


struct INode inode[64]; // the max number to open files
int fd[64];
char pwd[1024];
struct Block nodeBlock;
struct Block dataBlock;
// struct Directory rootDirList;
struct Directory currectDirList;
struct DirectoryEntry currectEntry;
struct SuperBlock superBlock;


int initBlock();
int runOneCommand(char *command[]);
int initSuperBlock(struct SuperBlock sb);
int make_fs(char *disk_name);
void splitInput(char *command[], char inputD[]);
int initlistD(int parentIndex);
int mount_fs(char *disk_name);
int  umount_fs(char* disk_name);
int wirteBacklistD();
int wirteBackBlock();
int wirteBackSuperBlock();
int wirteDirEntry();
int initEntry(int index);
int fs_create(char *name);
int initINode();
char* copy(char* des, char* src, int index,int size);
char* writeCopy(char* des, char* src, int index, int size);