#include "stdafx.h"

#include <stdarg.h>
#include "VertexFormat.h"
using namespace LibRendererDll;

//This holds the size if bytes for each vertex attribute type
unsigned int VertexFormat::VertexAttributeTypeSize[VertexFormat::VAT_MAX] =
{
	0,  // AT_NONE
	4,  // AT_FLOAT1
	8,  // AT_FLOAT2
	12, // AT_FLOAT3
	16, // AT_FLOAT4
	4,  // AT_HALF2
	8,  // AT_HALF4
	4,  // AT_UBYTE4
	4,  // AT_SHORT2
	8   // AT_SHORT4
};

VertexFormat::VertexFormat(unsigned int attributeCount)
	: m_nAttributeCount(attributeCount)
	, m_nStride(0)
{
	m_pElements = new VertexElement[m_nAttributeCount];

	for (unsigned int i = 0; i < m_nAttributeCount; i++)
	{
		m_pElements[i].Offset = 0;
		m_pElements[i].Type	= VAT_NONE;
		m_pElements[i].Usage = VAU_NONE;
		m_pElements[i].UsageIdx = 0;
	}
}

void VertexFormat::Initialize(VertexAttributeUsage usage, VertexAttributeType type, unsigned int usageIdx, ...)
{
	unsigned int offset = 0;

	SetAttribute(0, offset, usage, type, usageIdx);
	offset += VertexAttributeTypeSize[type];

	va_list args;
	va_start(args, usageIdx);
	for (unsigned int i = 1; i < m_nAttributeCount; i++)
	{
		VertexAttributeUsage tempUsage = va_arg(args, VertexAttributeUsage);
		VertexAttributeType tempType = va_arg(args, VertexAttributeType);
		unsigned int tempUsageIdx = va_arg(args, unsigned int);
		SetAttribute(i, offset, tempUsage, tempType, tempUsageIdx);
		offset += VertexAttributeTypeSize[type];
	}
	va_end(args);

	SetStride(offset);
}

VertexFormat::~VertexFormat()
{
	delete[] m_pElements;
}
			 
void VertexFormat::SetAttribute(unsigned int attrIdx, unsigned int offset,
	VertexFormat::VertexAttributeUsage usage, VertexFormat::VertexAttributeType type, unsigned int usageIdx)
{
	assert(attrIdx < m_nAttributeCount);

#ifdef _DEBUG
	if (attrIdx > 0)
		assert(offset > m_pElements[attrIdx - 1].Offset);
#endif //_DEBUG

	m_pElements[attrIdx].Offset = offset;
	m_pElements[attrIdx].Type = type;
	m_pElements[attrIdx].Usage = usage;
	m_pElements[attrIdx].UsageIdx = usageIdx;
}

unsigned int VertexFormat::CalculateStride() const
{
	unsigned int stride = 0;
	for (unsigned int i = 0; i < m_nAttributeCount; i++)
		stride += VertexAttributeTypeSize[m_pElements[i].Type];
	return stride;
}
