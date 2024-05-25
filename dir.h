#ifndef DIR
#define DIR

#define ENTRY_SIZE 32   // 2 bytes for inode number, 16 bytes for name, 14 bytes reserved

struct directory *directory_open(int inode_num);
void mkfs(void);

#endif