#pragma once

class FileTag
{
public:
	FileTag();
	~FileTag();

	char* getFileName();
	void setFileName(char* newFilrName);

	int getFileID();
	void setFileID(int newFileID);

	int getFileSize();
	void setFileSize(int newFileSize);

	unsigned int getFileOffset();
	void setFileOffset(unsigned int newFileOffset);

	char* getChecksum();
	void setChecksum(char* newChecksum);

	char* object2bytes();

private:
	char fileName[256];
	int fileID;
	int fileSize;
	unsigned int fileOffset;
	char checksum[16];
};


class ChainListNode
{
public:
	ChainListNode();
	~ChainListNode();

private:

};

class ChainList
{
public:
	ChainList();
	~ChainList();

private:

};
