/*
 * writeback.c
 *
 *  Last Edit: Dec 4, 2019
 *      Author: Junxiang Wen
 */
#include "final4.h"
#include "disk.h"

// storing the dir entry into disk
int wirteDirEntry()
{
	//get the index of entry
	int index1 = currectEntry.index;
	if (index1 == 0)
	{
		return 0;
	}

	// storing current dir entry
	int j;
	char buf[ENTRY_SIZE];
	char temp[BLOCK_SIZE];
	memcpy(buf, &currectEntry, ENTRY_SIZE);
	for (int k = 0; k < 2; k++)
	{
		writeCopy(temp, buf, k, ENTRY_SIZE);
		j = block_write(index1 + k, temp);
		if (j != 0)
		{
			return -1;
		}
	}
	memset(&currectEntry, 0, sizeof(struct DirectoryEntry));

	return 0;
}// end wirteDirEntry()

// storing the dir into disk
int wirteBacklistD()
{
	// get the index of current dir 
	int index1 = currectDirList.index;
	//storing dir
	int j;
	char buf[DIR_SIZE];
	char temp[BLOCK_SIZE];
	memcpy(buf, &currectDirList, DIR_SIZE);
	for (int k = 0; k < 2; k++)
	{
		writeCopy(temp, buf, k, DIR_SIZE);
		j = block_write(index1 + k, temp);

		if (j != 0)
		{
			fprintf(stderr, "Storing Directory failed\n");
			return -1;
		}
	}

	memset(&currectDirList, 0, sizeof(struct Directory));

	if (j == 0)
	{
		return 0;
	}
	return -1;
} // end wirteBacklistD()

//	storing block into disk
int wirteBackBlock()
{
	// sotre node block
	char buf[DOUBLE_BLOCK_SIZE];
	char temp[BLOCK_SIZE];

	memcpy(buf, &nodeBlock, DOUBLE_BLOCK_SIZE);
	for (int i = 1; i < 3; i++)
	{
		if (i == 1)
		{
			writeCopy(temp, buf, i - 1, DOUBLE_BLOCK_SIZE);
			block_write(i, buf);
		}
		else if (i == 2)
		{
			writeCopy(temp, buf, i - 1, DOUBLE_BLOCK_SIZE);
			block_write(i, temp);
		}
	}

	// sotre data block
	memcpy(buf, &dataBlock, DOUBLE_BLOCK_SIZE);
	for (int i = 3; i < 5; i++)
	{
		if (i == 3)
		{
			writeCopy(temp, buf, i - 3, DOUBLE_BLOCK_SIZE);
			block_write(i, buf);
		}
		else if (i == 4)
		{
			writeCopy(temp, buf, i - 3, DOUBLE_BLOCK_SIZE);
			block_write(i, temp);
			memset(&nodeBlock, 0, sizeof(struct Block));
			memset(&dataBlock, 0, sizeof(struct Block));
			return 0;
		}
	}
	return -1;
}	// end wirteBackBlock()

// storing superblock into disk
int wirteBackSuperBlock()
{

	char buf[BLOCK_SIZE];
	memcpy(buf, &superBlock, BLOCK_SIZE);
	int i = block_write(0, buf);
	if (i == 0)
	{
		memset(&superBlock, 0, sizeof(struct SuperBlock));
		return 0;
	}

	return -1;
} // end wirteBackSuperBlock()