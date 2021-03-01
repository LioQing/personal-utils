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
					 
String String_construct()        
{                       
	String new_vec = string_default; 
					 
	new_vec.data = (char*)malloc(new_vec.capacity * sizeof(char));                    
					 
	return new_vec; 
}
				   
String* String_new()      
{                  
	String* new_vec = (String*)malloc(sizeof(*new_vec)); 
				   
	new_vec->size = 0; 
	new_vec->capacity = 1;     
	new_vec->data = (char*)malloc(new_vec->capacity * sizeof(char));              
					 
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
					 
	free(this->data);      
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
					 
	this->data = (char*)realloc(this->data, sizeof(char) * new_cap);   
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
	char* tmp = this->data;
	this->data = other->data;
	other->data = tmp;

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