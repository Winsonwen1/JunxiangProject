/*
 * inital.c
 *
 *  Last Edit: Dec 4, 2019
 *      Author: Junxiang Wen
 */

#include "final4.h"
#include "disk.h"
//  initialize superblock
int initSuperBlock(struct SuperBlock sb)
{
	// set up basic configuration for superblock
	sb.rootDir = 5;
	sb.iNodeNum = 256;
	sb.blockNum = 8192;
	sb.iNodeUsedNum = 0;
	sb.blockUsedNum = 0;
	sb.fileNum = 1;
	sb.totalSize = 0;
	sb.fileOpen = 0;
	sb.blockUsedNum = 0;
	//record disk name
	strcpy(sb.disk_name, diskName);

	// save super block into disk
	char buf[BLOCK_SIZE];
	memcpy(buf, &sb, SUPER_BLOCK_SIZE);
	int i = block_write(0, buf);
	if (i == 0)
	{
		fprintf(stderr, "initial successful\n");
		return 0;
	}
	return -1;
} // end initSuperBlock()

//  initialize  struct block
int initBlock()
{
	block nb;  //temporary nodeBlock
	block nb2; //temporary data Block

	// initialize data in block
	for (int i; i < DOUBLE_BLOCK_SIZE; i++)
	{
		nb.used[i] = false;
		nb2.used[i] = false;
	}
	nb.used[0] = true;
	nb.used[1] = true;

	//storing data
	char buf[DOUBLE_BLOCK_SIZE];
	char buf2[DOUBLE_BLOCK_SIZE];
	char temp[BLOCK_SIZE];
	memcpy(buf, &nb, DOUBLE_BLOCK_SIZE);
	memcpy(buf2, &nb2, DOUBLE_BLOCK_SIZE);
	for (int i = 1; i < 5; i++)
	{
		//store data for nb
		if (i == 1)
		{
			writeCopy(temp, buf, i - 1, DOUBLE_BLOCK_SIZE);
			block_write(i, temp);
		}
		else if (i == 2)
		{
			writeCopy(temp, buf, i - 1, DOUBLE_BLOCK_SIZE);
			block_write(i, temp);
		}
		else if (i == 3)
		{ //store data for nb2
			writeCopy(temp, buf2, i - 3, DOUBLE_BLOCK_SIZE);
			block_write(i, temp);
		}
		else if (i == 4)
		{
			writeCopy(temp, buf2, i - 3, DOUBLE_BLOCK_SIZE);
			block_write(i, temp);

			return 0;
		}
	}
	return -1;
}// end initBlock()

 // initialize dir entry
int initEntry(int index)
{
	// temporary dir entry
	struct DirectoryEntry dirEntry = {};
	dirEntry.index = index;	//set up the index


	int j;	// if block_write failed
	//store data into disk
	char buf[ENTRY_SIZE];
	char temp[BLOCK_SIZE];
	memcpy(buf, &dirEntry, ENTRY_SIZE);
	for (int k = 0; k < 2; k++)
	{
		writeCopy(temp, buf, k, ENTRY_SIZE);
		j = block_write(dirEntry.index + k, temp);
		if (j != 0)
		{
			fprintf(stderr, "Directory initial failed\n");
			return -1;
		}
	}
	if (j == 0)
	{
		nodeBlock.used[dirEntry.index] = true;
		return dirEntry.index;
	}

	return -1;
}// end initEntry()

// initialize dir
int initlistD(int parentIndex)
{
	// temporary dir
	struct Directory dir = {};
	// set up created time
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	dir.year = 1900 + timeinfo->tm_year;
	dir.month = 1 + timeinfo->tm_mon;
	dir.day = timeinfo->tm_mday;
	dir.hour = timeinfo->tm_hour;
	dir.min = timeinfo->tm_min;
	dir.sec = timeinfo->tm_sec;

	// find the available block for dir entry and creat a dir entry for it
	//if it's a root dir
	if (nodeBlock.used[DIRECTORY_ENTRY_BEGIN_INDEX] == false)
	{
		dir.dirEntry = initEntry(DIRECTORY_ENTRY_BEGIN_INDEX);
		if (dir.dirEntry == -1)
		{
			printf("%s\n", "DirEntry created filed.");
			return -1;
		}
	}
	else //when it's not root dir
	{
		for (int i = DIRECTORY_ENTRY_BEGIN_INDEX; i < DIRECTORY_ENTRY_END_INDEX; i += 5)
		{
			if (nodeBlock.used[i] == false)
			{
				dir.dirEntry = initEntry(i);
				if (dir.dirEntry == -1)
				{
					printf("%s\n", "DirEntry created filed.");
					return -1;
				}
			}
		}
	}

	//store the dir 
	//when it's root dir
	if (parentIndex == 0)
	{
		dir.index = DIRECTORY_BEGIN_INDEX;
		dir.parent = parentIndex;
		int j;
		char buf[DIR_SIZE];
		char temp[BLOCK_SIZE];
		memcpy(buf, &dir, DIR_SIZE);
		for (int k = 0; k < 2; k++)
		{
			writeCopy(temp, buf, k, DIR_SIZE);
			j = block_write(k + dir.index, temp);
			if (j != 0)
			{
				fprintf(stderr, "Directory initial failed\n");
				return -1;
			}
		}
		if (j == 0)
		{
			printf("%s%4d-%02d-%02d %02d:%02d:%02d\n", "Disk created in: ", dir.year, dir.month, dir.day, dir.hour, dir.min, dir.sec);
			return dir.index;
		}
	}
	else
	{ //when it's not root dir
		int j;
		// find the available block for dir
		for (int i = DIRECTORY_BEGIN_INDEX; i < DIRECTORY_END_INDEX; i += 5)
			if (nodeBlock.used[i] == false)
			{
				//set up
				dir.parent = parentIndex;
				dir.index = i;
				// storing dir
				char buf[DIR_SIZE];
				char temp[BLOCK_SIZE];
				memcpy(buf, &dir, DIR_SIZE);
				for (int k = 0; k < 2; k++)
				{
					writeCopy(temp, buf, k, DIR_SIZE);
					j = block_write(k + dir.index, temp);
					if (j != 0)
					{
						fprintf(stderr, "Directory initial failed\n");
						return -1;
					}
				}
				if (j == 0)
				{
					superBlock.fileNum++;
					printf("%s%4d-%02d-%02d %02d:%02d:%02d\n", "File created in: ", dir.year, dir.month, dir.day, dir.hour, dir.min, dir.sec);
					return dir.index;
				}
			}
	}
	fprintf(stderr, "Directory initial failed\n");
	return -1;
} //end initlistD()

// inilist a inode
int initINode()
{	
	struct INode inodeTemp = {};	// temporary iNode
	time_t rawtime;	
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	// find a available block for inode
	for (int i = INODE_BEGIN_INDEX; i <= INODE_END_INDEX; i += 5)
	{
		if (nodeBlock.used[i] == false)
		{

			inodeTemp.size = 0;
			// set up created time for inode
			inodeTemp.year = 1900 + timeinfo->tm_year;
			inodeTemp.month = 1 + timeinfo->tm_mon;
			inodeTemp.day = timeinfo->tm_mday;
			inodeTemp.hour = timeinfo->tm_hour;
			inodeTemp.min = timeinfo->tm_min;
			inodeTemp.sec = timeinfo->tm_sec;

			//store inode
			int check;
			char buf[INODE_SIZE];
			char temp[BLOCK_SIZE] = {};
			memcpy(buf, &inodeTemp, INODE_SIZE);
			for (int j = 0; j < 5; j++)
			{
				writeCopy(temp, buf, j, INODE_SIZE);
				check = block_write(i + j, temp);
				if (check != 0)
				{
					return -1;
				}
			}
			nodeBlock.used[i] = true;
			superBlock.iNodeUsedNum++;
			superBlock.fileNum++;
			printf("%s%4d-%02d-%02d %02d:%02d:%02d\n", "File created in: ", inodeTemp.year, inodeTemp.month, inodeTemp.day, inodeTemp.hour, inodeTemp.min, inodeTemp.sec);
			return i;
		}
	}
	return -1;
} // end initINode()
