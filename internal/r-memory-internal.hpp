#ifndef R_MEMORY_INTERNAL_HPP
#define R_MEMORY_INTERNAL_HPP

#include "r-memory.hpp"

/**********************************************************************************/
/* ARENA                                                                          */
/**********************************************************************************/

struct RMemoryArena {
    r_index   index;
    r_memory  commit;
    r_address position;
};

/**********************************************************************************/
/* REGION                                                                         */
/**********************************************************************************/

struct RMemoryRegion {
    r_index       reservation_index;
    r_size        arena_count;
    r_size        arena_size;
    r_memory      arena_start;
    RMemoryArena* arenas;
};

/**********************************************************************************/
/* STACK                                                                          */
/**********************************************************************************/

struct RMemoryStack {
    r_memory  start;
    r_size    size;
    r_address position;
};

namespace r_mem_internal {

    r_internal const r_b8                stack_create   (const r_size stack_size);
    r_internal const r_b8                stack_can_push (const r_size size);
    r_internal const r_memory            stack_push     (const r_size size);
    r_internal const RMemoryReservation* stack_push_reservation(r_void);  
    r_internal const RMemoryRegion*      stack_push_region(r_void);
    r_internal const RMemoryArena*       stack_push_arenas(const r_size arena_count);
};

/**********************************************************************************/
/* BLOCK ALLOCATOR                                                                */
/**********************************************************************************/

struct RMemoryBlock {
    r_index  index;
    r_index  arena_index;
    r_memory start;
};

struct RMemoryBlockAllocator {
    RMemoryBlockAllocator* next;
    r_index                reservation_index;
    r_index                region_index;
    r_index                block_allocator_index;
    r_size                 block_count;
    r_size                 block_size;
    RMemoryBlock*          blocks;
};

/**********************************************************************************/
/* RESERVATION                                                                    */
/**********************************************************************************/

struct RMemoryReservation {
    RMemoryReservation*   next;
    RMemoryRegion*        region_list;
    RMemoryBlockAllocator block_allocator_list;
    r_memory              start;
    r_size                size;
    r_address             position;
    r_index               index;
};

/**********************************************************************************/
/* MEMORY MANAGER                                                                 */
/**********************************************************************************/

struct RMemoryManager {
    RMemoryReservation* reservation_list;
    RPlatformMemoryApi  platform_memory_api;
    RMemoryStack        stack;
    r_size              maximum_reserved_size;
    r_size              minimum_reservation_size;
    r_size              minimum_arena_size;
};

r_global RMemoryManager _r_memory_manager;

namespace r_mem_internal {

    inline r_memory platform_memory_reserve              (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.reserve(start,size));            }
    inline r_memory platform_memory_commit               (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.commit(start,size));             }
    inline r_memory platform_memory_reserve_and_commit   (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.reserve_and_commit(start,size)); }
    inline r_b8     platform_memory_decommit             (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.decommit(start,size));           }
    inline r_b8     platform_memory_release              (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.release(start,size));            }

    inline       RMemoryStack&       memory_manager_get_stack            (r_void) { return(_r_memory_manager.stack); };
    inline const RMemoryReservation* memory_manager_get_reservation_list (r_void) { return(_r_memory_manager.stack); };
};

#endif //R_MEMORY_INTERNAL_HPP