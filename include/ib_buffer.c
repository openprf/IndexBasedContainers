/*
Index Based Buffer of constant size chunks. Pointers aren't used inside.

MIT License

Copyright (c) 2020 Alexey Afanasyev
 */

#include <glob.h>
#include <stdlib.h>
#include <limits.h>

#include "ib_buffer.h"

#define NO_ELEMENT UINT_MAX

struct element_header_t{
    size_t next;
    size_t prev;
};

struct buffer_header_t{
    size_t size;
    size_t capasity;

    size_t free_head;
    size_t busy_head;

    size_t element_size;
};

struct buffer_t{
    struct buffer_header_t header;
    struct element_header_t elements[];
};

///Internal
char* get_storage(struct buffer_t* buffer){
    return (char*)(buffer->elements) + buffer->header.capasity * sizeof(struct element_header_t);
}

void* get_data_ptr_by_index(struct buffer_t* buffer, size_t index){
    char * storage = get_storage(buffer);
    return storage + index * buffer->header.element_size;
}

size_t get_index_by_data_ptr(struct buffer_t* buffer, char* data_ptr){
    char * storage = get_storage(buffer);
    return (data_ptr - storage) / buffer->header.element_size;
}

///Interface
size_t ibb_get_buffer_memory_size(size_t number_of_elements, size_t size_of_element){
    return sizeof(struct buffer_header_t) + number_of_elements * (size_of_element + sizeof(struct element_header_t));
}

size_t ibb_buffer_size(void* memory_buffer){
    if(!memory_buffer)
        return 0;
    struct buffer_t* buffer = (struct buffer_t*) memory_buffer;
    return ibb_get_buffer_memory_size(buffer->header.capasity, buffer->header.element_size);
}

int ibb_init_buffer(size_t number_of_elements, size_t size_of_element, void* memory_buffer){
    if(!memory_buffer)
        return -1;

    struct buffer_t* buffer = (struct buffer_t*) memory_buffer;
    buffer->header.size = 0;
    buffer->header.capasity = number_of_elements;
    buffer->header.element_size = size_of_element;

    buffer->header.free_head = 0;
    buffer->header.busy_head = NO_ELEMENT;

    //init free elements list;
    buffer->elements[0].prev = NO_ELEMENT;
    for(size_t i = 1; i<number_of_elements; i++){
        buffer->elements[i-1].next = i;
        buffer->elements[i].prev = i-1;
    }
    buffer->elements[number_of_elements-1].next = NO_ELEMENT;

    return 0;
}

void* ibb_create_buffer(size_t number_of_elements, size_t size_of_element)
{
    void *mem_buffer = malloc(ibb_get_buffer_memory_size(number_of_elements, size_of_element));
    ibb_init_buffer(number_of_elements, size_of_element, mem_buffer);
    return mem_buffer;
}

void* ibb_alloc(void* memory_buffer){
    struct buffer_t* buffer = (struct buffer_t*) memory_buffer;
    if(!buffer || buffer->header.size == (buffer->header.capasity))
        return NULL;

    size_t index = buffer->header.free_head;

    //remove index from free list
    buffer->header.free_head = buffer->elements[index].next;
    if(buffer->header.free_head != NO_ELEMENT)
        buffer->elements[buffer->header.free_head].prev = NO_ELEMENT;

    //add index to busy list
    buffer->elements[index].prev = NO_ELEMENT;
    if(buffer->header.busy_head == NO_ELEMENT){
        buffer->elements[index].next = NO_ELEMENT;
    }else{
        buffer->elements[buffer->header.busy_head].prev = index;
        buffer->elements[index].next = buffer->header.busy_head;
    }
    buffer->header.busy_head = index;

    buffer->header.size++;

    return get_data_ptr_by_index(buffer, index);
}

void ibb_free(void* memory_buffer, void *element){
    struct buffer_t* buffer = (struct buffer_t*) memory_buffer;
    if(!buffer || buffer->header.size == 0)
        return;

    size_t index = get_index_by_data_ptr(buffer, (char*) element);

    //remove index from busy list
    size_t prev = buffer->elements[index].prev;
    size_t next = buffer->elements[index].next;

    if(prev == NO_ELEMENT){
        //delete last record from busy list
        buffer->header.busy_head = NO_ELEMENT;
    }else{
        buffer->elements[prev].next = next;
        if(next != NO_ELEMENT){
            buffer->elements[next].prev = prev;
        }
    }

    //add index to free list
    buffer->elements[index].prev = NO_ELEMENT;
    if(buffer->header.free_head == NO_ELEMENT){
        buffer->elements[index].next = NO_ELEMENT;
    }else{
        buffer->elements[buffer->header.free_head].prev = index;
        buffer->elements[index].next = buffer->header.free_head;
    }
    buffer->header.free_head = index;

    buffer->header.size--;
}

size_t ibb_get_allocated_ptrs(void* memory_buffer, void** allocatad_chuncs){
    if(!memory_buffer)
        return 0;

    struct buffer_t* buffer = (struct buffer_t*) memory_buffer;

    if(buffer->header.size == 0)
        return 0;

    size_t index = buffer->header.busy_head;
    for(size_t i = 0; i < buffer->header.size; i++){
        allocatad_chuncs[i] = get_data_ptr_by_index(buffer, index);
        index = buffer->elements[index].next;
    }

    return buffer->header.size;
}

size_t ibb_size(void *memory_buffer) {
    if(!memory_buffer)
        return 0;

    struct buffer_t* buffer = (struct buffer_t*) memory_buffer;
    return buffer->header.size;
}
