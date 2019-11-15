typedef struct INode {
	int size;	// number of block
	block blockNum[8192];	
	int ctime;//inode creating time
	char* owner; //owner of this file
	int otime;//file open time
} iNode;


typedef struct Block {
	int isUsed;
} block ;



typedef struct DirectoryEntry {
	int INode_id;			// the id of file
	char* dirName;			// the name of dir
	directory_entry* next;	// the child of current directory
	directory_entry* inDir2;	// the directory in current directory
	directory_entry* inDir;	// file in Same directory
} directory_entry;

typedef struct Directory {
	int size;
} list_directory;

typedef struct SuperBlock {
	directory_entry* root;
	char* disk_name;
	int first_inode_index;
	int first_inode_idle_index;
	int first_inode_block_index;
	int blockIndex;
	int iNodeNum;
	int blockNum;
	int iNodeUsedNum;
	int blockUsedNum;
	int iNodeIdleNum;
	int blockIdleNum;

} super_block;


