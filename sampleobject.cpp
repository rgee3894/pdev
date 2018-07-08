
// BEGIN: Generated from SampleObject.definition
#include "sampleobject.h"
#include "standardfilesystem.h"
#include "string.h"

SampleObject::SampleObjectTypeDesc SampleObject::s_Desc;

SampleObject::SampleObjectTypeDesc::SampleObjectTypeDesc()
{
	m_name = "SampleObject";
	m_memberDesc.push_back(MemberDesc("MyIntVariable", Type_Int, 1, offsetof(SampleObject, m_MyIntVariable)));
	m_memberDesc.push_back(MemberDesc("MyBoolVariable", Type_Bool, 1, offsetof(SampleObject, m_MyBoolVariable)));
	m_memberDesc.push_back(MemberDesc("MyFloatVariable", Type_Float, 1, offsetof(SampleObject, m_MyFloatVariable)));
	m_memberDesc.push_back(MemberDesc("MyDoubleVariable", Type_Double, 1, offsetof(SampleObject, m_MyDoubleVariable)));
	m_memberDesc.push_back(MemberDesc("MyArray", Type_Int, 12, offsetof(SampleObject, m_MyArray)));
}

SampleObject::SampleObject() : m_MyIntVariable(0), m_MyBoolVariable(false), m_MyFloatVariable(0.0f), m_MyDoubleVariable(0.0), m_MyArray{0}
{
}

SampleObject::~SampleObject()
{

}
// END: Generated from SampleObject.definition

// BEGIN: Custom Code
#include <iostream>
void SampleObject::myCustomFunction()
{
	std::cout << "Custom Function!" << std::endl;
}
// END: Custom Code

void SampleObject::Load(StandardFileSystem fs, const char* fileName)
{
    IFile* openedFile = fs.openFile(fileName);
    if (!openedFile)
    {
		std::cout << "Failed to open file" << std::endl;
		return;
	}

	int intVar;
	bool boolVar;
	float floatVar;
	double doubleVar;
	int intArray[12];

	size_t size = openedFile->length();
	char* destBuffer = new char[size];
	char * ptr;

    //Reading file contents into buffer
	if (!openedFile->read(destBuffer, size))
	{
		std::cout << "Failed to read from file!" << std::endl;
		return;
	}
    const char delim[4] = "\n";
    const char intArrDelim[4] = ", ";

    /*Reading from buffer */

    //Reading int variable
    ptr = strtok(destBuffer,delim);
    intVar = std::stoi(ptr);

    //Reading bool variable
    ptr=strtok(NULL,delim);
    char * boolString = ptr;

    //Reading float variable
    ptr=strtok(NULL,delim);
    floatVar = std::stof(ptr);

    //Reading double variable
    ptr=strtok(NULL,delim);
    doubleVar = std::stod(ptr);

    //Reading int array
    ptr=strtok(NULL,delim);
    char* intArr = ptr;

    ptr = strtok(intArr, intArrDelim);
    for(int i = 0; i < sizeof(intArray)/sizeof(int); i++)
    {
        intArray[i] = std::stoi(ptr);
        ptr = strtok(NULL, intArrDelim);
    }
    //Setting the member values
    this->setMemberValue("MyIntVariable", intVar);
	this->setMemberValue("MyBoolVariable", boolVar);
	this->setMemberValue("MyFloatVariable", floatVar);
	this->setMemberValue("MyDoubleVariable", doubleVar);
	this->setMemberArray("MyArray", intArray, 12);
}

void SampleObject::Save(StandardFileSystem fs, const char* fileName)
{
	// If fileName exists, delete it
	if (fs.fileExists(fileName))
	{
		fs.deleteFile(fileName);
	}

	// Create fileName
	IFile* createdFile = fs.createFile(fileName);
	if (!createdFile)
	{
		std::cout << "Failed to create a file" << std::endl;
		return;
	}

	int intVar;
	bool boolVar;
	float floatVar;
	double doubleVar;
	int intArray[12];

	this->getMemberValue("MyIntVariable", intVar);
	this->getMemberValue("MyBoolVariable", boolVar);
	this->getMemberValue("MyFloatVariable", floatVar);
	this->getMemberValue("MyDoubleVariable", doubleVar);
	this->getMemberArray("MyArray", intArray, 12);

	std::string objectValues = "";
	objectValues += std::to_string(intVar); objectValues += "\n";
	objectValues += boolVar ? "true" : "false"; objectValues += "\n";
	objectValues += std::to_string(floatVar); objectValues += "\n";
	objectValues += std::to_string(doubleVar); objectValues += "\n";

	for (int i = 0; i < 12; i++)
	{
		objectValues += std::to_string(intArray[i]);

		if (i != 11)
		{
			objectValues += ", ";
		}
	}

	const char* writeBuffer = objectValues.c_str();

	if (!createdFile->write(writeBuffer, strlen(writeBuffer) + 1 /* add 1 to length for null terminator */))
	{
		std::cout << "Failed to write to file" << std::endl;
	}
	// Close the file
	delete createdFile;
	std::cout << "Successfully saved file as " << fileName << std::endl;
}
// END: Custom Code
