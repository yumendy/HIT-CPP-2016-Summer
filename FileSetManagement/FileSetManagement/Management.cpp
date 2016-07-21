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
	memset(fileName, 0, 256);
	fileFlag = 0; // 0 -> unused;
	fileSize = 0;
	fileOffset = 0;
	memset(checksum, 0, 16);
}

FileTag::FileTag(char * newFileName, int newFileFlag, int newFileSize, __int64 newFileOffset, char * newChecksum)
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
	memset(fileName, 0, 256);
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

__int64 FileTag::getFileOffset()
{
	return fileOffset;
}

void FileTag::setFileOffset(__int64 newFileOffset)
{
	fileOffset = newFileOffset;
}

char * FileTag::getChecksum()
{
	return checksum;
}

void FileTag::setChecksum(char * newChecksum)
{
	memcpy(checksum, newChecksum, 16);
}

char * FileTag::object2bytes()
{
	char result[288];
	memset(result, 0, 288);
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

	fileTagsList = new FileTag*[maxFileNumber];
	for (int i = 0; i < maxFileNumber; i++)
	{
		fileTagsList[i] = new FileTag(strsub(bytes, 32 + 288 * i, 288));
	}
}

SetHeader::SetHeader(int newFileNumber)
{
	memcpy(setMark, MARK, 4);
	fileSize = 32 + newFileNumber * 288;
	maxFileNumber = newFileNumber;
	memset(checksum, 0, 16);
	fileTagsList = new FileTag*[newFileNumber];

	for (int i = 0; i < newFileNumber; i++)
	{
		fileTagsList[i] = new FileTag();
	}

}

SetHeader::SetHeader()
{
}

SetHeader::~SetHeader()
{
	delete[] fileTagsList;
}

char * SetHeader::getSetMark()
{
	return setMark;
}

void SetHeader::setSetMark(char * newMark)
{
	memcpy(setMark, newMark, 4);
}

__int64 SetHeader::getFileSize()
{
	return fileSize;
}

void SetHeader::setFileSize(__int64 newFileSize)
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
	memcpy(checksum, newChecksum, 16);
}

FileTag ** SetHeader::getFileTagsList()
{
	return fileTagsList;
}

void SetHeader::setFileTagsList(FileTag ** newFileTags)
{
	fileTagsList = newFileTags;
}

char * SetHeader::object2bytes()
{
	char* result = new char[32 + maxFileNumber * 288];
	memset(result, 0, 32 + maxFileNumber * 288);
	
	memcpy(result, setMark, 4);
	memcpy(result + 4, long2bytes(fileSize), 8);
	memcpy(result + 12, int2bytes(maxFileNumber), 4);
	memcpy(result + 16, checksum, 16);

	for (int i = 0; i < maxFileNumber; i++)
	{
		memcpy(result + 32 + i * 288, fileTagsList[i]->object2bytes(), 288);
	}
	return result;
}

FileSet::FileSet(FILE * filePoint)
{
	char* temp;
	fp = filePoint;

	temp = new char[4];

	fread_s(temp, 4, 4, 1, fp);

	if (!strcmpn(temp, MARK, 4))
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
		header = new SetHeader(temp);
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
	header = new SetHeader(fileNumber);
	fwrite(header->object2bytes(), headerLength, 1, fp);
}

FileSet::FileSet()
{
}

FileSet::~FileSet()
{
}

SetHeader* FileSet::getHeader()
{
	return header;
}

void FileSet::setHeader(SetHeader* newHeader)
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

bool FileSet::addFile(char * fileName, int fileLength, char * data, char* checksum)
{
	bool savedFlag = false;
	for (int i = 0; i < header->getMaxFileNumber(); i++)
	{
		FileTag* temp = header->getFileTagsList()[i];
		if (temp->getFileFlag() == 0) // empty space
		{
			temp->setFileName(fileName);
			temp->setFileSize(fileLength);
			temp->setChecksum(checksum);
			temp->setFileOffset(ftell(fp));
			temp->setFileFlag(1);
			fwrite(data, fileLength, 1, fp);
			header->setFileSize(header->getFileSize() + fileLength);
			savedFlag = true;
			break;
		}
		else if (temp->getFileFlag() == 2) // the file has been deleted
		{
			if (temp->getFileSize() >= fileLength)
			{
				temp->setFileName(fileName);
				temp->setFileSize(fileLength);
				temp->setChecksum(checksum);
				temp->setFileFlag(1);
				_fseeki64(fp, temp->getFileOffset(), SEEK_SET);
				fwrite(data, fileLength, 1, fp);
				fseek(fp, 0, SEEK_END);
				savedFlag = true;
				break;
			}
		}
	}
	if (!savedFlag)
	{
		cout << "this file set is full!" << endl;
		return false;
	}
	return true;
}

bool FileSet::removeFile(char * fileName)
{
	for (int i = 0; i < header->getMaxFileNumber(); i++)
	{
		FileTag* temp = header->getFileTagsList()[i];
		if (!strcmpn(temp->getFileName(), fileName, strlen(fileName)))
		{
			temp->setFileFlag(2);
			break;
		}
	}
	return true;
}

char* FileSet::fetchFile(char * fileName)
{
	char* result;
	for (int i = 0; i < header->getMaxFileNumber(); i++)
	{
		FileTag* temp = header->getFileTagsList()[i];
		if (!strcmpn(temp->getFileName(), fileName, strlen(fileName)))
		{
			result = new char[temp->getFileSize() + 1];
			memset(result, 0, temp->getFileSize() + 1);
			_fseeki64(fp, temp->getFileOffset(), SEEK_SET);
			fread_s(result, temp->getFileSize(), temp->getFileSize(), 1, fp);
			_fseeki64(fp, 0, SEEK_END);
			return result;
		}
	}
	return NULL;
}

bool FileSet::printFileList()
{
	FileTag* (*tagList) = getHeader()->getFileTagsList();
	FileTag* temp;

	cout << "File name" << "\t" << "File size(bytes)" << "\t" << "File offset(bytes)" << endl;
	for (int i = 0; i < getHeader()->getMaxFileNumber(); i++)
	{
		temp = tagList[i];
		if (temp->getFileFlag() == 1)
		{
			cout << temp->getFileName() << "\t" << temp->getFileSize() << "\t" << temp->getFileOffset() << endl;
		}

	}
	return true;
}

bool FileSet::close()
{
	__int64 hashStringLenght = header->getFileSize() - 32;
	char* hashChars = new char[hashStringLenght];

	fseek(fp, 32, SEEK_SET);
	fread(hashChars, hashStringLenght, 1, fp);

	SHA1* sha1 = new SHA1();
	sha1->addBytes(hashChars, hashStringLenght);
	unsigned char* digest = sha1->getDigest();

	char* digest_128bit = new char[16];
	memcpy(digest_128bit, digest, 16);

	header->setChecksum(digest_128bit);

	fseek(fp, 0, SEEK_SET);
	fwrite(header->object2bytes(), headerLength, 1, fp);

	fclose(fp);
	delete sha1;

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

bool Management::openFileSet(char * filePath)
{

	if (fileSet != NULL)
	{
		cout << "You have already opened a file. Please close it and try again." << endl;
		return false;
	}

	errno_t err;
	FILE *fp;
	err = fopen_s(&fp, filePath, "rb+");
	fileSet = new FileSet(fp);

	return true;
}

bool Management::addFileToFileSet(char * filePath)
{
	char* buffer;

	errno_t err;
	FILE* fp;
	err = fopen_s(&fp, filePath, "rb");

	// get file length
	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(fp);

	//read file to buffer
	buffer = new char[fileLength];

	fseek(fp, 0, SEEK_SET);

	fread(buffer, fileLength, 1, fp);
	fclose(fp);


	// get file name
	char* fileNamePos = strrchr(filePath, '\\');
	char* fileName = new char[256];
	memset(fileName, 0, 256);

	// get checksum
	SHA1* sha1 = new SHA1();
	sha1->addBytes(buffer, fileLength);
	unsigned char* digist = sha1->getDigest();
	char* checksum = new char[16];
	memcpy(checksum, digist, 16);
	delete sha1;

	if (fileNamePos == NULL)
	{
		memcpy(fileName, filePath, strlen(filePath));
	}
	else
	{
		strcpy_s(fileName,256, fileNamePos + 1);
	}


	// add a file to set;
	fileSet->addFile(fileName, fileLength, buffer, checksum);

	return false;
}

bool Management::deleteFileFromFileSet(char * fileName)
{
	fileSet->removeFile(fileName);
	return false;
}

bool Management::fetchFileFromFileSet(char * fileName, char* newPathAndName)
{
	char* data = fileSet->fetchFile(fileName);
	if (data == NULL)
	{
		cout << "Not found the file!" << endl;
		return false;
	}
	else
	{
		errno_t err;
		FILE *fp;
		err = fopen_s(&fp, newPathAndName, "wb");
		fwrite(data, strlen(data), 1, fp);
		fclose(fp);
		return true;
	}
	
}

bool Management::printFileListInFileSet()
{
	fileSet->printFileList();
	return true;
}

bool Management::closeFileSet()
{
	fileSet->close();
	return true;
}
