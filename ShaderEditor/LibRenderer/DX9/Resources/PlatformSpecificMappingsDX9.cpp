#include "stdafx.h"

#include "PlatformSpecificMappingsDX9.h"

namespace LibRendererDll
{
	//Translates buffer usage flags from platform independent format to D3D9 format
	DWORD BufferUsageDX9[Buffer::BU_MAX] =
	{
		D3DUSAGE_WRITEONLY,         // BU_STATIC
		D3DUSAGE_DYNAMIC,           // BU_DYNAMIC
		D3DUSAGE_RENDERTARGET,      // BU_RENDERTARGET
		D3DUSAGE_DEPTHSTENCIL,      // BU_DEPTHSTENCIL
	};

	//Translates buffer locking flags from platform independent format to D3D9 format
	DWORD BufferLockingDX9[Buffer::BL_MAX] =
	{
		D3DLOCK_READONLY,           // BL_READ_ONLY
		0,                          // BL_WRITE_ONLY
		0                           // BL_READ_WRITE
	};

	//Translates vertex attribute type flags from platform independent format to D3D9 format
	BYTE VertexAttributeTypeDX9[VertexFormat::VAT_MAX] =
	{
		255,                        // VAT_NONE
		D3DDECLTYPE_FLOAT1,         // VAT_FLOAT1
		D3DDECLTYPE_FLOAT2,         // VAT_FLOAT2
		D3DDECLTYPE_FLOAT3,         // VAT_FLOAT3
		D3DDECLTYPE_FLOAT4,         // VAT_FLOAT4
		D3DDECLTYPE_FLOAT16_2,      // VAT_HALF2
		D3DDECLTYPE_FLOAT16_4,      // VAT_HALF4
		D3DDECLTYPE_D3DCOLOR,       // VAT_UBYTE4
		D3DDECLTYPE_SHORT2,         // VAT_SHORT2
		D3DDECLTYPE_SHORT4          // VAT_SHORT4
	};

	//Translates vertex attribute usage flags from platform independent format to D3D9 format
	BYTE VertexAttributeUsageDX9[VertexFormat::VAU_MAX] =
	{
		255,                        // VAU_NONE
		D3DDECLUSAGE_POSITION,      // VAU_POSITION
		D3DDECLUSAGE_NORMAL,        // VAU_NORMAL
		D3DDECLUSAGE_TANGENT,       // VAU_TANGENT
		D3DDECLUSAGE_BINORMAL,      // VAU_BINORMAL
		D3DDECLUSAGE_TEXCOORD,      // VAU_TEXCOORD
		D3DDECLUSAGE_COLOR,         // VAU_COLOR
		D3DDECLUSAGE_BLENDINDICES,  // VAU_BLENDINDICES
		D3DDECLUSAGE_BLENDWEIGHT,   // VAU_BLENDWEIGHT
		D3DDECLUSAGE_TESSFACTOR,    // VAU_FOGCOORD
		D3DDECLUSAGE_PSIZE          // VAU_PSIZE
	};
}