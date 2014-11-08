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
	//This is a platform independent class which allows the creation and manipulation of a vertex buffer.
	class VertexBuffer : public Buffer
	{
	public:
						VertexBuffer(VertexFormat* vertexFormat, unsigned int vertexCount, BufferUsage usage = BU_STATIC);
		virtual			~VertexBuffer();

		//This also enables the proper vertex format for the vertex buffer so you don't have to
		virtual	void	Enable(unsigned int offset = 0) = 0;
		//This also disabled the proper vertex format for the vertex buffer so you don't have to
		virtual	void	Disable() = 0;
		//Locking the buffer allows for modifications to be made to its' contents. The flow is Lock -> Update -> Unlock.
		virtual	void	Lock(BufferLocking lockMode) = 0;
		//Unlock our buffer
		virtual	void	Unlock() = 0;
		//Sync our local modifications to the buffer's content
		virtual	void	Update() = 0;

		VertexFormat*	GetVertexFormat() const { return m_pVertexFormat; }

		template <typename T>
		//Position accessor for our vertex buffer
		inline	T&		Position(unsigned int vertexIdx);
		inline	bool	HasPosition() const;

		template <typename T>
		//Normal accessor for our vertex buffer
		inline	T&		Normal(unsigned int vertexIdx);
		inline	bool	HasNormal() const;

		template <typename T>
		//Tangent accessor for our vertex buffer
		inline	T&		Tangent(unsigned int vertexIdx);
		inline	bool	HasTangent() const;

		template <typename T>
		//Binormal accessor for our vertex buffer
		inline	T&		Binormal(unsigned int vertexIdx);
		inline	bool	HasBinormal() const;

		template <typename T>
		//TexCoord accessor for our vertex buffer
		inline	T&		TexCoord(unsigned int vertexIdx, unsigned int usageIdx);
		inline	bool	HasTexCoord(unsigned int usageIdx) const;

		template <typename T>
		//Color accessor for our vertex buffer
		inline	T&		Color(unsigned int vertexIdx, unsigned int usageIdx);
		inline	bool	HasColor(unsigned int usageIdx) const;

		template <typename T>
		//BlendIndices accessor for our vertex buffer
		inline	T&		BlendIndices(unsigned int vertexIdx);
		inline	bool	HasBlendIndices() const;

		template <typename T>
		//BlendWeight accessor for our vertex buffer
		inline	T&		BlendWeight(unsigned int vertexIdx);
		inline	bool	HasBlendWeight() const;

	protected:
		VertexFormat*	m_pVertexFormat;
		void*			m_pTempBuffer;
	};

//Include our inline file
#include "VertexBuffer.inl"
}

#endif //VERTEXBUFFER_H
