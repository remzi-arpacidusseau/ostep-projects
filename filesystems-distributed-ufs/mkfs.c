#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ufs.h"

void usage() {
    fprintf(stderr, "usage: mkfs -f <image_file> [-d <num_data_blocks] [-i <num_inodes>]\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    int ch;
    char *image_file = NULL;
    int num_inodes = 32;
    int num_data = 32;
    int visual = 0;

    while ((ch = getopt(argc, argv, "i:d:f:v")) != -1) {
	switch (ch) {
	case 'i':
	    num_inodes = atoi(optarg);
	    break;
	case 'd':
	    num_data = atoi(optarg);
	    break;
	case 'f':
	    image_file = optarg;
	    break;
	case 'v':
	    visual = 1;
	    break;
	default:
	    usage();
	}
    }
    argc -= optind;
    argv += optind;

    if (image_file == NULL)
	usage();

    unsigned char *empty_buffer;
    empty_buffer = calloc(UFS_BLOCK_SIZE, 1);
    if (empty_buffer == NULL) {
	perror("calloc");
	exit(1);
    }

    int fd = open(image_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
	perror("open");
	exit(1);
    }

    assert(num_inodes >= 32);
    assert(num_data >= 32);

    // presumed: block 0 is the super block
    super_t s;

    // totals
    s.num_inodes = num_inodes;
    s.num_data = num_data;

    // inode bitmap
    int bits_per_block = (8 * UFS_BLOCK_SIZE); // remember, there are 8 bits per byte

    s.inode_bitmap_addr = 1;
    s.inode_bitmap_len = num_inodes / bits_per_block;
    if (num_inodes % bits_per_block != 0)
	s.inode_bitmap_len++;

    // data bitmap
    s.data_bitmap_addr = s.inode_bitmap_addr + s.inode_bitmap_len;
    s.data_bitmap_len = num_data / bits_per_block;
    if (num_data % bits_per_block != 0)
	s.data_bitmap_len++;

    // inode table
    s.inode_region_addr = s.data_bitmap_addr + s.data_bitmap_len;
    int total_inode_bytes = num_inodes * sizeof(inode_t);
    s.inode_region_len = total_inode_bytes / UFS_BLOCK_SIZE;
    if (total_inode_bytes % UFS_BLOCK_SIZE != 0)
	s.inode_region_len++;

    // data blocks
    s.data_region_addr = s.inode_region_addr + s.inode_region_len;
    s.data_region_len = num_data;

    int total_blocks = 1 + s.inode_bitmap_len + s.data_bitmap_len + s.inode_region_len + s.data_region_len;

    // super block is the first block
    int rc = pwrite(fd, &s, sizeof(super_t), 0);
    if (rc != sizeof(super_t)) {
	perror("write");
	exit(1);
    }

    printf("total blocks        %d\n", total_blocks);
    printf("  inodes            %d [size of each: %lu]\n", num_inodes, sizeof(inode_t));
    printf("  data blocks       %d\n", num_data);
    printf("layout details\n");
    printf("  inode bitmap address/len %d [%d]\n", s.inode_bitmap_addr, s.inode_bitmap_len);
    printf("  data bitmap address/len  %d [%d]\n", s.data_bitmap_addr, s.data_bitmap_len);

    // first, zero out all the blocks
    int i;
    for (i = 1; i < total_blocks; i++) {
	rc = pwrite(fd, empty_buffer, UFS_BLOCK_SIZE, i * UFS_BLOCK_SIZE);
	if (rc != UFS_BLOCK_SIZE) {
	    perror("write");
	    exit(1);
	}
    }

    //
    // need to allocate first inode in inode bitmap
    //
    typedef struct {
	unsigned int bits[UFS_BLOCK_SIZE / sizeof(unsigned int)];
    } bitmap_t;
    assert(sizeof(bitmap_t) == UFS_BLOCK_SIZE);

    bitmap_t b;
    for (i = 0; i < 1024; i++)
	b.bits[i] = 0;
    b.bits[0] = 0x1 << 31; // first entry is allocated
    
    rc = pwrite(fd, &b, UFS_BLOCK_SIZE, s.inode_bitmap_addr * UFS_BLOCK_SIZE);
    assert(rc == UFS_BLOCK_SIZE);

    //
    // need to allocate first data block in data bitmap
    // (can just reuse this to write out data bitmap too)
    //
    rc = pwrite(fd, &b, UFS_BLOCK_SIZE, s.data_bitmap_addr * UFS_BLOCK_SIZE);
    assert(rc == UFS_BLOCK_SIZE);

    //
    // need to write out inode
    //
    typedef struct {
	inode_t inodes[UFS_BLOCK_SIZE / sizeof(inode_t)];
    } inode_block;

    inode_block itable;
    itable.inodes[0].type = UFS_DIRECTORY;
    itable.inodes[0].size = 2 * sizeof(dir_ent_t); // in bytes
    itable.inodes[0].direct[0] = s.data_region_addr;
    for (i = 1; i < DIRECT_PTRS; i++)
	itable.inodes[0].direct[i] = -1;

    rc = pwrite(fd, &itable, UFS_BLOCK_SIZE, s.inode_region_addr * UFS_BLOCK_SIZE);
    assert(rc == UFS_BLOCK_SIZE);

    // 
    // need to write out root directory contents to first data block
    // create a root directory, with nothing in it
    // 
    typedef struct {
	dir_ent_t entries[128];
    } dir_block_t;
    // xxx assumes 4096 block, 32 byte entries
    assert(sizeof(dir_ent_t) * 128 == UFS_BLOCK_SIZE);

    dir_block_t parent;
    strcpy(parent.entries[0].name, ".");
    parent.entries[0].inum = 0;

    strcpy(parent.entries[1].name, "..");
    parent.entries[1].inum = 0;

    for (i = 2; i < 128; i++)
	parent.entries[i].inum = -1;

    rc = pwrite(fd, &parent, UFS_BLOCK_SIZE, s.data_region_addr * UFS_BLOCK_SIZE);
    assert(rc == UFS_BLOCK_SIZE);

    if (visual) {
	int i;
	printf("\nVisualization of layout\n\n");
	printf("S");
	for (i = 0; i < s.inode_bitmap_len; i++)
	    printf("i");
	for (i = 0; i < s.data_bitmap_len; i++)
	    printf("d");
	for (i = 0; i < s.inode_region_len; i++)
	    printf("I");
	for (i = 0; i < s.data_region_len; i++)
	    printf("D");
	printf("\n\n");
    }

    (void) fsync(fd);
    (void) close(fd);
    
    return 0;
}

