#pragma once
#ifndef SAMPLEOBJECT_H
#define SAMPLEOBJECT_H
#include "reflection.h"
#include "standardfilesystem.h"

/*
	SampleObject.definition
	-----------------------


	TYPE SampleObject
	{
		PUBLIC INT MyIntVariable = 0;
		PUBLIC BOOL MyBoolVariable = FALSE;
		PUBLIC FLOAT MyFloatVariable = 0.0;
		PUBLIC DOUBLE MyDoubleVariable = 0.0;
		PUBLIC INT[12] MyArray = {0};
	}

*/

struct Employee
{
	public: int EmpId = 0;
	public: bool IsHired = false;
	public: float GPA = 0.0;
	public: double Salary = 0.0;
	public: int ContactNos [12] = { 0 };
};

class SampleObject : public ReflectionObject
{
// BEGIN: Generated from SampleObject.definition
public:
	SampleObject();
	~SampleObject();

	int intvar;
	bool boolvar;
	float floatvar;
	double doublevar;
	int arrayvar[12];

	const TypeDesc& typeDescription() const override { return s_Desc; }
	static const TypeDesc& staticTypeDescription() { return s_Desc; }
private:

	class SampleObjectTypeDesc : public TypeDesc
	{
	public:
		SampleObjectTypeDesc();
	};
	static SampleObjectTypeDesc s_Desc;
// END: Generate4d from SampleObject.definition

// BEGIN: Custom Code
public:

	void myCustomFunction();
	void Load(StandardFileSystem fs, const char* fileName);
	void Save(StandardFileSystem fs, const char* fileName);
// END: Custom Code

};
#endif
