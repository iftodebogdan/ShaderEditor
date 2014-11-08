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

#include <string.h>
#include "IndexBuffer.h"
using namespace LibRendererDll;

unsigned int IndexBuffer::IndexBufferFormatSize[IndexBuffer::IBF_MAX] =
{
	2,  // IBF_INDEX16
	4,  // IBF_INDEX32
};

IndexBuffer::IndexBuffer(unsigned int indexCount, IndexBufferFormat indexFormat, BufferUsage usage)
	: Buffer(indexCount, IndexBufferFormatSize[indexFormat], usage)
	, m_nOffset(0)
	, m_pTempBuffer(nullptr)
{}

IndexBuffer::~IndexBuffer()
{}

void IndexBuffer::SetIndex(unsigned int indexIdx, unsigned int indexVal)
{
	assert(indexIdx < m_nElementCount);
	int sizeshort = sizeof(unsigned short);
	int sizeint = sizeof(unsigned int);
	int sizechar = sizeof(char);
	memcpy((char*)m_pData + (indexIdx * m_nElementSize), &indexVal, m_nElementSize);
}

void IndexBuffer::SetIndices(unsigned int indicesVal[], unsigned int size, unsigned int offset)
{
	for (int i = 0; i < size; i++)
		SetIndex(i + offset, indicesVal[i]);
}
