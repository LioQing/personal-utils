#include "String.h"

#include <stdio.h>
#include <string.h>

String String_new()
{
	String new_str = (String)malloc(sizeof(*new_str));

	new_str->c_str = (char*)malloc(sizeof(char));
	strcpy(new_str->c_str, "");
	new_str->size = 0;

	return new_str;
}

void String_delete(String this)
{
	if (!this) return;

	free(this->c_str);
	free(this);
}

void String_copy_c_str(String this, const char* other)
{
	this->c_str = (char*)realloc(this->c_str, sizeof(char) * (strlen(other) + 1));
	
	strcpy(this->c_str, other);
	this->size = strlen(other);
}

void String_copy(String this, const String other)
{
	String_copy_c_str(this, other->c_str);
}

char String_at(const String this, size_t index)
{
	if (index >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\nindex out of range\n", __LINE__, __FILE__);
		exit(1);
	}
	return this->c_str[index];
}

void String_set_at(String this, size_t index, char c)
{
	if (index >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\nindex out of range\n", __LINE__, __FILE__);
		exit(1);
	}
	this->c_str[index] = c;
}

void String_set_at_if(String this, char c, int (*pred)(char))
{
	for (size_t i = 0; i < this->size; ++i)
	{
		if ((*pred)(String_at(this, i)))
		{
			String_set_at(this, i, c);
		}
	}
}

void String_set_at_if_pos(String this, char c, int (*pred)(size_t))
{
	for (size_t i = 0; i < this->size; ++i)
	{
		if ((*pred)(i))
		{
			String_set_at(this, i, c);
		}
	}
}

int String_empty(const String this)
{
	return this->size == 0;
}

void String_clear(String this)
{
	this->c_str = (char*)realloc(this, sizeof(char));

	strcpy(this->c_str, "");
	this->size = 0;
}

void String_append_c_str(String this, const char* other)
{
	char* tmp = (char*)malloc(sizeof(char) * (this->size + strlen(other) + 1));
	strcpy(tmp, this->c_str);
	strcat(tmp, other);

	String_copy_c_str(this, tmp);

	free(tmp);
}

void String_append(String this, const String other)
{
	String_append_c_str(this, other->c_str);
}

void String_append_char(String this, char c)
{
	char* tmp = (char*)malloc(sizeof(char) * (this->size + 2));
	strcpy(tmp, this->c_str);
	tmp[this->size] = c;
	tmp[this->size + 1] = '\0';

	String_copy_c_str(this, tmp);

	free(tmp);
}

void String_append_char_fill(String this, size_t count, char c)
{
	for (size_t i = 0; i < count; ++i)
	{
		String_append_char(this, c);
	}
}

int String_compare_c_str(const String this, const char* other)
{
	return strcmp(this->c_str, other);
}

int String_compare(const String this, const String other)
{
	return strcmp(this->c_str, other->c_str);
}

String String_substr(const String this, size_t pos, size_t len)
{
	if (pos >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\npos out of range\n", __LINE__, __FILE__);
		exit(1);
	}
	else if (pos + len > this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\npos + len out of range\n", __LINE__, __FILE__);
		exit(1);
	}

	String new_str = String_new();

	for (size_t i = pos; i < pos + len; ++i)
	{
		String_append_char(new_str, String_at(this, i));
	}

	return new_str;
}

void String_insert_c_str(String this, size_t pos, const char* other)
{
	if (pos >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\npos out of range\n", __LINE__, __FILE__);
		exit(1);
	}

	String tmp_str = String_substr(this, pos, this->size - pos);
	String tmp_str1 = String_substr(this, 0, pos);
	String_copy(this, tmp_str1);
	String_delete(tmp_str1);

	String_append_c_str(this, other);
	String_append(this, tmp_str);

	String_delete(tmp_str);
}

void String_insert(String this, size_t pos, const String other)
{
	String_insert_c_str(this, pos, other->c_str);
}

void String_insert_char(String this, size_t pos, char c)
{
	if (pos >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\npos out of range\n", __LINE__, __FILE__);
		exit(1);
	}

	String tmp_str = String_substr(this, pos, this->size - pos);
	String tmp_str1 = String_substr(this, 0, pos);
	String_copy(this, tmp_str1);
	String_delete(tmp_str1);

	String_append_char(this, c);
	String_append(this, tmp_str);

	String_delete(tmp_str);
}

void String_insert_char_fill(String this, size_t pos, size_t count, char c)
{
	if (pos >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\npos out of range\n", __LINE__, __FILE__);
		exit(1);
	}

	String tmp_str = String_substr(this, pos, this->size - pos);
	String tmp_str1 = String_substr(this, 0, pos);
	String_copy(this, tmp_str1);
	String_delete(tmp_str1);

	for (size_t i = 0; i < count; ++i)
		String_append_char(this, c);
	String_append(this, tmp_str);

	String_delete(tmp_str);
}

void String_erase(String this, size_t pos, size_t len)
{
	if (pos >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\npos out of range\n", __LINE__, __FILE__);
		exit(1);
	}

	size_t i;
	for (i = pos; i + len < this->size; ++i)
	{
		String_set_at(this, i, String_at(this, i + len));
	}
	String_set_at(this, i, '\0');

	this->c_str = (char*)realloc(this->c_str, sizeof(char) * (this->size - len + 1));
	this->size = this->size - len;
}

void String_erase_if(String this, int (*pred)(char))
{
	for (size_t i = 0; i < this->size; ++i)
	{
		if ((*pred)(String_at(this, i)))
		{
			String_erase(this, i, 1);
			--i;
		}
	}
}

void String_erase_if_pos(String this, int (*pred)(size_t))
{
	for (size_t i = 0, j = 0; i < this->size; ++i, ++j)
	{
		if ((*pred)(j))
		{
			String_erase(this, i, 1);
			--i;
		}
	}
}

void String_replace(String this, size_t pos, const String other)
{
	if (pos >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\npos out of range\n", __LINE__, __FILE__);
		exit(1);
	}

	for (size_t i = pos; (i < this->size) && (i - pos < other->size); ++i)
	{
		String_set_at(this, i, String_at(other, i - pos));
	}
}

void String_replace_append(String this, size_t pos, const String other)
{
	if (pos >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\npos out of range\n", __LINE__, __FILE__);
		exit(1);
	}

	if (pos + other->size < this->size)
	{
		String_replace(this, pos, other);
		return;
	}

	String tmp_str = String_substr(this, 0, pos);
	String_copy(this, tmp_str);
	String_delete(tmp_str);

	String_append(this, other);
}

void String_replace_c_str(String this, size_t pos, const char* other)
{
	if (pos >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\npos out of range\n", __LINE__, __FILE__);
		exit(1);
	}

	for (size_t i = pos; (i < this->size) && (i - pos < strlen(other)); ++i)
	{
		String_set_at(this, i, other[i - pos]);
	}
}


void String_replace_append_c_str(String this, size_t pos, const char* other)
{
	if (pos >= this->size)
	{
		fprintf(stderr, "at line no. %d from file %s\npos out of range\n", __LINE__, __FILE__);
		exit(1);
	}

	if (pos + strlen(other) < this->size)
	{
		String_replace_c_str(this, pos, other);
		return;
	}

	String tmp_str = String_substr(this, 0, pos);
	String_copy(this, tmp_str);
	String_delete(tmp_str);

	String_append_c_str(this, other);
}