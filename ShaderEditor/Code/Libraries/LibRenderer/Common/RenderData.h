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
#ifndef RENDERDATA_H
#define RENDERDATA_H

#include <string>
#include <vector>
#include <gmtl/gmtl.h>
using namespace gmtl;

namespace LibRendererDll
{
	enum API
	{
		API_NULL,	// Null render

		API_DX9,	// Direct3D 9
	};

	// BUFFERS /////////////////////////////////////////////////////////
	// Usage options that identify how resources are to be used
	enum BufferUsage
	{
		BU_NONE,

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

	// Specifies the format of the index buffer
	enum IndexBufferFormat
	{
		IBF_INDEX16,	// 16bit index buffer
		IBF_INDEX32,	// 32bit index buffer

		IBF_MAX			// DO NOT USE! INTERNAL USAGE ONLY!
	};
	///////////////////////////////////////////////////////////

	// RENDER STATES //////////////////////////////////////////
	enum Blend
	{
		BLEND = 0,

		BLEND_ZERO,
		BLEND_ONE,
		BLEND_SRCCOLOR,
		BLEND_INVSRCCOLOR,
		BLEND_SRCALPHA,
		BLEND_INVSRCALPHA,
		BLEND_DESTALPHA,
		BLEND_INVDESTALPHA,
		BLEND_DESTCOLOR,
		BLEND_INVDESTCOLOR,
		BLEND_SRCALPHASAT,
		BLEND_BOTHSRCALPHA,
		BLEND_BOTHINVSRCALPHA,
		BLEND_BLENDFACTOR,
		BLEND_INVBLENDFACTOR,

		BLEND_MAX
	};

	enum Cmp
	{
		CMP = BLEND_MAX + 1,

		CMP_NEVER,
		CMP_LESS,
		CMP_EQUAL,
		CMP_LESSEQUAL,
		CMP_GREATER,
		CMP_NOTEQUAL,
		CMP_GREATEQUAL,
		CMP_ALWAYS,

		CMP_MAX,
	};

	enum Cull
	{
		CULL = CMP_MAX + 1,

		CULL_NONE,
		CULL_CW,
		CULL_CCW,

		CULL_MAX
	};

	enum ZBuffer
	{
		ZB = CULL_MAX + 1,

		ZB_ENABLED,
		ZB_DISABLED,
		ZB_USEW,

		ZB_MAX,
	};

	enum StencilOp
	{
		STENCILOP = ZB_MAX + 1,

		STENCILOP_KEEP,
		STENCILOP_ZERO,
		STENCILOP_REPLACE,
		STENCILOP_INCRSAT,
		STENCILOP_DECRSAT,
		STENCILOP_INVERT,
		STENCILOP_INCR,
		STENCILOP_DECR,

		STENCILOP_MAX
	};

	enum Fill
	{
		FILL = STENCILOP_MAX + 1,

		FILL_POINT,
		FILL_WIREFRAME,
		FILL_SOLID,

		FILL_MAX
	};

	enum
	{
		RS_MAX = FILL_MAX + 1
	};
	///////////////////////////////////////////////////////////

	// SAMPLER STATES /////////////////////////////////////////
	enum
	{
		MAX_NUM_VSAMPLERS = 4,  // vs3.0 has 4, vs2.0 has 0.
		MAX_NUM_PSAMPLERS = 16  // ps2.0 and ps3.0 have 16.
	};

	enum SamplerFilter
	{
		SF_NONE,

		SF_MIN_MAG_POINT_MIP_NONE,
		SF_MIN_MAG_LINEAR_MIP_NONE,
		SF_MIN_MAG_POINT_MIP_POINT,
		SF_MIN_MAG_POINT_MIP_LINEAR,
		SF_MIN_MAG_LINEAR_MIP_POINT,
		SF_MIN_MAG_LINEAR_MIP_LINEAR,

		SF_MAX
	};

	enum SamplerAddressingMode
	{
		SAM_NONE,

		SAM_CLAMP,
		SAM_WRAP,
		SAM_MIRROR,
		SAM_BORDER,

		SAM_MAX
	};

	struct SamplerStateDesc
	{
		float					fAnisotropy;
		float					fLodBias;
		SamplerFilter			eFilter;
		Vec4f					vBorderColor;
		SamplerAddressingMode	eAddressingMode[3];
	};
	//////////////////////////////////////////////////////////////

	// SHADERS //////////////////////////////////////////////////
	enum ShaderProgramType
	{
		SPT_NONE,

		SPT_VERTEX,
		SPT_PIXEL,

		SPT_MAX
	};

	enum InputType
	{
		IT_NONE,

		IT_VOID,
		IT_BOOL,
		IT_INT,
		IT_FLOAT,
		IT_TEXTURE,
		IT_TEXTURE1D,
		IT_TEXTURE2D,
		IT_TEXTURE3D,
		IT_TEXTURECUBE,
		IT_SAMPLER,
		IT_SAMPLER1D,
		IT_SAMPLER2D,
		IT_SAMPLER3D,
		IT_SAMPLERCUBE,

		IT_MAX
	};

	enum RegisterType
	{
		RT_NONE,

		RT_BOOL,
		RT_INT4,
		RT_FLOAT4,
		RT_SAMPLER,

		RT_MAX
	};

	struct ShaderInputDesc
	{
		std::string szName;
		InputType eInputType;
		RegisterType eRegisterType;
		unsigned int nRegisterIndex;
		unsigned int nRegisterCount;
		unsigned int nRows;
		unsigned int nColumns;
		unsigned int nArrayElements;
		unsigned int nBytes;
		unsigned int nOffsetInBytes;
	};
	/////////////////////////////////////////////////////////

	// TEXTURES /////////////////////////////////////////////
	// Specifies the format of the pixel
	enum PixelFormat
	{
		PF_NONE,

		// Small-bit color formats
		PF_R5G6B5,
		PF_A1R5G5B5,
		PF_A4R4G4B4,

		// 8-bit integer formats
		PF_A8,
		PF_L8,
		PF_A8L8,
		PF_R8G8B8,		// can be saved into file
		PF_A8R8G8B8,	// can be saved into file
		PF_A8B8G8R8,

		// 16-bit integer formats
		PF_L16,
		PF_G16R16,
		PF_A16B16G16R16,

		// 16-bit floating-point formats ('half float' channels)
		PF_R16F,
		PF_G16R16F,
		PF_A16B16G16R16F,

		// 32-bit floating-point formats ('float' channels)
		PF_R32F,
		PF_G32R32F,
		PF_A32B32G32R32F,

		// DXT compressed formats
		PF_DXT1,	// can be saved into file
		PF_DXT3,	// can be saved into file
		PF_DXT5,	// can be saved into file

		// Depth-stencil format
		PF_D24S8,

		PF_MAX		// DO NOT USE! INTERNAL USAGE ONLY!
	};

	// Specifies the type of the texture
	enum TexType
	{
		TT_1D,		// One-dimensional texture
		TT_2D,		// Two-dimensional texture
		TT_3D,		// Three-dimensional texture
		TT_CUBE,	// Cubic environment maps

		TT_MAX		// DO NOT USE! INTERNAL USAGE ONLY!
	};

	enum
	{
		// This is the maximum number of mipmap levels for a texture.
		// A 32768x32768 texture has a maximum of 16 levels.
		TEX_MAX_MIPMAP_LEVELS = 16
	};
	/////////////////////////////////////////////////////////////////////

	// VERTEX FORMATS ///////////////////////////////////////////////////
	enum
	{
		VF_MAX_ATTRIBUTES = 16,
		VF_MAX_TCOORD_UNITS = 8,
		VF_MAX_COLOR_UNITS = 2
	};

	//These flags describe the type of data which compose the vertex attributes
	enum VertexAttributeType
	{
		VAT_NONE,
		VAT_FLOAT1,
		VAT_FLOAT2,
		VAT_FLOAT3,
		VAT_FLOAT4,
		VAT_HALF2,
		VAT_HALF4,
		VAT_UBYTE4,
		VAT_SHORT2,
		VAT_SHORT4,

		VAT_MAX
	};

	//These flags describe an attribute's purpose
	enum VertexAttributeUsage
	{
		VAU_NONE,
		VAU_POSITION,
		VAU_NORMAL,
		VAU_TANGENT,
		VAU_BINORMAL,
		VAU_TEXCOORD,
		VAU_COLOR,
		VAU_BLENDINDICES,
		VAU_BLENDWEIGHT,
		VAU_FOGCOORD,
		VAU_PSIZE,

		VAU_MAX
	};

	struct VertexElement
	{
		unsigned int			nOffset;	// The element's offset in the vertex format
		VertexAttributeType		eType;		// The data type of the element
		VertexAttributeUsage	eUsage;		// The usage/semantic of the element
		unsigned int			nUsageIdx;	// The usage/semantic index of the element
	};
	//////////////////////////////////////////////////////////////////

	// DEVICE CAPS ///////////////////////////////////////////////////
	struct DeviceCaps
	{
		bool bCanAutoGenMipmaps;
		bool bDynamicTextures;
		bool bPresentIntervalImmediate; // no vsync
		bool bPresentIntervalOne; // 60Hz
		bool bPresentIntervalTwo; // 30Hz
		bool bPresentIntervalThree; // 20Hz
		bool bPresentIntervalFour; // 15Hz
		bool bMrtIndependentBitDepths;
		bool bMrtPostPixelShaderBlending;
		bool bAnisotropicFiltering;
		bool bDepthBias;
		bool bSlopeScaledDepthBias;
		bool bMipmapLodBias;
		bool bWBuffer;
		bool bTexturePow2;
		unsigned int nMaxTextureWidth;
		unsigned int nMaxTextureHeight;
		unsigned int nMaxTextureDepth;
		unsigned int nVertexShaderVersionMajor;
		unsigned int nVertexShaderVersionMinor;
		unsigned int nPixelShaderVersionMajor;
		unsigned int nPixelShaderVersionMinor;
		unsigned int nNumSimultaneousRTs;

		struct ScreenFormat
		{
			unsigned int nWidth;
			unsigned int nHeight;
			unsigned int nRefreshRate;
		};
		std::vector<ScreenFormat> arrSupportedScreenFormats;
	};
}

#endif // RENDERDATA_H