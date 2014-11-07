#ifndef FLAGMAPPINGDX9_H
#define FLAGMAPPINGDX9_H

#include <d3d9.h>

#include "../../Resources/Buffer.h"
#include "../../Resources/VertexFormat.h"

namespace LibRendererDll
{
	//These are used to translate platform independent flags to D3D9 format
	extern DWORD BufferUsageDX9[Buffer::BU_MAX];
	extern DWORD BufferLockingDX9[Buffer::BL_MAX];
	extern BYTE VertexAttributeTypeDX9[VertexFormat::VAT_MAX];
	extern BYTE VertexAttributeUsageDX9[VertexFormat::VAU_MAX];
}

#endif //FLAGMAPPINGDX9_H