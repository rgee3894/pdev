
// BEGIN: Generated from SampleObject.definition
#include "sampleobject.h"
#include "standardfilesystem.h"

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

void SampleObject::Load(StandardFileSystem* fs, char * fileName)
{
    IFile* openedFile = fs->openFile(fileName);
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
}
