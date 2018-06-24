#pragma once
#ifndef ARCHIVE_H
#define ARCHIVE_H
#include <stddef.h>
#include <vector>
#include <stdio.h>
#include <inttypes.h>
#include <string>
class Archive
{
public:
	Archive();
	~Archive();

	class ArchiveFileBuffer
	{
	public:
		ArchiveFileBuffer(char* buffer, int64_t size) : m_buffer(buffer), m_size(size) {};
		~ArchiveFileBuffer() { delete[] m_buffer; }
		ArchiveFileBuffer(const ArchiveFileBuffer&) = delete;
		ArchiveFileBuffer& operator = (const ArchiveFileBuffer&) = delete;
		const char* buffer() { return m_buffer; }
		uint64_t size() const { return m_size; }
	private:
		char* m_buffer;
		uint64_t m_size;
	};
	
	//! Opens or creates an archive
	/*!
	\param filename The name of the file to create/open
	\return true of successful, false otherwise
	*/
	bool openArchive(const char* filename);
	
	//! Closes the archive
	void close();

	//! Reads a file in the archive
	/*!
	\param filename The name of the file to read
	\return An ArchiveFileBuffer containing the contents of the file if successfull, nullptr otherwise
	*/
	ArchiveFileBuffer* readFile(const char* filename);

	//! Writes a file into the archive. If the file exists, it is overwritten
	/*!
	\param filename The name of the file to write
	\param buffer The contents of the file
	\param size The size of the buffer to write
	\return true if the write succeeded, false otherwise
	*/
	bool writeFile(const char* filename, const char* buffer, int64_t size);

	//! Deletes a file from the archive.
	/*!
	\param filename The name of the file to write
	\return true if the delete succeeded, false otherwise
	*/
	bool deleteFile(const char* filename);

	//! Checks if a file exists in the archive
	/*!
	\param filename The name of the file to check
	\return true if the file exists, false otherwise
	*/
	bool fileExists(const char* filename);

	//! Re-writes the archive, removing any unused data from deletion/overwriting
	/*!
	\return true if the operation succeeds, false otherwise
	*/
	bool compact();
private:
#pragma pack(push, 1)
	struct FileInfo
	{
		char m_filename[256];
		int64_t m_size;
		int64_t m_fileOffset;
	};
	struct ArchiveHeader
	{
		unsigned int m_magic;
		unsigned int m_version;
		int64_t m_numFiles;
	};
#pragma pack(pop)

	ArchiveHeader m_header;
	std::vector<FileInfo> m_fileInfos;
	FILE* m_fileHandle;
	int64_t m_currentEnd;
	std::string m_archiveFile;
	bool updateHeaderAndTables();
};
#endif