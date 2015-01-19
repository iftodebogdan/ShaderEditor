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
	// This is the platform independent implementation of the index buffer class
	class IndexBuffer : public Buffer
	{
	public:

		// Enable the index buffer
		virtual void	Enable() = 0;
		// Disable the index buffer
		virtual void	Disable() = 0;
		// Lock the index buffer for reading/writing
		virtual void	Lock(const BufferLocking lockMode) = 0;
		// Unlock the index buffer
		virtual void	Unlock() = 0;
		// Update the index buffer with the changes made
		virtual void	Update() = 0;

		// Set an index
		void	SetIndex(const unsigned int indexIdx, const unsigned int indexVal);
		// Copy 'size' elements to the index buffer from the array, starting from the 'offset'-th element of the index buffer
		void	SetIndices(const unsigned int indicesVal[], const unsigned int size, const unsigned int offset = 0);

	protected:
						IndexBuffer(const unsigned int indexCount, const IndexBufferFormat indexFormat = IBF_INDEX16, const BufferUsage usage = BU_STATIC);
		virtual			~IndexBuffer();

		// An array containing the sizes in bytes of indices of the specified format
		static const unsigned int IndexBufferFormatSize[IBF_MAX];
	};
}

#endif //INDEXBUFFER_H
