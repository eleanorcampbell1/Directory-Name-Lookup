#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "dirLookup.h"
/*
Search given directory for a particular name and return inode 
that corresponds to that component name in the directory.
*/
CS313ino_t dirLookup(const char *filename, const char *component) {

	int fd = open(filename, O_RDONLY);

	if(fd == -1){                       //check for valid filename
        printf("Invalid file\n");
        perror("Failed: ");
        return BAD_DIR_ENTRY_INODE_NUMBER;
    }
	//get file size
	struct stat st;
    stat(filename, &st);
    int file_size = st.st_size;

	int offset = 0;
	int inode_offset = (sizeof(uint8_t) * 2) + sizeof(uint16_t);
	int name_offset = inode_offset + sizeof(CS313ino_t);
	int name_len_offset = sizeof(uint8_t);
	uint8_t name_len;
	CS313ino_t inode;
	uint8_t buffer[file_size];
	char namebuf[MAX_NAME_LEN + 1];

	int readval = read(fd, buffer, file_size);					//read block into buffer
	if(readval != file_size){
		return BAD_DIR_ENTRY_INODE_NUMBER;
	}

	while(offset < file_size){

		name_len = (uint8_t) buffer[offset + name_len_offset];	//get name length

		inode = (CS313ino_t) (buffer[offset + inode_offset] | buffer[offset + inode_offset + 1] << 8);
		if(inode != 0){
			//read name into namebuf
			for(int j = 0; j < name_len; j++){
				namebuf[j] = (char) buffer[offset + name_offset + j];
			}

			//terminate name
			namebuf[name_len] = '\0';

			//check if component matches
			if(!strcmp((char *) namebuf, component)){
				return inode;
			}
		}
		offset = offset + name_offset + name_len;

		// align offset
		if((offset % 4) != 0){
			offset = offset + (4 - (offset % 4));
		}
	}
	
	return BAD_DIR_ENTRY_INODE_NUMBER;
}
