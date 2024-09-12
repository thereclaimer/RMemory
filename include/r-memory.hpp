#ifndef R_MEMORY_HPP
#define R_MEMORY_HPP

#include <r-common.hpp>

typedef r_handle RMemoryReservationHandle;
typedef r_handle RMemoryRegionHandle;
typedef r_handle RMemoryArenaHandle;
typedef r_handle RMemoryBlockAllocatorHandle;


/**********************************************************************************/
/* MEMORY MANAGER                                                                 */
/**********************************************************************************/

namespace r_mem {

    r_external const r_b8   memory_manager_create                   (const r_size core_stack_size);
    r_external const r_size memory_manager_minimum_reservation_size (r_void);
    r_external const r_size memory_manager_minimum_arena_size       (r_void);
    r_external const r_size memory_manager_reservation_count        (r_void);

    r_external const RMemoryReservationHandle memory_manager_reservation_list (r_void);
};

/**********************************************************************************/
/* RESERVATION                                                                    */
/**********************************************************************************/

namespace r_mem {

    r_external const RMemoryReservationHandle reserve (const r_cstr tag_value, const r_size minimum_size);

    r_external const RMemoryReservationHandle reservation_next (const RMemoryReservationHandle reservation);

    r_external const r_size reservation_size_total   (const RMemoryReservationHandle reservation_handle);
    r_external const r_size reservation_size_free    (const RMemoryReservationHandle reservation_handle);
    r_external const r_size reservation_size_used    (const RMemoryReservationHandle reservation_handle);
    r_external const r_size reservation_region_count (const RMemoryReservationHandle reservation_handle);
    r_external const r_cstr reservation_tag_value    (const RMemoryReservationHandle reservation_handle);
};

/**********************************************************************************/
/* REGION                                                                         */
/**********************************************************************************/

namespace r_mem {

    r_external const RMemoryRegionHandle 
    region_create(
        const r_cstr                   tag_value,
        const RMemoryReservationHandle reservation_handle,
        const r_size                   region_size_minimum,
        const r_size                   arena_size_minimum);

    r_external const RMemoryRegionHandle 
    region_create_arena_pool(
        const r_cstr                   tag_value,
        const RMemoryReservationHandle reservation_handle,
        const r_size                   arena_size_minimum,
        const r_size                   arena_count);

    r_external const RMemoryRegionHandle 
    region_create_struct_pool(
        const r_cstr                   tag_value,
        const RMemoryReservationHandle reservation_handle,
        const r_size                   arena_size_minimum,
        const r_size                   struct_size,
        const r_size                   struct_alignment,
        const r_size                   struct_count);

    r_external const r_size region_arena_size              (const RMemoryRegionHandle region_handle);
    r_external const r_size region_size_total              (const RMemoryRegionHandle region_handle);
    r_external const r_size region_size_committed          (const RMemoryRegionHandle region_handle);
    r_external const r_size region_size_uncommitted        (const RMemoryRegionHandle region_handle);
    r_external const r_size region_arena_count_total       (const RMemoryRegionHandle region_handle);
    r_external const r_size region_arena_count_committed   (const RMemoryRegionHandle region_handle);
    r_external const r_size region_arena_count_uncommitted (const RMemoryRegionHandle region_handle);
    
    r_external void region_reset (const RMemoryRegionHandle region_handle);
};

/**********************************************************************************/
/* ARENA                                                                         */
/**********************************************************************************/

namespace r_mem {

    r_external const RMemoryArenaHandle arena_commit(const RMemoryRegionHandle region_handle);

    r_external r_void arena_decommit(const RMemoryArenaHandle arena_handle);
    
    r_external const r_size arena_size_total (const RMemoryArenaHandle arena_handle);
    r_external const r_size arena_size_free  (const RMemoryArenaHandle arena_handle);
    r_external const r_size arena_size_used  (const RMemoryArenaHandle arena_handle);

    r_external r_memory arena_push_bytes         (const RMemoryArenaHandle arena_handle, const r_size size);
    r_external r_memory arena_pull_bytes         (const RMemoryArenaHandle arena_handle, const r_size size);
    r_external r_memory arena_push_bytes_aligned (const RMemoryArenaHandle arena_handle, const r_size size, const r_size alignment);
    r_external r_memory arena_pull_bytes_aligned (const RMemoryArenaHandle arena_handle, const r_size size, const r_size alignment);

    r_external void arena_reset(const RMemoryArenaHandle arena_handle);
};

/**********************************************************************************/
/* BLOCK ALLOCATOR                                                                */
/**********************************************************************************/

namespace r_mem {

    r_external const RMemoryBlockAllocatorHandle
    block_allocator_create(
        const r_cstr                   tag_value,
        const RMemoryReservationHandle reservation_handle,
        const r_size                   block_size,
        const r_size                   block_alignment,
        const r_size                   block_count);

    r_external r_memory block_allocator_commit   (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external r_void   block_allocator_decommit (const RMemoryBlockAllocatorHandle block_allocator_handle);
    
    r_external const r_size block_allocaor_count_blocks_total       (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocaor_count_blocks_committed   (const RMemoryBlockAllocatorHandle block_allocator_handle);
    r_external const r_size block_allocaor_count_blocks_uncommitted (const RMemoryBlockAllocatorHandle block_allocator_handle);
};

#endif //R_MEMORY_HPP