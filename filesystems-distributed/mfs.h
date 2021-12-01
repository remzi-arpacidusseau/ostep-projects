#ifndef __MFS_h__
#define __MFS_h__

#define MFS_DIRECTORY    (0)
#define MFS_REGULAR_FILE (1)
#define BUFFER_SIZE (4096)
#define MFS_BLOCK_SIZE   (4096)
#define DIR_NAME_LENGTH (28)
#define NUM_INODES (4096)

#define INIT (0)
#define LOOKUP (1)
#define STAT (2)
#define WRITE (3)
#define READ (4)
#define CREATEF (5)
#define CREATED (6)
#define UNLINK (7)
#define SHUTDOWN (8) 


typedef struct __MFS_Stat_t {
    int type;   // MFS_DIRECTORY or MFS_REGULAR
    int size;   // bytes
    // note: no permissions, access times, etc.
} MFS_Stat_t;

typedef struct __MFS_DirEnt_t {
    char name[28];  // up to 28 bytes of name in directory (including \0)
    int  inum;      // inode number of entry (-1 means entry not used)
} MFS_DirEnt_t;

typedef struct __request{
  int action;                              //The action requested from the server
  int inum;                                //inode number of file to be acted on
  int pinum;                               //inode number of parent directory--used when creating files
  int block;                               //Block to be read
  int port;                                //portname to connect to server on
  char buf[MFS_BLOCK_SIZE];                //Multiple meanings, depending on the action requested
}request;

typedef struct __reply{
  int inum;
  MFS_Stat_t m;
  int rc;
  char read_data[MFS_BLOCK_SIZE];
}reply;
typedef struct checkpoint{
  int endptr;
  int mapptrs[256];
}checkpoint;


typedef struct inode{
  int size;
  int type;
  int ptrs[14];

}inode;



int MFS_Init(char *hostname, int port);
int MFS_Lookup(int pinum, char *name);
int MFS_Stat(int inum, MFS_Stat_t *m);
int MFS_Write(int inum, char *buffer, int block);
int MFS_Read(int inum, char *buffer, int block);
int MFS_Creat(int pinum, int type, char *name);
int MFS_Unlink(int pinum, char *name);
int MFS_Shutdown(void);
#endif // __MFS_h__
