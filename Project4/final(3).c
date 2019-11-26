
#include "final4.h"
#include "disk.h"

// struct Directory dir;
int main()
{
	// printf("444");
	char inputD[1024] = {}; // input data from user
	char *command[1024] = {};
	isMout = 0;
	isOpen = 0;
	// int r; //store return value

	while (1)
	{
		//	display the instruction
		//	line = get line from user;
		//	commands <- split line by ' ';
		//	runOneCommand(command);

		// printf("34");
		fprintf(stderr, "%s", "Please type your command: ");
		// printf("12");
		fgets(inputD, sizeof(char) * MAX_BUFFER, stdin);
		// gets(inputD);
		// printf("5566");
		//			for(int i; command[i]!=NULL; i++) {
		//		printf("%s\n",command[i]);
		//	}

		splitInput(command, inputD);
		// fprintf(stderr, "%s", "1111 ");
		runOneCommand(command);
	}
}

int runOneCommand(char *command[])
{
	//check command
	int r;
	if (command[0] == NULL)
	{
		fprintf(stderr, "%s\n", "Can't find this command!! Retry!'");
		return 0;
	}
	if (strcmp(command[0], "mkDisk") == 0)
	{

		r = make_disk(command[1]);
		if (r == 0)
		{
		}
		return 1;
	}
	else if (strcmp(command[0], "oDisk") == 0)
	{
		r = open_disk(command[1]);

		if (r == 0)
		{
			isOpen = 1;
			fprintf(stderr, "open disk successful \n");
			diskName = command[1];
		}

		return 2;
	}
	else if (strcmp(command[0], "cDisk") == 0)
	{
		r = close_disk(command[1]);

		if (r == 0)
		{
			isOpen = 0;
			diskName = NULL;
			fprintf(stderr, "close disk successful \n");
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
	else if (strcmp(command[0], "pause") == 0)
	{
		// return;
	}
	else if (strcmp(command[0], "quit") == 0)
	{
		umount_fs(diskName);
		fprintf(stderr, "%s\n", "See you soon!!!");
		exit(1);
		return -1;
	}
	else
	{
		fprintf(stderr, "%s\n", "Can't find this command!! Retry!");
		return 0;
	}
	return -1;
}

int fs_create(char *name)
{
	//The maximum length for a file name is 15 characters
	//at most 256 files in the directory
	//The max number of block is 8192
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
	if (superBlock.fileNum > MAX_FILES)
	{
		fprintf(stderr, "%s\n", "Over 256 files in the directory, Request Reject!");
		return -1;
	}
	if (superBlock.blockUsedNum > HALF_BLOCK)
	{
		fprintf(stderr, "%s\n", "Over the max number of block(8192), Request Reject!");
		return -1;
	}
	if (superBlock.iNodeUsedNum > MAX_INODE)
	{
		fprintf(stderr, "%s\n", "Over the max number of INode(256), Request Reject!");
		return -1;
	}

	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strcmp(currectEntry.dirName[i], name) == 0)
		{
			fprintf(stderr, "%s\n", "There are same name of file in currect directory, Request Reject!");
			return -1;
		}
	}
	// printf("---------%s-----------\n",currectEntry.dirName[0]);
	// printf("lllllll   %d   llllllllll\n",strcmp(currectEntry.dirName[0], name));
	// 	printf("lllllll   %d   llllllllll\n",strcmp(currectEntry.dirName[0], ""));
	// printf("=========%s=======\n",name);
	for (int i = 0; i < MAX_FILES; i++)
	{
		if (strcmp(currectEntry.dirName[i], "") == 0)
		{
			int j = initINode();
			if (j == -1)
			{
				fprintf(stderr, "%s\n", "Can't create file!");
				return -1;
			}

			strcpy(currectEntry.dirName[i], name);
			currectEntry.INode_id[i] = j;
			fprintf(stderr, "%s\n", "File created successful!");
			return 0;
		}
	}
	fprintf(stderr, "%s\n", "Can't create file!");
	return -1;
}

int initINode()
{
	struct INode inodeTemp;
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

int umount_fs(char *disk_name)
{
	//o write back all meta-information so that the disk persistently reflects
	//all changes that were made to the file system
	//close the disk. The function returns 0 on success, and -1 when the
	//disk disk_name could not be closed or when data could not be written to the disk

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

	close_disk(disk_name);
	isOpen = 0;
	return -1;
}

int mount_fs(char *disk_name)
{
	//open the disk and then load the meta-information
	// that is necessary to handle the file system operations
	// The function returns 0 on success, and -1 when the disk disk_name could not
	//be opened or when the disk does not contain a valid file system
	char buf2[BLOCK_SIZE];
	char blockBuf[DOUBLE_BLOCK_SIZE];

	if (open_disk(disk_name) == -1)
	{

		return -1;
	}
	isOpen = 1;
	//initial super block
	block_read(0, buf2);
	memcpy(&superBlock, buf2, SUPER_BLOCK_SIZE);
	printf("==========%d\n", 444);
	if (superBlock.blockNum == 0)
	{
		fprintf(stderr, "%s\n", "File System is not initail yet, Ple make a File System for this disk first ");
		close_disk(disk_name);
		isOpen = 0;
		return -1;
	}

	printf("==========%d\n", 333);
	// initial nodeBlock and data Block
	for (int i = 1; i < 5; i++)
	{
		if (i == 1)
		{
			block_read(i, buf2);
			// strncpy(blockBuf, buf2, BLOCK_SIZE);
			copy(blockBuf, buf2, i - 1, DOUBLE_BLOCK_SIZE);
			// 	// strncat(buf2, temp2, BLOCK_SIZE);
		}
		else if (i == 2)
		{
			block_read(i, buf2);
			copy(blockBuf, buf2, i - 1, DOUBLE_BLOCK_SIZE);
			// strncat(blockBuf, buf2, BLOCK_SIZE);
			memcpy(&nodeBlock, blockBuf, DOUBLE_BLOCK_SIZE);
		}
		else if (i == 3)
		{
			block_read(i, buf2);
			copy(blockBuf, buf2, i - 3, DOUBLE_BLOCK_SIZE);
			// strncpy(blockBuf, buf2, BLOCK_SIZE);
		}
		else if (i == 4)
		{
			block_read(i, buf2);
			copy(blockBuf, buf2, i - 3, DOUBLE_BLOCK_SIZE);
			// strncat(blockBuf, buf2, BLOCK_SIZE);
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
			// strncpy(dirBuf, buf2, BLOCK_SIZE);
			continue;
		}
		// if(k==21){
		// 	block_read(k, buf2);
		// 	strncat(dirBuf, buf2, 3828);
		// 	break;
		// }

		block_read(k, buf2);
		copy(dirBuf, buf2, k - 5, DIR_SIZE);
		// strncat(dirBuf, buf2, BLOCK_SIZE);
	}

	memcpy(&currectDirList, dirBuf, DIR_SIZE);
	// printf("==========%d\n", currectDirList.index);

	// update pwd
	// strcpy(pwd, diskName);
	for (int j = 0; disk_name[j] != '\0'; j++)
	{

		pwd[j] = disk_name[j];
	}
	strcat(pwd, ":/root/");

	// fprintf(stderr, "----%d----\n", superBlock.blockNum);
	fprintf(stderr, "----%d----\n", nodeBlock.used[1]);
	fprintf(stderr, "----%d----\n", currectDirList.index);
	fprintf(stderr, "%s\n", "Mounted Disk successful!");
	isMout = 1;

	diskName = disk_name;
	return 0;
}

int make_fs(char *disk_name)
{
	// first invoke make_disk(disk_name) to create a new disk
	//  open this disk and write/initialize the necessary meta-information
	// for file system
	// The function returns 0 on success, and -1 when the disk
	// disk_name could not be created, opened, or properly initialized.

	char inputD[1024] = {};
	char buf2[BLOCK_SIZE];
	struct SuperBlock sb2;

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
	// printf("-----------%d--------", sb2.blockNum);

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

	if (make_disk(disk_name) == 0)
	{
		if (open_disk(disk_name) == 0)
		{
			isOpen = 1;
			diskName = disk_name;
			if (initlistD(0) == 0)
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
}

/*  Function "runOneCommand"  :  used to Run a command */

/*  Function "splitInput"  : split input for execute */
void splitInput(char *command[], char inputD[])
{

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

char *writeCopy(char *des, char *src, int index, int size)
{
	int i = index * BLOCK_SIZE;
	int end = (index + 1) * BLOCK_SIZE;
	if (end > size)
	{
		end = size;
	}
	int j = 0;

	for (; i < end; i++, j++)
	{
		des[j] = src[i];
		// printf("%c",des[j] );
	}
	printf("\n%s====1=====%d\n", des, end);
	return des;
}

char *copy(char *des, char *src, int index, int size)
{
	int i = index * BLOCK_SIZE;
	int end = (index + 1) * BLOCK_SIZE;
	if (end > size)
	{
		end = size;
	}
	int j = 0;

	for (; i < end; i++, j++)
	{
		des[i] = src[j];
		// printf("%c",src[j] );
	}
	printf("\n%s====2=====%d\n", src, end);
	return des;
}
