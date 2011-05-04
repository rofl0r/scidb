// ======================================================================
// Author : $Author$
// Version: $Revision: 1 $
// Date   : $Date: 2011-05-04 00:04:08 +0000 (Wed, 04 May 2011) $
// Url    : $URL$
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#include "u_block_file.h"

#include "m_algorithm.h"
#include "m_utility.h"
#include "m_bit_functions.h"
#include "m_fstream.h"
#include "m_assert.h"

#include <string.h>

using namespace util;


inline static void
zero(void* p, unsigned size)
{
	memset(p, 0, size);	// NOTE: not really neccessary
}


inline static unsigned
modulo(unsigned a, unsigned m)
{
	unsigned result = a & m;
	return result ? result : m + 1;
}


inline static void
putUint24(unsigned char* data, unsigned size)
{
	data[0] = size >> 16;
	data[1] = size >> 8;
	data[2] = size;
}


inline static unsigned
getUint24(unsigned char const* data)
{
	return (unsigned(data[0]) << 16) | (unsigned(data[1]) << 8) | unsigned(data[2]);
}


BlockFileReader::Buffer::Buffer()
	:m_capacity(0)
	,m_size(0)
	,m_number(BlockFile::InvalidBlock)
	,m_data(0)
{
}


// TODO: use member m_mtime
BlockFile::BlockFile(mstl::fstream* stream, unsigned blockSize, Mode mode)
	:m_stream(stream)
	,m_view(*this)
	,m_mode(mode)
	,m_blockSize(blockSize)
	,m_shift(mstl::bf::msb_index(blockSize))
	,m_mask(blockSize - 1)
	,m_mtime(0)
	,m_isDirty(false)
	,m_isClosed(false)
	,m_countWrites(0)
{
	M_REQUIRE(stream);
	M_REQUIRE(stream->is_open());
	M_REQUIRE(stream->good());
	M_REQUIRE(stream->mode() & mstl::ios_base::in);
	M_REQUIRE(stream->mode() & mstl::ios_base::binary);
	M_REQUIRE(stream->is_unbuffered());
	M_REQUIRE(blockSize && !(blockSize & (blockSize - 1)));	// blockSize must be power of two

	m_mtime = stream->mtime();
	computeBlockCount();
}


BlockFile::BlockFile(mstl::fstream* stream, unsigned blockSize, Mode mode, mstl::string const& magic)
	:m_stream(stream)
	,m_view(*this)
	,m_mode(mode)
	,m_blockSize(blockSize)
	,m_shift(mstl::bf::msb_index(blockSize))
	,m_mask(blockSize - 1)
	,m_mtime(0)
	,m_isDirty(false)
	,m_isClosed(false)
	,m_countWrites(0)
{
	M_REQUIRE(stream);
	M_REQUIRE(stream->is_open());
	M_REQUIRE(stream->good());
	M_REQUIRE(stream->mode() & mstl::ios_base::in);
	M_REQUIRE(stream->mode() & mstl::ios_base::binary);
	M_REQUIRE(stream->is_unbuffered());
	M_REQUIRE(blockSize && !(blockSize & (blockSize - 1)));	// blockSize must be power of two
	M_REQUIRE(magic.size() < blockSize);

	m_mtime = stream->mtime();
	computeBlockCount();
	putMagic(magic);
}


BlockFile::BlockFile(unsigned blockSize, Mode mode)
	:m_stream(0)
	,m_view(*this)
	,m_mode(mode)
	,m_blockSize(blockSize)
	,m_shift(mstl::bf::msb_index(blockSize))
	,m_mask(blockSize - 1)
	,m_mtime(0)
	,m_isDirty(false)
	,m_isClosed(false)
	,m_countWrites(0)
{
	M_REQUIRE(blockSize && !(blockSize & (blockSize - 1)));	// blockSize must be power of two
}


BlockFile::BlockFile(unsigned blockSize, Mode mode, mstl::string const& magic)
	:m_stream(0)
	,m_view(*this)
	,m_mode(mode)
	,m_blockSize(blockSize)
	,m_shift(mstl::bf::msb_index(blockSize))
	,m_mask(blockSize - 1)
	,m_mtime(0)
	,m_isDirty(false)
	,m_isClosed(false)
	,m_countWrites(0)
{
	M_REQUIRE(blockSize && !(blockSize & (blockSize - 1)));	// blockSize must be power of two
	M_REQUIRE(magic.size() < blockSize);

	putMagic(magic);
}


BlockFile::~BlockFile() throw()
{
	deallocate();
}


void
BlockFile::computeBlockCount()
{
	unsigned size = m_stream->size();

	if (size)
	{
		m_sizeInfo.insert(m_sizeInfo.end(), (size + m_blockSize - 1)/m_blockSize, m_blockSize);
		m_sizeInfo.back() = ::modulo(size, m_mask);
	}
}


unsigned
BlockFile::fileSize()
{
	M_REQUIRE(!isMemoryOnly());
	return m_stream->size();
}


void
BlockFile::putMagic(mstl::string const& magic)
{
	M_ASSERT(magic.size() < blockSize());

	if (magic.empty() || !m_sizeInfo.empty())
		return;

	resize(m_view, 1);
	m_view.m_buffer.m_size = magic.size() + 1;
	m_view.m_buffer.m_number = 0;

	::memcpy(m_view.m_buffer.m_data, magic.c_str(), magic.size() + 1);

	if (m_stream == 0)
		m_cache.push_back(m_view.m_buffer.m_data);

	m_sizeInfo.push_back(magic.size() + 1);
	m_isDirty = true;
}


bool
BlockFile::isReadWrite() const
{
	return !m_stream || m_stream->mode() & mstl::ios_base::out;
}


inline
unsigned
BlockFile::lastBlockSize() const
{
	M_ASSERT(!m_sizeInfo.empty());
	return m_sizeInfo.back();
}


unsigned
BlockFile::size() const
{
	if (countBlocks() == 0)
		return 0;

	return fileOffset(countBlocks() - 1) + lastBlockSize();
}


void
BlockFile::deallocate() throw()
{
	if (m_stream)
	{
		delete [] m_view.m_buffer.m_data;
		m_view.m_buffer.m_data = 0;

		for (unsigned i = 0; i < m_asyncViews.size(); ++i)
			delete [] m_asyncViews[i]->m_buffer.m_data;
	}
	else
	{
		unsigned i = 0;

		while (i < m_cache.size())
		{
			delete [] m_cache[i];
			i += mstl::max(1u, countSpans(m_sizeInfo[i]));
		}

		m_cache.clear();
	}

	for (unsigned i = 0; i < m_asyncViews.size(); ++i)
		delete m_asyncViews[i];

	m_asyncViews.clear();
}


bool
BlockFile::close()
{
	if (m_isClosed)
		return true;

	bool rc = !m_isDirty || sync();

	deallocate();
	m_view.m_buffer.m_size = 0;
	m_view.m_buffer.m_capacity = 0;
	m_view.m_buffer.m_number = InvalidBlock;
	m_view.m_countReads = 0;
	m_countWrites = 0;
	m_isDirty = false;
	m_isClosed = true;
	m_sizeInfo.clear();

	if (m_stream)
	{
		m_stream->close();
		m_stream = 0;
	}

	return rc;
}


bool
BlockFile::attach(mstl::fstream* stream)
{
	M_REQUIRE(isOpen());
	M_REQUIRE(isMemoryOnly());
	M_REQUIRE(isInSyncMode());
	M_REQUIRE(stream);
	M_REQUIRE(stream->is_open());
	M_REQUIRE(stream->good());
	M_REQUIRE(stream->mode() & mstl::ios_base::in);
	M_REQUIRE(stream->mode() & mstl::ios_base::out);
	M_REQUIRE(stream->mode() & mstl::ios_base::binary);
	M_REQUIRE(stream->is_unbuffered());

	M_ASSERT(m_sizeInfo.size() == m_cache.size());

	m_mtime = stream->mtime();

	stream->flush();
	stream->seekp(0);

	bool rc = save(*stream);
	m_countWrites = m_cache.size();

	// we keep the current block, because it may be part of a span of blocks
	Byte* data = new Byte[mstl::mul2(m_view.m_buffer.m_capacity)];
	::memcpy(data, m_view.m_buffer.m_data, m_view.m_buffer.m_size);
	m_view.m_buffer.m_data = 0;
	deallocate();
	m_view.m_buffer.m_data = data;
	m_stream = stream;
	if (size() == 0)
		computeBlockCount();

	return rc;
}


bool
BlockFile::save(mstl::ofstream& stream)
{
	M_REQUIRE(isOpen());
	M_REQUIRE(isMemoryOnly());
	M_REQUIRE(stream.is_open());
	M_REQUIRE(stream.good());
	M_REQUIRE(stream.mode() & mstl::ios_base::binary);
	M_REQUIRE(stream.is_unbuffered());

	M_ASSERT(m_sizeInfo.size() == m_cache.size());

	if (!m_cache.empty())
	{
		for (unsigned i = 0, n = m_cache.size() - 1; i < n; ++i)
		{
			if (__builtin_expect(!stream.write(m_cache[i], m_blockSize), 0))
				return false;
		}

		if (__builtin_expect(!stream.write(m_cache.back(), m_sizeInfo.back()), 0))
			return false;
	}

	m_isDirty = false;
	return true;
}


bool
BlockFile::sync()
{
	M_REQUIRE(isOpen());
	M_REQUIRE(isInSyncMode());

	if (!m_stream || !m_isDirty)
		return true;

	M_ASSERT(m_view.m_buffer.m_data);
	M_ASSERT(m_view.m_buffer.m_number != InvalidBlock);

	m_stream->seekp(fileOffset(m_view.m_buffer.m_number), mstl::ios_base::beg);
	M_ASSERT(m_stream->tellp() == m_view.m_buffer.m_number*m_blockSize);

	if (__builtin_expect(!m_stream->write(m_view.m_buffer.m_data, m_view.m_buffer.m_size), 0))
		return false;

	m_countWrites += countSpans(m_view.m_buffer.m_size);
	m_isDirty = false;
	m_mtime = m_stream->mtime();

	return true;
}


bool
BlockFile::resize(View& view, unsigned span)
{
	M_ASSERT(span >= 1);

	if (view.m_buffer.m_capacity >= fileOffset(span))
		return false;

	view.m_buffer.m_capacity = fileOffset(span);
	delete [] view.m_buffer.m_data;

	unsigned capacity = m_stream ? mstl::mul2(view.m_buffer.m_capacity) : view.m_buffer.m_capacity;
	view.m_buffer.m_data = new Byte[capacity];

	return true;
}


unsigned
BlockFile::fetch(View& view, unsigned blockNumber, unsigned offset, unsigned span)
{
	M_ASSERT(isOpen());
	M_ASSERT(countBlocks() > 0);
	M_ASSERT(blockNumber < m_sizeInfo.size());
	M_ASSERT(blockNumber + span <= countBlocks());
	M_ASSERT(offset < m_blockSize);

	unsigned size = 0;

	if (view.m_buffer.m_number != blockNumber)
	{
		if (m_stream)
		{
			if (m_isDirty && !sync())
				return SyncFailed;

			if (m_mode == ReadWriteLength)
			{
				m_stream->seekg(fileOffset(blockNumber), mstl::ios_base::beg);

				view.m_buffer.m_size = m_sizeInfo[blockNumber];
				resize(view, 1);

				if (__builtin_expect(!m_stream->read(view.m_buffer.m_data, view.m_buffer.m_size), 0))
					return ReadError;

				if (offset >= m_blockSize - 3)
					return IllegalOffset;

				size = ::getUint24(view.m_buffer.m_data + offset);
				span = countSpans(size);

				if (span > 1)
				{
					M_ASSERT(m_sizeInfo[blockNumber] <= span*m_blockSize);

					if (offset > 0)
						return IllegalOffset;

					m_sizeInfo[blockNumber] = span*m_blockSize;

					if (resize(view, span))
					{
						m_stream->seekg(fileOffset(blockNumber), mstl::ios_base::beg);
						view.m_buffer.m_size = m_sizeInfo[blockNumber];

						if (__builtin_expect(!m_stream->read(view.m_buffer.m_data, view.m_buffer.m_size), 0))
							return ReadError;
					}
					else
					{
						M_ASSERT(view.m_buffer.m_size > m_blockSize);

						if (__builtin_expect(!m_stream->read(	view.m_buffer.m_data + m_blockSize,
																			view.m_buffer.m_size - m_blockSize), 0))
						{
							return ReadError;
						}
					}
				}
			}
			else
			{
				M_ASSERT(span > 0);

				if (span > 1)
				{
					if (offset > 0)
						return IllegalOffset;
					M_ASSERT(m_sizeInfo[blockNumber] <= span*m_blockSize);
					m_sizeInfo[blockNumber] = span*m_blockSize;
				}

				m_stream->seekg(blockNumber*m_blockSize, mstl::ios_base::beg);

				resize(view, span);
				view.m_buffer.m_size = m_sizeInfo[blockNumber];

				if (__builtin_expect(!m_stream->read(view.m_buffer.m_data, view.m_buffer.m_size), 0))
					return ReadError;

				view.m_countReads += span;
			}
		}
		else
		{
			M_ASSERT(blockNumber < m_cache.size());

			view.m_buffer.m_size = m_sizeInfo[blockNumber];
			view.m_buffer.m_data = m_cache[blockNumber];

			if (m_mode == ReadWriteLength)
				size = ::getUint24(view.m_buffer.m_data + offset);
		}

		view.m_buffer.m_number = blockNumber;
	}
	else if (m_mode == ReadWriteLength)
	{
		size = ::getUint24(view.m_buffer.m_data + offset);
	}

	return size;
}


void
BlockFile::copy(ByteStream const& buf, unsigned nbytes)
{
	M_ASSERT(isInSyncMode());

	if (m_mode == ReadWriteLength)
	{
		::putUint24(m_view.m_buffer.m_data + m_view.m_buffer.m_size, nbytes);
		::memcpy(m_view.m_buffer.m_data + m_view.m_buffer.m_size + 3, buf.data(), nbytes - 3);
	}
	else
	{
		::memcpy(m_view.m_buffer.m_data + m_view.m_buffer.m_size, buf.data(), nbytes);
	}
}


unsigned
BlockFile::put(ByteStream const& buf, unsigned offset, unsigned minSize)
{
	M_REQUIRE(isOpen());
	M_REQUIRE(isReadWrite());
	M_REQUIRE(isInSyncMode());
	M_REQUIRE(minSize <= MaxSpanSize);
	M_REQUIRE(buf.size() <= MaxSpanSize);
	M_REQUIRE(offset/blockSize() < countBlocks());
	M_REQUIRE(	(offset + minSize - 1)/blockSize() == offset/blockSize()	// fits into a single block
				|| offset % blockSize() == 0);										// or starts at block offset 0

	unsigned nbytes = buf.size();

	if (nbytes == 0)
		return 0;

	M_ASSERT(m_view.m_buffer.m_data);
	M_ASSERT(m_view.m_buffer.m_number != InvalidBlock);

	unsigned blockNo = blockNumber(offset);

	if (m_mode == ReadWriteLength)
	{
		minSize = fetch(m_view, blockNo, blockOffset(offset), 0);

		if (minSize > MaxFileSize)
			return minSize;	// it's an error code

		nbytes += 3;
	}

	unsigned oldSpan = countSpans(minSize);
	unsigned newSpan = countSpans(nbytes);

	if (nbytes <= minSize)
	{
		unsigned size = fetch(m_view, blockNo, 0);

		if (size > MaxFileSize)
			return size;	// it's an error code

		copy(buf, nbytes);
		::zero(m_view.m_buffer.m_data + nbytes, nbytes - minSize);
		m_isDirty = true;

		if (newSpan < oldSpan)
		{
			m_view.m_buffer.m_capacity = fileOffset(newSpan);
			m_view.m_buffer.m_size = nbytes;

			unsigned span = oldSpan - newSpan - 1;

			if (span == 0)
			{
				if (m_view.m_buffer.m_number + newSpan == countBlocks())
				{
					if (m_sizeInfo[m_view.m_buffer.m_number] == minSize)
						m_sizeInfo[m_view.m_buffer.m_number + newSpan] = 0;

					m_sizeInfo[m_view.m_buffer.m_number] = nbytes;
				}
			}
			else
			{
				size = fetch(m_view, blockNo + newSpan, minSize);

				if (size > MaxFileSize)
					return size;	// it's an error code

				m_view.m_buffer.m_size = 0;
				m_view.m_buffer.m_capacity = fileOffset(span);
				m_sizeInfo[m_view.m_buffer.m_number] = 0;
			}
		}
		else if (m_view.m_buffer.m_size == minSize)
		{
			m_view.m_buffer.m_size = nbytes;

			if (m_view.m_buffer.m_number + newSpan == countBlocks())
				m_sizeInfo[m_view.m_buffer.m_number] = nbytes;
		}
	}
	else if (	blockNo == countBlocks() - 1
				&& m_sizeInfo[blockNo] == blockOffset(offset) + minSize
				&& ::modulo(offset, m_mask) + nbytes <= m_blockSize)
	{
		unsigned size = fetch(m_view, blockNo, minSize);

		if (size > MaxFileSize)
			return size;	// it's an error code

		copy(buf, nbytes);
		m_isDirty = true;
		m_view.m_buffer.m_size += nbytes - minSize;
		m_sizeInfo[blockNo] = m_view.m_buffer.m_size;
	}
	else
	{
		offset = put(buf);
	}

	return offset;
}


unsigned
BlockFile::put(ByteStream const& buf)
{
	M_REQUIRE(isOpen());
	M_REQUIRE(isReadWrite());
	M_REQUIRE(isInSyncMode());
	M_REQUIRE(buf.size() <= MaxSpanSize);

	unsigned nbytes = buf.size();

	if (nbytes == 0)
		return 0;

	if (m_mode == ReadWriteLength)
		nbytes += 3;

	unsigned span = countSpans(nbytes);

	if (m_view.m_buffer.m_capacity == 0)
	{
		M_ASSERT(m_view.m_buffer.m_number == InvalidBlock);
		M_ASSERT(m_view.m_buffer.m_size == 0);
		M_ASSERT(m_isDirty == false);

		// create first block
		resize(m_view, span);

		if (m_sizeInfo.empty())
		{
			if (m_stream == 0)
				m_cache.push_back(m_view.m_buffer.m_data);

			m_sizeInfo.push_back(0);
			m_view.m_buffer.m_number = 0;
		}
		else if ((m_view.m_buffer.m_size = m_sizeInfo.back()) + nbytes <= m_blockSize)
		{
			fetch(m_view, m_sizeInfo.size() - 1, 0, span);
		}
	}

	M_ASSERT(m_view.m_buffer.m_data);
	M_ASSERT(m_view.m_buffer.m_number != InvalidBlock);
	M_ASSERT(!m_sizeInfo.empty());

	if (m_view.m_buffer.m_size + nbytes <= m_blockSize)
	{
		// use current block
	}
	else if (lastBlockSize() + nbytes <= m_blockSize)
	{
		// use last block
		unsigned size = fetch(m_view, countBlocks() - 1, 0);

		if (size > MaxFileSize)
			return size;	// it's an error code
	}
	else
	{
		// need new block

		if (fileOffset(countBlocks() + 1) > MaxFileSize)
			return MaxFileSizeExceeded;

		if (m_stream)
		{
			if (!sync())
				return SyncFailed;

			resize(m_view, span);
			::zero(m_view.m_buffer.m_data, m_view.m_buffer.m_capacity);
		}
		else
		{
			m_view.m_buffer.m_size = fileOffset(span);
			m_view.m_buffer.m_data = new Byte[m_view.m_buffer.m_size];
			m_view.m_buffer.m_capacity = mstl::max(m_view.m_buffer.m_capacity, m_view.m_buffer.m_size);

			for (unsigned i = 0; i < span; ++i)
				m_cache.push_back(m_view.m_buffer.m_data + fileOffset(i));
		}

		m_view.m_buffer.m_number = m_sizeInfo.size();
		m_view.m_buffer.m_size = 0;

		if (span >= 2)
		{
			m_sizeInfo.push_back(fileOffset(span));
			m_sizeInfo.insert(m_sizeInfo.end(), span - 2, m_blockSize);
		}

		m_sizeInfo.push_back(0);
	}

	M_ASSERT(m_view.m_buffer.m_size + nbytes <= m_view.m_buffer.m_capacity);
	M_ASSERT(m_view.m_buffer.m_size + nbytes <= m_blockSize || nbytes > m_blockSize);

	copy(buf, nbytes);
	m_isDirty = true;

	unsigned offset = fileOffset(m_view.m_buffer.m_number) + m_view.m_buffer.m_size;

	M_ASSERT(offset <= MaxFileSize);

	m_view.m_buffer.m_size += nbytes;
	m_sizeInfo[m_view.m_buffer.m_number + span - 1] = ::modulo(m_view.m_buffer.m_size, m_mask);

	return offset;
}


unsigned
BlockFile::get(View& view, ByteStream& result, unsigned offset, unsigned size)
{
	if (m_mode == ReadWriteLength)
	{
		size = fetch(view, blockNumber(offset), blockOffset(offset), 0);

		if (size <= MaxFileSize)	// otherwise it's an error code
			result.setup(view.m_buffer.m_data + blockOffset(offset) + 3, size - 3);
	}
	else if (size > 0)
	{
		unsigned rc = fetch(view, blockNumber(offset), blockOffset(offset), countSpans(size));

		if (rc <= MaxFileSize)	// otherwise it's an error code
			result.setup(view.m_buffer.m_data + blockOffset(offset), size);
		else
			size = rc;
	}

	return size;
}


bool
BlockFile::viewIsActive(Reader* reader) const
{
	M_REQUIRE(reader);
	return mstl::find(m_asyncViews.begin(), m_asyncViews.end(), reader) != m_asyncViews.end();
}


BlockFileReader*
BlockFile::openAsyncReader()
{
	m_asyncViews.push_back(new Reader(*this));
	return m_asyncViews.back();
}


void
BlockFile::closeAsyncReader(Reader*& reader)
{
	M_REQUIRE(reader == 0 || viewIsActive(reader));

	if (reader)
	{
		if (m_stream)
			delete [] reader->m_buffer.m_data;

		m_view.m_countReads += reader->m_countReads;
		m_asyncViews.erase(mstl::find(m_asyncViews.begin(), m_asyncViews.end(), reader));
		delete reader;
		reader = 0;
	}
}

// vi:set ts=3 sw=3:
