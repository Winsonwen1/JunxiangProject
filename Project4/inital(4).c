#include "final4.h"
#include "disk.h"

int initSuperBlock(struct SuperBlock sb)
{

	sb.rootDir = 5;
	// sb.first_inode_index = 6192;
	// sb.first_inode_idle_index = 6192;
	// sb.first_inode_idle_index = 6192;
	// sb.first_inode_block_index = 8192;
	// sb.blockIndex = 8192;
	sb.iNodeNum = 256;
	sb.blockNum = 8192;
	sb.iNodeUsedNum = 0;
    sb.blockUsedNum = 0;
	sb.fileNum = 1;
    sb.totalSize=0;
	sb.fileOpen = 0;

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
	block nb2;
	char buf[DOUBLE_BLOCK_SIZE];
	char buf2[DOUBLE_BLOCK_SIZE];
	char temp[BLOCK_SIZE];
	
	for(int i;i<DOUBLE_BLOCK_SIZE;i++){
		nb.used[i]=false;
		nb2.used[i]=false;
	}
	nb.used[0] = true;
	nb.used[1] = true;

	memcpy(buf, &nb, DOUBLE_BLOCK_SIZE);
	memcpy(buf2, &nb2, DOUBLE_BLOCK_SIZE);
	for (int i = 1; i < 5; i++)
	{
		if (i == 1)
		{
			writeCopy(temp, buf, i - 1, DOUBLE_BLOCK_SIZE);
			// strncpy(temp, buf, BLOCK_SIZE);
			block_write(i, temp);
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

			// return 0;
		}else if(i==3){
			writeCopy(temp, buf2, i - 3, DOUBLE_BLOCK_SIZE);
			// strncpy(temp, buf, BLOCK_SIZE);
			block_write(i, temp);
		}else if(i==4){
			writeCopy(temp, buf2, i - 3, DOUBLE_BLOCK_SIZE);
			// strncpy(temp, buf + BLOCK_SIZE, BLOCK_SIZE);
			block_write(i, temp);

			return 0;
		}
	}

	return -1;
}

int initEntry(int index)
{

	struct DirectoryEntry dirEntry={};

	// if (index == DIRECTORY_ENTRY_BEGIN_INDEX)
	// {
	// fprintf(stderr, "lllllllllllllll\n");
	dirEntry.index = index;
	// }
	// else
	// {
	// 	//when it's not root dirEntry

	// }

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
	// fprintf(stderr, "dirEntry===%d\n", dirEntry.index);

	if (j == 0)
	{
		nodeBlock.used[dirEntry.index] = true;
		return dirEntry.index;
	}

	return -1;
}

int initlistD(int parentIndex)
{

	struct Directory dir = {};
	//when it's root dir
	// int index;
	// for (int i = 0; i < MAX_FILES; i++)
	// 		{
	// 			dir.dirName[i][0]='\0';

	// 		}
	
	if (nodeBlock.used[DIRECTORY_ENTRY_BEGIN_INDEX] == false)
	{
		// fprintf(stderr, "Come on /n");
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

	//when it's root dir
	if (parentIndex == 0)
	{
		dir.index = DIRECTORY_BEGIN_INDEX;

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
            // sb.fileNum++;
			return dir.index;
		}
	}
	else
	{ //when it's not root dir
		int j;
		for (int i = DIRECTORY_BEGIN_INDEX; i < DIRECTORY_END_INDEX; i += 5)
			if (nodeBlock.used[i] == false)
			{
				dir.parent = parentIndex;
				dir.index = i;

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
                    superBlock.fileNum++;
					return dir.index;
				}
			}
	}

	return -1;
}

int initINode()
{
	struct INode inodeTemp={};
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	for (int i = INODE_BEGIN_INDEX; i <= INODE_END_INDEX; i += 5)
	{
		if (nodeBlock.used[i] == false)
		{

			inodeTemp.size = 0;

			inodeTemp.year = 1900 + timeinfo->tm_year;
			inodeTemp.month = 1 + timeinfo->tm_mon;
			inodeTemp.day = timeinfo->tm_mday;
			inodeTemp.hour = timeinfo->tm_hour;
			inodeTemp.min = timeinfo->tm_min;
			inodeTemp.sec = timeinfo->tm_sec;

			int check;
			char buf[INODE_SIZE];

			char temp[BLOCK_SIZE] = {};

			memcpy(buf, &inodeTemp, INODE_SIZE);

			for (int j = 0; j < 5; j++)
			{
				writeCopy(temp, buf, j, INODE_SIZE);
				check = block_write(i + j, temp);
				// fprintf(stderr, "===%d\n", i+j);
				// k = BLOCK_SIZE * j;
				// strncpy(temp, buf + k, BLOCK_SIZE);
				// block_read(i + j, temp);
				// fprintf(stderr, "---%d\n", i+j);
				// strncat(buf2, temp, BLOCK_SIZE);

				if (check != 0)
				{
					return -1;
				}
			}

			// struct INode inodeTemp2;
			// char temp2[BLOCK_SIZE] = {};
			// char buf2[INODE_SIZE];
			// for (int j = 0; j < 5; j++)
			// {
			// 	block_read(i + j, temp2);
			// 	// fprintf(stderr, "---%d\n", i+j);
			// 	copy(buf2, temp2, j,INODE_SIZE);
			// 	// strncat(buf2, temp2, BLOCK_SIZE);
			// 	// if (j == 0)
			// 	// {
			// 		// block_read(i+j, temp2);

			// 	// 	fprintf(stderr, "---%d\n", i+j);
			// 	// 	copy(buf2, temp2, j);
			// 	// 	continue;
			// 	// }

			// 	// printf("\n=========\n",i,end );
			// }
			// memcpy(&inodeTemp2, buf2, INODE_SIZE);
			// fprintf(stderr, "%d\n", i);
			// fprintf(stderr, "%d\n", inodeTemp2.size);
			// fprintf(stderr, "%d\n", inodeTemp.day);
			// fprintf(stderr, "%d\n", inodeTemp2.day);

			nodeBlock.used[i] = true;
			superBlock.iNodeUsedNum++;
			superBlock.fileNum++;

			fprintf(stderr, " fileNum: %d AAA \n", superBlock.fileNum);
			fprintf(stderr, "iNodeUsedNum:   %d AAA\n", superBlock.iNodeUsedNum);
			fprintf(stderr, "iNode index:   %d AAA\n", i);
			return i;

			// memcpy(buf, &inodeTemp, INODE_SIZE);
			// for(int i =0;i<INODE_SIZE;i++){
			// 	buf2[i] =buf[i];
			// }
			// strncat(buf2, buf,INODE_SIZE);
			// buf2[INODE_SIZE]='\0';
			// 	sleep(1);
			// 	puts("\n");
			// for(int i ;i<INODE_SIZE;i++){
			// 	printf("%c",buf[i] );
			// }
			// puts("\n");
			// sleep(1);
			// for(int i ;i<INODE_SIZE;i++){
			// 	printf("%c",buf2[i] );
			// }
			// puts("\n");
			// sleep(1);
			// for(int i ;i<INODE_SIZE;i++){
			// 	printf("%c",buf[i] );
			// }
			// puts("\n");
			// sleep(1);
		}
	}
	return -1;
}
