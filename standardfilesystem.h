#pragma once
#ifndef STANDARD_FILESYSTEM_H
#define STANDARD_FILESYSTEM_H

#include "filesystem.h"
#include <string>

class StandardFileSystem : public IFileSystem
{
public:
	//! StandardFileSystem Constructor
	/*!
	\param rootDirectory The root directory of the filesystem
	*/
	StandardFileSystem(const char* rootDirectory);
	~StandardFileSystem();
	
	// Inherited via IFileSystem
	IFile * createFile(const char * filename) override;
	IFile * openFile(const char * filename) override;
	bool fileExists(const char * filename) override;
	bool deleteFile(const char* filename) override;
private:
	std::string m_rootDirectory;
	std::string convertFilePath(const char* path);
};

#endif