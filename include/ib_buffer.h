/*
Index Based Buffer of constant size chunks. Pointers aren't used inside.

MIT License

Copyright (c) 2020 Alexey Afanasyev
 */

#ifndef CONTAINERS_IB_BUFFER_H
#define CONTAINERS_IB_BUFFER_H

/**
 * @param number_of_elements
 * @param size_of_element
 * @return size of memory that should be allocated for buffer object
 */
size_t ibb_get_buffer_memory_size(size_t number_of_elements, size_t size_of_element);

/**
 * Get Buffer size including service data
 * @param memory_buffer
 * @return buffer_size
 */
size_t ibb_buffer_size(void* memory_buffer);

/**
 * Initialaize new buffer
 * @param number_of_elements
 * @param size_of_element
 * @param memory_buffer
 */
int ibb_init_buffer(size_t number_of_elements, size_t size_of_element, void* memory_buffer);

/**
 * Allocate memoru for Buffer and init it
 * @param number_of_elements
 * @param size_of_element
 * @return pointer to Buffer object or NULL if error occurred
 */
void* ibb_create_buffer(size_t number_of_elements, size_t size_of_element);

/**
 * Get free chunk. Complexity O(1).
 * @param memory_buffer
 * @return pointer to free  or NULL if error occurred
 */
void* ibb_alloc(void* memory_buffer);

/**
 * retur chunk to unused list. Complexity O(1).
 * @param memory_buffer
 * @param element
 */
void ibb_free(void* memory_buffer, void *element);

/**
 * Get list of allocates chaunks. Complexity O(n), n - number of allocatad chunks.
 * @param memory_buffer
 * @param allocatad_chuncs
 * @return number of allocated chunks
 */
size_t ibb_get_allocated_ptrs(void* memory_buffer, void** allocatad_chuncs);

/**
 *
 * @param memory_buffer. Complexity O(1).
 * @return number of allocated elements
 */
size_t ibb_size(void* memory_buffer);

#endif //CONTAINERS_IB_BUFFER_H
