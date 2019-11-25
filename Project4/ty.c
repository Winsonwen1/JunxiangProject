// mkDisk
// oD
// cD
// mkFS






#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_BUFFER 1024
static int active = 0; /* is the virtual disk open (active) */
static int handle;     /* file handle to virtual disk       */
#include "disk.h"


typedef struct DirectoryEntry
{
  int INode_id;                  // the id of file
  char *dirName;                 // the name of dir
  struct DirectoryEntry *next;   // the child of current directory
  struct DirectoryEntry *inDir2; // the directory in current directory
  struct DirectoryEntry *inDir;  // file in Same directory
} directory_entry;

typedef struct SuperBlock
{
  directory_entry *root;
  char *disk_name;
  int first_inode_index;
  int first_inode_idle_index;
  int first_inode_block_index;
  int blockIndex;
  int iNodeNum;
  int blockNum;
  int iNodeUsedNum;
  int blockUsedNum;

} super_block;


int main(int argc, char *argv[])
{

    char buf[MAX_BUFFER];
    char buf2[MAX_BUFFER];

    struct SuperBlock sb ;
    struct SuperBlock sb2 ;
    sb.blockNum = 1844;
    sb.disk_name = "5566";
    sb.blockUsedNum = 897987;
    	sb.first_inode_index = 6192;
			sb.first_inode_idle_index = 6192;
			sb.first_inode_idle_index = 6192;
			sb.first_inode_block_index = 8192;
			sb.blockIndex = 8192;
			sb.iNodeNum = 256;
			sb.blockNum = 8192;
			sb.iNodeUsedNum = 0;
			sb.blockUsedNum = 5789;

    int f;
    memcpy(buf, &sb, MAX_BUFFER);

     f = open("ni.txt", O_RDWR, 0644);
    write(f, buf, MAX_BUFFER);
    close(f);
 printf("\n-------------%s--------------\n", buf);

    f = open("ni.txt", O_RDWR, 0644);
    read(f, buf2, MAX_BUFFER);
    memcpy(&sb2, buf2, MAX_BUFFER);
    close(f);

    printf("\n-------------%s--------------\n", buf2);
    // printf("===========%ld============\n", sizeof(sb));
    // printf("===========%d============\n", sb->blockNum);
    printf("===========%d============", sb2.blockNum);
    printf("===========%d============", sb2.blockUsedNum);
}



