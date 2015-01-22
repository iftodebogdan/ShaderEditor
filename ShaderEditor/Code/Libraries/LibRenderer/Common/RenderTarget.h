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
#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "RenderData.h"
#include "Texture.h"

namespace LibRendererDll
{
	class RenderTarget
	{
	public:
		RenderTarget(const unsigned int targetCount, PixelFormat pixelFormat,
			const unsigned int width, const unsigned int height, bool hasMipmaps, bool hasDepthStencil);
		virtual ~RenderTarget();

		virtual void	Enable() = 0;
		virtual void	Disable() = 0;
		virtual void	CopyColorBuffer(const unsigned int colorBufferIdx, Texture*& texture) = 0;

		const unsigned int		GetTargetCount() const { return m_nTargetCount; }
		const PixelFormat		GetFormat() const { return m_pColorBuffer[0]->GetTextureFormat(); }
		const unsigned int		GetWidth() const { return m_pColorBuffer[0]->GetWidth(); }
		const unsigned int		GetHeight() const { return m_pColorBuffer[0]->GetHeight(); }
		const Texture* const	GetColorBuffer(const unsigned int colorBufferIdx) const { assert(colorBufferIdx < m_nTargetCount); return m_pColorBuffer[colorBufferIdx]; }
		const Texture* const	GetDepthBuffer() const { return m_pDepthBuffer; }
		const bool				HasMipmaps() const { return m_bHasMipmaps; }
		const bool				HasDepthBuffer() const { return m_pDepthBuffer != 0; }

	protected:
		unsigned int	m_nTargetCount;
		bool			m_bHasMipmaps;

		// The depth buffer is shared across all color buffer,
		// because all the color writes occur in the same pixel
		// shader and the depth is computed only once
		Texture**		m_pColorBuffer;
		Texture*		m_pDepthBuffer;
	};
}

#endif // RENDERTARGET_H