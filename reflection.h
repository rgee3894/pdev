#pragma once
#ifndef REFLECTION_H
#define REFLECTION_H
#include <string>
#include <vector>
#include <assert.h>
#include <string.h>
enum MemberType
{
	Type_Bool,
	Type_Int,
	Type_Float,
	Type_Double
};

class MemberDesc
{
public:
	MemberDesc(const char* name, MemberType type, size_t elems, size_t offset) :
		m_name(name), m_type(type), m_elems(elems), m_offset(offset)
	{
	}

	const char* name() const { return m_name.c_str(); }
	MemberType type() const { return m_type; }
	size_t numElems() const { return m_elems; }
	size_t offset() const { return m_offset; }
	size_t elemSize() const
	{
		switch (m_type)
		{
		case Type_Bool:
			return sizeof(bool);
		case Type_Int:
			return sizeof(int);
		case Type_Float:
			return sizeof(float);
		case Type_Double:
			return sizeof(double);
		default:
			assert(false);
			return 0;
		}
	}
private:
	std::string m_name;
	MemberType m_type;
	size_t m_elems;
	size_t m_offset;
};

class TypeDesc
{
public:
	const char* name() const { return m_name.c_str(); }
	const std::vector<MemberDesc>& memberDescs() const { return m_memberDesc; }

protected:
	std::string m_name;
	std::vector<MemberDesc> m_memberDesc;
};

namespace reflectionUtil
{
    template<typename T>
	inline bool isCompatible(MemberType type) { return false; }

	template<>
	inline bool isCompatible<bool>(MemberType type) { return type == Type_Bool; }

	template<>
	inline bool isCompatible<int>(MemberType type) { return type == Type_Int; }

	template<>
	inline bool isCompatible<float>(MemberType type) { return type == Type_Float; }

	template<>
	inline bool isCompatible<double>(MemberType type) { return type == Type_Double; }
}

class IFileSystem;
class ReflectionObject
{
public:
	virtual ~ReflectionObject() {}

	virtual const TypeDesc& typeDescription() const = 0;

	bool Load(IFileSystem& fs, const char* filename);
	bool Save(IFileSystem& fs, const char* filename);

	template<typename T>
	bool getMemberValue(const char* varName, T& outValue) const
	{
		return getMemberArray(varName, &outValue, 1);
	}

	template<typename T>
	bool getMemberArray(const char* varName, T* outArray, size_t num) const
	{
		const TypeDesc& desc = typeDescription();
		for (const MemberDesc& memberDesc : desc.memberDescs())
		{
			if (!strcmp(memberDesc.name(), varName))
			{
				if (!reflectionUtil::isCompatible<T>(memberDesc.type()))
					return false;
				if (memberDesc.numElems() != num)
					return false;
				const T* src = reinterpret_cast<T*>((char*)this + memberDesc.offset());
				for (size_t i = 0; i < num; i++)
					outArray[i] = src[i];
				return true;
			}
		}
		return false;
	}

	template<typename T>
	bool setMemberValue(const char* varName, const T& value)
	{
		return setMemberArray(varName, &value, 1);
	}

	template<typename T>
	bool setMemberArray(const char* varName, const T* values, size_t num)
	{
		const TypeDesc& desc = typeDescription();
		for (const MemberDesc& memberDesc : desc.memberDescs())
		{
			if (!strcmp(memberDesc.name(), varName))
			{
				if (!reflectionUtil::isCompatible<T>(memberDesc.type()))
					return false;
				if (memberDesc.numElems() != num)
					return false;
				T* dest = reinterpret_cast<T*>((char*)this + memberDesc.offset());
				for (size_t i = 0; i < num; i++)
					dest[i] = values[i];
				return true;
			}
		}
		return false;
	}
};
#endif
