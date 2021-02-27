#pragma once

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define Vector_define(T) \
 \
    struct _Vector_##T \
    { \
        size_t size; \
        size_t capacity; \
        T* data; \
    }; \
     \
    typedef struct _Vector_##T* Vector_##T; \
    typedef const struct _Vector_##T* ConstVector_##T; \
                       \
    Vector_##T Vector_##T##_new();     \
    void Vector_##T##_delete(Vector_##T this); \
                         \
    void Vector_##T##_reserve(Vector_##T this, size_t new_cap); \
    void Vector_##T##_resize(Vector_##T this, size_t count);    \
                         \
    void Vector_##T##_assign_array(Vector_##T this, const T* other, size_t count); \
    void Vector_##T##_assign(Vector_##T this, ConstVector_##T other);              \
                         \
    T Vector_##T##_at(ConstVector_##T this, size_t index);      \
    void Vector_##T##_set_at(Vector_##T this, size_t index, T element); \
                       \
    Vector_##T Vector_##T##_new()      \
    {                  \
        Vector_##T new_list = (Vector_##T)malloc(sizeof(*new_list)); \
                       \
        new_list->data = (T*)malloc(2 * sizeof(T));              \
        new_list->size = 0; \
        new_list->capacity = 2;     \
        \
        return new_list; \
    }                  \
                       \
    void Vector_##T##_delete(Vector_##T this)                          \
    {                     \
        if (!this) return;\
                       \
        free(this->data); \
        free(this); \
    }                       \
                         \
    void Vector_##T##_reserve(Vector_##T this, size_t new_cap)    \
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
    void Vector_##T##_resize(Vector_##T this, size_t count)        \
    {                       \
        if (count <= this->capacity) this->size = count;              \
                         \
        size_t new_cap = 1;   \
        while (new_cap < count) \
            new_cap *= 2; \
                         \
        Vector_##T##_reserve(this, new_cap);  \
        this->size = count; \
    }                       \
                         \
    void Vector_##T##_assign_array(Vector_##T this, const T* other, size_t count)     \
    {                       \
        if (count > this->size) Vector_##T##_resize(this, count);           \
                         \
        memcpy(this->data, other, sizeof(T) * count); \
    }                       \
                         \
    void Vector_##T##_assign(Vector_##T this, ConstVector_##T other)               \
    {                       \
        Vector_##T##_assign_array(this, other->data, other->size); \
    }                       \
                         \
    T Vector_##T##_at(ConstVector_##T this, size_t index)       \
    {                       \
        if (index >= this->size)                  \
        {                      \
            fprintf(stderr, "index out of range at line no. %d in file %s\n", __LINE__, __FILE__); \
            exit(1); \
        }                      \
        return this->data[index]; \
    }                       \
                         \
    void Vector_##T##_set_at(Vector_##T this, size_t index, T element)         \
    {                       \
        if (index >= this->size)                  \
        {                      \
            fprintf(stderr, "index out of range at line no. %d in file %s\n", __LINE__, __FILE__); \
            exit(1); \
        }                      \
        this->data[index] = element; \
    }\
