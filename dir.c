#include "dir.h"
#include "block.h"
#include "inode.h"
#include <stdlib.h>

struct directory {
    struct inode *in;
    unsigned int offset;
};

struct direntry {
    unsigned int inode_num; // inode number of the file 2 bytes
    char name[16];          // name of the file 16 bytes
    char reserved[14];      // 14 reserved bytes
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

void mkfs(void){
    // Get new inode with ialloc
    struct inode *in = ialloc();

    // Call alloc to get a new data block
    int block_num = alloc();

    // Initialize the inode returned from ialloc
    // flags = 2, size = byte size of directory (64 bytes, 2 entries), block_ptr[0] needs to point to the block we got from alloc
    in->flags = 2;
    in->size = 2 * sizeof(struct directory);
    in->block_ptr[0] = block_num;

    // Make unsigned char block[BLOCK_SIZE] that you can populate with new directory data
    unsigned char block[BLOCK_SIZE];
    memset(block, 0, BLOCK_SIZE);

    // Add the directory entries
    // First entry: inode number of the directory itself, name is "."
    // Second entry: inode number of the parent directory, name is ".."
    struct direntry *entry1 = (struct directory *)block;
    entry1->inode_num= in->inode_num;
    entry1->name[0] = '.';

    struct direntry *entry2 = (struct directory *)(block + sizeof(struct directory));
    entry2->inode_num = in->inode_num;
    entry2->name[0] = '..';

    // Write the block to the disk with bwrite
    bwrite(block_num, block);

    // call iput() to write the new direcory inode out to disk and free up the in-core inode
    iput(in);
}