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

#include "Texture.h"
using namespace LibRendererDll;

const unsigned int Texture::ms_nDimensionCount[Texture::TT_MAX] =
{
	1,	// TT_1D
	2,	// TT_2D
	3,	// TT_3D
	2	// TT_CUBE
};

const unsigned int Texture::ms_nPixelSize[Texture::TF_MAX] =
{
	0,	// TF_NONE
	2,	// TF_R5G6B5
	2,	// TF_A1R5G5B5
	2,	// TF_A4R4G4B4
	1,	// TF_A8
	1,	// TF_L8
	2,	// TF_A8L8
	3,	// TF_R8G8B8
	4,	// TF_A8R8G8B8
	4,	// TF_A8B8G8R8
	2,	// TF_L16
	4,	// TF_G16R16
	8,	// TF_A16B16G16R16
	2,	// TF_R16F
	4,	// TF_G16R16F
	8,	// TF_A16B16G16R16F
	4,	// TF_R32F
	8,	// TF_G32R32F
	16,	// TF_A32B32G32R32F,
	0,	// TF_DXT1 (calculated at runtime)
	0,	// TF_DXT3 (calculated at runtime)
	0,	// TF_DXT5 (calculated at runtime)
	4	// TF_D24S8
};

const bool Texture::ms_bIsMipmapable[Texture::TF_MAX] =
{
	false,	// TF_NONE
	true,	// TF_R5G6B5
	true,	// TF_A1R5G5B5
	true,	// TF_A4R4G4B4
	true,	// TF_A8
	true,	// TF_L8
	true,	// TF_A8L8
	true,	// TF_R8G8B8
	true,	// TF_A8R8G8B8
	true,	// TF_A8B8G8R8
	true,	// TF_L16
	true,	// TF_G16R16
	true,	// TF_A16B16G16R16
	true,	// TF_R16F
	true,	// TF_G16R16F
	true,	// TF_A16B16G16R16F
	false,	// TF_R32F
	false,	// TF_G32R32F
	false,	// TF_A32B32G32R32F,
	true,	// TF_DXT1 (calculated at runtime)
	true,	// TF_DXT3 (calculated at runtime)
	true,	// TF_DXT5 (calculated at runtime)
	false	// TF_D24S8
};

Texture::Texture(
	const TexFormat texFormat, const TexType texType,
	const unsigned int sizeX, const unsigned int sizeY, const unsigned int sizeZ,
	const unsigned int mipmapLevelCount, const BufferUsage usage)
	: Buffer(0, 0, usage)	// We don't know at this stage how much memory we need
	, m_eTexFormat(texFormat)
	, m_eTexType(texType)
	, m_nMipmapLevelCount(mipmapLevelCount)
	, m_pTempBuffer(nullptr)
{
	assert(sizeX > 0);
	assert(sizeY > 0);
	assert(sizeZ > 0);

	m_nDimensionCount = GetDimensionCount(texType);
	m_nElementSize = GetPixelSize(texFormat);

	m_nDimension[0][0] = sizeX;

	if (m_nDimensionCount >= 2)
	{
		if (m_eTexType == TT_CUBE)
			m_nDimension[0][1] = sizeX;	// Cube maps have the same size on X and Y axes ... they're cube faces ...
		else
			m_nDimension[0][1] = sizeY;
	}
	else
		m_nDimension[0][1] = 1;	// 1D textures have a height of one texel

	if (m_nDimensionCount == 3)
		m_nDimension[0][2] = sizeZ;
	else
		m_nDimension[0][2] = 1;	// 1D or 2D textures have a depth of one texel

	// Calculate mipmap properties (dimensions, memory used, etc.)
	ComputeMipmapProperties();

	// Allocate memory for our texture
	m_pData = new byte[m_nSize];
}

Texture::~Texture()
{}

void Texture::ComputeMipmapProperties()
{
	if (!IsMipmapable())
	{
		m_nMipmapLevelCount = 1;
	}
	else
	{
		// _BitScanReverse() is an intrinsic function which is specific to MSVC.
		// It searches from the most significant bit to the least significant bit
		// and returns the position of the first set bit (1). This value coincides
		// with the integer approximation of logarithm to the base 2 of the value
		// we feed to _BitScanReverse(), which in turn coincides with the maximum
		// number of mipmaps we can have for that texture size - 1 (because math!).
		// It's fast, but most likely not cross-platform so when the time comes,
		// a replacement will have to be found.
		unsigned int maxMipmapLevelsX, maxMipmapLevelsY, maxMipmapLevelsZ;
		_BitScanReverse((unsigned long*)&maxMipmapLevelsX, m_nDimension[0][0]);
		_BitScanReverse((unsigned long*)&maxMipmapLevelsY, m_nDimension[0][1]);
		_BitScanReverse((unsigned long*)&maxMipmapLevelsZ, m_nDimension[0][2]);
		unsigned int maxMipmapLevels = (unsigned int)Math::Max(maxMipmapLevelsX, maxMipmapLevelsY, maxMipmapLevelsX) + 1;

		if (m_nMipmapLevelCount == 0)
			m_nMipmapLevelCount = maxMipmapLevels;
		assert(m_nMipmapLevelCount <= maxMipmapLevels && m_nMipmapLevelCount > 0);
	}

	// Now we calculate and save the size of each mipmap
	unsigned int totalPixelCount = 0;
	unsigned int sizeX = m_nDimension[0][0];
	unsigned int sizeY = m_nDimension[0][1];
	unsigned int sizeZ = m_nDimension[0][2];
	for (unsigned int level = 0; level < m_nMipmapLevelCount; level++)
	{
		totalPixelCount += sizeX * sizeY * sizeZ;
		m_nDimension[level][0] = sizeX;
		m_nDimension[level][1] = sizeY;
		m_nDimension[level][2] = sizeZ;

		if (sizeX > 1)
			sizeX /= 2;
		if (sizeY > 1)
			sizeY /= 2;
		if (sizeZ > 1)
			sizeZ /= 2;
	}
	// Save the total number of pixels so that we can calculate the space
	// required to store the texture in memory
	m_nElementCount = totalPixelCount * (m_eTexType == TT_CUBE ? 6u : 1u);

	// Calculate the size in memory of each mipmap level
	unsigned int totalByteCount = 0;
	m_nMipmapLevelOffset[0] = 0; // Initialize the first element of the array
	for (unsigned int level = 0; level < m_nMipmapLevelCount; level++)
	{
		m_nMipmapLevelByteCount[level] =
			m_nDimension[level][0] *
			m_nDimension[level][1] *
			m_nDimension[level][2] *
			GetPixelSize();
		if (level < m_nMipmapLevelCount - 1)
			m_nMipmapLevelOffset[level + 1] = m_nMipmapLevelOffset[level] + m_nMipmapLevelByteCount[level];
		totalByteCount += m_nMipmapLevelByteCount[level];
	}
	// Calculate the total memory space required to store the texture
	m_nSize = totalByteCount * (m_eTexType == TT_CUBE ? 6u : 1u);
}