#pragma once

#include <stdlib.h>

typedef struct _base_darr
{
	void* arr;
	size_t size;
	size_t capacity;
} base_darr;

#define darr(type) \
	struct \
	{ \
		type* arr; \
		size_t size; \
		size_t capacity; \
	}* 

base_darr* darr_new_(size_t type_size);
void darr_delete(base_darr* vec);
void darr_reserve(base_darr* vec, size_t new_size);
void darr_resize(base_darr* vec, size_t count);
size_t darr_size(base_darr* vec);
size_t darr_capacity(base_darr* vec);

#define darr_new(type) (darr(type)) darr_new_(sizeof(type));

#define darr_push(vec, val) \
	if (vec->size >= vec->capacity) \
		darr_reserve(vec, vec->capacity * 2); \
	vec->arr[vec->size++] = val;

#define darr_ptr_at(vec, index) \
({ \
	if (index >= vec->size) \
	{ \
		fprintf(stderr, "\nat line number %d in file %s\nfrom function darr_ptr_at(vec, index) at line number 33 in darr.h\nerror: index %u out of range\n\n", __LINE__, __FILE__, index); \
		exit(-1); \
	} \
	&vec->arr[index]; \
})

#define darr_at(vec, index) \
({ \
	if (index >= vec->size) \
	{ \
		fprintf(stderr, "\nat line number %d in file %s\nfrom function darr_at(vec, index) at line number 43 in darr.h\nerror: index %u out of range\n\n", __LINE__, __FILE__, index); \
		exit(-1); \
	} \
	vec->arr[index]; \
})