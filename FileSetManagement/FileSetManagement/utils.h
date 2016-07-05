#pragma once

char* int2bytes(int number);
int bytes2int(char *bytes);

char* long2bytes(long number);
long bytes2long(char *bytes);

char* strsub(char* source, int startIndex, int length);