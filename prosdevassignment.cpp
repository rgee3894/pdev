
#include <iostream>
#include "standardfilesystem.h"
#include "reflection.h"
#include "sampleobject.h"

void fileSystemExample()
{
	StandardFileSystem* fs = new StandardFileSystem("D:/test");

	// If newfile.txt exists, delete it
	if (fs->fileExists("newfile.txt"))
	{
		fs->deleteFile("newfile.txt");
	}

	// Create newfile.txt
	const char* writeBuffer = "Hello World";
	IFile* createdFile = fs->createFile("newfile.txt");
	if (!createdFile)
	{
		std::cout << "Failed to create a file" << std::endl;
		return;
	}

	if (!createdFile->write(writeBuffer, strlen(writeBuffer) + 1 /* add 1 to length for null terminator */))
	{
		std::cout << "Failed to write to file" << std::endl;
	}
	// Close the file
	delete createdFile;
	std::cout << "Successfully created file" << std::endl;

	// Read newfile.txt
	IFile* openedFile = fs->openFile("newfile.txt");
	if (!openedFile)
	{
		std::cout << "Failed to open file" << std::endl;
	}
	size_t size = openedFile->length();
	char* destBuffer = new char[size];
	if (!openedFile->read(destBuffer, size))
	{
		std::cout << "Failed to read from file!" << std::endl;
	}
	else
	{
		std::cout << "File Contents:" << std::endl;
		std::cout << destBuffer << std::endl;
	}
	// Close the file
	delete openedFile;
}

void showVars(const SampleObject& obj)
{
	const TypeDesc& desc = obj.typeDescription();
	std::cout << desc.name() << " Contents" << std::endl;
	int intVar;
	bool boolVar;
	float floatVar;
	double doubleVar;
	int intArray[12];

	if (obj.getMemberValue("MyIntVariable", intVar))
		std::cout << "  MyIntVariable = " << intVar << std::endl;
	else
		std::cout << "  Cannot read MyIntVariable" << std::endl;

	if (obj.getMemberValue("MyBoolVariable", boolVar))
		std::cout << "  MyBoolVariable = " << (boolVar ? "true" : "false") << std::endl;
	else
		std::cout << "  Cannot read MyBoolVariable" << std::endl;

	if (obj.getMemberValue("MyFloatVariable", floatVar))
		std::cout << "  MyFloatVariable = " << floatVar << std::endl;
	else
		std::cout << "  Cannot read MyFloatVariable" << std::endl;

	if (obj.getMemberValue("MyDoubleVariable", doubleVar))
		std::cout << "  MyDoubleVariable = " << doubleVar << std::endl;
	else
		std::cout << "  Cannot read MyDoubleVariable" << std::endl;
	
	if (obj.getMemberArray("MyArray", intArray, 12))
	{
		std::cout << "  MyArray = {";
		for (int i = 0; i < 12; i++)
		{
			std::cout << " " << intArray[i] << " ";
		}
		std::cout << "}" << std::endl;
	}
	else
		std::cout << "  Cannot read MyArray" << std::endl;

}

void reflectionExample()
{
	SampleObject obj;
	const TypeDesc& typeDesc = SampleObject::staticTypeDescription();
	// List member variables of SampleObject
	std::cout << "Member Variables of " << typeDesc.name() << std::endl;
	for (const MemberDesc& desc : typeDesc.memberDescs())
	{
		std::cout << "  " << desc.name() << std::endl;
	}
	std::cout << std::endl;
	// Set variables directly
	obj.m_MyIntVariable = 0;
	obj.m_MyBoolVariable = true;
	obj.m_MyFloatVariable = 2.0f;
	obj.m_MyDoubleVariable = 3.0;
	for (int i = 0; i < 12; i++)
		obj.m_MyArray[i] = i;
	
	// Print out variables
	showVars(obj);

	std::cout << std::endl;

	// Set variables using accessors
	obj.setMemberValue("MyIntVariable", 12);
	obj.setMemberValue("MyBoolVariable", false);
	obj.setMemberValue("MyFloatVariable", 23.0f);
	obj.setMemberValue("MyDoubleVariable", 10.0);

	int tempArray[12];
	for (int i = 0; i < 12; i++)
		tempArray[i] = 11 - i;
	obj.setMemberArray("MyArray", tempArray, 12);
	// Print out variables
	showVars(obj);
	std::cout << std::endl;

	// Test Save
	std::cout << "Testing Save..." << std::endl;
	//StandardFileSystem fs("D:/test");
	//obj.Save(fs, "sampleObj.dat");

	// Test Load
	std::cout << "Testing Load..." << std::endl;
	SampleObject newObj;
	//newObj.Load(fs, "sampleObj.dat");
	showVars(newObj);
}

int main()
{
	//fileSystemExample();
	reflectionExample();
    return 0;
}

