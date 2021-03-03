#pragma once

#include <stdlib.h>

struct _String;

typedef struct _String String;

struct _String
{
	void (*delete)(const String* this);

	void (*copy)(String* this, const String* other);
	void (*copy_array)(String* this, const char* other, size_t count);
	void (*copy_c_str)(String* this, const char* other);

	void (*reserve)(String* this, size_t new_cap);
	void (*resize)(String* this, size_t count);

	size_t (*count)(const String* this);
	size_t (*length)(const String* this);
	int (*empty)(const String* this);

	char (*get_at)(const String* this, size_t index);
	void (*set_at)(String* this, size_t index, char element);

	void (*append_char)(String* this, char ch);
	void (*append_c_str)(String* this, const char* c_str);
	void (*append)(String* this, const String* other);

	void (*push_back)(String* this, char element);
	void (*pop_back)(String* this);

	void (*clear)(String* this);

	void (*insert)(String* this, size_t pos, char element);
	void (*erase)(String* this, size_t pos);

	void (*swap)(String* this, String* other);

	const char* (*c_str)(String* this);

	size_t size;
	size_t capacity;
	char* data;
};

String String_construct();
String* String_new();
void String_delete(const String* this);

void String_copy_array(String* this, const char* other, size_t count);
void String_copy_c_str(String* this, const char* other);
void String_copy(String* this, const String* other);

void String_reserve(String* this, size_t new_cap);
void String_resize(String* this, size_t count);

size_t String_count(const String* this);
size_t String_length(const String* this);
int String_empty(const String* this);

char String_get_at(const String* this, size_t index);
void String_set_at(String* this, size_t index, char element);

void String_append_char(String* this, char ch);
void String_append_c_str(String* this, const char* c_str);
void String_append(String* this, const String* other);

void String_push_back(String* this, char element);
void String_pop_back(String* this);

void String_clear(String* this);

void String_insert(String* this, size_t pos, char element);
void String_erase(String* this, size_t pos);

void String_swap(String* this, String* other);

const char* String_c_str(String* this);

struct _String string_default;

#define delete(this) (*this).delete(this)

#define copy_array(this, other, count) (*this).copy_array(this, other, count)
#define copy_c_str(this, other) (*this).copy_c_str(this, other)
#define copy(this, other) (*this).copy(this, other)

#define reserve(this, new_cap) (*this).reserve(this, new_cap)
#define resize(this, count) (*this).resize(this, count)

#define count(this) (*this).count(this)
#define length(this) (*this).length(this)
#define empty(this) (*this).empty(this)

#define get_at(this, index) (*this).get_at(this, index)
#define set_at(this, index, element) (*this).set_at(this, index, element)

#define append_char(this, ch) (*this).append_char(this, ch)
#define append_c_str(this, c_str) (*this).append_c_str(this, c_str)
#define append(this, other) (*this).append(this, other)

#define push_back(this, element) (*this).push_back(this, element)
#define pop_back(this) (*this).pop_back(this)

#define clear(this) (*this).clear(this)

#define insert(this, pos, element) (*this).insert(this, pos, element)
#define erase(this, pos) (*this).erase(this, pos)

#define swap(this, other) (*this).swap(this, other)

#define c_str(this) (*this).c_str(this)