#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VectorIterator.h"

#define Vector_declare(T) \
                         \
    struct _Vector_##T;                     \
                         \
 	typedef struct _Vector_##T Vector_##T; \
                          \
	VectorIterator_declare(T) \
                          \
    struct _Vector_##T \
    { \
        void (*delete)(const Vector_##T* this);          \
        				 \
        void (*copy)(Vector_##T* this, const Vector_##T* other);              \
        void (*copy_array)(Vector_##T* this, const T* other, size_t count); \
        \
        void (*reserve)(Vector_##T* this, size_t new_cap); \
        void (*resize)(Vector_##T* this, size_t count); \
                         \
        size_t (*count)(const Vector_##T* this);          \
        int (*empty)(const Vector_##T* this); 	       \
                         \
        T (*get_at)(const Vector_##T* this, size_t index); \
        void (*set_at)(Vector_##T* this, size_t index, T element);            \
                         \
        void (*push_back)(Vector_##T* this, T element); \
        void (*pop_back)(Vector_##T* this);  \
                         \
        void (*clear)(Vector_##T* this);     \
                         \
        void (*insert)(Vector_##T* this, size_t pos, T element);              \
        void (*erase)(Vector_##T* this, size_t pos);    \
                         \
        void (*swap)(Vector_##T* this, Vector_##T* other);                    \
        \
        size_t size; \
        size_t capacity; \
        T* data;         \
    };                    \
                          \
    Vector_##T Vector_##T##_construct();    \
    Vector_##T* Vector_##T##_new();          \
    void Vector_##T##_delete(const Vector_##T* this); \
                         \
    void Vector_##T##_copy_array(Vector_##T* this, const T* other, size_t count); \
    void Vector_##T##_copy(Vector_##T* this, const Vector_##T* other);        \
                         \
    void Vector_##T##_reserve(Vector_##T* this, size_t new_cap); \
    void Vector_##T##_resize(Vector_##T* this, size_t count);    \
                         \
    size_t Vector_##T##_count(const Vector_##T* this);    \
    int Vector_##T##_empty(const Vector_##T* this); \
                         \
    T Vector_##T##_get_at(const Vector_##T* this, size_t index);      \
    void Vector_##T##_set_at(Vector_##T* this, size_t index, T element);      \
                         \
    void Vector_##T##_push_back(Vector_##T* this, T element);                 \
    void Vector_##T##_pop_back(Vector_##T* this); \
    		           \
    void Vector_##T##_clear(Vector_##T* this);          \
                         \
    void Vector_##T##_insert(Vector_##T* this, size_t pos, T element);        \
    void Vector_##T##_erase(Vector_##T* this, size_t pos);                    \
                         \
    void Vector_##T##_swap(Vector_##T* this, Vector_##T* other); \
                         \
	struct _Vector_##T vector_##T##_default; \

#define Vector_define(T) \
 \
	struct _Vector_##T vector_##T##_default =  \
	{                       \
		&Vector_##T##_delete,  \
                         \
		&Vector_##T##_copy,        \
		&Vector_##T##_copy_array,         \
		 \
		&Vector_##T##_reserve,           \
		&Vector_##T##_resize,  \
		\
		&Vector_##T##_count, \
		&Vector_##T##_empty, \
		 \
		&Vector_##T##_get_at,        \
		&Vector_##T##_set_at,        \
		 \
		&Vector_##T##_push_back,  \
		&Vector_##T##_pop_back,\
		\
		&Vector_##T##_clear,          \
		 \
		&Vector_##T##_insert,        \
		&Vector_##T##_erase,          \
		 \
		&Vector_##T##_swap,    \
                         \
		0,                     \
		1,                     \
	};                      \
                         \
	Vector_##T Vector_##T##_construct()        \
	{                       \
		Vector_##T new_vec = vector_##T##_default; \
                         \
		new_vec.data = (T*)malloc(new_vec.capacity * sizeof(T));                    \
                         \
		return new_vec; \
	}\
                       \
    Vector_##T* Vector_##T##_new()      \
    {                  \
        Vector_##T* new_vec = (Vector_##T*)malloc(sizeof(*new_vec)); \
                       \
        new_vec->size = 0; \
        new_vec->capacity = 1;     \
        new_vec->data = (T*)malloc(new_vec->capacity * sizeof(T));              \
                         \
        new_vec->delete = &Vector_##T##_delete;             \
        new_vec->copy_array = &Vector_##T##_copy_array;         \
        new_vec->copy = &Vector_##T##_copy;        \
                         \
        new_vec->reserve = &Vector_##T##_reserve;           \
        new_vec->resize = &Vector_##T##_resize;        \
        \
        new_vec->count = &Vector_##T##_count; \
        new_vec->empty = &Vector_##T##_empty; \
                         \
        new_vec->get_at = &Vector_##T##_get_at;        \
        new_vec->set_at = &Vector_##T##_set_at;        \
                         \
        new_vec->push_back = &Vector_##T##_push_back;  \
        new_vec->pop_back = &Vector_##T##_pop_back;    \
        \
        new_vec->clear = &Vector_##T##_clear;          \
                         \
        new_vec->insert = &Vector_##T##_insert;        \
        new_vec->erase = &Vector_##T##_erase;          \
                         \
        new_vec->swap = &Vector_##T##_swap; 		   \
        \
        return new_vec; \
    }                    \
                         \
    void Vector_##T##_delete(const Vector_##T* this)          \
	{                       \
		if (!this) return;     \
                         \
		free(this->data);      \
		free((Vector_##T*)this); \
	}\
                         \
    void Vector_##T##_copy_array(Vector_##T* this, const T* other, size_t count)     \
    {                       \
        Vector_##T##_resize(this, count);           \
                         \
        memcpy(this->data, other, sizeof(T) * count); \
    }                       \
                         \
    void Vector_##T##_copy(Vector_##T* this, const Vector_##T* other)               \
    {                       \
        Vector_##T##_copy_array(this, other->data, other->size); \
    }                    \
                         \
    void Vector_##T##_reserve(Vector_##T* this, size_t new_cap)    \
    {                       \
        if (new_cap <= this->capacity) return;    \
                         \
        T* tmp = (T*)malloc(sizeof(T) * this->size); \
        memcpy(tmp, this->data, sizeof(T) * this->size);             \
                         \
        this->data = (T*)realloc(this->data, sizeof(T) * new_cap);   \
        memcpy(this->data, tmp, sizeof(T) * this->size);             \
        this->capacity = new_cap; \
                         \
        free(tmp); \
    }                       \
                         \
    void Vector_##T##_resize(Vector_##T* this, size_t count)        \
    {                       \
        size_t new_cap = 1;   \
        while (new_cap < count) \
            new_cap *= 2; \
                         \
        Vector_##T##_reserve(this, new_cap);  \
        this->size = count; \
    }                       \
                         \
    size_t Vector_##T##_count(const Vector_##T* this)     \
	{                       \
		return this->size;\
	}                       \
                         \
	int Vector_##T##_empty(const Vector_##T* this)          \
	{                       \
		return (int)(this->size == 0); \
	} 					 \
                         \
    T Vector_##T##_get_at(const Vector_##T* this, size_t index)       \
    {                       \
        if (index < 0 || index >= this->size)                  \
        {                      \
            fprintf(stderr, "index out of range at line no. %d in file %s\n", __LINE__, __FILE__); \
            exit(1); \
        }                      \
        return this->data[index]; \
    }                       \
                         \
    void Vector_##T##_set_at(Vector_##T* this, size_t index, T element)         \
    {                       \
        if (index < 0 || index >= this->size)                  \
        {                      \
            fprintf(stderr, "index out of range at line no. %d in file %s\n", __LINE__, __FILE__); \
            exit(1); \
        }                      \
        this->data[index] = element; \
    }                    \
                         \
    void Vector_##T##_push_back(Vector_##T* this, T element)                  \
	{                       \
		Vector_##T##_resize(this, this->size + 1);       \
		this->data[this->size - 1] = element;\
	}                       \
	\
    void Vector_##T##_pop_back(Vector_##T* this)\
	{                       \
		if (this->size <= 0)   \
		{                      \
			fprintf(stderr, "failed to pop back vector at line no. %d in file %s\n", __LINE__, __FILE__);   \
			exit(1); \
		}                      \
		Vector_##T##_resize(this, this->size - 1); \
	}                       \
                         \
	void Vector_##T##_clear(Vector_##T* this)   \
	{                       \
		Vector_##T##_resize(this, 0); \
	}                       \
	\
    void Vector_##T##_insert(Vector_##T* this, size_t pos, T element)         \
	{                       \
        if (pos < 0 || pos >= this->size)                  \
        {                      \
            fprintf(stderr, "pos out of range at line no. %d in file %s\n", __LINE__, __FILE__); \
            exit(1); \
        }                \
                         \
        Vector_##T##_resize(this, this->size + 1); \
	\
    	for (size_t i = this->size; i > pos; --i)         \
			this->data[i] = this->data[i - 1];        \
                         \
        this->data[pos] = element; \
	}                       \
	\
    void Vector_##T##_erase(Vector_##T* this, size_t pos)                     \
    {                    \
    	if (pos < 0 || pos >= this->size)                  \
        {                      \
            fprintf(stderr, "pos out of range at line no. %d in file %s\n", __LINE__, __FILE__); \
            exit(1); \
        }                \
                         \
        for (size_t i = pos + 1; i < this->size; ++i)  \
        	this->data[i - 1] = this->data[i];     \
                         \
        Vector_##T##_resize(this, this->size - 1);     \
    }                    \
                         \
    void Vector_##T##_swap(Vector_##T* this, Vector_##T* other)                \
	{                       \
		T* tmp = this->data; \
		this->data = other->data;          \
		other->data = tmp;     \
                         \
		this->size = this->size ^ other->size;    \
		other->size = this->size ^ other->size;   \
		this->size = this->size ^ other->size;    \
		\
		this->capacity = this->capacity ^ other->capacity;    \
		other->capacity = this->capacity ^ other->capacity;   \
		this->capacity = this->capacity ^ other->capacity;    \
	}                       \
                         \
    VectorIterator_define(T) \

#define Vector(T) Vector_##T

#define Vector_construct(T) Vector_##T##_construct()
#define Vector_new(T) Vector_##T##_new()
#define delete(this) (*this).delete(this)

#define copy_array(this, other, count) (*this).copy_array(this, other, count)
#define copy(this, other) (*this).copy(this, other)

#define reserve(this, new_cap) (*this).reserve(this, new_cap)
#define resize(this, count) (*this).resize(this, count)

#define count(this) (*this).count(this)
#define empty(this) (*this).empty(this)

#define get_at(this, index) (*this).get_at(this, index)
#define set_at(this, index, element) (*this).set_at(this, index, element)

#define push_back(this, element) (*this).push_back(this, element)
#define pop_back(this) (*this).pop_back(this)

#define clear(this) (*this).clear(this)

#define insert(this, pos, element) (*this).insert(this, pos, element)
#define erase(this, pos) (*this).erase(this, pos)

#define swap(this, other) (*this).swap(this, other)
