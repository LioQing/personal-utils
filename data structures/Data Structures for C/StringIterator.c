#include "StringIterator.h"

#include "stdlib.h"
#include "stdio.h"

struct _Iterator_StringIterator _string_iterator =
{                               
	&StringIterator_get,           
	&StringIterator_set,      
	
	&StringIterator_compare,
							 
	&StringIterator_advance,       
	&StringIterator_distance,      
	&StringIterator_next,          
	&StringIterator_prev, 
};                              
							 
struct _StringIterator string_iterator_default =                       
{                               
	&StringIterator_delete, 
	
	&StringIterator_copy,
							 
	&_string_iterator,
}; 

StringIterator StringIterator_construct(String* vec, size_t pos)           
{                               
	StringIterator new_vec = string_iterator_default;
	
	new_vec.vec = vec;      
	new_vec.pos = pos;             
							 
	return new_vec;
}                               

StringIterator* StringIterator_new(String* vec, size_t pos)           
{                               
	StringIterator* new_itr = (StringIterator*)malloc(sizeof(*new_itr));   
							 
	new_itr->vec = vec;      
							 
	new_itr->pos = pos;          
							 
	new_itr->iterator = &_string_iterator;             
							 
	new_itr->delete = &StringIterator_delete;                
	new_itr->copy = &StringIterator_copy; 
							 
	return new_itr; 
}                               

void StringIterator_delete(const StringIterator* this)
{                               
	if (!this) return;             
							 
	free((StringIterator*)this); 
}
							 
void StringIterator_copy(StringIterator* this, const StringIterator* other)  
{                               
	this->vec = other->vec; 
	this->pos = other->pos; 
}                 

int StringIterator_compare(const StringIterator* this, const StringIterator* other)
{
	return (int)this->pos - (int)other->pos;
}
							 
char StringIterator_get(const StringIterator* this)            
{                               
	return String_get_at(this->vec, this->pos); 
}                               
							 
void StringIterator_set(StringIterator* this, char element)                    
{                               
	String_set_at(this->vec, this->pos, element); 
}                               
							 
void StringIterator_advance(StringIterator* this, long n)                   
{                               
	this->pos += n;                 
							 
	if (this->pos < 0 || this->pos > this->vec->size) 
	{                      
		fprintf(stderr, "pos out of range at line no. %d in file %s\n", __LINE__, __FILE__); 
		exit(1); 
	}                
}                               
							 
size_t StringIterator_distance(const StringIterator* first, const StringIterator* last)  
{                               
	if (last->pos < first->pos) 
	{                      
		fprintf(stderr, "last->pos out of range at line no. %d in file %s\n", __LINE__, __FILE__); 
		exit(1); 
	}                        
							 
	return last->pos - first->pos; 
}                               
							 
StringIterator StringIterator_next(const StringIterator* this)                
{                               
	if (this->pos >= this->vec->size) 
	{                      
		fprintf(stderr, "last->pos + 1 out of range at line no. %d in file %s\n", __LINE__, __FILE__); 
		exit(1); 
	}                        
							 
	return StringIterator_construct(this->vec, this->pos + 1); 
}                               
							 
StringIterator StringIterator_prev(const StringIterator* this)     
{                               
	if (this->pos <= 0) 
	{                      
		fprintf(stderr, "last->pos - 1 out of range at line no. %d in file %s\n", __LINE__, __FILE__); 
		exit(1); 
	}                        
							 
	return StringIterator_construct(this->vec, this->pos - 1);
}