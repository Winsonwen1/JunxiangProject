/*
 * final4.c
 *
 *  Last Edit: Dec 4, 2019
 *      Author: Junxiang Wen
 */
#include "final4.h"
#include "disk.h"

// main function
int main()
{
	//initialize fd[]
	for (int i = 0; i < 64; i++)
	{

		fd[i] = -1;
	}

	char inputD[MAX_BUFFER] = {}; // input data from user
	char *command[MAX_ARGS] = {};	//command for user

	isMout = 0;
	isOpen = 0;

	//quick test
	// make_fs("bb");
	// mount_fs("bb");
	// fs_mkdir("bb");
	// fs_delete("bb");
	// fs_write(0, rBuf, 0);
	// umount_fs("bb");
	// mount_fs("bb");
	// fs_open("b");
	// fs_write(0, rBuf, 0);
	// fs_close(0);
	
	showInstr();

	//loop for getting command
	while (1)
	{
		fprintf(stderr, "%s", "Please type your command: ");

		fgets(inputD, sizeof(char) * MAX_BUFFER, stdin);
		splitInput(command, inputD);

		runOneCommand(command);
	}// end loop
}// end main()

// run a command from user
int runOneCommand(char *command[])
{
	int r; // state the result from function 
	if (command[0] == NULL)
	{
		fprintf(stderr, "%s\n", "Can't find this command!! Retry!'");
		return 0;
	}

	if (strcmp(command[0], "mkDisk") == 0)
	{
	 	r =make_disk(command[1]);
		if (r == 0)
		{
			isOpen = 1;
			fprintf(stderr, "created disk successful \n");
			diskName = command[1];
		}


		 return 0;
	}
	else if (strcmp(command[0], "oDisk") == 0)
	{
		r = open_disk(command[1]);
		if (r == 0)
		{
			isOpen = 1;
			fprintf(stderr, "opened disk successful \n");
			diskName = command[1];
		}
		return -1;
	}
	else if (strcmp(command[0], "cDisk") == 0)
	{
		r = close_disk(command[1]);

		if (r == 0)
		{
			isOpen = 0;
			diskName = NULL;
			fprintf(stderr, "closed disk successful \n");
		}

		return 3;
	}
	else if (strcmp(command[0], "mkFS") == 0)
	{
		make_fs(command[1]);
		return 4;
	}
	else if (strcmp(command[0], "mouFS") == 0)
	{
		mount_fs(command[1]);
		return 5;
	}
	else if (strcmp(command[0], "umouFS") == 0)
	{
		umount_fs(command[1]);
		return 6;
	}
	else if (strcmp(command[0], "mkFile") == 0)
	{
		fs_create(command[1]);
		return 7;
	}
	else if (strcmp(command[0], "mkDir") == 0)
	{
		fs_mkdir(command[1]);
		return 8;
	}
	else if (strcmp(command[0], "ls") == 0)
	{
		call_ls();
		return 8;
	}
	else if (strcmp(command[0], "pwd") == 0)
	{
		fprintf(stderr, "%s%s\n", "PWD = ", pwd);
		return 8;
	}
	else if (strcmp(command[0], "opFile") == 0)
	{
		fs_open(command[1]);
		return 9;
	}
	else if (strcmp(command[0], "trFile") == 0)
	{
		// check command[1] and command[2] are valided
		if (command[1] == NULL || command[2] == NULL)
		{
			fprintf(stderr, "%s\n", "Ple enter correct number for this command.");
			return -1;
		}
		if (isdigit(command[1][0]) == 0 || isdigit(command[2][0]) == 0)
		{
			fprintf(stderr, "%s\n", "Ple enter an number for this command.");
			return -1;
		}
		int num = atoi(command[1]);
		int num2 = atoi(command[2]);

		if (num < 0 || num >= 64)
		{
			fprintf(stderr, "%s\n", "Invaile number");
			return -1;
		}

		fs_truncate(num, num2);
		return 9;
	}
	else if (strcmp(command[0], "cFile") == 0)
	{
		if (isdigit(command[1][0]) == 0)
		{
			fprintf(stderr, "%s\n", "Ple enter an interger.");
			return -1;
		}
		int num = atoi(command[1]);
		fs_close(num);
		return 9;
	}
	else if (strcmp(command[0], "deFile") == 0)
	{

		fs_delete(command[1]);
		return 9;
	}
	else if (strcmp(command[0], "getSize") == 0)
	{
		if (isdigit(command[1][0]) == 0)
		{
			fprintf(stderr, "%s\n", "Ple enter an interger.");
			return -1;
		}
		int num = atoi(command[1]);
		fs_get_filesize(num);
		sleep(1);
		return 9;
	}
	else if (strcmp(command[0], "reFile") == 0)
	{
		if (command[1] == NULL || command[2] == NULL)
		{
			fprintf(stderr, "%s\n", "Ple enter correct number for this command.");
			return -1;
		}
		if (isdigit(command[1][0]) == 0 || isdigit(command[2][0]) == 0)
		{
			fprintf(stderr, "%s\n", "Ple enter an number for this command.");
			return -1;
		}
		int num = atoi(command[1]);
		int num2 = atoi(command[2]);
		if (num < 0 || num >= 64)
		{
			fprintf(stderr, "%s\n", "Invaile number");
			return -1;
		}
		fs_read(num, rBuf, num2);

		return 9;
	}
	else if (strcmp(command[0], "setSeek") == 0)
	{
		if (command[1] == NULL)
		{
			fprintf(stderr, "%s\n", "Ple enter correct number for this command.");
			return -1;
		}
		if (isdigit(command[1][0]) == 0)
		{
			fprintf(stderr, "%s\n", "Ple enter an number for this command.");
			return -1;
		}
		int num = atoi(command[1]);

		if (num < 0 || num >= 64)
		{
			fprintf(stderr, "%s\n", "Invaile number");
			return -1;
		}

		int index = fs_get_filesize(num);

		if (command[2] == NULL)
		{

			if (index != -1)
				fs_lseek(num, index);
			return 0;
		}
		int num2 = atoi(command[2]);

		if (num2 > index)
		{
			fprintf(stderr, "%s\n", "Offset is too big, Try again");
			return -1;
		}

		if (num2 < 0 || isdigit(command[2][0]) == 0)
		{
			fprintf(stderr, "%s\n", "Invaile number");
			return -1;
		}

		fs_lseek(num, num2);

		return 9;
	}
	else if (strcmp(command[0], "wrFile") == 0)
	{
		// printf("444");
		if (command[1] == NULL)
		{
			fprintf(stderr, "%s\n", "Ple enter correct number for this command.");
			return -1;
		}
		if (isdigit(command[1][0]) == 0)
		{
			fprintf(stderr, "%s\n", "Ple enter an number for this command.");
			return -1;
		}
		// printf("555");
		int num = atoi(command[1]);
		// printf("666");
		if (num < 0 || num >= 64)
		{
			fprintf(stderr, "%s\n", "Invaile number");
			return -1;
		}
		fs_write(num, wBuf, 0);

		return 9;
	}
	else if (strcmp(command[0], "quit") == 0)
	{
		umount_fs(diskName);
		fprintf(stderr, "%s\n", "See you soon!!!");
		exit(1);
		return -1;
	}
	else if (strcmp(command[0], "help") == 0)
	{
		showInstr();
		return -1;
	}
	else
	{
		fprintf(stderr, "%s\n", "Can't find this command!! Retry!");
		return 0;
	}
	return -1;
}

void showInstr(){
	fprintf(stderr, "%s\n", "Befor mount a disk:");
	fprintf(stderr, "	%-30s%s\n", "mkDisk + disk_name:", "creat a disk with name disk_name");
	fprintf(stderr, "	%-30s%s\n", "oDisk + disk_name:", "Open a disk with name disk_name");
	fprintf(stderr, "	%-30s%s\n", "cDisk + disk_name:", "Close a disk with name disk_name");
	fprintf(stderr, "	%-30s%s\n", "mkFS + disk_name:", "Creat file System for a disk with name disk_name");
	fprintf(stderr, "	%-30s%s\n", "mouFS + disk_name:", "mount a file system for a disk with name disk_name");

	fprintf(stderr, "\n%s\n", "After mount a disk:");
	fprintf(stderr, "	%-30s%s\n", "umouFS + disk_name:", "unmount a file system for a disk with name disk_name");
	fprintf(stderr, "	%-30s%s\n", "mkFile + file_name:", "creat a file in file system with name file_name");
	fprintf(stderr, "	%-30s%s\n", "mkFile + directory_name:", "creat a directory file in file system with name directory_name");
	fprintf(stderr, "\n%s\n", "After creat a file:");
	fprintf(stderr, "	%-30s%s\n", "opFile + file_name:", "open a file with name file_name");
	fprintf(stderr, "	%-30s%s\n", "trFile + num1 + length:", "truncated a file with fd(File descriptor) to be truncated to length bytes");
	fprintf(stderr, "	%-30s%s\n", "cFile + fd:", "close a file with fd(File descriptor)");
	fprintf(stderr, "	%-30s%s\n", "deFile + file_name:", "delete a file or directory with name file_name");
	fprintf(stderr, "	%-30s%s\n", "getSize + fd:", "get the size of a file with fd(File descriptor)");
	fprintf(stderr, "	%-30s%s\n", "reFile + fd + length:", "read a file for length byte with fd(File descriptor) (length = 0 to read the whole file)");
	fprintf(stderr, "	%-30s%s\n", "wrFile + fd:", "write a file with fd(File descriptor)");
	fprintf(stderr, "	%-30s%s\n", "setSeek + fd + offset:", "set the offset of the file  with fd(File descriptor) ");
	fprintf(stderr, "	%-30s%s\n", "setSeek + fd:", "set the offset of the file  to the end of file with fd(File descriptor) ");

	fprintf(stderr, "\n%s\n", "Extra command");
	fprintf(stderr, "	%-30s%s\n", "quit:", "quit the progarm");
	fprintf(stderr, "	%-30s%s\n", "help:", "show this instruction again");
	fprintf(stderr, "	%-30s%s\n", "ls:", "list all file name in current directory");
	fprintf(stderr, "	%-30s%s\n", "pwd:", "list the current path");







}



// truncate a file
int fs_truncate(int fildes, off_t length)
{
	//check it can work or not
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}
	if (fd[fildes] == -1)
	{
		fprintf(stderr, "%s\n", "File is not opened yet. Try it again.");
		return -1;
	}

	int size = inode[fildes].size;

	if (length > size)
	{
		fprintf(stderr, "%s\n", "The length of truncation is too larger. Try it again.");
		return -1;
	}

	inode[fildes].offset = 0; //reset offset
	int blockSize = length / BLOCK_SIZE + 1;	// the number of block it need after truncate
	superBlock.totalSize = superBlock.totalSize - inode[fildes].size + length;
	//get the number of block it used
	int k = 0;
	for (; inode[fildes].blockNum[k] != 0; k++)
	{
	}
	
	//get  data in file from disk
	for (int i = 0; i < blockSize; i++)
	{

		copyFile(wBuf, inode[fildes].blockNum[i], i, length);
	}
	wBuf[length] = '\0';
	inode[fildes].size = strlen(wBuf);

	// write back into disk
	writeFile(fildes, wBuf, inode[fildes].size);
	//reset unused block
	for (int x = blockSize; x < k; x++)
	{
		dataBlock.used[inode[fildes].blockNum[x]] = false;
		inode[fildes].blockNum[x] = 0;
		superBlock.blockUsedNum--;
	}

	fprintf(stderr, "%s\n", "truncate success.");
	return 0;
}	// end fs_truncate()

// set a offset for a file
int fs_lseek(int fildes, off_t offset)
{	
	//check it can work or not
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}
	if (fd[fildes] == -1)
	{
		fprintf(stderr, "%s\n", "File is not opened yet. Try it again.");
		return -1;
	}

	//set up offset 
	inode[fildes].offset = offset;
	fprintf(stderr, "%s\n", "Setup successful.");
	return 0;
}// end fs_lseek()

// delete a file
int fs_delete(char *name)
{
	//check it can work or not
	if (!name)
	{
		fprintf(stderr, "%s\n", "Invaile name");
		return -1;
	}
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}

	// check the file is available ornot
	//delete File
	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strcmp(currectEntry.fileName[i], name) == 0)
		{
			// fprintf(stderr, "%s\n", "111");
			char buf[BLOCK_SIZE];
			struct INode iTemp;
			int nodeIndex = currectEntry.INode_id[i];
			char temp2[BLOCK_SIZE];
			char buf2[INODE_SIZE] = {};
			// fprintf(stderr, "%s\n", "444");
			for (int x = 0; x < 64; x++)
			{
				if (fd[x] == currectEntry.INode_id[x])
				{

					fprintf(stderr, "%s\n",  "Ple close the file first.");
					return -1;
				}
			}
			
			// clear the data in block
			for (int k = 0; k < 5; k++)
			{

				block_read(k + nodeIndex, temp2);
				copy(buf2, temp2, k, INODE_SIZE);
			}
			//reset inode
			memcpy(&iTemp, buf2, INODE_SIZE);
			memset(buf, 0, BLOCK_SIZE);
			for (int j = 0; j < HALF_BLOCK; j++)
			{
				if (iTemp.blockNum[j] > 1000)
				{
					dataBlock.used[j] = false;
					block_write(iTemp.blockNum[j], buf);
					// iTemp.blockNum[j]=0;
					superBlock.blockUsedNum--;
				}
			}
			nodeBlock.used[i] = false;
			superBlock.iNodeUsedNum--;
			superBlock.fileNum--;
			int n;
			memset(&iTemp, 0, INODE_SIZE);
			memcpy(&iTemp, buf2, INODE_SIZE);
			for (int k = 0; k < 5; k++)
			{
				writeCopy(buf2, buf, k, ENTRY_SIZE);
				n = block_write(nodeIndex + k, buf2);
				if (n != 0)
				{
					fprintf(stderr, "%s\n", "Delete File Failed");
					return -1;
				}
			}
			strcpy(currectEntry.fileName[i], "");
			currectEntry.INode_id[i] = 0;
			fprintf(stderr, "%s\n", "Delete File successful");
			return 0;
		}
	}

	// check the file is available ornot
	//delete Dir
	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strcmp(currectDirList.dirName[i], name) == 0)
		{
			//reset dir
			char buf2[BLOCK_SIZE];
			int listIndex = currectDirList.childList[i];
			struct Directory dirTemp;
			char dirBuf[DIR_SIZE];
			// fprintf(stderr, "listinDex%d\n", currectDirList.childList[i]);
			for (int k = 0; k < 2; k++)
			{
				block_read(k + listIndex, buf2);
				copy(dirBuf, buf2, k, DIR_SIZE);
			}
			memcpy(&dirTemp, dirBuf, DIR_SIZE);

			for (int y = 0; y < MAX_FILES; y++)
			{
				if (strlen(dirTemp.dirName[y]) != 0)
				{
					printf("%s\n", "Please empty the dir files first.");
					return -1;
				}
			}
			//reset entry for this dir
			struct DirectoryEntry entryTemp;
			char entryBuf[ENTRY_SIZE];
			for (int k = 0; k < 2; k++)
			{
				block_read(k + dirTemp.dirEntry, buf2);
				copy(entryBuf, buf2, k, ENTRY_SIZE);
			}

			memcpy(&entryTemp, entryBuf, DIR_SIZE);
			for (int i = 0; i < MAX_FILES; i++)
			{
				if (strcmp(entryTemp.fileName[i], "") != 0)
				{
					fprintf(stderr, "%s\n", "555");
					printf("%s\n", "Please empty the dir files first.");
					return -1;
				}
			}
			memset(&entryTemp, 0, ENTRY_SIZE);
			memcpy(entryBuf, &entryTemp, ENTRY_SIZE);
			int x;
			for (int k = 0; k < 2; k++)
			{
				// strncpy(temp, buf + BLOCK_SIZE * k, BLOCK_SIZE);
				writeCopy(buf2, entryBuf, k, ENTRY_SIZE);
				x = block_write(k + dirTemp.dirEntry, buf2);
				if (x != 0)
				{
					return -1;
				}
			}

			memset(&dirTemp, 0, ENTRY_SIZE);
			memcpy(dirBuf, &dirTemp, DIR_SIZE);
			for (int k = 0; k < 2; k++)
			{
				// strncpy(temp, buf + BLOCK_SIZE * k, BLOCK_SIZE);
				writeCopy(buf2, dirBuf, k, DIR_SIZE);
				x = block_write(listIndex + k, buf2);

				if (x != 0)
				{
					fprintf(stderr, "Delete Directory failed\n");
					return -1;
				}
			}

			strcpy(currectDirList.dirName[i], "");
			currectDirList.childList[i] = 0;
			superBlock.fileNum--;
			printf("%s\n", "Delete Dir file successful.");
			return 0;
		}
	}

	printf("%s\n", "No such dir in current directory.");
	return -1;
} // end fs_delete()

// write a file
int fs_write(int fildes, void *buf, size_t nbyte)
{
	//check it can work or not
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}
	if (fd[fildes] == -1)
	{
		fprintf(stderr, "%s\n", "File is not opened yet. Try it again.");
		return -1;
	}


	// sort the blockNum in inode
	int indexTemp = 0;
	for (int i = 0; i < HALF_BLOCK; i++)
	{
		if (inode[fildes].blockNum[i] > 100)
		{
			if (i != indexTemp)
			{
				inode[fildes].blockNum[indexTemp] = inode[fildes].blockNum[i];
				inode[fildes].blockNum[i] = 0;
			}
			indexTemp++;
		}
	}

	
	char inputD[MAX_BUFFER] = {};	//buf that come from read file 
	char inputD2[BLOCK_SIZE] = {};	// buf taht user input
	int k = 0;	// block number of file 
	int j = 0;		//index of data that write into file
	int i = inode[fildes].offset;	// for offset
	superBlock.totalSize -= inode[fildes].size;
	//if user read a file
	if (isRead == 1)
	{
		fprintf(stderr, "%s\n", " Type '1' if u wanna write the data that you just read,");
		fgets(inputD, sizeof(char) * MAX_BUFFER, stdin);
		//if user want to write the data that just read into write file
		if (inputD[0] == '1')
		{
			//get data from write file
			for (; inode[fildes].blockNum[k] != 0; k++)
			{
				copyFile(buf, inode[fildes].blockNum[k], k, inode[fildes].size);
			}
			// write data
			if (inode[fildes].offset == 0)
			{
				for (; i < MAX_RW_BUFF && rBuf[j] != '\0'; i++, j++)
				{

					wBuf[i] = rBuf[j];
					// printf("%c====??======%c\n", wBuf[i], rBuf[j]);
				}
			}
			else
			{

				for (i--; i < MAX_RW_BUFF && rBuf[j] != '\0'; i++, j++)
				{

					wBuf[i] = rBuf[j];
				}
			}
			// set up end of file
			if (i == MAX_RW_BUFF)
			{
				wBuf[i - 1] = '\0';
				inode[fildes].size = MAX_RW_BUFF;
			}
			else
			{
				wBuf[i] = '\0';
				inode[fildes].size = strlen(wBuf);
			}
		}
		else
		{
			//get data from user
			fprintf(stderr, "%s", "Please type the data that you wanna store into this file(Max char 4096 each):");
			fgets(inputD2, sizeof(char) * BLOCK_SIZE, stdin);
			//get data from write file
			for (; inode[fildes].blockNum[k] != 0; k++)
			{
				copyFile(buf, inode[fildes].blockNum[k], k, inode[fildes].size);
			}

			if (inode[fildes].offset == 0)
			{
				for (; i < MAX_RW_BUFF && inputD2[j] != '\0'; i++, j++)
				{
					wBuf[i] = inputD2[j];
				}
			}
			else
			{
				for (i--; i < MAX_RW_BUFF && inputD2[j] != '\0'; i++, j++)
				{

					wBuf[i] = inputD2[j];
				}
			}
			
			//set up end of file
			if (i == MAX_RW_BUFF)
			{
				wBuf[i - 1] = '\0';
				inode[fildes].size = MAX_RW_BUFF;
			}
			else
			{
				wBuf[i - 1] = '\0';
				inode[fildes].size = strlen(wBuf);
			}
		}
	}
	else
	{
		//get data from user
		fprintf(stderr, "%s", "Please type the data that you wanna store into this file(Max char 4096 each):");
		fgets(inputD2, sizeof(char) * BLOCK_SIZE, stdin);
		//get data from write file
		for (; inode[fildes].blockNum[k] != 0; k++)
		{
			copyFile(buf, inode[fildes].blockNum[k], k, inode[fildes].size);
		}

		if (inode[fildes].offset == 0)
		{
			for (; i < MAX_RW_BUFF && inputD2[j] != '\0'; i++, j++)
			{

				wBuf[i] = inputD2[j];
			}
		}
		else
		{
			for (i--; i < MAX_RW_BUFF && inputD2[j] != '\0'; i++, j++)
			{

				wBuf[i] = inputD2[j];
			}
		}

		//set up the end of file 
		if (i == MAX_RW_BUFF)
		{
			wBuf[i - 1] = '\0';
			inode[fildes].size = MAX_RW_BUFF;
		}
		else
		{
			wBuf[i - 1] = '\0';
			inode[fildes].size = strlen(wBuf);
		}
	}
	//stored file 
	writeFile(fildes, wBuf, inode[fildes].size);
	//set up the offset
	inode[fildes].offset = inode[fildes].size + 1;
	//sort the block num
	indexTemp = 0;
	for (int i = 0; i < HALF_BLOCK; i++)
	{
		if (inode[fildes].blockNum[i] > 100)
		{
			if (i != indexTemp)
			{
				inode[fildes].blockNum[indexTemp] = inode[fildes].blockNum[i];
				inode[fildes].blockNum[i] = 0;
			}
			indexTemp++;
		}
	}
	isRead = 0;
	fprintf(stderr, "%s\n", "Write successful.");
	return 0;
} // end fs_write()

// read a file
int fs_read(int fildes, void *buf, size_t nbyte)
{
	//check it can work or not
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}

	if (fd[fildes] == -1)
	{
		fprintf(stderr, "%s\n", "File is not opened yet. Try it again.");
		return -1;
	}
	if (nbyte == 0)
	{
		nbyte = inode[fildes].size;
	}
	if (inode[fildes].size == 0)
	{
		fprintf(stderr, "%s\n", "No data in such file. Try it again.");
		return -1;
	}

	// read data from read file
	int k = 0;
	for (; inode[fildes].blockNum[k] != 0; k++)
	{
		copyFile(buf, inode[fildes].blockNum[k], k, inode[fildes].size);
	}
	rBuf[inode[fildes].size] = '\0';

	//get what user want and print
	cutData(buf, inode[fildes].offset, nbyte, inode[fildes].size);

	isRead = 1;

	return 0;
} //end fs_read()

// // get size of a file
int fs_get_filesize(int fildes)
{
	//check it can work or not
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}
	if (fildes < 0 || fildes >= 64)
	{
		fprintf(stderr, "%s\n", "Invaile number");
		return -1;
	}

	if (fd[fildes] == -1)
	{
		fprintf(stderr, "%s\n", "File is not opened yet. Try it again.");
		return -1;
	}

	// print the size 
	fprintf(stderr, "The size of this file is %d byte.\n", inode[fildes].size);
	return inode[fildes].size;
}//end fs_get_filesize()

 // close a file
int fs_close(int fildes)
{
	//check it can work or not
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}
	if (fildes < 0 || fildes >= 64)
	{
		fprintf(stderr, "%s\n", "Invaile number");
		return -1;
	}
	if (fd[fildes] == -1)
	{
		fprintf(stderr, "%s\n", "File is not opened yet. Try it again.");
		return -1;
	}

	//store the inode into disk
	int j;
	char buf[INODE_SIZE];
	char temp[BLOCK_SIZE];
	memcpy(buf, &inode[fildes], INODE_SIZE);
	int index1 = fd[fildes];
	for (int k = 0; k < 5; k++)
	{
		writeCopy(temp, buf, k, INODE_SIZE);
		j = block_write(index1 + k, temp);
		if (j != 0)
		{
			return -1;
		}
	}
	// fprintf(stderr, "%s\n", "File closed successful.");
	superBlock.fileOpen--;
	memset(&inode[fildes], 0, INODE_SIZE);
	fd[fildes] = -1;
	fprintf(stderr, "%s\n", "File closed successful.");
	return 0;
}	// end fs_close(

//open a file
int fs_open(char *name)
{
	//check it can work or not
	if (!name)
	{
		fprintf(stderr, "%s\n", "Invaile name");
		return -1;
	}
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}
	if (strlen(name) > MAX_DIR_CHAR)
	{
		fprintf(stderr, "%s\n", "the name is too long (it exceeds 15 characters), Request Reject!");
		return -1;
	}
	if (superBlock.fileOpen >= 64)
	{
		fprintf(stderr, "%s\n", "The maximum of file descriptors(64) is used, Request Reject!");
		return -1;
	}
	int inodeIndex;
	int fdIndex;
	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strcmp(currectEntry.fileName[i], name) == 0)
		{
			inodeIndex = currectEntry.INode_id[i];
			break;
		}
		else if (i == MAX_FILES - 1)
		{
			fprintf(stderr, "%s\n", "No such file in current directory, Request Reject!");
			return -1;
		}
	}

	// find the available fd
	for (int i = 0; i <= 64; i++)
	{
		if (fd[i] == -1)
		{
			fdIndex = i;
			fd[i] = inodeIndex;
			break;
		}
	}
	superBlock.fileOpen++;

	//get inode data from disk
	char temp2[BLOCK_SIZE];
	char buf2[INODE_SIZE] = {};
	for (int k = 0; k < 5; k++)
	{
		block_read(k + inodeIndex, temp2);
		copy(buf2, temp2, k, INODE_SIZE);
	}
	memcpy(&inode[fdIndex], buf2, INODE_SIZE);
	inode[fdIndex].offset = 0;
	fprintf(stderr, "%s\n", "File open successful!");
	fprintf(stderr, "%s%d\n", "File descriptor is ", fdIndex);
	printf("%s%4d-%02d-%02d %02d:%02d:%02d\n", "File created in: ", inode[fdIndex].year, inode[fdIndex].month, inode[fdIndex].day, inode[fdIndex].hour, inode[fdIndex].min, inode[fdIndex].sec);
	printf("%s%d\n", "The size of File is ", inode[fdIndex].size);
	return fdIndex;
}	//end  fs_open()

// print file neme in current dir
int call_ls()
{
	//check it can work or not
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}

	fprintf(stderr, "%s\n", "Directory Files in current directory:");
	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strlen(currectDirList.dirName[i]) != 0)
		{
			printf("%s\n", currectDirList.dirName[i]);
		}
	}
	fprintf(stderr, "\n%s\n", "Files in current directory:");
	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strlen(currectEntry.fileName[i]) != 0)
		{
			printf("%s\n", currectEntry.fileName[i]);
		}
	}
	return 0;
} // end call_ls()

// creat a dir
int fs_mkdir(char *name)
{
	//The maximum length for a file name is 15 characters
	//at most 256 files in the directory
	//The max number of block is 8192
	//check it can work or not
	if (!name)
	{
		fprintf(stderr, "%s\n", "Invaile name");
		return -1;
	}
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}
	if (strlen(name) > MAX_DIR_CHAR)
	{
		fprintf(stderr, "%s\n", "the directory name is too long (it exceeds 15 characters), Request Reject!");
		return -1;
	}
	if (superBlock.fileNum >= MAX_FILES)
	{
		fprintf(stderr, "%s\n", "Over 256 files in the directory, Request Reject!");
		return -1;
	}
	if (superBlock.blockUsedNum >= HALF_BLOCK)
	{
		fprintf(stderr, "%s\n", "Over the max number of block(8192), Request Reject!");
		return -1;
	}
	if (superBlock.iNodeUsedNum >= MAX_INODE)
	{
		fprintf(stderr, "%s\n", "Over the max number of INode(256), Request Reject!");
		return -1;
	}

	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strcmp(currectDirList.dirName[i], name) == 0)
		{
			fprintf(stderr, "%s\n", "There are same name of directory in currect directory, Request Reject!");
			return -1;
		}
	}

	//created a dir
	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strcmp(currectDirList.dirName[i], "") == 0)
		{
			
			int j = initlistD(currectDirList.index);
			if (j == -1)
			{
				fprintf(stderr, "%s\n", "Can't create file!");
				return -1;
			}

			strcpy(currectDirList.dirName[i], name);
			currectDirList.childList[i] = j;
			fprintf(stderr, "%s\n", "Directory created successful!");
			return 0;
		}
	}
	fprintf(stderr, "%s\n", "Can't create file!");
	return -1;
}	// end fs_mkdir()

//creat a file
int fs_create(char *name)
{
	//The maximum length for a file name is 15 characters
	//at most 256 files in the directory
	//The max number of block is 8192
	//check it can work or not
	if (!name)
	{
		fprintf(stderr, "%s\n", "Invaile name");
		return -1;
	}
	if (isMout == 0)
	{
		fprintf(stderr, "%s\n", "Please mount a disk first.");
		return -1;
	}
	if (strlen(name) > MAX_DIR_CHAR)
	{
		fprintf(stderr, "%s\n", "the file name is too long (it exceeds 15 characters), Request Reject!");
		return -1;
	}
	if (superBlock.fileNum >= MAX_FILES)
	{
		fprintf(stderr, "%s\n", "Over 256 files in the directory, Request Reject!");
		return -1;
	}
	if (superBlock.blockUsedNum >= HALF_BLOCK)
	{
		fprintf(stderr, "%s\n", "Over the max number of block(8192), Request Reject!");
		return -1;
	}
	if (superBlock.iNodeUsedNum >= MAX_INODE)
	{
		fprintf(stderr, "%s\n", "Over the max number of INode(256), Request Reject!");
		return -1;
	}
	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strcmp(currectEntry.fileName[i], name) == 0)
		{
			fprintf(stderr, "%s\n", "There are same name of file in currect directory, Request Reject!");
			return -1;
		}
	}

	//creat a file
	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strcmp(currectEntry.fileName[i], "") == 0)
		{
			int j = initINode();
			if (j == -1)
			{
				fprintf(stderr, "%s\n", "Can't create file!");
				return -1;
			}

			strcpy(currectEntry.fileName[i], name);
			currectEntry.INode_id[i] = j;
			fprintf(stderr, "%s\n", "File created successful!");
			// fprintf(stderr, "currectEntry:%d\n", currectEntry.index);
			return 0;
		}
	}
	fprintf(stderr, "%s\n", "Can't create file!");
	return -1;
}// end fs_create()

// umoun a file system
int umount_fs(char *disk_name)
{
	//o write back all meta-information so that the disk persistently reflects
	//all changes that were made to the file system
	//close the disk. The function returns 0 on success, and -1 when the
	//disk disk_name could not be closed or when data could not be written to the disk

	//check it can work or not
	if (isMout != 1)
	{
		fprintf(stderr, "File system is not mounted yet!! \n");
		return -1;
	}
	if (strcmp(disk_name, diskName) != 0)
	{
		fprintf(stderr, "File system is not mounted yet!! \n");
		return -1;
	}

	//start umout
	if (closeFD() == 0)
	{
		if (wirteBacklistD(DIRECTORY_BEGIN_INDEX) == 0)
		{
			if (wirteBackBlock() == 0)
			{

				if (wirteBackSuperBlock() == 0)
				{
					if (wirteDirEntry() == 0)
					{
						if (close_disk(disk_name) == 0)
						{

							isOpen = 0;
							strcpy(pwd, "");
							diskName = NULL;
							isMout = 0;
							fprintf(stderr, "umounted file system successful\n");
							return 0;
						}

						return -1;
					}
					fprintf(stderr, "Storing Dir Entry failed\n");
					return -1;
				}
				fprintf(stderr, "Storing super block failed\n");
				return -1;
			}
			fprintf(stderr, "Storing nodeBlockk or dataBlock failed\n");
			return -1;
		}
		fprintf(stderr, "Store dir failed\n");
		return -1;
	}
	close_disk(disk_name);
	isOpen = 0;
	fprintf(stderr, "Close files failed\n");
	return -1;
}// end umount_fs

// close all files that are opened
int closeFD()
{
	for (int i = 0; i < 64; i++)
	{
		if (fd[i] != -1)
		{
			if (fs_close(i) == -1)
			{
				return -1;
			}
		}
	}
	return 0;
} // end closeFD()

//mount a file system
int mount_fs(char *disk_name)
{
	//open the disk and then load the meta-information
	// that is necessary to handle the file system operations
	// The function returns 0 on success, and -1 when the disk disk_name could not
	//be opened or when the disk does not contain a valid file system
	char buf2[BLOCK_SIZE];
	char blockBuf[DOUBLE_BLOCK_SIZE];

	//check it can work or not
	if (open_disk(disk_name) == -1)
	{

		return -1;
	}
	isOpen = 1;
	//initial super block
	block_read(0, buf2);
	memcpy(&superBlock, buf2, SUPER_BLOCK_SIZE);

	if (superBlock.blockNum == 0)
	{
		fprintf(stderr, "%s\n", "File System is not initail yet, Ple make a File System for this disk first ");
		close_disk(disk_name);
		isOpen = 0;
		return -1;
	}

	// initial nodeBlock and data Block
	for (int i = 1; i < 5; i++)
	{
		if (i == 1)
		{
			block_read(i, buf2);
			copy(blockBuf, buf2, i - 1, DOUBLE_BLOCK_SIZE);
		}
		else if (i == 2)
		{
			block_read(i, buf2);
			copy(blockBuf, buf2, i - 1, DOUBLE_BLOCK_SIZE);
			memcpy(&nodeBlock, blockBuf, DOUBLE_BLOCK_SIZE);
		}
		else if (i == 3)
		{
			block_read(i, buf2);
			copy(blockBuf, buf2, i - 3, DOUBLE_BLOCK_SIZE);
		}
		else if (i == 4)
		{
			block_read(i, buf2);
			copy(blockBuf, buf2, i - 3, DOUBLE_BLOCK_SIZE);
			memcpy(&dataBlock, blockBuf, DOUBLE_BLOCK_SIZE);
		}
	}

	//initial rootDirlist
	char dirBuf[DIR_SIZE];
	for (int k = 5; k < 7; k++)
	{
		if (k == 5)
		{
			block_read(k, buf2);
			copy(dirBuf, buf2, k - 5, DIR_SIZE);
			continue;
		}


		block_read(k, buf2);
		copy(dirBuf, buf2, k - 5, DIR_SIZE);

	}
	memcpy(&currectDirList, dirBuf, DIR_SIZE);
	
	// initial pwd
	for (int j = 0; disk_name[j] != '\0'; j++)
	{

		pwd[j] = disk_name[j];
	}
	strcat(pwd, ":/root/");

	//initial dir entry
	char entryBuf[ENTRY_SIZE];
	for (int k = 600; k < 602; k++)
	{
		if (k == 600)
		{
			block_read(k, buf2);
			copy(entryBuf, buf2, k - 600, ENTRY_SIZE);
			continue;
		}
		block_read(k, buf2);
		copy(entryBuf, buf2, k - 600, DIR_SIZE);
	}

	memcpy(&currectEntry, entryBuf, ENTRY_SIZE);
	fprintf(stderr, "%s\n", "Mounted Disk successful!");
	isMout = 1;
	diskName = disk_name;
	return 0;
}// end mount_fs()

// creat a file system
int make_fs(char *disk_name)
{
	// first invoke make_disk(disk_name) to create a new disk
	//  open this disk and write/initialize the necessary meta-information
	// for file system
	// The function returns 0 on success, and -1 when the disk
	// disk_name could not be created, opened, or properly initialized.

	char inputD[1024] = {};
	char buf2[BLOCK_SIZE];
	struct SuperBlock sb2 = {};
	//check it can work or not
	if (isOpen == 1)
	{
		fprintf(stderr, "%s\n", "Please closed disk first ");
		return -1;
	}
	if (open_disk(disk_name) == -1)
	{
		if (make_disk(disk_name) == -1)
		{

			return -1;
		}
		if (open_disk(disk_name) == -1)
		{
			return -1;
		}
	}

	block_read(0, buf2);
	memcpy(&sb2, buf2, BLOCK_SIZE);
	close_disk(disk_name);
	isOpen = 0;

	// check it's created already or not
	if (sb2.blockNum != 0)
	{
		fprintf(stderr, "%s\n", "File System is initail already, Type '1' if u wanna continue ");
		fgets(inputD, sizeof(char) * MAX_BUFFER, stdin);
		if (inputD[0] != '1')
		{
			fprintf(stderr, "%s\n", "Back to the main menu ");
			return 0;
		}
	}
	// initial data for disk
	if (make_disk(disk_name) == 0)
	{
		if (open_disk(disk_name) == 0)
		{
			isOpen = 1;
			diskName = disk_name;
			if (initlistD(0) != -1)
			{
				if (initBlock() == 0)
				{
					if (initSuperBlock(sb2) == 0)
					{
						if (close_disk(disk_name) == 0)
						{
							isOpen = 0;
							diskName = NULL;
							return 0;
						}

						return -1;
					}
					fprintf(stderr, "initial super block failed\n");
					return -1;
				}
				fprintf(stderr, "initial nodeBlockk or dataBlock failed\n");
				return -1;
			}
			fprintf(stderr, "initial rootDirList failed\n");
			return -1;
		}
		fprintf(stderr, "initial superblock failed\n");
	}

	return -1;
}// end make_fs()



// split command from user
void splitInput(char *command[], char inputD[])
{
	for (int i1 = 0; i1 < MAX_ARGS; i1++)
	{
		command[i1] = NULL;
	}
	char temp[1024][300];
	int i = 0; // the index of iputD
	int j = 0;
	int c = 0;	 // the index for temp
	int index = 0; // index for commands
	int bool1 = 1; //when input if not end
	int tick = 0;  // if not first 'space' or enter

	if (inputD[0] == '\0' || inputD[0] == 10)
	{

		return;
	}

	while (bool1)
	{
		while (inputD[i] == 10 || inputD[i] == ' ' || inputD[i] == '\t' || inputD[i] == '\0')
		{
			if (tick == 0)
			{
				i++;
				continue;
			}
			if (j == 0)
			{
				temp[index][c] = '\0';
				command[index] = temp[index];
				index += 1;
				c = 0;
				j++;
			}

			if (inputD[i] == '\0')
			{
				bool1 = 0;
				break;
			}
			i++;
		}

		temp[index][c] = inputD[i];

		tick = 1;
		c++;
		j = 0;
		i++;
	}

	command[index] = NULL;

} // end Function "splitInput"

//store struct data in disk separately
char *writeCopy(char *des, char *src, int index, int size)
{
	int i = index * BLOCK_SIZE;		// the beginning of src 
	int end = (index + 1) * BLOCK_SIZE;	//the end of src
	if (end > size)
	{
		end = size;
	}
	int j = 0;

	for (; i < end; i++, j++)
	{

		des[j] = src[i];
	}
	return des;
}// end writeCopy

 //combined a struct data in disk
char *copy(char *des, char *src, int index, int size)
{
	int i = index * BLOCK_SIZE; // the beginning of src 
	int end = (index + 1) * BLOCK_SIZE; //the end of src
	if (end > size)
	{
		end = size;
	}
	int j = 0;

	for (; i < end; i++, j++)
	{
		des[i] = src[j];
	}
	return des;
} //end copy()

// store data for a file
int writeFile(int fildes, char *buf, int size)
{
	char des[BLOCK_SIZE];
	int blockNum = (size / BLOCK_SIZE + 1);

	size_t j;
	for (size_t i = 0; i < blockNum; i++)
	{
		writeCopy(des, buf, i, size);
		if (inode[fildes].blockNum[i] > 1000)
		{
			block_write(inode[fildes].blockNum[i], des);
		}
		else
		{
			for (j = 0; j < HALF_BLOCK; j++)
			{
				if (dataBlock.used[j] == false)
				{
					dataBlock.used[j] = true;
					inode[fildes].blockNum[i] = j + DOUBLE_BLOCK_SIZE;
					block_write(j + DOUBLE_BLOCK_SIZE, des);
					superBlock.blockUsedNum++;
					break;
				}
			}
			if (j == HALF_BLOCK)
			{
				superBlock.totalSize = superBlock.totalSize + size - size % BLOCK_SIZE;
				printf("%s", "All data block is used already. Can't write all data for this file. Ple try again.");
				return -1;
			}
		}
	}
	superBlock.totalSize += size;
	return 0;
}// end writeFile

// get data of file from disk
char *copyFile(char *des, short blockIndex, int index, int size)
{
	char buf[BLOCK_SIZE];
	block_read(blockIndex, buf);
	int i = index * BLOCK_SIZE;
	int end = (index + 1) * BLOCK_SIZE;

	if (end > size)
	{
		end = size;
	}
	int j = 0;

	for (; i < end; i++, j++)
	{
		des[i] = buf[j];
	}

	des[i] = '\0';

	return des;
}// end copyFile()

// cut out the required contextx
int cutData(char *buf, int offset, size_t nbyte, int size)
{
	int j = 0;
	if (offset + nbyte >= size)
	{

		for (int i = offset; i <= size; i++, j++)
		{
			buf[j] = buf[i];
		}
		buf[j] = '\0';
		fprintf(stderr, "%s\n%s\n", "The file that you read:", buf);
	}
	else
	{

		int i = offset;
		for (; i < offset + nbyte; i++, j++)
		{
			buf[j] = buf[i];
		}
		buf[j] = '\0';
		fprintf(stderr, "%s\n%s\n", "The file that you read:", buf);
	}
	return 0;
}// end cutData()
