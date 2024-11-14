// ======================================================================
// Author : $Author$
// Version: $Revision: 880 $
// Date   : $Date: 2013-07-08 21:37:41 +0000 (Mon, 08 Jul 2013) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _u_block_file_included
#define _u_block_file_included

#include "u_byte_stream.h"

#include "m_string.h"
#include "m_vector.h"
#include "m_utility.h"

namespace mstl { class fstream; }
namespace mstl { class ostream; }

namespace util {

class BlockFile;
class Progress;


class BlockFileReader
{
public:

	BlockFile const& blockFile() const;

	unsigned get(ByteStream& result, unsigned offset, unsigned length = 0);

private:

	struct Buffer
	{
		Buffer();

		unsigned	m_capacity;
		unsigned	m_size;
		unsigned	m_number;
		unsigned m_span;

		unsigned char* m_data;

		void reset();
	};

	friend class BlockFile;

	BlockFileReader(BlockFile& blockFile);

	BlockFile&	m_blockFile;
	Buffer		m_buffer;
	unsigned		m_countReads;
};


class BlockFile : public mstl::noncopyable
{
public:

	typedef BlockFileReader Reader;

	enum Mode { ReadWriteLength, RequireLength };

	static unsigned const MaxFileSize = 1 << 31;	//  2 GB
	static unsigned const MaxSpanSize = 1 << 24; // 16 MB

	static unsigned const MaxFileSizeExceeded	= unsigned(-1);
	static unsigned const WriteFailed			= unsigned(-2);
	static unsigned const SyncFailed				= unsigned(-3);
	static unsigned const ReadError				= unsigned(-4);
	static unsigned const IllegalOffset			= unsigned(-5);
	static unsigned const SizeTooLarge			= unsigned(-6);

	BlockFile(mstl::fstream* stream, unsigned blockSize, Mode mode);
	BlockFile(mstl::fstream* stream, unsigned blockSize, Mode mode, mstl::string const& magic);
	BlockFile(unsigned blockSize, Mode mode);
	BlockFile(unsigned blockSize, Mode mode, mstl::string const& magic);
	~BlockFile() throw();

	bool isClosed() const;
	bool isOpen() const;
	bool isEmpty() const;
	bool isMemoryOnly() const;
	bool isReadOnly() const;
	bool isReadWrite() const;
	bool isInSyncMode() const;
	bool isInAsyncMode() const;

	Mode mode() const;

	unsigned size() const;
	unsigned blockSize() const;
	unsigned countBlocks() const;
	unsigned countReads() const;
	unsigned countWrites() const;
	unsigned countSpans(unsigned size) const;
	unsigned fileSize();
	unsigned blockNumber(unsigned fileOffset) const;

	bool save(mstl::ostream& stream, Progress* progress = 0);
	bool attach(mstl::fstream* stream, Progress* progress = 0);
	void clear();
	bool close();
	bool sync();

	unsigned put(ByteStream const& buf);
	unsigned put(ByteStream const& buf, unsigned offset, unsigned minLength = 0);
	unsigned get(ByteStream& result, unsigned offset, unsigned length = 0);

	unsigned shrink(unsigned newLength, unsigned offset, unsigned minLength = 0);

	void removeBlocks(unsigned firstBlockNo, unsigned lastBlockNo);

	Reader& reader();

	Reader* openAsyncReader();
	void closeAsyncReader(Reader*& reader);
	bool viewIsActive(Reader* reader) const;

private:

	friend class BlockFileReader;
	friend class BlockFileReader::Buffer;

	typedef unsigned char	Byte;
	typedef BlockFileReader	View;

	typedef mstl::vector<Byte*>		Cache;
	typedef mstl::vector<unsigned>	SizeInfo;
	typedef mstl::vector<View*>		AsyncViews;

	static unsigned const InvalidBlock = unsigned(-1);

	unsigned put(View& view, ByteStream const& buf);
	unsigned put(View& view, ByteStream const& buf, unsigned offset, unsigned minLength);
	unsigned get(View& view, ByteStream& result, unsigned offset, unsigned length);

	unsigned lastBlockSize() const;
	unsigned blockOffset(unsigned fileOffset) const;
	unsigned fileOffset(unsigned blockNumber) const;
	unsigned computeCapacity(unsigned span) const;

	void computeBlockCount();
	unsigned fetch(View& view, unsigned blockNumber, unsigned span = 1);
	unsigned retrieve(View& view, unsigned blockNumber, unsigned offset);
	void resize(View& view, unsigned span);
	void deallocate() throw();
	void putMagic();

	void copy(ByteStream const& buf, unsigned offset, unsigned nbytes);

	mstl::fstream*	m_stream;

	View				m_view;
	Mode				m_mode;
	unsigned			m_blockSize;
	unsigned			m_shift;
	unsigned			m_mask;
	unsigned			m_mtime;
	bool				m_isDirty;
	bool				m_isClosed;
	unsigned			m_countWrites;
	SizeInfo			m_sizeInfo;
	Cache				m_cache;
	AsyncViews		m_asyncViews;
	mstl::string	m_magic;
};

} // namespace util

#include "u_block_file.ipp"

#endif // _u_block_file_included

// vi:set ts=3 sw=3:
