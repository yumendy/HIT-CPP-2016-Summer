#pragma once

#define MARK "HIT--CPP"

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

	long getFileOffset();
	void setFileOffset(long newFileOffset);

	char* getChecksum();
	void setChecksum(char* newChecksum);

	char* object2bytes();

private:
	char fileName[256];
	int fileFlag;
	int fileSize;
	long fileOffset;
	char checksum[16];
};


class SetHeader
{
public:
	SetHeader(char* bytes);
	SetHeader();
	~SetHeader();

	char* getSetMark();
	void setSetMark(char* newMark);

	int getFileSize();
	void setFileSize(int newFileSize);

	int getMaxFileNumber();
	void setMaxFileNumber(int newSetMaxNumber);

	char* getChecksum();
	void setChecksum(char* newChecksum);

	FileTag* getFileTagsList();
	void setFileTagsList(FileTag* newFileTags);

	char* object2bytes();

private:
	char setMark[8];
	int fileSize;
	int maxFileNumber;
	char checksum[16];
	FileTag* fileTagsList;
};

class FileSet
{
public:
	FileSet(FILE* filePoint);
	FileSet();
	~FileSet();

	SetHeader getHeader();
	void setHeader(SetHeader newHeader);

	FILE* getFilePoint();
	void setFilePoint(FILE* newFilePoint);

	int getHeaderLength();
	void setHeaderLength(int newHeaderLength);

private:
	SetHeader header;
	int headerLength;
	FILE* fp;
};
