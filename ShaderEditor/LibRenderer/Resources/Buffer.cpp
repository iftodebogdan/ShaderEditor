#include "stdafx.h"

#include "Buffer.h"
using namespace LibRendererDll;

Buffer::Buffer(unsigned int elementCount, unsigned int elementSize, Buffer::BufferUsage usage)
	: m_nElementCount(elementCount)
	, m_nElementSize(elementSize)
	, m_eBufferUsage(usage)
	, m_nSize(elementCount * elementSize)
{
	assert(elementCount > 0);
	assert(elementSize > 0);

	m_pData = new char[m_nSize];
}

Buffer::~Buffer()
{
	delete[] m_pData;
}

const unsigned int Buffer::GetElementCount() const
{
	return m_nElementCount;
}

void Buffer::SetElementCount(unsigned int numElements)
{
	assert(numElements > 0);

	m_nElementCount = numElements;
}

const unsigned int Buffer::GetElementSize() const
{
	return m_nElementSize;
}

const Buffer::BufferUsage Buffer::GetUsage() const
{
	return m_eBufferUsage;
}

const unsigned int Buffer::GetSize() const
{
	return m_nSize;
}

const void* Buffer::GetData() const
{
	return m_pData;
}