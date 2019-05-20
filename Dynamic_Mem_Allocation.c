////////////////////////////////////////////////////////////////////////////////
// Main File:         mem.c
// This File:         mem.c
// Other Files:       None
// Semester:          CS 354 Spring 2019
//
// Author:           Mudit Joshi
// Email:            mjoshi6@wisc.edu
// CS Login:         mudit
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          NONE
//                   Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   NONE
//                   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "mem.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct block_header {
    int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit 
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    * 
    * End Mark: 
    *  The end of the available memory is indicated using a size_status of 1.
    * 
    * Examples:
    * 
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27
    *      If the previous block is free, size_status should be 25
    * 
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26
    *      If the previous block is free, size_status should be 24
    *    Footer:
    *      size_status should be 24
    */
} block_header;

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */

block_header *start_block = NULL;

/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block on success.
 * Returns NULL on failure.
 * This function should:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
 * - Use BEST-FIT PLACEMENT POLICY to find the block closest to the required block size
 * - Use SPLITTING to divide the chosen free block into two if it is too large.
 * - Update header(s) and footer as needed.
 * Tips: Be careful with pointer arithmetic.
 */
void* Alloc_Mem(int size) {

    int Head_Size = 4;
    if(size <= 0 ){
        return NULL;
    }
    int Padd;
    if((size + Head_Size) > 8){
        Padd = (8-((size + Head_Size)%8));
    }else if ((  (size+Head_Size)%8) == 0 ) {
        Padd = 0;
    }else{
        Padd = (8 - (size + Head_Size)%8);
    }
    int Blk_Size = (size + Head_Size + Padd);
    block_header *traver = start_block;
    block_header *Block_Best = start_block;
    while( (Block_Best->size_status%8 == 1 || Block_Best->size_status%8 == 3) ||  (Block_Best->size_status - (Block_Best->size_status % 8)) <= Blk_Size ){
        if(Block_Best->size_status == 1){
            return NULL;
        }
        Block_Best = (block_header*)( (void*)Block_Best + Block_Best->size_status - Block_Best->size_status%8 );
    }

    int Newblock_Best = 0;
    while( (traver->size_status) != 1){
        if( (traver->size_status - (traver->size_status % 8)) >= Blk_Size ){
            if (( ((traver)->size_status % 8)%2)==0 ){
                if( (Block_Best->size_status - ( Block_Best->size_status%8)) >= (traver->size_status - (traver->size_status % 8)) ){
                    Newblock_Best++;

                    Block_Best = traver;
                }
            }
        }
        traver = (block_header*)((void*)traver + (traver->size_status - (traver->size_status % 8 )) );

    }


    if(Newblock_Best == 0){
        return NULL;
    }

    int Rem_Size = ( (Block_Best->size_status - (Block_Best->size_status % 8 )) - Blk_Size   ) ;
    Block_Best->size_status = Blk_Size + 3;


    if(Rem_Size>0){
        block_header *footer  = (block_header*)( (void*)Block_Best + Blk_Size );
        footer->size_status = Rem_Size + 2;
        footer = (block_header*)((void*)footer + footer->size_status - 4 - 2);
        footer->size_status = Rem_Size;

    }
    return (block_header*)((void*)Block_Best+ sizeof(block_header));
}

/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
 * - Update header(s) and footer as needed.
 */
int Free_Mem(void *ptr) {
    if(ptr==NULL){
        return -1;

    }if(((int)ptr % 8)!=0) {
        return -1;
    }

    block_header *Free_Block = (block_header*)((void*)ptr - 4);
    if(Free_Block->size_status%8 == 0 || Free_Block->size_status%8 == 2 ){
        return -1;
    }


    int Size_Old = Free_Block->size_status - Free_Block->size_status%8;
    Free_Block->size_status = Free_Block->size_status - 1 ;
    block_header *footer = (block_header*)( (void*)Free_Block + Size_Old - 4);
    footer->size_status = Size_Old;
    block_header *next = (block_header*)(( void*)Free_Block + Free_Block->size_status - Free_Block->size_status%8);
    next->size_status = next->size_status - 2;
    block_header *nextFooter;
    int Counter = 0;

    if( next->size_status%8 == 0 || next->size_status%8 == 2 ){

        Counter++; 

        int next_newSize = next->size_status - next->size_status%8;
        Free_Block->size_status = Free_Block->size_status + next_newSize ;
        nextFooter = (block_header*) ( (void*)next + next_newSize - 4);

        nextFooter->size_status =  Free_Block->size_status - (Free_Block->size_status % 8);
    }

    if(Free_Block->size_status % 8 == 0){

        block_header *prev_footer = (block_header*)((void*)Free_Block - 4);
        block_header *prev =  (block_header*)((void*)Free_Block - prev_footer->size_status );

        prev->size_status = prev->size_status + Free_Block->size_status - Free_Block->size_status%8;


        if(Counter==0){

            footer->size_status = prev->size_status - (prev->size_status%8);
        }else{
            nextFooter->size_status = prev->size_status - (prev->size_status%8);

        }

    }
    return 0;
}

/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
int Init_Mem(int sizeOfRegion) {
    int pagesize;
    int padsize;
    int fd;
    int alloc_size;
    void* space_ptr;
    block_header* end_mark;
    static int allocated_once = 0;

    if (0 != allocated_once) {
        fprintf(stderr,
                "Error:mem.c: Init_Mem has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }


    pagesize = getpagesize();


    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;


    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE,
                     fd, 0);
    if (MAP_FAILED == space_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }

    allocated_once = 1;


    alloc_size -= 8;


    start_block = (block_header*) space_ptr + 1;
    end_mark = (block_header*)((void*)start_block + alloc_size);


    start_block->size_status = alloc_size;


    start_block->size_status += 2;


    end_mark->size_status = 1;


    block_header *footer = (block_header*) ((char*)start_block + alloc_size - 4);
    footer->size_status = alloc_size;

    return 0;
}

/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */
void Dump_Mem() {
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end = NULL;
    int t_size;

    block_header *current = start_block;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");

    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;

        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "used");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "used");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_used)
            used_size += t_size;
        else
            free_size += t_size;

        t_end = t_begin + t_size - 1;

        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status,
                p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

        current = (block_header*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total used size = %d\n", used_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", used_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;
}         
