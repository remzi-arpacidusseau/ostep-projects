#ifndef __mapreduce_h__
#define __mapreduce_h__

typedef char *(*Getter)();

typedef void (*Mapper)(char *file_name);
typedef void (*Reducer)(char *key, Getter get_func, int partition_number);
typedef unsigned long (*Partitioner)(char *key, int num_buckets);

void MR_Emit(char *key, char *value);

unsigned long MR_DefaultHashPartition(char *key, int num_buckets);

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition);

#endif // __mapreduce_h__
