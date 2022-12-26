#ifndef DIR_LOOKUP_H
#define DIR_LOOKUP_H

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>


#define BLOCK_SIZE 1024
#define MAX_NAME_LEN 255
#define BAD_DIR_ENTRY_INODE_NUMBER 0

typedef uint16_t CS313ino_t;
typedef struct dir_entry
{
    uint8_t  de_type;		  // file type of file (not used in rev #0)
    uint8_t  de_namelen;	  // string length of the file name field
    uint16_t de_reserved;         // reserved for future use
    CS313ino_t de_ino;		        // inode number
    char de_name[MAX_NAME_LEN+1]; // name string
} dir_entry_t;

#define	DT_DIR	5
#define	DT_REG	8
#define	DT_LNK	13

CS313ino_t dirLookup(const char *filename, const char *component);

#endif
