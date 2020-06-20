/*
Index Based Map (binary tree). Pointers aren't used inside.

MIT License

Copyright (c) 2020 Alexey Afanasyev
 */
#ifndef CONTAINERS_IB_MAP_H
#define CONTAINERS_IB_MAP_H

#include <glob.h>

/**
 * @param key_size
 * @param value_size
 * @return size of memory that should be allocated for buffer object
 */
size_t ibmap_get_memory_size(size_t key_size, size_t value_size, size_t number_of_entries);

/**
 * Get Buffer size including service data
 * @param memory_buffer
 * @return buffer_size
 */
size_t ibmap_memory_size(void* memory_buffer);

/**
 * Initialaize new buffer
 * @param number_of_elements
 * @param size_of_element
 * @param memory_buffer
 */
int ibmap_init(size_t key_size, size_t value_size, size_t number_of_entries, void* memory_buffer);


void* ibmap_create(size_t key_size, size_t value_size, size_t number_of_entries);

size_t ibmap_size(void* memory_buffer);

void* ibmap_alloc(void* memory_buffer, void* key_ptr);

size_t ibmap_erase(void* memory_buffer, void* key_ptr);

size_t ibmap_erase_value(void* memory_buffer, void* value_ptr);

void* ibmap_find(void* memory_buffer, void* key_ptr);

void* ibmap_foreach(void* memory_buffer, int (*action)(const void*, void*));


#endif //CONTAINERS_IB_MAP_H
