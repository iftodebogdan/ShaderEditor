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
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "Buffer.h"

namespace LibRendererDll
{
	class IndexBuffer : public Buffer
	{
	public:
		enum IndexBufferFormat
		{
			IBF_INDEX16,
			IBF_INDEX32,

			IBF_MAX
		};

						IndexBuffer(unsigned int indexCount, IndexBufferFormat indexFormat, BufferUsage usage = BU_STATIC);
		virtual			~IndexBuffer();

		void			SetOffset(unsigned int offset) { m_nOffset = offset; }
		unsigned int	GetOffset() const { return m_nOffset; }

		virtual void	Enable() = 0;
		virtual void	Disable() = 0;
		virtual void	Lock(BufferLocking lockMode) = 0;
		virtual void	Unlock() = 0;
		virtual void	Update() = 0;

		void			SetIndex(unsigned int indexIdx, unsigned int indexVal);
		void			SetIndices(unsigned int indicesVal[], unsigned int size, unsigned int offset = 0);

	protected:
		unsigned int	m_nOffset;
		void*			m_pTempBuffer;

		static unsigned int IndexBufferFormatSize[IBF_MAX];
	};
}

#endif //INDEXBUFFER_H
