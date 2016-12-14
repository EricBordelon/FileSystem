#include <stdio.h>

#include "LibFS.h"

int
main(int argc, char *argv[])
{
	char* path;
	if (argc == 2) {
		path = argv[1];
	}
	else{
		printf("This program needs the name of a file to save/load data from:\n>>");
		scanf("%s",path);
	}
	
	int choice  = 0;
	int exit = 0;

	FS_Boot(path);
	FS_Sync();

	printf("\nThis program implements a small file-system.\n");

	char* nFile = "";
	int fileD = 0;
	int sz = 0;
	do{
		printf("Enter 1 to create a directory,\n");
		printf("\t2 to output the files in a directory,\n");
		printf("\t3 to delete a directory,\n");
		printf("\t4 to create a file,\n");
		printf("\t5 to open a file,\n");
		printf("\t6 to read from a file,\n");
		printf("\t7 to write to a file,\n");
		printf("\t8 to seek in a file,\n");
		printf("\t9 to close a file,\n");
		printf("\t10 to delete a file,\n");
		printf("\t11 to save the file system, or\n");
		printf("\t12 to exit...\n");

		scanf("%i", choice);

		switch (choice){
		case 1:
			printf("Please enter the full pathname of the new directory:\n");
			scanf("%s",nFile);

			Dir_Create(nFile);
			break;
		case 2:
			printf("Enter the full pathname of the diredtory:\n");
			scanf("%s", nFile);

			//Dir_Read();
			break;
		case 3:
			printf("Please enter the full pathname of the directory to delete:\n");
			scanf("%s", nFile);

			Dir_Unlink(nFile);
			break;
		case 4:
			printf("Please enter the full pathname of the new file:\n");
			scanf("%s", nFile);

			File_Create(nFile);
			break;
		case 5:
			printf("Enter the full pathname of the file to open:\n");
			scanf("%s", nFile);

			File_Open(nFile);
			break;
		case 6:
			printf("Enter the file descriptor of the target file:\n");
			scanf("%i", fileD);


			break;
		case 7:
			printf("Enter the file descriptor of the target file:\n");
			scanf("%i", fileD);

			break;
		case 8:
			printf("Enter the file descriptor of the target file and the offset to move to:\n");
			scanf("%i %i", fileD, sz);

			File_Seek(fileD, sz);
			break;
		case 9:
			printf("Enter the file descriptor of the target file:\n");
			scanf("%i", fileD);

			File_Close(fileD);
			break;
		case 10:
			printf("Please enter the full pathname of the file to dilete:\n");
			scanf("%s", nFile);

			File_Unlink(nFile);
			break;
		case 11:
			printf("Saving...\n");

			FS_Sync();
			break;
		case 12:
			printf("Now saving and exiting...");
			
			FS_Sync();
			exit = 1;
			break;
		default:
			printf("Error, invalid response\n ");
		}
	} while (exit == 0);

	return 0;
}