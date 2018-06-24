#include "archive.h"
#include <string.h>
#include <sys/stat.h>

#ifdef _MSC_VER
#	define FOPEN	fopen
#	define FCLOSE	fclose
#	define FREAD	fread
#	define FWRITE	fwrite
#	define FSEEK	_fseeki64
#	define FTELL	_ftelli64
#else
#	define FOPEN	fopen64
#	define FCLOSE	fclose
#	define FREAD	fread
#	define FWRITE	fwrite
#	define FSEEK	fseeko64
#	define FTELL	ftello64
#endif

Archive::Archive() : m_fileHandle(nullptr)
{
}

Archive::~Archive()
{
	if (m_fileHandle)
		FCLOSE(m_fileHandle);
}

bool Archive::openArchive(const char * filename)
{
	if (m_fileHandle)
	{
		return false;
	}
	struct stat st;
	int result = stat(filename, &st);
	if (result == 0)
	{
		// File exists, open it for read and write
		m_fileHandle = FOPEN(filename, "r+b");
		if (!m_fileHandle)
			return false;

		// Read the header and check if it is a valid file
		if (FREAD(&m_header, sizeof(ArchiveHeader), 1, m_fileHandle) != 1)
			return false;
		if (m_header.m_magic != 0xabcd1234 || m_header.m_version != 1)
		{
			FCLOSE(m_fileHandle);
			m_fileHandle = nullptr;
			return false;
		}
		if (m_header.m_numFiles > 0)
		{
			m_fileInfos.resize(m_header.m_numFiles);
			FSEEK(m_fileHandle, -(int64_t)(sizeof(FileInfo) * m_header.m_numFiles), SEEK_END);
			m_currentEnd = ftell(m_fileHandle);

			// Read the file table
			if (FREAD(&m_fileInfos[0], sizeof(FileInfo), m_header.m_numFiles, m_fileHandle) != m_header.m_numFiles)
			{
				FCLOSE(m_fileHandle);
				m_fileHandle = nullptr;
				return false;
			}
		}
	}
	else
	{
		// File doesn't exist create the file
		m_fileHandle = FOPEN(filename, "w+b");
		if (!m_fileHandle)
			return false;

		// Initialize the file header
		m_header.m_magic = 0xabcd1234;
		m_header.m_version = 1;
		m_header.m_numFiles = 0;
		if (FWRITE(&m_header, sizeof(ArchiveHeader), 1, m_fileHandle) != 1)
		{
			FCLOSE(m_fileHandle);
			m_fileHandle = nullptr;
			return false;
		}
		m_fileInfos.clear();
		m_currentEnd = sizeof(m_header);
	}
	m_archiveFile = filename;
	return true;
}

void Archive::close()
{
	if (!m_fileHandle)
		return;
	FCLOSE(m_fileHandle);
	m_fileHandle = nullptr;
	m_fileInfos.clear();
	m_header = { 0 };
	m_currentEnd = 0;
	m_archiveFile = "";
}

Archive::ArchiveFileBuffer * Archive::readFile(const char * filename)
{
	if (!m_fileHandle) return nullptr;

	// Find the file entry for this file
	for (FileInfo& inf : m_fileInfos)
	{
		if (!strcmp(inf.m_filename, filename))
		{
			// Found the file, seek to the specified file offset
			if (FSEEK(m_fileHandle, inf.m_fileOffset, SEEK_SET) != 0)
				return nullptr;

			// Read the file contents
			char* bytes = new char[inf.m_size];
			if (FREAD(bytes, 1, inf.m_size, m_fileHandle) != inf.m_size)
				return nullptr;

			// Return the contents
			return new ArchiveFileBuffer(bytes, inf.m_size);
		}
	}
	return nullptr;
}

bool Archive::writeFile(const char * filename, const char * buffer, int64_t size)
{
	if (!m_fileHandle) return false;
	int64_t newOffset = 0;
	FileInfo* existingFileInfo = nullptr;
	// Find the file entry for this file
	for (FileInfo& inf : m_fileInfos)
	{
		if (!strcmp(inf.m_filename, filename))
		{
			if (inf.m_size >= size)
			{
				existingFileInfo = &inf;
				// Found the file and the current storage is big enough for the new write
				newOffset = inf.m_fileOffset;
			}
		}
	}

	// This is a new file
	if (newOffset == 0)
	{
		newOffset = m_currentEnd;
	}

	if (existingFileInfo)
	{
		existingFileInfo->m_fileOffset = newOffset;
		existingFileInfo->m_size = size;
	}
	else
	{
		FileInfo newInfo;
		strncpy(newInfo.m_filename, filename, 255);
		newInfo.m_fileOffset = newOffset;
		newInfo.m_size = size;
		m_fileInfos.push_back(newInfo);
	}

	// Seek to the position where we will perform the write
	if (FSEEK(m_fileHandle, newOffset, SEEK_SET) != 0)
		return false;

	// Write the buffer
	if (FWRITE(buffer, 1, size, m_fileHandle) != size)
		return false;


	// Update the end position
	if (m_currentEnd == newOffset)
		m_currentEnd += size;

	return updateHeaderAndTables();
}

bool Archive::deleteFile(const char* filename)
{
	if (!m_fileHandle) return false;

	// Find the file entry for this file
	for (auto iter = m_fileInfos.begin(); iter != m_fileInfos.end(); ++iter)
	{
		FileInfo& inf = *iter;
		if (!strcmp(inf.m_filename, filename))
		{
			m_fileInfos.erase(iter);
			return updateHeaderAndTables();
		}
	}
	return false;
}

bool Archive::updateHeaderAndTables()
{
	// Write the updated file table
	FSEEK(m_fileHandle, m_currentEnd, SEEK_SET);
	if (FWRITE(&m_fileInfos[0], sizeof(FileInfo), m_fileInfos.size(), m_fileHandle) != m_fileInfos.size())
	{
		return false;
	}
	// Update the header
	FSEEK(m_fileHandle, 0, SEEK_SET);
	m_header.m_numFiles = m_fileInfos.size();
	if (FWRITE(&m_header, sizeof(ArchiveHeader), 1, m_fileHandle) != 1)
	{
		return false;
	}
	return true;
}

bool Archive::compact()
{
	if (m_fileHandle == nullptr)
		return false;

	// Read all files into memory and update their offsets
	std::vector<ArchiveFileBuffer*> buffers;
	std::vector<FileInfo> newInfos;
	buffers.reserve(m_fileInfos.size());
	newInfos.reserve(m_fileInfos.size());
	int64_t currentOffset = sizeof(ArchiveHeader);
	for (FileInfo& info : m_fileInfos)
	{
		ArchiveFileBuffer* buf = readFile(info.m_filename);
		if (!buf)
			return false;
		buffers.push_back(buf);
		info.m_fileOffset = currentOffset;
		currentOffset += info.m_size;
	}
	
	// Close the old file and delete
	FCLOSE(m_fileHandle);
	remove(m_archiveFile.c_str());
	
	// Create a new file and re-write everything
	m_fileHandle = FOPEN(m_archiveFile.c_str(), "w+b");

	if (!m_fileHandle)
		return false;

	// Rewrite everything to disk
	if (FSEEK(m_fileHandle, 0, SEEK_SET) != 0)
		return false;

	// Rewrite header
	if (FWRITE(&m_header, sizeof(ArchiveHeader), 1, m_fileHandle) != 1)
		return false;

	// Rewrite file data
	for (ArchiveFileBuffer* buf : buffers)
	{
		if (FWRITE(buf->buffer(), 1, buf->size(), m_fileHandle) != buf->size())
			return false;
	}
	m_currentEnd = FTELL(m_fileHandle);
	// Rewrite file table
	if (FWRITE(&m_fileInfos[0], sizeof(FileInfo), m_fileInfos.size(), m_fileHandle) != m_fileInfos.size())
		return false;

	// Delete buffers
	for (ArchiveFileBuffer* buf : buffers)
		delete buf;

	return true;
}

bool Archive::fileExists(const char * filename)
{
	if (!m_fileHandle) return false;

	// Find the file entry for this file
	for (FileInfo& inf : m_fileInfos)
	{
		if (!strcmp(inf.m_filename, filename))
		{
			return true;
		}
	}
	return false;
}

