#include "alloc.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ALIGNMENT 16 /**< The alignment of the memory blocks */

static free_block *HEAD = NULL; /**< Pointer to the first element of the free list */

/**
 * Split a free block into two blocks
 *
 * @param block The block to split
 * @param size The size of the first new split block
 * @return A pointer to the first block or NULL if the block cannot be split
 */
void *split(free_block *block, int size) {
    if((block->size < size + sizeof(free_block))) {
        return NULL;
    }

    void *split_pnt = (char *)block + size + sizeof(free_block);
    free_block *new_block = (free_block *) split_pnt;

    new_block->size = block->size - size - sizeof(free_block);
    new_block->next = block->next;

    block->size = size;

    return block;
}

/**
 * Find the previous neighbor of a block
 *
 * @param block The block to find the previous neighbor of
 * @return A pointer to the previous neighbor or NULL if there is none
 */
free_block *find_prev(free_block *block) {
    free_block *curr = HEAD;
    while(curr != NULL) {
        char *next = (char *)curr + curr->size + sizeof(free_block);
        if(next == (char *)block)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

/**
 * Find the next neighbor of a block
 *
 * @param block The block to find the next neighbor of
 * @return A pointer to the next neighbor or NULL if there is none
 */
free_block *find_next(free_block *block) {
    char *block_end = (char*)block + block->size + sizeof(free_block);
    free_block *curr = HEAD;

    while(curr != NULL) {
        if((char *)curr == block_end)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

/**
 * Remove a block from the free list
 *
 * @param block The block to remove
 */
void remove_free_block(free_block *block) {
    free_block *curr = HEAD;
    if(curr == block) {
        HEAD = block->next;
        return;
    }
    while(curr != NULL) {
        if(curr->next == block) {
            curr->next = block->next;
            return;
        }
        curr = curr->next;
    }
}

/**
 * Coalesce neighboring free blocks
 *
 * @param block The block to coalesce
 * @return A pointer to the first block of the coalesced blocks
 */
void *coalesce(free_block *block) {
    if (block == NULL) {
        return NULL;
    }

    free_block *prev = find_prev(block);
    free_block *next = find_next(block);

    // Coalesce with previous block if it is contiguous.
    if (prev != NULL) {
        char *end_of_prev = (char *)prev + prev->size + sizeof(free_block);
        if (end_of_prev == (char *)block) {
            prev->size += block->size + sizeof(free_block);

            // Ensure prev->next is updated to skip over 'block', only if 'block' is directly next to 'prev'.
            if (prev->next == block) {
                prev->next = block->next;
            }
            block = prev; // Update block to point to the new coalesced block.
        }
    }

    // Coalesce with next block if it is contiguous.
    if (next != NULL) {
        char *end_of_block = (char *)block + block->size + sizeof(free_block);
        if (end_of_block == (char *)next) {
            block->size += next->size + sizeof(free_block);

            // Ensure block->next is updated to skip over 'next'.
            block->next = next->next;
        }
    }

    return block;
}

/**
 * Call sbrk to get memory from the OS
 *
 * @param size The amount of memory to allocate
 * @return A pointer to the allocated memory
 * increments and returns a pointer to the top of the heap - memory from the OS
 * TO-Do: STEP 1!!!
 */
void *do_alloc(size_t size) {
  
    return NULL;

}

/**
 * Allocates memory for the end user
 *
 * @param size The amount of memory to allocate
 * @return A pointer to the requested block of memory
 */

 typedef struct header {
    size_t size; /**< Size of the block */
    int magic; /**< Magic number for error checking */
} header;

void *tumalloc(size_t size) {
    free_block *curr = HEAD;
    void *ptr = NULL;
    free_block *prev = NULL;

    if (HEAD == NULL) {
        ptr = do_alloc(size);
        return ptr; 
    }
    else {
        for (block in free_list){
            if (size <= block.size){
                ptr = split(block.size | sizeof(header));
                remove_free_block(header);
                header(size) = size;
                void header_magic = 0x01234567;
                return ptr;
            }
        else {
            ptr = do_alloc(size); 
            return ptr; 
        }
    }
        }
    }
    
    
    //return NULL;


/**
 * Allocates and initializes a list of elements for the end user
 *
 * @param num How many elements to allocate
 * @param size The size of each element
 * @return A pointer to the requested block of initialized memory
 */

//looking at implimenting "memset"? Sets nonheaders to 0
//used https://stackoverflow.com/questions/57130051/how-to-use-malloc-and-memset for ideas
void *tucalloc(size_t num, size_t size) {
    for (block in free_list){
        block *new_block = (block *)tumalloc(sizeof(block));
        memset(new_block, 0, sizeof(block));
    }
}

/**
 * Reallocates a chunk of memory with a bigger size
 *
 * @param ptr A pointer to an already allocated piece of memory
 * @param new_size The new requested size to allocate
 * @return A new pointer containing the contents of ptr, but with the new_size
 */

 //researched and found that memcpy could be used to copy data over 
 // Disclaimer: Asked ChatGPT How memcpy is used in c for syntax
void *turealloc(void *ptr, size_t new_size) {
    void old_size = curr *;
    if (new_size == 0){
        free(ptr);
        return NULL;
    }
    void *memcpy(void *ptr, const void *src, size_t new_size)
    tufree(ptr *);
    return ptr;
    }

/**
 * Removes used chunk of memory and returns it to the free list
 *
 * @param ptr Pointer to the allocated piece of memory
 */
void tufree(void *ptr) {
    free_block *curr = HEAD;
    //header_magic = NULL:
    header *hdr = (header *)((char *)ptr - sizeof(header));
    if (header -> magic == 0x01234567){
        free_block *free_block = (free_block *)hdr;
        free_block -> size = hdr ->size;
        free_block -> next = HEAD;
        HEAD = free_block;
        // code vocab: (coalesce) This is a placeholder for the logic that would attempt to 
        // merge adjacent free blocks into one larger block.
        coalesce(free_block);
    }
    else {
        print("Memory corruption detected");
        abort();
    }
    
}
