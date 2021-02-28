#pragma once

#define Iterator_define(Itr, T) \
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
	typedef struct _Iterator_##Itr Iterator_##Itr; \

#define get(itr) itr->iterator->get(itr)
#define set(itr, element) itr->iterator->set(itr, element)

#define advance(itr, n) itr->iterator->advance(itr, n)
#define distance(first, last) first->iterator->distance(first, last)
#define next(itr) itr->iterator->next(itr)
#define prev(itr) itr->iterator->prev(itr)