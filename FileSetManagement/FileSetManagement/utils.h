#pragma once

char* int2bytes(int number);
int bytes2int(char *bytes);

char* long2bytes(__int64 number);
__int64 bytes2long(char *bytes);

char* strsub(char* source, int startIndex, int length);

int strcmpn(char* string1, char* string2, int length);