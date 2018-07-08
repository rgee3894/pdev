
// BEGIN: Generated from SampleObject.definition
#include "sampleobject.h"

SampleObject::SampleObjectTypeDesc SampleObject::s_Desc;

SampleObject::SampleObjectTypeDesc::SampleObjectTypeDesc() 
{
	m_name = "Employee";
	m_memberDesc.push_back(MemberDesc("EmpId", Type_Int, 1, offsetof(SampleObject, intvar)));
	m_memberDesc.push_back(MemberDesc("IsHired", Type_Bool, 1, offsetof(SampleObject, boolvar)));
	m_memberDesc.push_back(MemberDesc("GPA", Type_Float, 1, offsetof(SampleObject, floatvar)));
	m_memberDesc.push_back(MemberDesc("Salary", Type_Double, 1, offsetof(SampleObject, doublevar)));
	m_memberDesc.push_back(MemberDesc("ContactNos", Type_Int, 12, offsetof(SampleObject, arrayvar)));
}

SampleObject::SampleObject() : intvar(0), boolvar(false), floatvar(0.0f), doublevar(0.0), arrayvar{0}
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
