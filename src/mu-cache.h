/******************************************************************************/
/* CACHE STRUCTURE                                                            */
/******************************************************************************/
#define NUM_CACHE_BLOCKS 16
#define WORD_PER_BLOCK 4


typedef struct CacheBlock_Struct {

  int valid; //indicates if the given block contains a valid data. Initially, this is 0
  uint32_t tag; //this field should contain the tag, i.e. the high-order 32 - (2+2+4)  = 24 bits
  uint32_t words[WORD_PER_BLOCK]; //this is where actual data is stored. Each word is 4-byte long, and each cache block contains 4 blocks.
  
  
} CacheBlock;

typedef struct Cache_Struct {

  CacheBlock blocks[NUM_CACHE_BLOCKS]; // there are 16 blocks in the cache
  
} Cache;



/***************************************************************/
/* CACHE STATS                                                 */
/***************************************************************/
uint32_t cache_misses; //need to initialize to 0 at the beginning of simulation start
uint32_t cache_hits;   //need to initialize to 0 at the beginning of simulation start


/***************************************************************/
/* CACHE OBJECT                                                */
/***************************************************************/
Cache L1Cache; //need to use this in the simulator

