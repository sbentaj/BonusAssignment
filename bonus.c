#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CACHE_SIZE 32
#define BLOCK_SIZE 4

typedef struct cache {
    int size;   // Size of the cache
    int assoc;  // Associativity of the cache
    int num_sets;   // Number of sets in the cache
    int block_size; // Size of a block in the cache
    int rp;     // Replacement policy (0 for LRU, 1 for Random)
    int hit;    // Number of cache hits
    int miss;   // Number of cache misses
    int **tag;  // Tag array
    int **valid;    // Valid array
    char **data;    // Data array
    int *lru;   // LRU array (only used for LRU replacement policy)
} Cache;

// Helper function to convert hexadecimal strings to integers
int hex_to_int(char *hex) {
    return (int) strtol(hex, NULL, 16);
}

// Helper function to initialize the cache
void init_cache(Cache *cache, int assoc, int rp) {
    int i, j;
    cache->assoc = assoc;
    cache->num_sets = cache->size / (assoc * cache->block_size);
    cache->rp = rp;
    cache->hit = 0;
    cache->miss = 0;
    cache->tag = (int **) malloc(cache->num_sets * sizeof(int *));
    cache->valid = (int **) malloc(cache->num_sets * sizeof(int *));
    cache->data = (char **) malloc(cache->num_sets * sizeof(char *));
    if (rp == 0) {
        cache->lru = (int *) malloc(cache->num_sets * sizeof(int));
        memset(cache->lru, 0, cache->num_sets * sizeof(int));
    }
    for (i = 0; i < cache->num_sets; i++) {
        cache->tag[i] = (int *) malloc(assoc * sizeof(int));
        cache->valid[i] = (int *) malloc(assoc * sizeof(int));
        cache->data[i] = (char *) malloc(cache->block_size * assoc * sizeof(char));
        for (j = 0; j < assoc; j++) {
            cache->tag[i][j] = -1;
            cache->valid[i][j] = 0;
            memset(&cache->data[i][j * cache->block_size], 0, cache->block_size * sizeof(char));
        }
    }
}

// Helper function to access the cache
void access_cache(Cache *cache, int addr) {
    int set_index = (addr / cache->block_size) % cache->num_sets;
    int tag = (addr / cache->block_size) / cache->num_sets;
    int i, j, lru_index, random_index;
    for (i = 0; i < cache->assoc; i++) {
        if (cache->valid[set_index][i] && cache->tag[set_index][i] == tag) {
            cache->hit++;
            if (cache->rp == 0) {
                cache->lru[set_index] = i;
}
return;
}
}
cache->miss++;
if (cache->rp == 0) {
lru_index = cache->lru[set_index];
for (i = 0; i < cache->assoc; i++) {
if (!cache->valid[set_index][i]) {
lru_index = i;
break;
}
if (cache->lru[set_index] > cache->lru[set_index + i]) {
lru_index = i;
}
}
cache->lru[set_index] = cache->lru[set_index] + 1;
if (lru_index == cache->assoc) {
lru_index = 0;
cache->lru[set_index] = 0;
}
cache->tag[set_index][lru_index] = tag;
cache->valid[set_index][lru_index] = 1;
memcpy(&cache->data[set_index][lru_index * cache->block_size], &addr, cache->block_size * sizeof(char));
} else {
random_index = rand() % cache->assoc;
cache->tag[set_index][random_index] = tag;
cache->valid[set_index][random_index] = 1;
memcpy(&cache->data[set_index][random_index * cache->block_size], &addr, cache->block_size * sizeof(char));
}
}

int main() {
Cache cache;
char hex[9];
int addr;
int i;
// Initialize cache with size 32, associativity 1, block size 4, and replacement policy LRU
cache.size = CACHE_SIZE;
cache.block_size = BLOCK_SIZE;
init_cache(&cache, 1, 0);
// Access addresses in a loop
for (i = 0; i < 16; i++) {
    // Read hexadecimal address from input
    scanf("%s", hex);
    // Convert hexadecimal address to integer
    addr = hex_to_int(hex);
    // Access the cache using the address
    access_cache(&cache, addr);
}

// Print cache statistics
printf("Hits: %d\n", cache.hit);
printf("Misses: %d\n", cache.miss);

return 0;
} 