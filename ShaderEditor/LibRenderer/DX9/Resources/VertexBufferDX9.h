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