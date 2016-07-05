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
	if (strlen(bytes) != 4)
	{
		return -1;
	}
	else
	{
		int sum = 0;
		for (int i = 0; i < 4; i++)
		{
			sum += (bytes[i] << (i * 8));
		}
		return sum;
	}
}

char* long2bytes(long number)
{
	char result[8];
	for (int i = 0; i < 8; i++)
	{
		result[i] = (number >> (i * 8)) & 0x000000ff;
	}
	return result;
}

long bytes2long(char* bytes)
{
	if (strlen(bytes) != 8)
	{
		return -1;
	}
	else
	{
		int sum = 0;
		for (int i = 0; i < 8; i++)
		{
			sum += (bytes[i] << (i * 8));
		}
		return sum;
	}
}

char * strsub(char * source, int startIndex, int length)
{
	char* result = new char[length];
	memcpy(result, source + startIndex, length);
	return result;
}
