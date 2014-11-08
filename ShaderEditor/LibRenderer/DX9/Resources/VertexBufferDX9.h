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
#ifndef VERTEXBUFFERDX9_H
#define VERTEXBUFFERDX9_H

#include "../../Resources/VertexBuffer.h"

namespace LibRendererDll
{
	class VertexFormatDX9;

	//This is the DX9 implementation of the VertexBuffer class
	class VertexBufferDX9 : public VertexBuffer
	{
	public:
						VertexBufferDX9(VertexFormatDX9* vertexFormat, unsigned int vertexCount, BufferUsage usage = BU_STATIC);
						~VertexBufferDX9();

		virtual	void	Enable(unsigned int offset = 0);
		virtual	void	Disable();
		virtual	void	Lock(BufferLocking lockMode);
		virtual	void	Unlock();
		virtual	void	Update();

	protected:
		IDirect3DVertexBuffer9*		m_pVertexBuffer;
	};
}

#endif //VERTEXBUFFERDX9_H
