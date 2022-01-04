/**
 * Procedure:
 * 1. Partition cli input into n number of threads within MR_Run (It is assumed
 *    that there is a 1-1 b/w files being consumed and threads created).
 * 2. Each thread (Map fn) emits a number of key-value pairs that must be added
 *    to n number of in-memory hashmaps (called partitions).
 * 3. Each partition is sorted in ascending order using its key.
 * 3. After all threads have finished doing work, close them all and create x 
 *    number of reducer threads. These will randomly process a partition
 *    process a h of the hashmap, reducing  
 *    that performs the reduce operation (based on  number of
 *    reducers specifieed by args)
 * 4. Sort the result
 */
#include "mapreduce.h"

void MR_Emit(char *key, char *value) {

}

void MR_Run(int argc, char *argv[],
            Mapper map, int num_mappers,
            Reducer reduce, int num_reducers,
            Partitioner partition)
{
  
}
