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
#include "stdafx.h"

#include "MappingsDX9.h"

namespace LibRendererDll
{
	//Translates buffer usage flags from platform independent format to D3D9 format
	const DWORD BufferUsageDX9[Buffer::BU_MAX] =
	{
		0,							// BU_NONE
		D3DUSAGE_WRITEONLY,			// BU_STATIC
		D3DUSAGE_DYNAMIC,			// BU_DYNAMIC
		D3DUSAGE_RENDERTARGET,		// BU_RENDERTARGET
		D3DUSAGE_DEPTHSTENCIL,		// BU_DEPTHSTENCIL
		0,							// BU_TEXTURE
	};

	//Translates buffer locking flags from platform independent format to D3D9 format
	const DWORD BufferLockingDX9[Buffer::BL_MAX] =
	{
		D3DLOCK_READONLY,		// BL_READ_ONLY
		0,						// BL_WRITE_ONLY
		0						// BL_READ_WRITE
	};

	//Translates vertex attribute type flags from platform independent format to D3D9 format
	const BYTE VertexAttributeTypeDX9[VertexFormat::VAT_MAX] =
	{
		255,						// VAT_NONE
		D3DDECLTYPE_FLOAT1,			// VAT_FLOAT1
		D3DDECLTYPE_FLOAT2,			// VAT_FLOAT2
		D3DDECLTYPE_FLOAT3,			// VAT_FLOAT3
		D3DDECLTYPE_FLOAT4,			// VAT_FLOAT4
		D3DDECLTYPE_FLOAT16_2,		// VAT_HALF2
		D3DDECLTYPE_FLOAT16_4,		// VAT_HALF4
		D3DDECLTYPE_D3DCOLOR,		// VAT_UBYTE4
		D3DDECLTYPE_SHORT2,			// VAT_SHORT2
		D3DDECLTYPE_SHORT4			// VAT_SHORT4
	};

	//Translates vertex attribute usage flags from platform independent format to D3D9 format
	const BYTE VertexAttributeUsageDX9[VertexFormat::VAU_MAX] =
	{
		255,						// VAU_NONE
		D3DDECLUSAGE_POSITION,		// VAU_POSITION
		D3DDECLUSAGE_NORMAL,		// VAU_NORMAL
		D3DDECLUSAGE_TANGENT,		// VAU_TANGENT
		D3DDECLUSAGE_BINORMAL,		// VAU_BINORMAL
		D3DDECLUSAGE_TEXCOORD,		// VAU_TEXCOORD
		D3DDECLUSAGE_COLOR,			// VAU_COLOR
		D3DDECLUSAGE_BLENDINDICES,	// VAU_BLENDINDICES
		D3DDECLUSAGE_BLENDWEIGHT,	// VAU_BLENDWEIGHT
		D3DDECLUSAGE_TESSFACTOR,	// VAU_FOGCOORD
		D3DDECLUSAGE_PSIZE			// VAU_PSIZE
	};

	const D3DFORMAT IndexBufferFormatDX9[IndexBuffer::IBF_MAX] =
	{
		D3DFMT_INDEX16,				// IBF_INDEX16
		D3DFMT_INDEX32				// IBF_INDEX32
	};

	const D3DFORMAT TextureFormatDX9[Texture::TF_MAX] =
	{
		D3DFMT_UNKNOWN,				// TF_NONE
		D3DFMT_R5G6B5,				// TF_R5G6B5
		D3DFMT_A1R5G5B5,			// TF_A1R5G5B5
		D3DFMT_A4R4G4B4,			// TF_A4R4G4B4
		D3DFMT_A8,					// TF_A8
		D3DFMT_L8,					// TF_L8
		D3DFMT_A8L8,				// TF_A8L8
		D3DFMT_R8G8B8,				// TF_R8G8B8
		D3DFMT_A8R8G8B8,			// TF_A8R8G8B8
		D3DFMT_A8B8G8R8,			// TF_A8B8G8R8
		D3DFMT_L16,					// TF_L16
		D3DFMT_G16R16,				// TF_G16R16
		D3DFMT_A16B16G16R16,		// TF_A16B16G16R16
		D3DFMT_R16F,				// TF_R16F
		D3DFMT_G16R16F,				// TF_G16R16F
		D3DFMT_A16B16G16R16F,		// TF_A16B16G16R16F
		D3DFMT_R32F,				// TF_R32F
		D3DFMT_G32R32F,				// TF_G32R32F
		D3DFMT_A32B32G32R32F,		// TF_A32B32G32R32F
		D3DFMT_DXT1,				// TF_DXT1
		D3DFMT_DXT3,				// TF_DXT3
		D3DFMT_DXT5,				// TF_DXT5
		D3DFMT_D24S8				// TF_D24S8
	};

	//const DWORD TextureAddressingModeDX9[Shader::SAM_MAX] =
	//{
	//	0,                          // SAM_NONE
	//	D3DTADDRESS_CLAMP,          // SAM_CLAMP
	//	D3DTADDRESS_WRAP,           // SAM_WRAP
	//	D3DTADDRESS_MIRROR,         // SAM_MIRROR
	//	D3DTADDRESS_BORDER,         // SAM_BORDER
	//};
	//
	//const DWORD MinMagFilterDX9[Shader::SF_MAX] =
	//{
	//	D3DTEXF_NONE,               // SF_NONE
	//	D3DTEXF_POINT,              // SF_MIN_MAG_POINT_MIP_NONE
	//	D3DTEXF_LINEAR,             // SF_MIN_MAG_LINEAR_MIP_NONE
	//	D3DTEXF_POINT,              // SF_MIN_MAG_POINT_MIP_POINT
	//	D3DTEXF_POINT,              // SF_MIN_MAG_POINT_MIP_LINEAR
	//	D3DTEXF_LINEAR,             // SF_MIN_MAG_LINEAR_MIP_POINT
	//	D3DTEXF_LINEAR              // SF_MIN_MAG_LINEAR_MIP_LINEAR
	//};
	//
	//const DWORD MipFilterDX9[Shader::SF_MAX] =
	//{
	//	D3DTEXF_NONE,               // SF_NONE
	//	D3DTEXF_NONE,               // SF_MIN_MAG_POINT_MIP_NONE
	//	D3DTEXF_NONE,               // SF_MIN_MAG_LINEAR_MIP_NONE
	//	D3DTEXF_POINT,              // SF_MIN_MAG_POINT_MIP_POINT
	//	D3DTEXF_LINEAR,             // SF_MIN_MAG_POINT_MIP_LINEAR
	//	D3DTEXF_POINT,              // SF_MIN_MAG_LINEAR_MIP_POINT
	//	D3DTEXF_LINEAR              // SF_MIN_MAG_LINEAR_MIP_LINEAR
	//};
}
