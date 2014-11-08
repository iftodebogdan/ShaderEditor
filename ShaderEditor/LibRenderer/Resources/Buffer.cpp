//////////////////////////////////////////////////////////////////////////
// This file is part of the "LibRenderer" 3D graphics library           //
//                                                                      //
// Copyright (C) 2014 - Iftode Bogdan-Marius <iftode.bogdan@gmail.com>  //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not, see <http://www.gnu.org/licenses/>. //
//////////////////////////////////////////////////////////////////////////
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
