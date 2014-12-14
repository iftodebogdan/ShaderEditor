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
#ifndef BUFFER_H
#define BUFFER_H

#include "RenderData.h"

namespace LibRendererDll
{
	// This is the base class which manages memory buffers
	class Buffer
	{
	public:
		// Usage options that identify how resources are to be used
		enum BufferUsage
		{
			BU_STATIC,			// Informs the system that the application writes only to the buffer
			BU_DYNAMIC,			// Set to indicate that the buffer requires dynamic memory use
			BU_RENDERTAGET,		// The resource will be a render target
			BU_DEPTHSTENCIL,	// The resource will be a depth stencil buffer
			BU_TEXTURE,			// The resource will be a texture

			BU_MAX				// DO NOT USE! INTERNAL USAGE ONLY!
		};

		// Locking options that identify how resources are locked for reading/writing
		enum BufferLocking
		{
			BL_READ_ONLY,	// The application will ONLY read the buffer
			BL_WRITE_ONLY,	// The application will ONLY write to the buffer
			BL_READ_WRITE,	// The application will both read and write to the buffer

			BL_MAX			// DO NOT USE! INTERNAL USAGE ONLY!
		};

		// Returns the number of elements (vertices, indices, pixels, etc.) in the buffer
		const	unsigned int	GetElementCount() const { return m_nElementCount; }
		// Returns the size in bytes of an element
		const	unsigned int	GetElementSize() const { return m_nElementSize; }
		// Returns the usage option of the buffer
		const	BufferUsage		GetUsage() const { return m_eBufferUsage; }
		// Returns the size in bytes of the entire buffer
		const	unsigned int	GetSize() const { return m_nSize; }
		// Returns a pointer to the beginning of the buffer
				byte*			GetData() const { return m_pData; }

	protected:
						Buffer(const unsigned int elementCount, const unsigned int elementSize, const BufferUsage usage);
		virtual			~Buffer();

		unsigned int	m_nElementCount;	// Holds the number of elements
		unsigned int	m_nElementSize;		// Holds the size in bytes of an element
		BufferUsage		m_eBufferUsage;		// Holds the type of usage of the buffer
		unsigned int	m_nSize;			// Holds the total size in bytes of the buffer
		byte*			m_pData;			// Pointer to the beginning of the buffer
	};
}

#endif //BUFFER_H
