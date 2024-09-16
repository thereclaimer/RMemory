#ifndef R_MEMORY_INTERNAL_HPP
#define R_MEMORY_INTERNAL_HPP

#include "r-memory.hpp"

/**********************************************************************************/
/* FORWARD DECLARATIONS                                                           */
/**********************************************************************************/

struct RMemoryArena;
struct RMemoryReservation;
struct RMemoryRegion;
struct RMemoryArena;

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

struct RMemoryRegionList{
    RMemoryRegion* first;
    RMemoryRegion* last;
    r_size         count;
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

    r_internal const r_b8          stack_create   (const r_size stack_size);
    r_internal const r_b8          stack_can_push (const r_size size);
    r_internal r_memory            stack_push     (const r_size size);
    r_internal RMemoryReservation* stack_push_reservation(r_void);  
    r_internal RMemoryRegion*      stack_push_region(r_void);
    r_internal RMemoryArena*       stack_push_arenas(const r_size arena_count);
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

struct RMemoryBlockAllocatorList {
    RMemoryBlockAllocator* first;
    RMemoryBlockAllocator* last;
    r_size                 count;
};

/**********************************************************************************/
/* STACK ALLOCATOR                                                                */
/**********************************************************************************/

struct RMemoryStackAllocator {
    RMemoryStackAllocator* next;
    r_memory               stack_start;
    r_size                 stack_size;
    r_address              stack_position;
    r_index                reservation_index;
    r_index                region_index;
    r_index                stack_allocator_index;
    r_index                current_arena_index;
};

struct RMemoryStackAllocatorList {
    RMemoryStackAllocator* first;
    RMemoryStackAllocator* last;
    r_size                 count;
};

/**********************************************************************************/
/* DOUBLE STACK ALLOCATOR                                                         */
/**********************************************************************************/

struct RMemoryDoubleStackAllocatorSubStack {
    r_memory  start;
    r_size    size;
    r_address position;
    r_index   current_arena_index;
    r_b32     top_down;
};

struct RMemoryDoubleStackAllocator {
    RMemoryDoubleStackAllocator*        next;
    r_index                             reservation_index;
    r_index                             region_index;
    r_index                             double_stack_allocator_index;
    RMemoryDoubleStackAllocatorSubStack stack_a;
    RMemoryDoubleStackAllocatorSubStack stack_b;
};

struct RMemoryDoubleStackAllocatorList {
    RMemoryDoubleStackAllocator* first;
    RMemoryDoubleStackAllocator* last;
    r_size                       count;
};


/**********************************************************************************/
/* RESERVATION                                                                    */
/**********************************************************************************/

struct RMemoryReservation {
    RMemoryReservation*             next;
    RMemoryRegionList               region_list;
    RMemoryBlockAllocatorList       block_allocator_list;
    RMemoryStackAllocatorList       stack_allocator_list;
    RMemoryDoubleStackAllocatorList double_stack_allocator_list;
    r_memory                        start;
    r_size                          size;
    r_address                       position;
    r_index                         index;
};

struct RMemoryReservationList {
    RMemoryReservation* first;        
    RMemoryReservation* last;
    r_size              count;
    r_size              total_reserved_size;
};

namespace r_mem_internal {

    inline RMemoryReservation* reservation_from_handle(const RHNDMemoryReservation reservation_handle) { return((RMemoryReservation*)reservation_handle); }    

    r_internal r_void reservation_list_add         (RMemoryReservation* reservation_ptr);
    r_internal r_b8   reservation_list_release_all (r_void);
    r_internal r_b8   reservation_list_can_add     (const r_size reservation_size);
};

/**********************************************************************************/
/* MEMORY MANAGER                                                                 */
/**********************************************************************************/

struct RMemoryManager {
    RMemoryReservationList reservation_list;
    RPlatformMemoryApi     platform_memory_api;
    RMemoryStack           stack;
    r_size                 maximum_reserved_size;
    r_size                 reservation_alignment;
    r_size                 arena_alignment;
};

r_global RMemoryManager _r_memory_manager;

namespace r_mem_internal {

    inline r_memory platform_memory_reserve              (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.reserve(start,size));            }
    inline r_memory platform_memory_commit               (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.commit(start,size));             }
    inline r_memory platform_memory_reserve_and_commit   (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.reserve_and_commit(start,size)); }
    inline r_b8     platform_memory_decommit             (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.decommit(start,size));           }
    inline r_b8     platform_memory_release              (const r_memory start, const r_size size) { return(_r_memory_manager.platform_memory_api.release(start,size));            }

    inline RMemoryStack&             memory_manager_get_stack            (r_void) { return(_r_memory_manager.stack); };
    inline RMemoryReservationList&   memory_manager_get_reservation_list (r_void) { return(_r_memory_manager.reservation_list); };
};

#endif //R_MEMORY_INTERNAL_HPP