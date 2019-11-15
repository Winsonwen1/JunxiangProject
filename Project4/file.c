#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

super_block* superBlock;
iNode inode[256];	// the max number of files
int fd[64];


void main() {

	while true {
	display the instruction
	line = get line from user;
	commands <- split line by ' ';


	runOneCommand(command);

	}
}
//run the specific function for the command you input
//use switch-case flow control to finish job.
//when input 'help', just print the help document content.
//when input 'quit', call system call 'exit' to leave program.
runOneCommand(command) {


	switch (command[0] )
	case 'makeDisk' then
			call make_fs(command[1]);
case ' mountFs' then
		call mount_fs(command[1]);
case 'umountFs' then
		call umount_fs(command[1]);
case 'fsOpen' then
		call  fs_open(command[1]);
case 'fsClose' then
		call fs_close(command[1]);
case 'fsCreate' then
		call fs_create(command[1]);
case 'fsDelete' then
		call fs_delete(command[1]);
case 'fsMkdir' then
		call  fs_mkdir(command[1]);
case 'fsRead' then
		call  fs_read(command[1],command[2]);
case 'fsWrite' then
		call  fs_write(command[1],command[2]);
case 'getFilesize' then
		call   fs_get_filesize(command[1]);
case 'fsLseek' then
		call   fs_lseek(command[1]);
case 'fsTruncate' then
		call    fs_truncate(command[1]);



}





// This function creates a fresh (and empty) file system
// on the virtual disk with name disk_name
int function make_fs(char* disk_name) {
	// first invoke make_disk(disk_name) to create a new disk
	//  open this disk and write/initialize the necessary meta-information
	// for file system
	// The function returns 0 on success, and -1 when the disk
	// disk_name could not be created, opened, or properly initialized.

	if (make_disk(disk_name) == 0) {
		if (open_disk(disk_name) == 0) {
			superBlock-> iNodeNum=8191;
			superBlock-> blockNum= 8192;
			superBlock->root = null;
			superBlock->disk_name = disk_name;
			all other integer value set to 0;

			char* rootData = combine all value with space in superBlock;
			block_write(0,rootData);
			close_disk();

			return 0;
		}


	}
	return -1;
}

//This function mounts a file system that is stored on a virtual disk with name disk_name
int function mount_fs(char* disk_name) {
	//open the disk and then load the meta-information
	// that is necessary to handle the file system operations
	// The function returns 0 on success, and -1 when the disk disk_name could not
	//be opened or when the disk does not contain a valid file system

	if (open_disk(disk_name) == 0) {
		char* rootData;
		block_read(0,rootData);

		value in superBlock = split rootData;
		int j =0;
		for(int i = superBlock->first_inode_index; i<= 8191 || j = superBlock->blockUsedNum ; i++) {

			if( block i is not empty ) {

				j++
				loading data into superBlock->root;
			}
		}
		return 0;
	}
	return -1;
}


//This function unmounts your file system from a virtual disk with name disk_nam
int function umount_fs(char* disk_name) {
	//o write back all meta-information so that the disk persistently reflects
	//all changes that were made to the file system
	//close the disk. The function returns 0 on success, and -1 when the
	//disk disk_name could not be closed or when data could not be written to the disk

	if (handler != 0) {

		for (all data in superBlock) {
			//write back to the disk in  appropriate locations
			block_write();
		}

		close_disk(disk_name);
		return 0;
	}
	return -1;
}

// The file specified by nameis opened for reading and writing, and the file descriptor
// corresponding to this file is returned to the calling function
int function fs_open(char* name) {
	//If successful, fs_open returns a nonnegative integer,
	//which is a file descriptor that can be used to subsequently access this file.

	//find the file in currecnt directory
	directory_entry* temp = superBlock->root;
	while (temp!=NULL) {

		if (strcmp(temp->name, name) == 0) {

			int id = temp->INode_id	;
			if(inode[id]->fdSize>64) {
				return -1;
			}

			int i=0;
			for(int i; fd[i]!= i; i++) {
			}
			fs_lseek(int id+9000, 0);
			fd[id]=id+9000;
			return i;
		}
		temp = temp-> inDir;
	}
	return -1;
}

//The file descriptor fildes is closed
int function fs_close(int fildes) {
	//Upon successful completion, a value of 0 is returned. In case the file
	//descriptor fildes does not exist or is not open, the function returns -1

	if(fd[fildes]>=9000) {
		fd[fildes] = 0;
		return 0;

	}
	return -1;
}

//This function creates a new file with name name in your file system
int fs_create(char *name) {

	//The maximum length for a file name is 15 characters
	//at most 256 files in the directory
	//The max number of block is 8192
	if (the legth of file > 15 || superBlock->iNodeNum > 256 || superBlock->blockUsedNum >= 8192) {
		return -1;
	}

	if () {
		return -1;
	}
	// when the file with name already exists
	directory_entry* temp = superBlock->root;
	while (temp!=NULL) {

		if (strcmp(temp->name, name) == 0) {
			return -1;
		}
		temp = temp-> inDir;
	}

	superBlock->iNodeUsedNum++;
	superBlock->iNodeIdleNum--;

	superBlock->root->INode_id = superBlock->iNodeIdleNum;

	superBlock->iNodeIdleNum= the next idle inode;

}

//This function deletes the file with the path and name name from the directory of your file
//system and frees all data blocks and meta-information that correspond to that file.
int fs_delete(char *name) {

	// when the file with name is not exists
	directory_entry* temp = superBlock->root;
	while (temp!=NULL) {

		if (strcmp(temp->name, name) == 0) {
			for(int i=0; i<64; i++) {
				// the file is currently open
				if(  fd[i]-9000 == temp->INode_id   ) {
					return -1;

				}
			}
			superBlock->iNodeUsedNum--;
			superBlock->iNodeIdleNum++;
			destory superBlock->root where the inode_id locate ;
			destory all block in inode[inode_id];
			destory inode[inode_id];

		}
		temp = temp-> inDir;
	}
	return -1
}

//This function attempts to create a directory with the name name. fs_mkdir()
int fs_mkdir(char *name) {


	//at most 256 files in the directory
	if (superBlock->iNodeNum > 256 ) {
		return -1;
	}


	// when the file with name is not exists
	directory_entry* temp = superBlock->root;
	while (temp!=NULL) {

		if (strcmp(temp->name, name) == 0) {
			return -1;
		}
		temp = temp-> inDir2;
	}

	superBlock->iNodeUsedNum++;
	superBlock->iNodeIdleNum--;

	superBlock->root->INode_id = superBlock->iNodeIdleNum;

	superBlock->iNodeIdleNum= the next idle inode;
}

//This function attempts to read nbyte bytes of data from the file referenced by the descriptor
//fildes into the buffer pointed to by buf
int fs_read(int fildes, void *buf, size_t nbyte) {
	//In case of failure, the function returns -1. It is a failure when the
	//file descriptor fildes is not valid. The read function implicitly increments the file pointer by the
	//number of bytes that were actually read.

	//fildes is invalid, the function returns -1.
	if(fd[fildes]<9000) {
		return -1;
	}
	char * buf;
	//get inode_id
	int id = fd[fildes] -9000;
	for ( blockNum in inode[id]   ) {
		block_read(blockNum, buf);
		printf buf

	}


}

//This function attempts to write nbyte bytes of data to the file referenced by the descriptor
//fildes from the buffer pointed to by buf.
int fs_write(int fildes, void *buf, size_t nbyte) {
	char * buf = get input from user;

	//fildes is invalid, the function returns -1.
	if(fd[fildes]<9000) {
		return -1;
	}

	//get inode_id
	int id = fd[fildes] -9000;
	while(!usertype==("quit22") ) {
		//The write function implicitly
		//increments the file pointer by the number of bytes that were actually written.
		if((superBlock->blockUsedNum +1) > 8192 ) {
			return -1;
		}
		superBlock->blockUsedNum++;
		superBlock->blockIdleNum--;

		block_write(first_inode_block_index, buf);
		set up the first_inode_block_index for next idel block;

	buf = get input from user;
}

}

int fs_get_filesize(int fildes) {
	//fildes is invalid, the function returns -1.
	if(fd[fildes]<9000) {
		return -1;
	}
	//get inode_id
	int id = fd[fildes] -9000;

	return inode[id]->size;

}

//This function sets the file pointer (the offset used for read and write operations) associated
// with the file descriptor fildes to the argument offset
int fs_lseek(int fildes, off_t offset) {
	//fildes is invalid, the function returns -1.
	if(fd[fildes]<9000) {
		return -1;
	}

	//get inode_id
	int id = fd[fildes] -9000;
	int seekNumber is ;
	return 0;


}

//This function causes the file referenced by fildes to be truncated to length bytes in size.
int fs_truncate(int fildes, off_t length) {
	//fildes is invalid, the function returns -1.
	if(fd[fildes]<9000) {
		return -1;
	}

	//get inode_id
	int id = fd[fildes] -9000;
	//It is a failure
	//when the requested length is larger than the file size.
	if(length>1024*inode->size) {
		return -1;
	}

	int i = length/1024;

	destory the last i block

	return 0;

}

CreationInode(){
}

CreationBlock(){
}


CreationDirectoryEntry(){
}


CreationDirectory(){
}


CreationSuperBlock(){
}


allocateInode(){
}


allocateBlock(){
}


allocateDirectoryEntry(){
}


allocateDirectory(){
}


allocateSuperBlock(){
}


destroyInode(){
}


destroyBlock(){
}


destroyDirectoryEntry(){
}


destroyDirectory(){
}


destroySuperBlock(){
}













