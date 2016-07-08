#include "stdafx.h"
#include "utils.h"

char* int2bytes(int number)
{
	char result[4];
	for (int i = 0; i < 4; i++)
	{
		result[i] = (number >> (i * 8)) & 0x000000ff;
	}
	return result;
}

int bytes2int(char* bytes)
{
	int sum = 0;
	for (int i = 0; i < 4; i++)
	{
		sum += (bytes[i] << (i * 8));
	}
	return sum;
}

char* long2bytes(__int64 number)
{
	char result[8];
	for (int i = 0; i < 8; i++)
	{
		result[i] = (number >> (i * 8)) & 0x00000000000000ff;
	}
	return result;
}

__int64 bytes2long(char* bytes)
{
	__int64 sum = 0;
	for (int i = 0; i < 8; i++)
	{
		sum += (bytes[i] << (i * 8));
	}
	return sum;
}

char * strsub(char * source, int startIndex, int length)
{
	char* result = new char[length];
	memcpy(result, source + startIndex, length);
	return result;
}

int strcmpn(char * string1, char * string2, int length)
{
	char* str_a = new char[length + 1];
	char* str_b = new char[length + 1];

	memset(str_a, 0, length + 1);
	memset(str_b, 0, length + 1);

	memcpy(str_a, string1, length);
	memcpy(str_b, string2, length);

	return strcmp(str_a, str_b);
}
