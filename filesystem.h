#pragma once
#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <stddef.h>
class IFile
{
public:
	virtual ~IFile() {}

	//! Returns the name of the file
	/*!
	\return A string pointing to the name of the file
	*/
	virtual const char* filename() const = 0;
	
	//! Returns the size of the file
	/*!
	\return The size of the file in bytes
	*/
	virtual size_t length() const = 0;

	//! Returns the current position of the file
	/*!
	\return The current position of the file in bytes
	*/
	virtual size_t pos() const = 0;

	//! Sets the file position
	/*!
	\param pos The position to seek to in bytes
	\return true if the seek succeeded, false otherwise
	*/
	virtual bool seek(size_t pos) = 0;

	//! Reads from the file at the current position
	/*!
	\param buffer The destination buffer
	\param size	  The size to read, the destination buffer must be able to hold the requested size
	\return true if the read succeeded, false otherwise
	*/
	virtual bool read(char* buffer, size_t size) = 0;

	//! Writes to the file at the current position
	/*!
	\param buffer The source buffer
	\param size	  The size to write, the source buffer must be larger or equal to the specified size
	\return true if the write succeeded, false otherwise
	*/
	virtual bool write(const char* buffer, size_t size) = 0;
};

class IFileSystem
{
public:
	//! Creats a new file
	/*!
	\param filename The name of the file to create
	\return An IFile object of the operation succeded, nullptr otherwise
	*/
	virtual IFile* createFile(const char* filename) = 0;

	//! Opens an existing file
	/*!
	\param filename The name of the file to read
	\return An IFile object of the operation succeded, nullptr otherwise
	*/
	virtual IFile* openFile(const char* filename) = 0;

	//! Checks whether a file exists
	/*!
	\param filename The name of the file to check
	\return true if the file exists, false otherwise
	*/
	virtual bool fileExists(const char* filename) = 0;

	//! Deletes a file
	/*!
	\param filename The name of the file to delete
	\return true if the file was deleted, false otherwise
	*/
	virtual bool deleteFile(const char* filename) = 0;
};

#endif
