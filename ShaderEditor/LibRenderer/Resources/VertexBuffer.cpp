#include "stdafx.h"

#include "VertexBuffer.h"
using namespace LibRendererDll;

VertexBuffer::VertexBuffer(VertexFormat* vertexFormat, unsigned int vertexCount, BufferUsage usage)
	: Buffer(vertexCount, vertexFormat->GetStride(), usage)
	, m_pVertexFormat(vertexFormat)
	, m_pTempBuffer(nullptr)
{
	assert(vertexFormat != nullptr);
}

VertexBuffer::~VertexBuffer()
{}
