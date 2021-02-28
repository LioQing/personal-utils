#pragma once

#include "Iterator.h"

#define VectorIterator_declare(T) \
                                 \
	struct _VectorIterator_##T;      \
                                 \
	typedef struct _VectorIterator_##T VectorIterator_##T; \
                                 \
    Iterator_declare(VectorIterator_##T, T) \
                                 \
	struct _VectorIterator_##T      \
	{                               \
        void (*delete)(const VectorIterator_##T* this);                         \
		\
        void (*copy)(VectorIterator_##T* this, const VectorIterator_##T* other); \
                                 \
		Iterator_VectorIterator_##T* iterator;                \
		\
		Vector_##T* vec;\
        size_t pos;              \
	};                               \
                                  \
	VectorIterator_##T VectorIterator_##T##_construct(Vector_##T* vec, size_t pos);          \
	VectorIterator_##T* VectorIterator_##T##_new(Vector_##T* vec, size_t pos);          \
	void VectorIterator_##T##_delete(const VectorIterator_##T* this); \
                                 \
	void VectorIterator_##T##_copy(VectorIterator_##T* this, const VectorIterator_##T* other); \
                                 \
	T VectorIterator_##T##_get(const VectorIterator_##T* this);           \
	void VectorIterator_##T##_set(VectorIterator_##T* this, T element); \
                                 \
	void VectorIterator_##T##_advance(VectorIterator_##T* this, long n);    \
	size_t VectorIterator_##T##_distance(const VectorIterator_##T* first, const VectorIterator_##T* last); \
	VectorIterator_##T VectorIterator_##T##_next(const VectorIterator_##T* this);       \
	VectorIterator_##T VectorIterator_##T##_prev(const VectorIterator_##T* this);       \
                                 \
	struct _Iterator_VectorIterator_##T _vector_iterator_##T; \
                                 \
	struct _VectorIterator_##T vector_iterator_##T##_default; \

#define VectorIterator_define(T) \
                                 \
	struct _VectorIterator_##T;      \
                                 \
	typedef struct _VectorIterator_##T VectorIterator_##T; \
                                 \
    Iterator_define(VectorIterator_##T, T) \
                                 \
	VectorIterator_##T VectorIterator_##T##_construct(Vector_##T* vec, size_t pos);          \
	VectorIterator_##T* VectorIterator_##T##_new(Vector_##T* vec, size_t pos);          \
	void VectorIterator_##T##_delete(const VectorIterator_##T* this); \
                                 \
	void VectorIterator_##T##_copy(VectorIterator_##T* this, const VectorIterator_##T* other); \
                                 \
	T VectorIterator_##T##_get(const VectorIterator_##T* this);           \
	void VectorIterator_##T##_set(VectorIterator_##T* this, T element); \
                                 \
	void VectorIterator_##T##_advance(VectorIterator_##T* this, long n);    \
	size_t VectorIterator_##T##_distance(const VectorIterator_##T* first, const VectorIterator_##T* last); \
	VectorIterator_##T VectorIterator_##T##_next(const VectorIterator_##T* this);       \
	VectorIterator_##T VectorIterator_##T##_prev(const VectorIterator_##T* this);       \
                                 \
	struct _Iterator_VectorIterator_##T _vector_iterator_##T =                 \
	{                               \
		&VectorIterator_##T##_get,           \
		&VectorIterator_##T##_set,           \
                                 \
		&VectorIterator_##T##_advance,       \
		&VectorIterator_##T##_distance,      \
		&VectorIterator_##T##_next,          \
        &VectorIterator_##T##_prev, \
	};                              \
                                 \
	struct _VectorIterator_##T vector_iterator_##T##_default =                       \
	{                               \
        &VectorIterator_##T##_delete, \
		\
		&VectorIterator_##T##_copy,    \
                                 \
		&_vector_iterator_##T,\
	}; \
	\
	VectorIterator_##T VectorIterator_##T##_construct(Vector_##T* vec, size_t pos)           \
	{                               \
        VectorIterator_##T new_vec = vector_iterator_##T##_default;\
		\
		new_vec.vec = vec;      \
		new_vec.pos = pos;             \
                                 \
		return new_vec;\
	}                               \
	\
	VectorIterator_##T* VectorIterator_##T##_new(Vector_##T* vec, size_t pos)           \
	{                               \
		VectorIterator_##T* new_itr = (VectorIterator_##T*)malloc(sizeof(*new_itr));   \
                                 \
		new_itr->vec = vec;      \
                                 \
		new_itr->pos = pos;            \
                                 \
		new_itr->iterator = &_vector_iterator_##T;             \
                                 \
		new_itr->delete = &VectorIterator_##T##_delete;                \
		new_itr->copy = &VectorIterator_##T##_copy; \
                                 \
		return new_itr; \
	}                               \
	\
	void VectorIterator_##T##_delete(const VectorIterator_##T* this)\
	{                               \
		if (!this) return;             \
                                 \
		free((VectorIterator_##T*)this); \
	}\
                                 \
	void VectorIterator_##T##_copy(VectorIterator_##T* this, const VectorIterator_##T* other)  \
	{                               \
		this->vec = other->vec; \
		this->pos = other->pos; \
	}                               \
                                 \
	T VectorIterator_##T##_get(const VectorIterator_##T* this)            \
	{                               \
		return Vector_##T##_get_at(this->vec, this->pos); \
	}                               \
                                 \
	void VectorIterator_##T##_set(VectorIterator_##T* this, T element)                    \
	{                               \
		Vector_##T##_set_at(this->vec, this->pos, element); \
	}                               \
                                 \
	void VectorIterator_##T##_advance(VectorIterator_##T* this, long n)                   \
	{                               \
		this->pos += n;                 \
                                 \
		if (this->pos < 0 || this->pos >= this->vec->size) \
        {                      \
            fprintf(stderr, "pos out of range at line no. %d in file %s\n", __LINE__, __FILE__); \
            exit(1); \
        }                \
	}                               \
                                 \
	size_t VectorIterator_##T##_distance(const VectorIterator_##T* first, const VectorIterator_##T* last)  \
	{                               \
		if (last->pos < first->pos) \
        {                      \
            fprintf(stderr, "last->pos out of range at line no. %d in file %s\n", __LINE__, __FILE__); \
            exit(1); \
        }                        \
                                 \
        return last->pos - first->pos; \
	}                               \
                                 \
	VectorIterator_##T VectorIterator_##T##_next(const VectorIterator_##T* this)                \
	{                               \
		if (this->pos >= this->vec->size) \
        {                      \
            fprintf(stderr, "last->pos + 1 out of range at line no. %d in file %s\n", __LINE__, __FILE__); \
            exit(1); \
        }                        \
                                 \
        return VectorIterator_##T##_construct(this->vec, this->pos + 1); \
	}                               \
                                 \
	VectorIterator_##T VectorIterator_##T##_prev(const VectorIterator_##T* this)     \
	{                               \
		if (this->pos <= 0) \
        {                      \
            fprintf(stderr, "last->pos - 1 out of range at line no. %d in file %s\n", __LINE__, __FILE__); \
            exit(1); \
        }                        \
                                 \
        return VectorIterator_##T##_construct(this->vec, this->pos - 1); \
	}                               \

#define VectorIterator(T) VectorIterator_##T

#define VectorIterator_construct(T, vec, pos) VectorIterator_##T##_construct(vec, pos)
#define VectorIterator_new(T, vec, pos) VectorIterator_##T##_new(vec, pos)
#define delete(this) (*this).delete(this)