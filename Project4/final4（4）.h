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
#define HALF_BLOCK 8192
#define MAX_FILES 256
#define MAX_INODE 256
#define SUPER_BLOCK_SIZE 2092
#define DOUBLE_BLOCK_SIZE 8192 //8129??
#define ENTRY_SIZE 4868
#define DIR_SIZE 4876
#define INODE_SIZE 16416
#define DIRECTORY_BEGIN_INDEX 5
#define DIRECTORY_END_INDEX 516
#define DIRECTORY_ENTRY_BEGIN_INDEX 600
#define DIRECTORY_ENTRY_END_INDEX 1111
// #define DATABLOCK_BEGIN_INDEX 8192
// #define DATABLOCK_END_INDEX 1111
#define INODE_BEGIN_INDEX 6912
#define INODE_END_INDEX 8191
#define MAX_DIR_CHAR 15	
#define MAX_RW_BUFF 33554432

 char *diskName;
 int isMout;     /* file handle to virtual disk       */
 int isOpen;
 int isRead;

typedef struct INode
{

	 // number of block
	short blockNum[HALF_BLOCK];
	//inode creating time
	int year;
	int month;
	int day; 
	int hour; 
	int min;
	int sec;	
		 int size;
	int offset;
} iNode;

typedef struct Block
{
	bool used[HALF_BLOCK];
} block;

typedef struct DirectoryEntry
{
	int index;
	int INode_id[256];		// the id of file
	char fileName[256][MAX_DIR_CHAR]; // the name of the file
} directory_entry;

typedef struct Directory
{
	int index;
	int parent;				// the parent of current directory
	int dirEntry;			// file  in current directory
	int childList[256];   //  the index of directory in current directory
	char dirName[256][MAX_DIR_CHAR]; // the name of dir in current directory
} list_directory;

typedef struct SuperBlock
{
	int rootDir;
	char disk_name[MAX_BUFFER];
	char pwd[MAX_BUFFER];
	// int first_inode_index;
	// int first_inode_idle_index;
	// int first_inode_block_index;
	// int blockIndex;
	int iNodeNum;
	int blockNum;
	int iNodeUsedNum;
	int blockUsedNum;
	int fileNum;
	int totalSize;
	int fileOpen;
} super_block;



struct INode inode[64]; // the max number to open files
int fd[64];
char pwd[1024];
char rBuf[MAX_RW_BUFF];
char wBuf[MAX_RW_BUFF];
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
int fs_mkdir(char *name);
int call_ls();
int fs_open(char* name);
int fs_close(int fildes);
int closeFD();
int fs_get_filesize(int fildes) ;
int fs_read(int fildes, void *buf, size_t nbyte);
int fs_write(int fildes, void *buf, size_t nbyte);
char *copyFile( char *des, short blockIndex, int index, int size);
int cutData(char* buf, int offset, size_t nbyte,int size);
int writeFile(int fildes, char* buf, int size);
int fs_delete(char *name);
int fs_lseek(int fildes, off_t offset);
int fs_truncate(int fildes, off_t length);
int call_cd(char* add);