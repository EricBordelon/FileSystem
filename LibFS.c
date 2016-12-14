#include "LibFS.h"
#include "LibDisk.h"

const int MAX_FILES = 1000;

// global errno value here
int osErrno;

//global string to hold the disk file
char* dFile;

//Buffer to read/write to disk
char diskBuff [SECTOR_SIZE];

//Structure of an inode
typedef struct iNode{
	char name[15];
	char type;
	short size;
	short data[30];
}inode;

//The entries in the open-file table
typedef struct openF{
	int fp;
	short size;
	short data[30];
}open_file;

open_file ofTable[256];

int filesOpen;

int
FS_Boot(char *path)
{
	printf("FS_Boot %s\n", path);

	//Initialize disk structure;check for errors
	if (Disk_Init() == -1) {
		printf("Error! Disk_Init() failed\n");
		osErrno = E_GENERAL;
		return -1;
	}

	//Try to load the disk at path
	if (Disk_Load(path) == -1)
	{
		if (diskErrno == E_READING_FILE)
		{
			printf("Error! Disk_Load() failed\n");
			osErrno = E_GENERAL;
			return -1;
		}
		if (diskErrno == E_INVALID_PARAM
			|| diskErrno == E_OPENING_FILE)
		{
			printf("The file containing your disk could not be found or read.\n");
			printf("Creating file %s instead\n", path);

			if (Disk_Save(path) == -1)
			{
				printf("Error! Disk_Save() failed\n");
				osErrno = E_GENERAL;
				return -1;
			}
			
			printf("Now writing to superblock...\n");
			if (Disk_Read(0, diskBuff) == -1)
			{
				printf("Error! Disk_Read() failed\n");
				osErrno = E_GENERAL;
				return -1;
			}
			
			diskBuff[0] = 26;
			diskBuff[0] = 54;

			if (Disk_Write(0, diskBuff) == -1)
			{
				printf("Error! Disk_Write() failed\n");
				osErrno = E_GENERAL;
				return -1;
			}

			//The next 20 sectors are to be used as maps of the disk
			//(Yes, 20. I couldn't figure out how to manipulate the bits directly)
			for (int i = 1; i <= 20; ++i)
			{
				if (Disk_Read(i, diskBuff) == -1)
				{
					printf("Error! Disk_Read() failed\n");
					osErrno = E_GENERAL;
					return -1;
				}
				for (int j = 0; j < SECTOR_SIZE; ++j)
				{
					diskBuff[j] = 0;
				}
				if (Disk_Write(i, diskBuff) == -1)
				{
					printf("Error! Disk_Write() failed\n");
					osErrno = E_GENERAL;
					return -1;
				}
			}

			//The next sector will serve as the inode table
			if (Disk_Read(21, diskBuff) == -1)
			{
				printf("Error! Disk_Read() failed\n");
				osErrno = E_GENERAL;
				return -1;
			}
			
			diskBuff[0] = 22;
			diskBuff[1] = 1;
			
			if (Disk_Write(21, diskBuff) == -1)
			{
				printf("Error! Disk_Write() failed\n");
				osErrno = E_GENERAL;
				return -1;
			}

			//Create root inode

			dFile = path;
			return 0;
		}
	}
	
	if (Disk_Read(0, diskBuff) == -1)
	{
		printf("Error! Disk_Read() failed\n");
		osErrno = E_GENERAL;
		return -1;
	}
	if (diskBuff[0] != 26 && diskBuff[1] != 54)
	{
		printf("Error! Bad control number in superblock\n");
		osErrno = E_GENERAL;
		return -1;
	}

	dFile = path;
	return 0;
}

int
FS_Sync()
{
	printf("FS_Sync\n");
	if (Disk_Save(dFile) == -1)
	{
		if (diskErrno == E_INVALID_PARAM
			|| diskErrno == E_OPENING_FILE)
		{
			printf("Error! %s could not be found or opened.\n", dFile);
			osErrno = E_GENERAL;
			return -1;
		}
		if (diskErrno == E_WRITING_FILE)
		{
			printf("Error writing to file %s \n", dFile);
			osErrno = E_GENERAL;
			return -1;
		}
	}
	return 0;
}


int
File_Create(char *file)
{
	printf("FS_Create\n");
	//Set a free inode to hold the file data
	//file does not need any sectors yet
	//Update parent dir with new file's info
	return 0;
}

int
File_Open(char *file)
{
	printf("FS_Open\n");
	if (filesOpen >= 256)
	{
		printf("Error! The max number of open files has been reached\n");
		osErrno = E_TOO_MANY_OPEN_FILES;
		return -1;
	}
	
	//Lookup file 
	//Give file an open entry in the file table
	//Set fp to 0
	// return the index in the ft

	++filesOpen;
	return 0;
}

int
File_Read(int fd, void *buffer, int size)
{
	printf("FS_Read\n");
	int offset;
	
	if (ofTable[fd].fp == -1)
	{
		printf("Error! File not open\n");
		osErrno = E_BAD_FD;
		return -1;
	}
	
	int fileP = ofTable[fd].fp;
	offset = fileP;
	int frame = 0;
	
	while ((offset - SECTOR_SIZE) >= 0)
	{
		++frame;
		offset = offset - SECTOR_SIZE;
	}
	//get the address of sector #frame from table
	return 0;
}

int
File_Write(int fd, void *buffer, int size)
{
	printf("FS_Write\n");
	int offset;

	if (ofTable[fd].fp == -1)
	{
		printf("Error! File not open\n");
		osErrno = E_BAD_FD;
		return -1;
	}

	int fileP = ofTable[fd].fp;
	offset = fileP;
	int frame = 0;

	while ((offset - SECTOR_SIZE) >= 0)
	{
		++frame;
		offset = offset - SECTOR_SIZE;
	}
	//get the address of sector #frame from table

	//if write reached end of file, update size
	return 0;
}

int
File_Seek(int fd, int offset)
{
	printf("FS_Seek\n");
	
	if (ofTable[fd].fp == -1)
	{
		printf("Error! File not open\n");
		osErrno = E_BAD_FD;
		return -1;
	}
	if (offset < 0 || offset >= ofTable[fd].size)
	{
		printf("Error! Attempt to seek out of bounds\n");
		osErrno = E_SEEK_OUT_OF_BOUNDS;
		return -1;
	}
	
	ofTable[fd].fp = offset;
	return 0;
}

int
File_Close(int fd)
{
	printf("FS_Close\n");
	
	if (ofTable[fd].fp == -1)
	{
		printf("Error! File not open\n");
		osErrno = E_BAD_FD;
		return -1;
	}
	
	ofTable[fd].fp = -1;

	--filesOpen;
	return 0;
}

int
File_Unlink(char *file)
{
	printf("FS_Unlink\n");
	//remove file's entry in parent dir
	//free its inodes and data blocks
	return 0;
}


// directory ops
int
Dir_Create(char *path)
{
	printf("Dir_Create %s\n", path);
	//Set a free inode to hold the directory
	//
	//Update parent dir
	return 0;
}

int
Dir_Size(char *path)
{
	printf("Dir_Size\n");
	//Lookup the inode of dir
	//inode contains an entry for size
	return 0;
}

int
Dir_Read(char *path, void *buffer, int size)
{
	printf("Dir_Read\n");
	//??? about this one
	return 0;
}

int
Dir_Unlink(char *path)
{
	printf("Dir_Unlink\n");

	if (path == "/")
	{
		osErrno = E_ROOT_DIR;
		return -1;
	}
	if (Dir_Size(path) != 0)
	{
		osErrno = E_DIR_NOT_EMPTY;
		return -1;
	}

	//Since we can only delete an empty folder,
	//we can simply free up the inode used

	//Remove folder's entry in parent dir

	return 0;
}