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

#include "RenderTarget.h"
#include "RendererDX9.h"
using namespace LibRendererDll;

RenderTarget::RenderTarget(const unsigned int targetCount, PixelFormat pixelFormat,
	const unsigned int width, const unsigned int height, bool hasMipmaps, bool hasDepthStencil)
	: m_nTargetCount(targetCount)
	, m_bHasMipmaps(hasMipmaps)
	, m_pColorBuffer(nullptr)
	, m_pDepthBuffer(nullptr)
{
	assert(targetCount > 0);
	assert(targetCount <= RendererDX9::GetInstance()->GetDeviceCaps().nNumSimultaneousRTs);

	// Actual texture creation will be done individually in each API implementation.
	// On the other hand, since the Renderer class is a singleton, we could retrieve
	// it and create the texture from here, which would be more intuitive;
	// Then again, if we ever wanted to support multiple renderers at the same time
	// (e.g. DX9 and OpenGL rendering in the same window), then we can't create
	// the textures from here since the renderer wouldn't be a singleton anymore.
	// TODO: Decide between code consistency and flexibility.
	m_pColorBuffer = new Texture*[m_nTargetCount];
	memset(m_pColorBuffer, 0, m_nTargetCount * sizeof(Texture*));
}

RenderTarget::~RenderTarget()
{
	if (m_pColorBuffer)
		delete[] m_pColorBuffer;
}