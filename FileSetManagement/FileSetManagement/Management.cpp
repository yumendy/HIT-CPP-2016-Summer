#include <string.h>
#include "Management.h"
#include "utils.h"

FileTag::FileTag()
{
}

FileTag::~FileTag()
{
}

char * FileTag::getFileName()
{
	return fileName;
}

void FileTag::setFileName(char* newFileName)
{
	strcpy_s(fileName, newFileName);
}

int FileTag::getFileID()
{
	return fileID;
}

void FileTag::setFileID(int newFileID)
{
	fileID = newFileID;
}

int FileTag::getFileSize()
{
	return fileSize;
}

void FileTag::setFileSize(int newFileSize)
{
	fileSize = newFileSize;
}

unsigned int FileTag::getFileOffset()
{
	return fileOffset;
}

void FileTag::setFileOffset(unsigned int newFileOffset)
{
	fileOffset = newFileOffset;
}

char * FileTag::getChecksum()
{
	return checksum;
}

void FileTag::setChecksum(char * newChecksum)
{
	strcpy_s(checksum, newChecksum);
}

char * FileTag::object2bytes()
{
	char result[288];
	memcpy(result, fileName, strlen(fileName) <= 256 ? strlen(fileName) : 256);
	memcpy(result + 256, int2bytes(fileID), 4);
	memcpy(result + 260, int2bytes(fileSize), 4);
	memcpy(result + 264, long2bytes(fileOffset), 8);
	memcpy(result + 272, checksum, 16);
	return result;
}

ChainListNode::ChainListNode()
{
}

ChainListNode::~ChainListNode()
{
}

ChainList::ChainList()
{
}

ChainList::~ChainList()
{
}