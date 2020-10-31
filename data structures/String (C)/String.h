#pragma once

#include <stdlib.h>

struct _String
{
	char* c_str;
	size_t size;
};

typedef struct _String* String;

String String_new();
void String_delete(String this);

void String_copy_c_str(String this, const char* other);
void String_copy(String this, const String other);

char String_at(const String this, size_t index);
void String_set_at(String this, size_t index, char c);
void String_set_at_if(String this, char c, int (*pred)(char));
void String_set_at_if_pos(String this, char c, int (*pred)(size_t));

int String_empty(const String this);
void String_clear(String this);

void String_append_c_str(String this, const char* other);
void String_append(String this, const String other);
void String_append_char(String this, char c);
void String_append_char_fill(String this, size_t count, char c);

int String_compare_c_str(const String this, const char* other);
int String_compare(const String this, const String other);

String String_substr(const String this, size_t pos, size_t len);

void String_insert_c_str(String this, size_t pos, const char* other);
void String_insert(String this, size_t pos, const String other);
void String_insert_char(String this, size_t pos, char c);
void String_insert_char_fill(String this, size_t pos, size_t count, char c);

void String_erase(String this, size_t pos, size_t len);
void String_erase_if(String this, int (*pred)(char));
void String_erase_if_pos(String this, int (*pred)(size_t));

void String_replace(String this, size_t pos, const String other);
void String_replace_append(String this, size_t pos, const String other);
void String_replace_c_str(String this, size_t pos, const char* other);
void String_replace_append_c_str(String this, size_t pos, const char* other);