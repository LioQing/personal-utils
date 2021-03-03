#include "String.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _String string_default =  
{                       
	&String_delete,  
					 
	&String_copy,        
	&String_copy_array,         
	&String_copy_c_str,
	 
	&String_reserve,           
	&String_resize,  
	
	&String_count, 
	&String_length,
	&String_empty, 
	 
	&String_get_at,        
	&String_set_at,        
	 
	&String_append_char,
	&String_append_c_str,
	&String_append,
	
	&String_push_back,  
	&String_pop_back,
	
	&String_clear,          
	 
	&String_insert,        
	&String_erase,          
	 
	&String_swap,    
	
	&String_c_str,
					 
	0,                     
	1,                     
};

static size_t _data_size = 0;
static size_t _freed_indices_cap = 0;
static size_t _freed_indices_size = 0;
static size_t* _freed_indices;
static char*** _data_ptr;
static char** _data;

void _gc_entry(char** data, size_t* allocator, int need_reserve)
{
	if (need_reserve)
	{
		if (_data_ptr == NULL)
		{
			_data_size = 1;
			_data_ptr = (char***)malloc(sizeof(void*));
			_data = (char**)malloc(sizeof(void*));
		}
		else
		{
			char*** _data_ptr_tmp = (char***)malloc(sizeof(void*) * _data_size);
			char** _data_tmp = (char**)malloc(sizeof(void*) * _data_size);

			memcpy(_data_ptr_tmp, _data_ptr, sizeof(void*) * _data_size);
			memcpy(_data_tmp, _data, sizeof(void*) * _data_size);

			_data_ptr = (char***)realloc(_data_ptr, sizeof(void*) * (_data_size + 1));
			_data = (char**)realloc(_data, sizeof(void*) * (_data_size + 1));

			memcpy(_data_ptr, _data_ptr_tmp, sizeof(void*) * _data_size);
			memcpy(_data, _data_tmp, sizeof(void*) * _data_size);

			++_data_size;

			free(_data_ptr_tmp);
			free(_data_tmp);
		}
	}
	
	_data_ptr[allocator[0]] = data;
	_data[allocator[0]] = (char*)(&allocator[1]);
}

void _gc_sweep()
{
	for (size_t i = 0; i < _data_size; ++i)
	{
		int is_freed = 0;
		for (size_t j = 0; j < _freed_indices_size; ++j)
		{
			if (_freed_indices[j] == i)
			{
				is_freed = 1;
				break;
			}
		}
		
		if (is_freed || *_data_ptr[i] == _data[i]) continue;
		
		if (_freed_indices == NULL)
		{
			_freed_indices = (size_t*)malloc(sizeof(size_t));
			_freed_indices_size = 1;
			_freed_indices_cap = 1;
		}
		else if (_freed_indices_size < _freed_indices_cap)
		{
			++_freed_indices_size;
		}
		else
		{
			size_t* tmp = (size_t*)malloc(sizeof(size_t) * _freed_indices_size);
			memcpy(tmp, _freed_indices, sizeof(size_t) * _freed_indices_size);

			_freed_indices = (size_t*)realloc(_freed_indices, sizeof(size_t) * (_freed_indices_size + 1));
			memcpy(_freed_indices, tmp, sizeof(size_t) * _freed_indices_size);
			
			++_freed_indices_size;
			++_freed_indices_cap;

			free(tmp);
		}
		
		_freed_indices[_freed_indices_size - 1] = i;

		size_t* allocator = &((size_t*)_data[i])[-1];
		free(allocator);
	}
}

size_t _gc_get_freed_index()
{
	if (_freed_indices_size == 0)
	{
		fprintf(stderr, "_freed_indices_size is 0 at line no. %d in file %s\n", __LINE__, __FILE__);
		exit(1);
	}
	
	return _freed_indices[--_freed_indices_size];
}

void _gc_swap(char** data1, char** data2)
{
	size_t* allocator1 = &((size_t*)*data1)[-1];
	size_t* allocator2 = &((size_t*)*data2)[-1];

	*data1 = (char*)(&allocator2[1]);
	*data2 = (char*)(&allocator1[1]);
	
	_data_ptr[allocator1[0]] = data2;
	_data_ptr[allocator2[0]] = data1;
}

void _gc_malloc(char** data, size_t size)
{
	_gc_sweep();
	
	size_t* allocator = (size_t*)malloc(sizeof(size_t) + size);
	allocator[0] = _data_size;
	int need_reserve = 1;
	
	if (_freed_indices_size != 0)
	{
		need_reserve = 0;
		allocator[0] = _gc_get_freed_index();
	}
	
	*data = (char*)(&allocator[1]);
	_gc_entry(data, allocator, need_reserve);
}

void _gc_realloc(char** data, size_t size)
{
	size_t* allocator = &((size_t*)*data)[-1];
	allocator = (size_t*)realloc(allocator, sizeof(size_t) + size);
	*data = (char*)(&allocator[1]);
	_gc_entry(data, allocator, 0);
}

String String_construct()        
{                       
	String new_vec = string_default; 
					 
	_gc_malloc(&new_vec.data, new_vec.capacity * sizeof(char));
					 
	return new_vec; 
}
				   
String* String_new()      
{                  
	String* new_vec = (String*)malloc(sizeof(*new_vec)); 
				   
	new_vec->size = 0; 
	new_vec->capacity = 1;
	_gc_malloc(&new_vec->data, new_vec->capacity * sizeof(char));          
					 
	new_vec->delete = &String_delete;             
	new_vec->copy_array = &String_copy_array;         
	new_vec->copy_c_str = &String_copy_c_str;
	new_vec->copy = &String_copy;        
					 
	new_vec->reserve = &String_reserve;           
	new_vec->resize = &String_resize;        
	
	new_vec->count = &String_count; 
	new_vec->length = &String_length,
	new_vec->empty = &String_empty; 
					 
	new_vec->get_at = &String_get_at;        
	new_vec->set_at = &String_set_at;        
					 
	new_vec->append_char = &String_append_char;
	new_vec->append_c_str = &String_append_c_str;
	new_vec->append = &String_append;
	
	new_vec->push_back = &String_push_back;  
	new_vec->pop_back = &String_pop_back;    
	
	new_vec->clear = &String_clear;          
					 
	new_vec->insert = &String_insert;        
	new_vec->erase = &String_erase;          
					 
	new_vec->swap = &String_swap; 	
	
	new_vec->c_str = &String_c_str;
	
	return new_vec; 
}                    
					 
void String_delete(const String* this)          
{                       
	if (!this) return;     
					 
	free((String*)this); 
}
					 
void String_copy_array(String* this, const char* other, size_t count)     
{                       
	String_resize(this, count);           
					 
	memcpy(this->data, other, sizeof(char) * count); 
}

void String_copy_c_str(String* this, const char* other)
{
	String_copy_array(this, other, strlen(other));
}

void String_copy(String* this, const String* other)               
{                       
	String_copy_array(this, other->data, other->size); 
}                    
					 
void String_reserve(String* this, size_t new_cap)    
{                       
	if (new_cap <= this->capacity) return;    
					 
	char* tmp = (char*)malloc(sizeof(char) * this->size); 
	memcpy(tmp, this->data, sizeof(char) * this->size);             
					 
	_gc_realloc(&this->data, sizeof(char) * new_cap);   
	memcpy(this->data, tmp, sizeof(char) * this->size);             
	this->capacity = new_cap; 
					 
	free(tmp); 
}                       
					 
void String_resize(String* this, size_t count)        
{                       
	size_t new_cap = 1;   
	while (new_cap < count) 
		new_cap *= 2; 
					 
	String_reserve(this, new_cap);  
	this->size = count; 
}                       
					 
size_t String_count(const String* this)     
{                       
	return this->size;
}

size_t String_length(const String* this)
{
	return strlen(this->data);
}
					 
int String_empty(const String* this)          
{                       
	return (int)(this->size == 0); 
} 					 
					 
char String_get_at(const String* this, size_t index)       
{                       
	if (index < 0 || index >= this->size)                  
	{                      
		fprintf(stderr, "index out of range at line no. %d in file %s\n", __LINE__, __FILE__); 
		exit(1); 
	}                      
	return this->data[index]; 
}                       
					 
void String_set_at(String* this, size_t index, char element)         
{                       
	if (index < 0 || index >= this->size)                  
	{                      
		fprintf(stderr, "index out of range at line no. %d in file %s\n", __LINE__, __FILE__); 
		exit(1); 
	}                      
	this->data[index] = element; 
}                    

void String_append_char(String* this, char ch)
{
	String_resize(this, this->size + 1);
	strncat(this->data, &ch, 1);
}

void String_append_c_str(String* this, const char* c_str)
{
	String_resize(this, this->size + strlen(c_str));
	strcat(this->data, c_str);
}

void String_append(String* this, const String* other)
{
	String_resize(this, String_length(this) + String_length(other) + 1);
	strcat(this->data, other->data);
}
					 
void String_push_back(String* this, char element)                  
{                       
	String_resize(this, this->size + 1);       
	this->data[this->size - 1] = element;
}                       

void String_pop_back(String* this)
{                       
	if (this->size <= 0)   
	{                      
		fprintf(stderr, "failed to pop back vector at line no. %d in file %s\n", __LINE__, __FILE__);   
		exit(1); 
	}                      
	String_resize(this, this->size - 1); 
}                       
					 
void String_clear(String* this)   
{                       
	String_resize(this, 0); 
}                       

void String_insert(String* this, size_t pos, char element)         
{                       
	if (pos < 0 || pos >= this->size)                  
	{                      
		fprintf(stderr, "pos out of range at line no. %d in file %s\n", __LINE__, __FILE__); 
		exit(1); 
	}                
					 
	String_resize(this, this->size + 1); 

	for (size_t i = this->size; i > pos; --i)         
		this->data[i] = this->data[i - 1];        
					 
	this->data[pos] = element; 
}                       

void String_erase(String* this, size_t pos)                     
{                    
	if (pos < 0 || pos >= this->size)                  
	{                      
		fprintf(stderr, "pos out of range at line no. %d in file %s\n", __LINE__, __FILE__); 
		exit(1); 
	}                
					 
	for (size_t i = pos + 1; i < this->size; ++i)  
		this->data[i - 1] = this->data[i];     
					 
	String_resize(this, this->size - 1);     
}

void String_swap(String* this, String* other)
{
	_gc_swap(&this->data, &other->data);

	this->size = this->size ^ other->size;
	other->size = this->size ^ other->size;
	this->size = this->size ^ other->size;

	this->capacity = this->capacity ^ other->capacity;
	other->capacity = this->capacity ^ other->capacity;
	this->capacity = this->capacity ^ other->capacity;
}

const char* String_c_str(String* this)
{
	return this->data;
}