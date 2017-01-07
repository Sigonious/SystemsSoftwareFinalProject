#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <complex>
#include <cmath>
#include <string>
#include <vector>
#include <time.h>

using namespace std;
/*
 * Project designed by Adam Murr and Justin Larkin
 */

class Folder
{
public:
string name;
int parentIndex, currentIndex;
time_t rawtime;

Folder(){}

Folder(string definedName, int thisIndex)
{
	name = "["+definedName+"]";
	currentIndex = thisIndex;
	time(&rawtime);
	//cout<<name<<" successfully created at: "<<ctime(&rawtime);
}

Folder(string definedName, int parentsIndex, int thisIndex)
{
	name = "["+definedName+"]";
	parentIndex = parentsIndex;
	currentIndex = thisIndex;
	time(&rawtime);
	cout<<name<<" successfully created at: "<<ctime(&rawtime);
}

void getDate()
{
	cout<<name<<" was created: "<<ctime(&rawtime);
}
};

vector<Folder> folderList;

class File
{
public:
string fileName;
string parentFolder;
int parentFolderIndex;
time_t rawtime;

File(){}

File(string definedFilename, int newIndex, string parentFolder1)
{
	fileName = definedFilename;
	parentFolderIndex = newIndex;
	parentFolder = parentFolder1;
	time(&rawtime);
	cout<<fileName<<" successfully created at: "<<ctime(&rawtime);
}

void getDate()
{
	cout<<fileName<<" was created: "<<ctime(&rawtime);
}
};

vector<File> fileList;

string checkFilenames(string input)
{
	int i;
	for(i = 0; i<fileList.size(); i++)
	{
		File temp = fileList.at(i);
		if(temp.fileName == input)
		{
			return temp.fileName;
		}
	}
	return "Could not find file!\n";
}

bool folderContains(string input)
{
	int i;
	for(i = 0; i<folderList.size(); i++)
	{
		Folder temp = folderList.at(i);
		if(input.compare(temp.name) == 0)
		{
			return true;
		}
	}
	return false;
}

int folderContainsIndex(string input)
{
	int i;
	for(i = 0; i<folderList.size(); i++)
	{
		Folder temp = folderList.at(i);
		if(input.compare(temp.name) == 0)
		{
			return temp.currentIndex;
		}
	}
	return -1;
}

bool fileContains(string input)
{
	int i;
	for(i = 0; i<fileList.size(); i++)
	{
		File temp = fileList.at(i);
		if(input.compare(temp.fileName) == 0)
		{
			return true;
		}
	}
	return false;
}

int fileContainsIndex(string input)
{
	int i;
	for(i = 0; i<fileList.size(); i++)
	{
		File temp = fileList.at(i);
		if(input.compare(temp.fileName) == 0)
		{
			return i;
		}
	}
	return -1;
}

int fileListHasIndex(int index)
{
	int i;
	for(i = 0; i<fileList.size(); i++)
	{
		File temp = fileList.at(i);
		if(temp.parentFolderIndex == index)
		{
			return -1;
		}
	}
	return 0;
}

void printFilenames(int findIndex)
{
	int i;

	for(i = 0; i<folderList.size(); i++)
	{
		Folder temp = folderList.at(i);
		if(temp.parentIndex == findIndex)
		{
			cout<<temp.name<<" ";
		}
	}

	for(i = 0; i<fileList.size(); i++)
	{
		File temp = fileList.at(i);
		if(temp.parentFolderIndex == findIndex)
		{
			cout<<temp.fileName<<" ";
		}
	}
	cout<<"\n";
}

void inputHandler()
{
	int currentIndex = 0;
	string userInput = "", currentDirectory = "MyFileSys", tempString = "";
	cout<<"Linux_Shell_prompt><MyFileSys>\n";
	cout<<"Welcome to GSU shell file system!\n";
	Folder FileSystem(currentDirectory, 0);
	folderList.push_back(FileSystem);
	while(true)
	{
		cout<<"My_shell_prompt>";
		getline(cin, userInput);
		if(userInput.substr(0,5) == "mkdir" && userInput.substr(5,1) == " ")
		{
			string tempDir = "["+userInput.substr(6, userInput.length())+"]";
			if(folderContains(tempDir) == true)
			{
				cout<<"Directory already exists!\n";
				continue;
			}
			Folder temp(userInput.substr(6, userInput.length()), currentIndex,folderList.size());
			folderList.push_back(temp);
		}
		else if(userInput.substr (0,2) == "cd" && userInput.substr(2,1) == " ")
		{
			string checkForFolder = "["+userInput.substr(3, userInput.length())+"]";
			if(folderContainsIndex(checkForFolder) != -1)
			{
				currentDirectory = checkForFolder;
				cout<<"Directory changed to: "<<currentDirectory<<"\n";
               	currentIndex = folderContainsIndex(checkForFolder);
			}
			else if(userInput.substr (3, userInput.length()) == "..")
			{
				if(folderContainsIndex(currentDirectory) != 0 && folderContainsIndex(currentDirectory) != -1)
				{
					Folder tempFold = folderList.at(folderContainsIndex(currentDirectory));
					Folder tempParent = folderList.at(tempFold.parentIndex);
					currentDirectory = tempParent.name;
					cout<<"Directory changed to: "<<currentDirectory<<"\n";
					currentIndex = tempParent.currentIndex;
				}
				else if(folderContainsIndex(checkForFolder) == 0)
				{
					cout<<"Already at root directory.\n";
				}
				else
				{
					cout<<"Can not go up a directory for unknown reason.\n";
				}
			}
			else
			{
           		cout<<"Directory does not exist!\n";
			}
		}
		else if(userInput.substr (0, 6) == "mkfile" && userInput.substr(6,1) == " ")
		{
			if(fileContains(userInput.substr(7, userInput.length())) == true)
			{
				cout<<"File: "<<userInput.substr(7, userInput.length())<<" already exists!\n";
				continue;
			}
			File temp(userInput.substr(7, userInput.length()), currentIndex, currentDirectory);
			fileList.push_back(temp);
		}
		else if(userInput.substr (0, 6) == "rmfile" && userInput.substr(6,1) == " ")
		{
			if(fileContainsIndex(userInput.substr(7, userInput.length())) != -1)
			{
				int temp = fileContainsIndex(userInput.substr(7, userInput.length()));
				fileList.erase(fileList.begin() + temp);
				cout<<"File "<<userInput.substr(7, userInput.length())<<" removed successfully!\n";
			}
			else
			{
				cout<<"Error, file does not exist!\n";
			}
		}
		else if(userInput.substr (0, 4) == "mkfs")
		{
			currentIndex = 0;
			userInput = "";
			currentDirectory = "MyFileSys";
			tempString = "";
			folderList.clear();
			fileList.clear();
			folderList.push_back(FileSystem);
			cout<<"OK, file system formatted and ready!\n";
		}
		else if(userInput.substr (0, 5) == "rmdir" && userInput.substr(5,1) == " ")
		{
			string tempDir = "["+userInput.substr(6,userInput.length())+"]";
			if(folderContainsIndex(tempDir) != -1)
			{
				Folder tempFolder = folderList.at(folderContainsIndex(tempDir));
				if(fileListHasIndex(tempFolder.currentIndex))
				{
					cout<<"Directory is not empty and can not be removed!\n";
					continue;
				}
				int temp = folderContainsIndex(tempDir);
				folderList.erase(folderList.begin() + temp);
				cout<<"You have removed directory "<<tempDir<<"!\n";
			}
			else
			{
				cout<<"Directory not found!\n";
			}
		}
		else if(userInput.substr (0,4) == "stat" && userInput.substr(4,1) == " ")
		{
			string checkForFolder = "["+userInput.substr(5, userInput.length())+"]";
			if(folderContainsIndex(checkForFolder) != -1)
			{
				int tempIndex = folderContainsIndex(checkForFolder);
				Folder tempFolder = folderList.at(tempIndex);
				tempFolder.getDate();
			}
			else if(fileContainsIndex(userInput.substr(5, userInput.length())) != -1)
			{
				int tempIndex = fileContainsIndex(userInput.substr(5, userInput.length()));
				File tempFile = fileList.at(tempIndex);
				tempFile.getDate();
			}
			else
			{
				cout<<userInput.substr(5, userInput.length())<<" not found!\n";
			}
		}
		else if (userInput.substr (0, 2) == "ls")
		{
			printFilenames(currentIndex);
		}
		else if(userInput.substr (0,4) == "exit")
		{
			break;
		}
		else
		{
			cout<<"Incorrect input, please try again!\n";
		}
	}
	cout<<"Goodbye!\n";
	exit(0);
}

int main(int argc, char *argv[])
{
	inputHandler();
    return 0;
}
