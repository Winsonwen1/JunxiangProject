#include "final4.h"
#include "disk.h"

int initSuperBlock(struct SuperBlock sb)
{

	sb.rootDir = 5;
	sb.first_inode_index = 6192;
	sb.first_inode_idle_index = 6192;
	sb.first_inode_idle_index = 6192;
	sb.first_inode_block_index = 8192;
	sb.blockIndex = 8192;
	sb.iNodeNum = 256;
	sb.blockNum = 8192;
	sb.iNodeUsedNum = 0;
	sb.fileNum = 1;
	strcpy(sb.disk_name, diskName);
	sb.blockUsedNum = 0;

	char buf[BLOCK_SIZE];
	memcpy(buf, &sb, SUPER_BLOCK_SIZE);
	int i = block_write(0, buf);

	if (i == 0)
	{
		fprintf(stderr, "initial successful\n");
		return 0;
	}

	return -1;
}

int initBlock()
{
	block nb;
	char buf[DOUBLE_BLOCK_SIZE];
	char temp[BLOCK_SIZE];
	nb.used[0] = true;
	nb.used[1] = true;

	memcpy(buf, &nb, DOUBLE_BLOCK_SIZE);

	for (int i = 1; i < 3; i++)
	{
		if (i == 1)
		{
			writeCopy(temp, buf, i - 1, DOUBLE_BLOCK_SIZE);
			// strncpy(temp, buf, BLOCK_SIZE);
			block_write(i, buf);
		}
		else if (i == 2)
		{
			writeCopy(temp, buf, i - 1, DOUBLE_BLOCK_SIZE);
			// strncpy(temp, buf + BLOCK_SIZE, BLOCK_SIZE);
			block_write(i, temp);

			//     char buf2[DOUBLE_BLOCK_SIZE];
			// 	char temp2[BLOCK_SIZE];

			// 	for (int k = 1; k < 3; k++)
			// 	{
			//         block_read(k, temp2);
			//         copy(buf2, temp2, k-1,DOUBLE_BLOCK_SIZE);
			// 	// block_read(k, temp2);
			// 	// 	strncat(buf2, temp2, BLOCK_SIZE);

			// 	}
			// 	memcpy(&nodeBlock, buf2, DOUBLE_BLOCK_SIZE);
			//     fprintf(stderr, "----%d----\n", nodeBlock.used[0]);
			// fprintf(stderr, "----%d----\n", nodeBlock.used[8126]);

			return 0;
		}
	}

	return -1;
}

int initEntry(int index)
{

	struct DirectoryEntry dirEntry;

	if (index == DIRECTORY_ENTRY_BEGIN_INDEX)
	{
		dirEntry.index = index;
	}
	else
	{
		// 当不是根目录
	}

	int j;
	char buf[ENTRY_SIZE];
	char temp[BLOCK_SIZE];
	memcpy(buf, &dirEntry, ENTRY_SIZE);

	for (int k = 0; k < 2; k++)
	{
		// strncpy(temp, buf + BLOCK_SIZE * k, BLOCK_SIZE);
		writeCopy(temp, buf, k, ENTRY_SIZE);
		j = block_write(dirEntry.index + k, temp);
		if (j != 0)
		{
			fprintf(stderr, "Directory initial failed\n");
			return -1;
		}
	}
	// fprintf(stderr, "rootindex2===%d\n", dir->index);
	if (j == 0)
	{
		return 0;
	}

	return -1;
}

int initlistD(int parentIndex)
{

	struct Directory dir;

	if (nodeBlock.used[600] == false)
	{
		dir.dirEntry = initEntry(DIRECTORY_ENTRY_BEGIN_INDEX);
	}
	else
	{
		// 当不是根目录
	}

	if (parentIndex == 0)
	{
		dir.index = DIRECTORY_BEGIN_INDEX;
	}
	else
	{
		// 当不是根目录
	}

	dir.parent = parentIndex;
	int j;
	char buf[DIR_SIZE];
	char temp[BLOCK_SIZE];
	// char temp3[BLOCK_SIZE];
	memcpy(buf, &dir, DIR_SIZE);
	for (int k = 0; k < 2; k++)
	{
		writeCopy(temp, buf, k, DIR_SIZE);
		// strncpy(temp, buf + (BLOCK_SIZE * k), BLOCK_SIZE);
		j = block_write(k + dir.index, temp);
		if (j != 0)
		{
			fprintf(stderr, "Directory initial failed\n");
			return -1;
		}
	}
	if (j == 0)
	{
		return 0;
	}

	return -1;
}
