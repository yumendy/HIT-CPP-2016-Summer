// FileSetManagement.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "utils.h"
#include "Management.h"

using namespace std;

void printChoiceList()
{
	cout << "==============================" << endl;
	cout << "1. Create a fileset." << endl;
	cout << "2. Open a fileset." << endl;
	cout << "3. Add a file to fileset." << endl;
	cout << "4. Fetch a file from fileset." << endl;
	cout << "5. Remove a file from fileset." << endl;
	cout << "6. List files in the fileset." << endl;
	cout << "7. Close the fileset." << endl;
	cout << "==============================" << endl;
}


int main()
{
	int userChoice;
	char* filePath;
	filePath = new char[256];
	char* fileName;
	fileName = new char[256];
	Management management = Management();

	cout << "Welcome to use fileset management system!" << endl;

	while (true)
	{
		printChoiceList();
		cout << "Please input your choice." << endl;
		cin >> userChoice;

		switch (userChoice)
		{
		case 1:
			memset(filePath, 0, 256);
			cout << "input your filepath:";
			cin >> filePath;
			cout << "Please input the size of your fileset:";
			int filesetSize;
			cin >> filesetSize;
			management.createFileSet(filePath, filesetSize);
			break;
		case 2:
			memset(filePath, 0, 256);
			cout << "input your filepath:";
			cin >> filePath;
			management.openFileSet(filePath);
			break;
		case 3:
			memset(filePath, 0, 256);
			cout << "input the filepath of the file you want to add to the fileset:";
			cin >> filePath;
			management.addFileToFileSet(filePath);
			break;
		case 4:
			memset(fileName, 0, 256);
			memset(filePath, 0, 256);
			cout << "input the file name you want to fetch." << endl;
			cin >> fileName;
			cout << "input the filepath of the file you want to save:";
			cin >> filePath;
			management.fetchFileFromFileSet(fileName, filePath);
			break;
		case 5:
			memset(fileName, 0, 256);
			cout << "input the file name you want to remove." << endl;
			cin >> fileName;
			management.deleteFileFromFileSet(fileName);
			break;
		case 6:
			management.printFileListInFileSet();
			break;
		case 7:
			management.closeFileSet();
			break;
		default:
			cout << "unvalued choice!" << endl;
			break;
		}
	}
	

	//management.createFileSet("test.dat", 2);
	//management.openFileSet("test.dat");
	//management.addFileToFileSet("test1.txt");
	//management.addFileToFileSet("test2.txt");
	//management.deleteFileFromFileSet("test2.txt");
	//management.addFileToFileSet("test1.txt");
	//management.fetchFileFromFileSet("test1.txt", "test.txt");
	//management.printFileListInFileSet();
	//management.closeFileSet();
	
    return 0;
}

