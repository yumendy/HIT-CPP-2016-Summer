#pragma once

#define MARK "HIT-"

class FileTag
{
public:
	FileTag(char* newFileName, int newFileID, int newFileSize, long newFileOffset, char* newChecksum);
	FileTag(char* bytes);
	FileTag();
	~FileTag();

	char* getFileName();
	void setFileName(char* newFileName);

	int getFileFlag();
	void setFileFlag(int newFileFlag);

	int getFileSize();
	void setFileSize(int newFileSize);

	__int64 getFileOffset();
	void setFileOffset(__int64 newFileOffset);

	char* getChecksum();
	void setChecksum(char* newChecksum);

	char* object2bytes();

private:
	char fileName[256];
	int fileFlag;
	int fileSize;
	__int64 fileOffset;
	char checksum[16];
};


class SetHeader
{
public:
	SetHeader(char* bytes);
	SetHeader(int newFileNumber);
	SetHeader();
	~SetHeader();

	char* getSetMark();
	void setSetMark(char* newMark);

	__int64 getFileSize();
	void setFileSize(__int64 newFileSize);

	int getMaxFileNumber();
	void setMaxFileNumber(int newSetMaxNumber);

	char* getChecksum();
	void setChecksum(char* newChecksum);

	FileTag** getFileTagsList();
	void setFileTagsList(FileTag** newFileTags);

	char* object2bytes();

private:
	char setMark[4];
	__int64 fileSize;
	int maxFileNumber;
	char checksum[16];
	FileTag *(*fileTagsList);
};

class FileSet
{
public:
	FileSet(FILE* filePoint);
	FileSet(FILE* filePoint, int fileNumber);
	FileSet();
	~FileSet();

	SetHeader* getHeader();
	void setHeader(SetHeader * newHeader);

	FILE* getFilePoint();
	void setFilePoint(FILE* newFilePoint);

	int getHeaderLength();
	void setHeaderLength(int newHeaderLength);

	bool addFile(char* fileName, int fileLength, char* data, char* checksum);
	bool removeFile(char* fileName);
	char* fetchFile(char* fileName);
	bool close();

private:
	SetHeader * header;
	int headerLength;
	FILE* fp;
};

class Management
{
public:
	Management();
	~Management();

	bool createFileSet(char* filePath, int maxFileNumberOfNewSet);
	bool openFileSet(char* filePath);
	bool addFileToFileSet(char* filePath);
	bool deleteFileFromFileSet(char* fileName);
	bool fetchFileFromFileSet(char* fileName, char* newPathAndName);
	bool closeFileSet();

private:
	FileSet* fileSet;

};