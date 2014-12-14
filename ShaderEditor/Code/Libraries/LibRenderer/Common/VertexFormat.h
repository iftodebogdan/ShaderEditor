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
					VertexFormat(const unsigned int attributeCount);
					//Initialize() takes n triples of the form (VertexAttributeUsage, VertexAttributeType, unsigned int),
					//where n is the number of attributes of the vertex format.
		void		Initialize(const VertexAttributeUsage usage, const VertexAttributeType type, const unsigned int usageIdx, ...);
		virtual		~VertexFormat();

							//Set individual attributes. Don't forget to call SetStride(CalculateStride()) when you're done.
		void				SetAttribute(const unsigned int attrIdx, const unsigned int offset,
								const VertexAttributeUsage usage,  const VertexAttributeType type, const unsigned int usageIdx);
							//This can be used in conjunction with CalculateStride()
		void				SetStride(const unsigned int stride) { m_nStride = stride; }
							//Calculates the vertex format stride based on the attributes' sizes.
		const unsigned int	CalculateStride() const;

									/* Get the number of attributes */
		const unsigned int			GetAttributeCount() const { return m_nAttributeCount; }
									/* Get the offset in bytes of an attribute */
		const unsigned int			GetOffset(const unsigned int attrIdx) const { assert(attrIdx < m_nAttributeCount); return m_pElements[attrIdx].Offset; }
									/* Get the attribute's data type */
		const VertexAttributeType	GetAttributeType(const unsigned int attrIdx) const { assert(attrIdx < m_nAttributeCount); return m_pElements[attrIdx].Type; }
									/* Get the usage/semantic of the attribute */
		const VertexAttributeUsage	GetAttributeUsage(const unsigned int attrIdx) const { assert(attrIdx < m_nAttributeCount); return m_pElements[attrIdx].Usage; }
									/* Get the usage/semantic index of the attribute */
		const unsigned int			GetUsageIndex(const unsigned int attrIdx) const { assert(attrIdx < m_nAttributeCount); return m_pElements[attrIdx].UsageIdx; }
									/* Get the vertex format's stride */
		const unsigned int			GetStride() const { return m_nStride; }

						// Enable the vertex format (also called by the associated vertex buffer)
		virtual void	Enable() = 0;
						// Disable the vertex format (also called by the associated vertex buffer)
		virtual void	Disable() = 0;
						// Update the vertex format with the changes made
		virtual	void	Update() = 0;

	protected:
		struct VertexElement
		{
			unsigned int			Offset;		// The element's offset in the vertex format
			VertexAttributeType		Type;		// The data type of the element
			VertexAttributeUsage	Usage;		// The usage/semantic of the element
			unsigned int			UsageIdx;	// The usage/semantic index of the element
		};

		unsigned int	m_nAttributeCount;	// The total number of attributes
		VertexElement*	m_pElements;		// A pointer to the array of elements
		unsigned int	m_nStride;			// The stride of the vertex format

		static const unsigned int VertexAttributeTypeSize[VAT_MAX];
	};
}

#endif //VERTEXFORMAT_H
