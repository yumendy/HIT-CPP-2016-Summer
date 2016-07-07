#include "stdafx.h"
#include "Management.h"
#include "utils.h"
#include "sha1.h"

using namespace std;

FileTag::FileTag(char* bytes)
{
	char* temp;
	memcpy(fileName, bytes, 256);

	temp = new char[4];

	temp = strsub(bytes, 256, 4);
	fileFlag = bytes2int(temp);

	temp = strsub(bytes, 260, 4);
	fileSize = bytes2int(temp);

	delete temp;
	temp = new char[8];

	temp = strsub(bytes, 264, 8);
	fileOffset = bytes2long(temp);

	memcpy(checksum, bytes + 272, 8);
}

FileTag::FileTag()
{
	memcpy(fileName, 0, 256);
	fileFlag = 0; // 0 -> unused;
	fileSize = 0;
	fileOffset = 0;
	memcpy(checksum, 0, 16);
}

FileTag::FileTag(char * newFileName, int newFileFlag, int newFileSize, long newFileOffset, char * newChecksum)
{
	strcpy_s(fileName, newFileName);
	fileFlag = newFileFlag;
	fileSize = newFileSize;
	fileOffset = newFileOffset;
	strcpy_s(checksum, newChecksum);
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

int FileTag::getFileFlag()
{
	return fileFlag;
}

void FileTag::setFileFlag(int newFileFlag)
{
	fileFlag = newFileFlag;
}

int FileTag::getFileSize()
{
	return fileSize;
}

void FileTag::setFileSize(int newFileSize)
{
	fileSize = newFileSize;
}

long FileTag::getFileOffset()
{
	return fileOffset;
}

void FileTag::setFileOffset(long newFileOffset)
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
	memcpy(result + 256, int2bytes(fileFlag), 4);
	memcpy(result + 260, int2bytes(fileSize), 4);
	memcpy(result + 264, long2bytes(fileOffset), 8);
	memcpy(result + 272, checksum, 16);
	return result;
}

SetHeader::SetHeader(char * bytes)
{
	memcpy(setMark, bytes, 4);
	fileSize = bytes2long(strsub(bytes, 4, 8));
	maxFileNumber = bytes2int(strsub(bytes, 12, 4));
	memcpy(checksum, bytes + 16, 16);

	fileTagsList = new FileTag[maxFileNumber];
	for (int i = 0; i < maxFileNumber; i++)
	{
		fileTagsList[i] = FileTag(strsub(bytes, 32 + 288 * i, 288));
	}
}

SetHeader::SetHeader(int newFileNumber)
{
	memcpy(setMark, MARK, 4);
	fileSize = 32 + newFileNumber * 288;
	maxFileNumber = newFileNumber;
	memset(checksum, 0, 16);
	fileTagsList = new FileTag[10];
	//fileTagsList = new FileTag[newFileNumber];

	for (int i = 0; i < newFileNumber; i++)
	{
		fileTagsList[i] = FileTag();
	}

}

SetHeader::SetHeader()
{
}

SetHeader::~SetHeader()
{
	delete fileTagsList;
}

char * SetHeader::getSetMark()
{
	return setMark;
}

void SetHeader::setSetMark(char * newMark)
{
	strcpy_s(setMark, newMark);
}

long SetHeader::getFileSize()
{
	return fileSize;
}

void SetHeader::setFileSize(long newFileSize)
{
	fileSize = newFileSize;
}

int SetHeader::getMaxFileNumber()
{
	return maxFileNumber;
}

void SetHeader::setMaxFileNumber(int newMaxFileNumber)
{
	maxFileNumber = newMaxFileNumber;
}

char * SetHeader::getChecksum()
{
	return checksum;
}

void SetHeader::setChecksum(char * newChecksum)
{
	strcpy_s(checksum, newChecksum);
}

FileTag * SetHeader::getFileTagsList()
{
	return fileTagsList;
}

void SetHeader::setFileTagsList(FileTag * newFileTags)
{
	fileTagsList = newFileTags;
}

char * SetHeader::object2bytes()
{
	char* result = new char[32 + maxFileNumber * 288];
	
	memcpy(result, setMark, 4);
	memcpy(result + 4, long2bytes(fileSize), 8);
	memcpy(result + 12, int2bytes(maxFileNumber), 4);
	memcpy(result + 16, checksum, 16);

	for (int i = 0; i < maxFileNumber; i++)
	{
		memcpy(result + 32 + i * 288, fileTagsList[i].object2bytes(), 288);
	}
	return result;
}

FileSet::FileSet(FILE * filePoint)
{
	char* temp;
	fp = filePoint;

	temp = new char[8];

	fread_s(temp, 8, 8, 1, fp);

	if (!strcmp(temp, MARK))
	{
		fseek(fp, 12, SEEK_SET);
		delete temp;
		temp = new char[4];
		fread_s(temp, 4, 4, 1, fp);

		headerLength = 32 + bytes2int(temp) * 288;
		delete temp;

		temp = new char[headerLength];
		fseek(fp, 0, SEEK_SET);
		fread_s(temp, headerLength, headerLength, 1, fp);
		header = SetHeader(temp);
	}
	else
	{
		printf("%s\n", "Unvaild File!");
	}
	

}

FileSet::FileSet(FILE * filePoint, int fileNumber)
{
	fp = filePoint;
	headerLength = 32 + 288 * fileNumber;
	header = SetHeader(fileNumber);
	fwrite(header.object2bytes(), headerLength, 1, fp);
}

FileSet::FileSet()
{
}

FileSet::~FileSet()
{
}

SetHeader FileSet::getHeader()
{
	return header;
}

void FileSet::setHeader(SetHeader newHeader)
{
	header = newHeader;
}

FILE * FileSet::getFilePoint()
{
	return fp;
}

void FileSet::setFilePoint(FILE * newFilePoint)
{
	fp = newFilePoint;
}

int FileSet::getHeaderLength()
{
	return headerLength;
}

void FileSet::setHeaderLength(int newHeaderLength)
{
	headerLength = newHeaderLength;
}

bool FileSet::close()
{
	long hashStringLenght = header.getFileSize() - 32;
	char* hashChars = new char[hashStringLenght];

	fseek(fp, 32, SEEK_SET);
	fread(hashChars, hashStringLenght, 1, fp);

	SHA1* sha1 = new SHA1();
	sha1->addBytes(hashChars, hashStringLenght);
	unsigned char* digest = sha1->getDigest();

	char* digest_128bit = new char[16];
	memcpy(digest_128bit, digest, 16);

	header.setChecksum(digest_128bit);

	fseek(fp, 0, SEEK_SET);
	fwrite(header.object2bytes(), headerLength, 1, fp);

	fclose(fp);

	return true;
}

Management::Management()
{
	fileSet = NULL;
}

Management::~Management()
{
}

bool Management::createFileSet(char * filePath, int maxFileNumberOfNewSet)
{
	if (fileSet != NULL)
	{
		cout << "You have already opened a file. Please close it and try again." << endl;
		return false;
	}

	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, filePath, "wb");

	fileSet = new FileSet(fp, maxFileNumberOfNewSet);

	return true;
}

bool Management::closeFileSet()
{
	fileSet->close();
	return true;
}
