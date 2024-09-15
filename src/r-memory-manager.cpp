#pragma once

#include "r-memory-internal.hpp"

r_external const r_b8
r_mem::memory_manager_create(
    RPlatformMemoryApi& platform_memory_api,
    r_size              maximum_reserved_size,
    r_size              minimum_reservation_size,
    r_size              minimum_arena_size,
    r_size              internal_stack_size) {

    //make sure the platform api is valid
    const r_b8 api_valid = 
        platform_memory_api.reserve            &&
        platform_memory_api.commit             &&
        platform_memory_api.reserve_and_commit &&
        platform_memory_api.decommit           &&
        platform_memory_api.release;
    
    if (!api_valid) {
        return(false);
    }

    //sanity check on our sizes
    const r_b8 sizes_valid =
        maximum_reserved_size    > 0                        &&
        minimum_reservation_size > 0                        &&
        minimum_arena_size       > 0                        &&
        internal_stack_size      > 0                        &&
        minimum_arena_size       < minimum_reservation_size &&
        minimum_reservation_size < maximum_reserved_size;

    if (!sizes_valid) {
        return(false);
    }

    _r_memory_manager.reservation_list         = NULL;
    _r_memory_manager.platform_memory_api      = platform_memory_api;
    _r_memory_manager.maximum_reserved_size    = maximum_reserved_size;
    _r_memory_manager.minimum_reservation_size = minimum_reservation_size;
    _r_memory_manager.minimum_arena_size       = minimum_arena_size;

    //create the stack
    const r_b8 stack_created = r_mem_internal::stack_create(internal_stack_size);
    if (!stack_created) {
        return(false);
    }

    //if we got through all of that, we're good to go
    return(true);
}

r_external const r_b8
r_mem::memory_manager_destroy(r_void) {
    //TODO
    
    return(false);
}

r_external const r_size
r_mem::memory_manager_maximum_reserved_size(r_void) {

    return(_r_memory_manager.maximum_reserved_size);
}

r_external const r_size
r_mem::memory_manager_minimum_reservation_size(r_void) {

    return(_r_memory_manager.minimum_reservation_size);
}

r_external const r_size
r_mem::memory_manager_minimum_arena_size(r_void) {

    return(_r_memory_manager.minimum_arena_size);
}

r_external const r_size
r_mem::memory_manager_current_reserved_size(r_void) {

}

r_external const r_size
r_mem::memory_manager_internal_stack_size_total(r_void) {

    

}

r_external const r_size
r_mem::memory_manager_internal_stack_size_used(r_void) {

}

r_external const r_size
r_mem::memory_manager_internal_stack_size_free(r_void) {

}