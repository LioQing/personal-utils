#pragma once

#include <stdlib.h>

struct _String
{
	char* c_str;
	size_t size;
};

typedef struct _String* String;
typedef const struct _String* ConstString;

String String_new();
void String_delete(String this);

void String_assign_c_str(String this, const char* other);
void String_assign(String this, ConstString other);

char String_at(ConstString this, size_t index);
void String_set_at(String this, size_t index, char c);
void String_set_at_if(String this, char c, int (*pred)(char));
void String_set_at_if_pos(String this, char c, int (*pred)(size_t));

int String_empty(ConstString this);
void String_clear(String this);

void String_append_c_str(String this, const char* other);
void String_append(String this, ConstString other);
void String_append_char(String this, char c);
void String_append_char_fill(String this, size_t count, char c);

int String_compare_c_str(ConstString this, const char* other);
int String_compare(ConstString this, ConstString other);

String String_substr(ConstString this, size_t pos, size_t len);

void String_insert_c_str(String this, size_t pos, const char* other);
void String_insert(String this, size_t pos, ConstString other);
void String_insert_char(String this, size_t pos, char c);
void String_insert_char_fill(String this, size_t pos, size_t count, char c);

void String_erase(String this, size_t pos, size_t len);
void String_erase_if(String this, int (*pred)(char));
void String_erase_if_pos(String this, int (*pred)(size_t));

void String_replace(String this, size_t pos, ConstString other);
void String_replace_append(String this, size_t pos, ConstString other);
void String_replace_c_str(String this, size_t pos, const char* other);
void String_replace_append_c_str(String this, size_t pos, const char* other);