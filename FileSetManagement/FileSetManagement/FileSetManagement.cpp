// FileSetManagement.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "utils.h"
#include "Management.h"


int main()
{
	Management management = Management();

	//management.createFileSet("test.dat", 2);
	management.openFileSet("test.dat");
	//management.addFileToFileSet("test1.txt");
	//management.addFileToFileSet("test2.txt");
	//management.deleteFileFromFileSet("test2.txt");
	//management.addFileToFileSet("test1.txt");
	management.fetchFileFromFileSet("test1.txt", "test.txt");

	management.closeFileSet();
	
    return 0;
}

