template <typename T>
inline T& VertexBuffer::Position(unsigned int vertexIdx)
{
	unsigned int positionOffset = 0;

	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_POSITION)
		{
			positionOffset = m_pVertexFormat->GetOffset(i);
			break;
		}

	return *(T*)((char*)m_pData + positionOffset + vertexIdx * GetVertexFormat()->GetStride());
}

inline bool VertexBuffer::HasPosition() const
{
	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_POSITION)
			return true;

	return false;
}

template <typename T>
inline T& VertexBuffer::Normal(unsigned int vertexIdx)
{
	unsigned int normalOffset = 0;

	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_NORMAL)
		{
			normalOffset = m_pVertexFormat->GetOffset(i);
			break;
		}

	return *(T*)((char*)m_pData + normalOffset + vertexIdx * GetVertexFormat()->GetStride());
}

inline bool VertexBuffer::HasNormal() const
{
	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_NORMAL)
			return true;

	return false;
}

template <typename T>
inline T& VertexBuffer::Tangent(unsigned int vertexIdx)
{
	unsigned int tangentOffset = 0;

	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_TANGENT)
		{
			tangentOffset = m_pVertexFormat->GetOffset(i);
			break;
		}

	return *(T*)((char*)m_pData + tangentOffset + vertexIdx * GetVertexFormat()->GetStride());
}

inline bool VertexBuffer::HasTangent() const
{
	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_TANGENT)
			return true;

	return false;
}

template <typename T>
inline T& VertexBuffer::Binormal(unsigned int vertexIdx)
{
	unsigned int binormalOffset = 0;

	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_BINORMAL)
		{
			binormalOffset = m_pVertexFormat->GetOffset(i);
			break;
		}

	return *(T*)((char*)m_pData + binormalOffset + vertexIdx * GetVertexFormat()->GetStride());
}

inline bool VertexBuffer::HasBinormal() const
{
	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_BINORMAL)
			return true;

	return false;
}

template <typename T>
inline T& VertexBuffer::TexCoord(unsigned int vertexIdx, unsigned int usageIdx)
{
	unsigned int tcoordOffset = 0;

	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_TEXCOORD
			&& m_pVertexFormat->GetUsageIndex(i) == usageIdx)
		{
			tcoordOffset = m_pVertexFormat->GetOffset(i);
			break;
		}

	return *(T*)((char*)m_pData + tcoordOffset + vertexIdx * GetVertexFormat()->GetStride());
}

inline bool VertexBuffer::HasTexCoord(unsigned int usageIdx) const
{
	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_TEXCOORD
			&& m_pVertexFormat->GetUsageIndex(i) == usageIdx)
			return true;

	return false;
}

template <typename T>
inline T& VertexBuffer::Color(unsigned int vertexIdx, unsigned int usageIdx)
{
	unsigned int colorOffset = 0;

	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_COLOR
			&& m_pVertexFormat->GetUsageIndex(i) == usageIdx)
		{
			colorOffset = m_pVertexFormat->GetOffset(i);
			break;
		}

	return *(T*)((char*)m_pData + colorOffset + vertexIdx * GetVertexFormat()->GetStride());
}

inline bool VertexBuffer::HasColor(unsigned int usageIdx) const
{
	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_COLOR
			&& m_pVertexFormat->GetUsageIndex(i) == usageIdx)
			return true;

	return false;
}

template <typename T>
inline T& VertexBuffer::BlendIndices(unsigned int vertexIdx)
{
	unsigned int blendIndOffset = 0;

	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_BLENDINDICES)
		{
			blendIndOffset = m_pVertexFormat->GetOffset(i);
			break;
		}

	return *(T*)((char*)m_pData + blendIndOffset + vertexIdx * GetVertexFormat()->GetStride());
}

inline bool VertexBuffer::HasBlendIndices() const
{
	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_BLENDINDICES)
			return true;

	return false;
}

template <typename T>
inline T& VertexBuffer::BlendWeight(unsigned int vertexIdx)
{
	unsigned int blendWeightOffset = 0;

	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_BLENDWEIGHT)
		{
			blendWeightOffset = m_pVertexFormat->GetOffset(i);
			break;
		}

	return *(T*)((char*)m_pData + blendWeightOffset + vertexIdx * GetVertexFormat()->GetStride());
}

inline bool VertexBuffer::HasBlendWeight() const
{
	for (int i = 0, n = m_pVertexFormat->GetAttributeCount(); i < n; i++)
		if (m_pVertexFormat->GetAttributeUsage(i) == VertexFormat::VAU_BLENDWEIGHT)
			return true;

	return false;
}