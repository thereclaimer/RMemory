#ifndef R_MEMORY_HPP
#define R_MEMORY_HPP

#include <r-common.hpp>

typedef r_handle RHNDMemoryReservation;
typedef r_handle RHNDMemoryRegion;
typedef r_handle RHNDMemoryArena;
typedef r_handle RHNDMemoryBlockAllocator;
typedef r_handle RHNDMemoryStackAllocator;
typedef r_handle RHNDMemoryDoubleStackAllocator;

/**********************************************************************************/
/* SIZES                                                                          */
/**********************************************************************************/

namespace r_mem {

    inline const r_size size_kilobytes(const r_size size) { return(size * 1024);               }
    inline const r_size size_megabytes(const r_size size) { return(size * 1024 * 1024);        }
    inline const r_size size_gigabytes(const r_size size) { return(size * 1024 * 1024 * 1024); }
};

/**********************************************************************************/
/* MEMORY MANAGER                                                                 */
/**********************************************************************************/

namespace r_mem {

    r_external const r_b8
    memory_manager_create(
        const RPlatformMemoryApi& platform_memory_api,
        const r_size              maximum_reserved_size,
        const r_size              minimum_reservation_size,
        const r_size              minimum_arena_size,
        const r_size              internal_stack_size);

    r_external const r_b8   memory_manager_destroy                   (r_void);
    r_external const r_size memory_manager_maximum_reserved_size     (r_void);
    r_external const r_size memory_manager_minimum_reservation_size  (r_void);
    r_external const r_size memory_manager_minimum_arena_size        (r_void);
    r_external const r_size memory_manager_current_reserved_size     (r_void);
    r_external const r_size memory_manager_internal_stack_size_total (r_void);
    r_external const r_size memory_manager_internal_stack_size_used  (r_void);
    r_external const r_size memory_manager_internal_stack_size_free  (r_void);
};

/**********************************************************************************/
/* RESERVATION                                                                    */
/**********************************************************************************/

namespace r_mem {

    r_external const RHNDMemoryReservation 
    reserve(
        const r_cstr reservation_tag,
        const r_size minimum_reservation_size);

    r_external const r_size reservation_size_total         (r_void);
    r_external const r_size reservation_size_used          (r_void);
    r_external const r_size reservation_size_free          (r_void);
    r_external const r_size reservation_arena_pool_count   (r_void);
    r_external const r_size reservation_arena_region_count (r_void);

    r_external const RHNDMemoryReservation reservation_list     (r_void);
    r_external const RHNDMemoryReservation reservation_next     (const RHNDMemoryReservation reservation_handle);
    r_external const RHNDMemoryReservation reservation_at_index (const r_index reservation_index);
};


/**********************************************************************************/
/* REGION                                                                   */
/**********************************************************************************/

namespace r_mem {

    r_external const RHNDMemoryRegion
    region_create(
        const RHNDMemoryReservation reservation_handle,
        const r_cstr                region_tag,
        const r_size                region_size_minimum,
        const r_size                arena_size_minimum);

    r_external const RHNDMemoryRegion
    region_create_arena_pool(
        const RHNDMemoryReservation reservation_handle,
        const r_cstr                region_tag,
        const r_size                arena_size_minimum,
        const r_size                arena_count);

    r_external const RHNDMemoryRegion
    region_create_struct_pool(
        const RHNDMemoryReservation reservation_handle,
        const r_cstr                region_tag,
        const r_size                arena_size_minimum,
        const r_size                struct_size,
        const r_size                struct_alignment,
        const r_size                struct_count);

    r_external const RHNDMemoryRegion region_list     (const RHNDMemoryReservation reservation_handle);
    r_external const RHNDMemoryRegion region_at_index (const RHNDMemoryReservation reservation_handle, const r_index);
    r_external const RHNDMemoryRegion region_next     (const RHNDMemoryRegion region_handle);

    r_external const r_size region_size_total              (r_void);
    r_external const r_size region_size_committed          (r_void);
    r_external const r_size region_size_decommitted        (r_void);
    r_external const r_size region_arena_size              (r_void);
    r_external const r_size region_arena_count_total       (r_void);
    r_external const r_size region_arena_count_committed   (r_void);
    r_external const r_size region_arena_count_decommitted (r_void);
};

/**********************************************************************************/
/* ARENA                                                                          */
/**********************************************************************************/

namespace r_mem {

    const RHNDMemoryArena arena_commit_next   (RHNDMemoryArenaRegion arena_region_handle);
    const RHNDMemoryArena arena_commit_index   (RHNDMemoryArenaRegion arena_region_handle, const r_index arena_index);

    const r_b8            arena_decommit      (const RHNDMemoryArena arena_handle);

};

/**********************************************************************************/
/* BLOCK ALLOCATOR                                                                */
/**********************************************************************************/

namespace r_mem {

    r_external const RHNDMemoryBlockAllocator 
    block_allocator_create(
        const r_cstr block_allocator_tag,
        const r_size block_size,
        const r_size block_alignment,
        const r_size block_count,
        const r_size minimum_arena_size);

    r_external       r_memory block_allocator_commit                  (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_b8     block_allocator_decommit                (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_b8     block_allocator_reset                   (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_size_total              (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_size_commited           (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_size_uncommited         (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_block_size              (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_block_count_total       (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_block_count_committed   (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_block_count_uncommitted (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_arena_size              (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_arena_count_total       (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_arena_count_committed   (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   block_allocator_arena_count_uncommitted (const RHNDMemoryBlockAllocator block_allocator_handle);
}

/**********************************************************************************/
/* BLOCK ALLOCATOR                                                                */
/**********************************************************************************/

namespace r_mem {

    r_external const RHNDMemoryStackAllocator
    stack_allocator_create(
        const r_cstr stack_allocator_tag,
        const r_size stack_allocator_size,
        const r_size minimum_arena_size);


    r_external r_memory stack_allocator_memory (const RHNDMemoryStackAllocator stack_allocator);
    r_external r_memory stack_allocator_push   (const RHNDMemoryStackAllocator stack_allocator, const r_size push_size);
    r_external r_memory stack_allocator_pull   (const RHNDMemoryStackAllocator stack_allocator, const r_size pull_size);
    
    r_external const r_b8 stack_allocator_set_save_point      (const RHNDMemoryStackAllocator stack_allocator);
    r_external const r_b8 stack_allocator_reset               (const RHNDMemoryStackAllocator stack_allocator);
    r_external const r_b8 stack_allocator_reset_to_save_point (const RHNDMemoryStackAllocator stack_allocator);
    
    r_external const r_size   stack_allocator_size_total              (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   stack_allocator_size_commited           (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   stack_allocator_size_uncommited         (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   stack_allocator_arena_size              (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   stack_allocator_arena_count_total       (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   stack_allocator_arena_count_committed   (const RHNDMemoryBlockAllocator block_allocator_handle);
    r_external const r_size   stack_allocator_arena_count_uncommitted (const RHNDMemoryBlockAllocator block_allocator_handle);
};