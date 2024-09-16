#pragma once

#include "r-memory-internal.hpp"

r_external const RHNDMemoryBlockAllocator 
r_mem::block_allocator_create(
    const RHNDMemoryReservation reservation_handle,
    const r_cstr                block_allocator_tag,
    const r_size                block_size,
    const r_size                block_alignment,
    const r_size                block_count,
    const r_size                minimum_arena_size) {

    //get the reservation
    RMemoryReservation* reservation_ptr = r_mem_internal::reservation_from_handle(reservation_handle);
    if (!reservation_ptr) {
        return(NULL);
    }

    //create a struct region for the allocator
    RMemoryRegion* region_ptr = 
        (RMemoryRegion*)r_mem::region_create_struct_pool(
            reservation_handle,
            block_allocator_tag,
            minimum_arena_size,
            block_size,
            block_alignment,
            block_count);

    if (!region_ptr) {
        return(NULL);
    }

    //now, we need to adjust our block count
    const r_size region_size        = r_mem::region_size_total();
    const r_size block_size_aligned = r_align_a_to_b(block_size,block_alignment);
    const r_size block_count_actual = region_size / block_size_aligned;  

    //push the blocks onto the stack
    RMemoryBlockAllocator* block_allocator_ptr = r_mem_internal::stack_push_block_allocator();
    RMemoryBlock*          block_ptr           = r_mem_internal::stack_push_blocks(block_count_actual);

    //sanity check
    if (!block_allocator_ptr || block_ptr) {
        return(NULL);
    }

    //calculate the blocks per arena
    const r_size arena_size = region_ptr->arena_size; 
    const r_size arena_count = region_ptr->arena_count;
    const r_size block_count_per_arena = arena_size / block_size_aligned;

    //for each arena, we are going to add the calculated number of blocks
    r_index current_block_index = 0;

    for (
        r_index arena_index = 0;
        arena_index < arena_count;
        ++arena_index) {

        for (
            r_index arena_block_index = 0;
            arena_block_index < block_count_per_arena
            ++arena_block_index) {
                        
            block_ptr[current_block_index].index       = arena_block_index;
            block_ptr[current_block_index].arena_index = arena_index;
            block_ptr[current_block_index].start       = NULL;

            ++current_block_index;
        }
    }

    //initialize the allocator
    block_allocator_ptr->next                  = NULL;
    block_allocator_ptr->reservation_index     = reservation_ptr->index;
    block_allocator_ptr->region_index          = region_ptr->region_index;
    block_allocator_ptr->block_allocator_index = 0;
    block_allocator_ptr->block_count_total     = block_count_actual;
    block_allocator_ptr->block_count_per_arena = block_count_per_arena;
    block_allocator_ptr->block_size            = block_size_aligned;
    block_allocator_ptr->blocks                = block_ptr;

    //TODO: add to list

    //return the allocator
    return(block_allocator_ptr);
}

r_external const RHNDMemoryBlock
r_mem::block_allocator_commit(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

    //get the block allocator
    RMemoryBlockAllocator* block_allocator_ptr = r_mem::block_allocator_from_handle(block_allocator_handle);

    //get the block region
    RMemoryRegion* region_ptr = 
        r_mem::region_at_index(
            block_allocator_ptr->reservation_index,
            block_allocator_ptr->region_index);

    //find the first uncommitted block
    RMemoryBlock* selected_block = NULL;
    const r_size block_array_size = sizeof(RMemoryBlock) * block_allocator_ptr->block_count_total;

    for (
        RMemoryBlock* current_block = block_allocator_ptr->blocks;
        current_block < block_allocator_ptr->blocks + block_array_size;
        ++current_block) {

        if (!current_block->start) {
            selected_block = current_block;
            break;
        }
    }

    //if we didn't find a block, we're done
    if (!selected_block) {
        return(NULL);
    }

    //if this is the first block in the arena, we need to commit the arena
    //otherwise, just grab it
    RMemoryArena* arena_ptr = 
        selected_block->index == 0
            ? (RMemoryArena*)r_mem::arena_commit_index(region_ptr, selected_block->arena_index)
            : (RMemoryArena*)r_mem::arena_at_index(region_ptr, selected_block->arena_index);

    //sanity check
    if (!arena_ptr) {
        return(NULL);
    }

    //grab the start of this arena
    const r_memory arena_start = arena_ptr->commit; 

    //calculate the offset of this block's memory
    const r_size offset = block_allocator_ptr->block_size * selected_block->index;
    
    //get the block memory
    r_memory block_memory_start = arena_start + offset;

    //update the block
    selected_block->start = block_memory_start;

    //return the block
    return(selected_block);
}

r_external const r_b8     
r_mem::block_allocator_decommit(
    const r_memory block_memory) {

    //the block start is the first in the block struct
    //so a pointer to the block memory is also a pointer to the block itself
    r_

}

r_external const r_b8     
r_mem::block_allocator_reset(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_size_total(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_size_commited(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_size_uncommited(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_block_size(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_block_count_total(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_block_count_committed(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_block_count_uncommitted(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_arena_size(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_arena_count_total(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_arena_count_committed(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const r_size   
r_mem::block_allocator_arena_count_uncommitted(
    const RHNDMemoryBlockAllocator block_allocator_handle) {

}

r_external const RHNDMemoryBlock 
r_mem::block_allocator_block_at_index(
    const RHNDMemoryBlockAllocator block_allocator_handle,
    const r_index                  block_index) {

}

r_external const RHNDMemoryBlock
r_mem::block_allocator_commit_at_index(
    const RHNDMemoryBlockAllocator block_allocator_handle,
    const r_index                  block_index) {

}

r_external const r_b8 
r_mem::block_allocator_decommit_at_index(
    const RHNDMemoryBlockAllocator block_allocator_handle, 
    const r_index                  block_index) {

}