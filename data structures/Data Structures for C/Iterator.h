#pragma once

#define Iterator_declare(Itr, T) \
                           \
 	struct _Iterator_##Itr \
	{                                   \
		T (*get)(const Itr* this);            \
		void (*set)(Itr* this, T element);         \
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

#define advance(this, n) (*this).iterator->advance(this, n)
#define distance(first, last) (*first).iterator->distance(first, last)
#define next(this) (*this).iterator->next(this)
#define prev(this) (*this).iterator->prev(this)