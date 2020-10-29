#include "darr.h"

base_darr* darr_new_(size_t type_size)
{
	base_darr* new_darr = malloc(sizeof(*new_darr));
	new_darr->arr = malloc(type_size * 8);

	new_darr->size = 0;
	new_darr->capacity = 8;

	return new_darr;
}

void darr_delete(base_darr* vec)
{
	free(vec->arr);
	free(vec);
}

void darr_reserve(base_darr* vec, size_t new_cap)
{
	size_t _new_cap = vec->capacity;
	while (_new_cap < new_cap)
		_new_cap *= 2;
	vec->arr = realloc(vec->arr, _new_cap);
	vec->capacity = new_cap;
}

void darr_resize(base_darr* vec, size_t count)
{
	if (count > vec->capacity)
		darr_reserve(vec, count);
	vec->size = count;
}

size_t darr_size(base_darr* vec)
{
	return vec->size;
}

size_t darr_capacity(base_darr* vec)
{
	return vec->capacity;
}