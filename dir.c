#include "dir.h"
#include "block.h"
#include "inode.h"
#include <stdlib.h>

struct directory {
    struct inode *in;
    unsigned int offset;
};

struct directory *directory_open(int inode_num){
    // Use iget() to get the inode for this file
    struct inode *in = iget(inode_num);

    // If it fails, directory_open() should return NULL
    if (in == NULL) {
        return NULL;
    }

    // malloc() space for a new struct directory
    struct directory *dir = malloc(sizeof(struct directory));

    // Set the inode and offset fields of the struct directory
    dir->in = in;
    dir->offset = 0;

    // Return the pointer to the struct directory
    return dir;
}