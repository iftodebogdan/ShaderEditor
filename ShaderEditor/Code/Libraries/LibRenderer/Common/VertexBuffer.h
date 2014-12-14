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
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "Buffer.h"
#include "VertexFormat.h"

namespace LibRendererDll
{
	class IndexBuffer;

	//This is a platform independent class which allows the creation and manipulation of a vertex buffer.
	class VertexBuffer : public Buffer
	{
	public:
						VertexBuffer(
							VertexFormat* const vertexFormat, const unsigned int vertexCount,
							IndexBuffer* const indexBuffer = nullptr, const BufferUsage usage = BU_STATIC);
		virtual			~VertexBuffer();

		//Enable the vertex buffer. You can specify offset N to start from the (N+1)th vertex.
		//This also enables the proper vertex format and index buffer (if it exists) for the vertex buffer so you don't have to.
		virtual	void	Enable(const unsigned int offset = 0) = 0;
		//This also disabled the vertex format and index buffer (if it exists) for the vertex buffer so you don't have to.
		virtual	void	Disable() = 0;
		//Locking the buffer allows for modifications to be made to its' contents. The flow is Lock -> Update -> Unlock.
		virtual	void	Lock(const BufferLocking lockMode) = 0;
		//Unlock our buffer
		virtual	void	Unlock() = 0;
		//Sync our local modifications to the buffer's content.
		virtual	void	Update() = 0;

		VertexFormat*		GetVertexFormat() const { return m_pVertexFormat; }
		void				SetIndexBuffer(IndexBuffer* const indexBuffer) { m_pIndexBuffer = indexBuffer; }
		IndexBuffer*		GetIndexBuffer() const { return m_pIndexBuffer; }

		template <typename T>
		//Position accessor
		inline	T&			Position(const unsigned int vertexIdx) const;
		inline	const bool	HasPosition() const;

		template <typename T>
		//Normal accessor
		inline	T&			Normal(const unsigned int vertexIdx) const;
		inline	const bool	HasNormal() const;

		template <typename T>
		//Tangent accessor
		inline	T&			Tangent(const unsigned int vertexIdx) const;
		inline	const bool	HasTangent() const;

		template <typename T>
		//Binormal accessor
		inline	T&			Binormal(const unsigned int vertexIdx) const;
		inline	const bool	HasBinormal() const;

		template <typename T>
		//TexCoord accessor
		inline	T&			TexCoord(const unsigned int vertexIdx, const unsigned int usageIdx) const;
		inline	const bool	HasTexCoord(const unsigned int usageIdx) const;

		template <typename T>
		//Color accessor
		inline	T&			Color(const unsigned int vertexIdx, const unsigned int usageIdx) const;
		inline	const bool	HasColor(const unsigned int usageIdx) const;

		template <typename T>
		//BlendIndices accessor
		inline	T&			BlendIndices(const unsigned int vertexIdx) const;
		inline	const bool	HasBlendIndices() const;

		template <typename T>
		//BlendWeight accessor
		inline	T&			BlendWeight(const unsigned int vertexIdx) const;
		inline	const bool	HasBlendWeight() const;

	protected:
		VertexFormat*	m_pVertexFormat;	// Holds a pointer to the associated vertex format
		IndexBuffer*	m_pIndexBuffer;		// Holds a pointer to the associated index buffer
		void*			m_pTempBuffer;		// Used in the Lock() -> Update() -> Unlock() flow
	};

//Include our inline functions file
#include "VertexBuffer.inl"
}

#endif //VERTEXBUFFER_H
