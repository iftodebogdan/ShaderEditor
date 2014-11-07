#ifndef VERTEXFORMATDX9_H
#define VERTEXFORMATDX9_H

#include "../../Resources/VertexFormat.h"

#include <d3d9.h>

namespace LibRendererDll
{
	//This is the DX9 implementation of the VertexFormat class
	class VertexFormatDX9 : public VertexFormat
	{
	public:
						VertexFormatDX9(unsigned int attributeCount);
						~VertexFormatDX9();

		virtual void	Enable();
		virtual void	Disable();
		virtual	void	Update();

	protected:
		D3DVERTEXELEMENT9 m_pVertexElements[VertexFormat::VF_MAX_ATTRIBUTES + 1];
		IDirect3DVertexDeclaration9* m_pVertexDeclaration;
	};
}

#endif //VERTEXFORMATDX9_H