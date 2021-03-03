#pragma once

#include "String.h"
#include "Iterator.h"

struct _StringIterator;

typedef struct _StringIterator StringIterator; 
							 
Iterator_declare(StringIterator, char);
							 
struct _StringIterator
{                               
	void (*delete)(const StringIterator* this);                         
	
	void (*copy)(StringIterator* this, const StringIterator* other);
							 
	Iterator_StringIterator* iterator;                
	
	String* vec; 
	size_t pos;              
};                               
							  
StringIterator StringIterator_construct(String* vec, size_t pos);          
StringIterator* StringIterator_new(String* vec, size_t pos);          
void StringIterator_delete(const StringIterator* this); 
							 
void StringIterator_copy(StringIterator* this, const StringIterator* other);
int StringIterator_compare(const StringIterator* this, const StringIterator* other);
							 
char StringIterator_get(const StringIterator* this);           
void StringIterator_set(StringIterator* this, char element); 
							 
void StringIterator_advance(StringIterator* this, long n);    
size_t StringIterator_distance(const StringIterator* first, const StringIterator* last); 
StringIterator StringIterator_next(const StringIterator* this);       
StringIterator StringIterator_prev(const StringIterator* this);       
							 
struct _Iterator_StringIterator _string_iterator; 
							 
struct _StringIterator string_iterator_default;

#define delete(this) (*this).delete(this)

#define copy(this, other) (*this).copy(this, other)

#define begin(this) StringIterator_construct(this, 0)
#define end(this) StringIterator_construct(this, (*this).size)