#pragma once

#define Iterator_declare(Itr, T) \
                           \
 	struct _Iterator_##Itr \
	{                                   \
		T (*get)(const Itr* this);            \
		void (*set)(Itr* this, T element); \
                                 \
		int (*compare)(const Itr* this, const Itr* other); \
		\
		void (*advance)(Itr* this, long n); \
		size_t (*distance)(const Itr* first, const Itr* last); \
		Itr (*next)(const Itr* this);      \
		Itr (*prev)(const Itr* this);      \
	};                             \
                                \
	typedef struct _Iterator_##Itr Iterator_##Itr;          \

#define get(this) (*this).iterator->get(this)
#define set(this, element) (*this).iterator->set(this, element)

#define compare(this, other) (*this).iterator->compare(this, other)

#define advance(this, n) (*this).iterator->advance(this, n)
#define distance(first, last) (*first).iterator->distance(first, last)
#define next(this) (*this).iterator->next(this)
#define prev(this) (*this).iterator->prev(this)

#define begin(Itr, this) Itr##_begin(this)
#define end(Itr, this) Itr##_end(this)

#define foreach(T, Itr, element, this) \
{ \
	Itr end_itr = end(Itr, this); \
	for (Itr itr = begin(Itr, this); compare(&itr, &end_itr) < 0; advance(&itr, 1)) \
    { \
        T element = get(&itr);               \

#define end_foreach() } }