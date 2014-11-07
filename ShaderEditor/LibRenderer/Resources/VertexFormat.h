#ifndef VERTEXFORMAT_H
#define VERTEXFORMAT_H

namespace LibRendererDll
{
	//This is a platform independent class with which you can define a vertex format for your vertex buffer.
	class VertexFormat
	{
	public:
		enum
		{
			VF_MAX_ATTRIBUTES = 16,
			VF_MAX_TCOORD_UNITS = 8,
			VF_MAX_COLOR_UNITS = 2
		};

		//These flags describe the type of data which compose the vertex attributes
		enum VertexAttributeType
		{
			VAT_NONE,
			VAT_FLOAT1,
			VAT_FLOAT2,
			VAT_FLOAT3,
			VAT_FLOAT4,
			VAT_HALF2,
			VAT_HALF4,
			VAT_UBYTE4,
			VAT_SHORT2,
			VAT_SHORT4,

			VAT_MAX
		};

		//These flags describe an attribute's purpose
		enum VertexAttributeUsage
		{
			VAU_NONE,
			VAU_POSITION,
			VAU_NORMAL,
			VAU_TANGENT,
			VAU_BINORMAL,
			VAU_TEXCOORD,
			VAU_COLOR,
			VAU_BLENDINDICES,
			VAU_BLENDWEIGHT,
			VAU_FOGCOORD,
			VAU_PSIZE,

			VAU_MAX
		};

								//When creating a vertex format, you must first set the number of attributes (this can't be changed later).
								//Each attribute can then be initialized separately using SetAttribute(), or all at once with Initialize().
								VertexFormat(unsigned int attributeCount);
								//Initialize() takes n triples of the form (VertexAttributeUsage, VertexAttributeType, unsigned int),
								//where n is the number of attributes of the vertex format.
		void					Initialize(VertexAttributeUsage usage, VertexAttributeType type, unsigned int usageIdx, ...);
		virtual					~VertexFormat();

								//Set individual attributes. Don't forget to call SetStride(CalculateStride()) when you're done.
		void					SetAttribute(unsigned int attrIdx, unsigned int offset,
									VertexAttributeUsage usage,  VertexAttributeType type, unsigned int usageIdx);
								//This can be used in conjunction with CalculateStride()
		void					SetStride(unsigned int stride) { m_nStride = stride; }
								//Calculates the vertex format stride based on the attributes' sizes.
		unsigned int			CalculateStride() const;

		unsigned int			GetAttributeCount() const {	return m_nAttributeCount; }
		unsigned int			GetOffset(unsigned int attrIdx) const { assert(attrIdx < m_nAttributeCount); return m_pElements[attrIdx].Offset; }
		VertexAttributeType		GetAttributeType(unsigned int attrIdx) const { assert(attrIdx < m_nAttributeCount); return m_pElements[attrIdx].Type; }
		VertexAttributeUsage	GetAttributeUsage(unsigned int attrIdx) const { assert(attrIdx < m_nAttributeCount); return m_pElements[attrIdx].Usage; }
		unsigned int			GetUsageIndex(unsigned int attrIdx) const { assert(attrIdx < m_nAttributeCount); return m_pElements[attrIdx].UsageIdx; }
		unsigned int			GetStride() const { return m_nStride; }

		virtual void			Enable() = 0;
		virtual void			Disable() = 0;
		virtual	void			Update() = 0;

	protected:
		struct VertexElement
		{
			unsigned int			Offset;
			VertexAttributeType		Type;
			VertexAttributeUsage	Usage;
			unsigned int			UsageIdx;
		};

		unsigned int	m_nAttributeCount;
		VertexElement*	m_pElements;
		unsigned int	m_nStride;

		static unsigned int VertexAttributeTypeSize[VAT_MAX];
	};
}

#endif //VERTEXFORMAT_H