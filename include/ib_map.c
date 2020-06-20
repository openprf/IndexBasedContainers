/*
Index Based Map (binary tree). Pointers aren't used inside.

MIT License

Copyright (c) 2020 Alexey Afanasyev
 */

#include "ib_map.h"
#include <stdlib.h>
#include <limits.h>
#define NO_ELEMENT UINT_MAX

struct entry_header_t{
    size_t parent;

    union{
        size_t red;
        size_t prev;
    };

    union{
        size_t black;
        size_t next;
    };

};

struct  ibmap_header_t{
    size_t size;    // number of added elements
    size_t capacity;

    size_t key_size;
    size_t value_size;

    size_t free_list;

    size_t head;

};

struct ibmap_t{
    struct ibmap_header_t header;
    struct entry_header_t entries[];
};

//Service functions

static inline size_t get_entry_size(struct ibmap_t* map){
    return map->header.key_size + map->header.value_size;
}

static inline char* get_storage(struct ibmap_t* map){
    return (char*)map->entries + map->header.capacity * sizeof(struct entry_header_t);
}

static inline void* get_key_by_index(struct ibmap_t* map, size_t key_index){
    return (void*) (get_storage(map) + key_index * get_entry_size(map));
}

static inline void* get_value_by_index(struct ibmap_t* map, size_t value_index){
    return (void*) (get_storage(map) + value_index * get_entry_size(map) + map->header.key_size);
}

///INTERFACE FUNCTIONS

size_t ibmap_get_memory_size(size_t key_size, size_t value_size, size_t number_of_entries) {
    return sizeof(struct ibmap_header_t) +
            number_of_entries * (sizeof(struct entry_header_t) + key_size + value_size);
}


size_t ibmap_memory_size(void* memory_buffer){
    struct ibmap_header_t* map_hdr = (struct ibmap_header_t*) memory_buffer;
    return ibmap_get_memory_size(map_hdr->key_size,map_hdr->value_size, map_hdr->capacity);
}


int ibmap_init(size_t key_size, size_t value_size, size_t number_of_entries, void* memory_buffer){
    struct ibmap_header_t* map_hdr = (struct ibmap_header_t*) memory_buffer;
    map_hdr->size = 0;
    map_hdr->capacity = number_of_entries;
    map_hdr->key_size = key_size;
    map_hdr->value_size = value_size;

    map_hdr->free_list = 0;
    map_hdr->head = NO_ELEMENT;

    struct ibmap_t* map = (struct ibmap_t*) memory_buffer;

    //init free elements list;
    map->entries[0].prev = NO_ELEMENT;
    for(size_t i = 1; i<map_hdr->capacity ; i++){
        map->entries[i-1].next = i;
        map->entries[i].prev = i-1;
    }
    map->entries[map_hdr->capacity -1].next = NO_ELEMENT;

    return 0;
}


void* ibmap_create(size_t key_size, size_t value_size, size_t number_of_entries){
    void* mem_buffer = malloc(ibmap_get_memory_size(key_size, value_size, number_of_entries));

    if(mem_buffer){
        ibmap_init(key_size, value_size, number_of_entries, mem_buffer);
    }
    return mem_buffer;
}

size_t ibmap_size(void* memory_buffer){
    struct ibmap_header_t* map_hdr = (struct ibmap_header_t*) memory_buffer;
    return map_hdr->size;
}

void* ibmap_alloc(void* memory_buffer, void* key_ptr){

}

size_t ibmap_erase(void* memory_buffer, void* key_ptr);
size_t ibmap_erase_value(void* memory_buffer, void* value_ptr){}

void* ibmap_find(void* memory_buffer, void* key_ptr){}

void* ibmap_foreach(void* memory_buffer, int (*action)(const void*, void*)){}