#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BUFFER 4096 // The max length of a command in one line is 1024 char
#define MAX_ARGS 64		// The max length of a command in one line is 64 commands
#define HALF_BLOCK 8192
#define MAX_FILES 256

static char *diskName;

typedef struct INode
{
	int size; // number of block
	short blockNum[8192];
	int ctime; //inode creating time
			   // char owner[MAX_BUFFER]; //owner of this file
} iNode;

typedef struct Block
{
	bool used[HALF_BLOCK];
} block;

typedef struct DirectoryEntry
{
	int INode_id;  // the id of file
	char *dirName; // the name of dir
	bool isFile;
} directory_entry;

typedef struct Directory
{
	int preant; // the parent of current directory
	int inDir2; // the directory in current directory
	int inDir;  // file  in Same directory
} list_directory;

typedef struct SuperBlock
{
	directory_entry *root;
	char *disk_name;
	char pwd[MAX_BUFFER];
	int first_inode_index;
	int first_inode_idle_index;
	int first_inode_block_index;
	int blockIndex;
	int iNodeNum;
	int blockNum;
	int iNodeUsedNum;
	int blockUsedNum;

} super_block;

int runOneCommand(char *command[]);
int initSuperBlock(super_block sb,char *disk_name);
void splitInput(char *command[], char inputD[]);