
#include "final4.h"
#include "disk.h"
struct SuperBlock superBlock;
// iNode inode[256]; // the max number of files
int fd[64];
char pwd[MAX_BUFFER];
block dataBlock;
block nodeBlock;

int main()
{

	// nihao();
	char inputD[1024] = {}; // input data from user
	char *command[1024] = {};
	strcpy(pwd, "root");
	// int r; //store return value
	while (1)
	{
		//	display the instruction
		//	line = get line from user;
		//	commands <- split line by ' ';
		//	runOneCommand(command);

		fprintf(stderr, "%s", "Please type your command: ");
		fgets(inputD, sizeof(char) * MAX_BUFFER, stdin);
		//			for(int i; command[i]!=NULL; i++) {
		//		printf("%s\n",command[i]);
		//	}
		splitInput(command, inputD);

		runOneCommand(command);
	}
}






int make_fs(char *disk_name)
{
	// first invoke make_disk(disk_name) to create a new disk
	//  open this disk and write/initialize the necessary meta-information
	// for file system
	// The function returns 0 on success, and -1 when the disk
	// disk_name could not be created, opened, or properly initialized.
	struct SuperBlock sb;

	char inputD[1024] = {};

	char buf2[MAX_BUFFER];
	struct SuperBlock sb2;
	open_disk(disk_name);
	block_read(0, buf2);
	memcpy(&sb2, buf2, MAX_BUFFER);
	close_disk(disk_name);
	printf("===========%d============", sb2.blockNum);
	printf("===========%d============", sb2.iNodeNum);
	printf("===========%d============", sb2.first_inode_idle_index);
	printf("===========%d============", sb2.iNodeUsedNum);
	if (sb2.blockNum != 0)
	{
		fprintf(stderr, "%s", "File System is initail already, Type '1' if u wanna continue ");
		fgets(inputD, sizeof(char) * MAX_BUFFER, stdin);
		if (inputD[0] != '1')
		{
			fprintf(stderr, "%s", "Back to the main menu ");
			return 0;
		}
	}

	if (make_disk(disk_name) == 0)
	{

		if (initSuperBlock(sb, disk_name) == 0)
		{
			if (close_disk(disk_name) == 0)
			{
				diskName = NULL;
				return 0;
			}

			return -1;
		}
		fprintf(stderr, "initial superblock failed\n");
		return -1;
	}

	return -1;
}

int initSuperBlock(struct SuperBlock sb3, char *disk_name2)
{
	make_disk("cc");
	char *disk_name = "cc";
	struct SuperBlock sb;
	if (open_disk(disk_name) == 0)
	{

		printf("66 \n");

		printf("111 \n");
		sb.first_inode_index = 6192;
		sb.first_inode_idle_index = 6192;
		sb.first_inode_idle_index = 6192;
		sb.first_inode_block_index = 8192;
		sb.blockIndex = 8192;
		sb.iNodeNum = 256;
		sb.blockNum = 8192;
		sb.iNodeUsedNum = 0;
		sb.disk_name = disk_name;
		sb.blockUsedNum = 0;

		char buf[MAX_BUFFER];
		memcpy(buf, &sb, MAX_BUFFER);
		int i = block_write(0, buf);
		// printf("==iiii=====%d=====\n", i);
		// printf("==sb=====%d=====\n", sb.blockNum);
		// printf("==temp=====%s=====\n", buf);
		// block_read(0, temp2);
		// memcpy(&sb2, temp2, MAX_BUFFER);
		printf("==sb2=====%d=====\n", sb.first_inode_idle_index);
		printf("==temp=====%s=====\n", buf);
		close_disk(disk_name);

		char buf2[MAX_BUFFER];
		struct SuperBlock sb2;
		open_disk(disk_name);
		block_read(0, buf2);
		memcpy(&sb2, buf2, MAX_BUFFER);

		//   printf("===========%ld============\n", sizeof(sb));
		//   printf("===========%d============\n", sb.blockNum);
		printf("\n===========%d============\n", sb2.blockNum);

		if (i == 0)
		{
			fprintf(stderr, "initial successful\n");
			return 0;
		}

		return -1;
	}
	return -1;
}

// int initDirectoryEntry(directory_entry **ide, int INode_id)
// {

// 	*ide = (super_block *)malloc(sizeof(ide));
// 	char *temp;
// 	if (*ide == NULL)
// 	{
// 		printf("Can't assign to node, application ends \n");
// 		exit(1);
// 	}

// 	if (block_read(0, temp) == 0)
// 	{
// 		if (temp == NULL)
// 		{
// 		}
// 	}

// 	printf("Can't initial Super Block \n");
// 	return -1;
// }

/*  Function "runOneCommand"  :  used to Run a command */

int runOneCommand(char *command[])
{
	//check command
	int r;

	if (strcmp(command[0], "mkDisk") == 0)
	{

		r = make_disk(command[1]);
		if (r == 0)
		{
		}
		return 1;
	}
	else if (strcmp(command[0], "oD") == 0)
	{
		r = open_disk(command[1]);

		if (r == 0)
		{
			fprintf(stderr, "open disk successful \n");
			diskName = command[1];
		}

		return 2;
	}
	else if (strcmp(command[0], "cD") == 0)
	{
		r = close_disk(command[1]);

		if (r == 0)
		{
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
	else if (strcmp(command[0], "environ") == 0)
	{

		return 5;
	}
	else if (strcmp(command[0], "echo") == 0)
	{
		return 6;
	}
	else if (strcmp(command[0], "help") == 0)
	{
		return 7;
	}
	else if (strcmp(command[0], "pause") == 0)
	{
		// return;
	}
	else if (strcmp(command[0], "quit") == 0)
	{
		return -1;
	}
	else
	{
		fprintf(stderr, "%s\n", "Can't find this command!! Retry!'");
		return 0;
	}
	return -1;
}

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
