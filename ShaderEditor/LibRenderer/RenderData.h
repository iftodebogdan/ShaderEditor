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

#include <gmtl/gmtl.h>
using namespace gmtl;

namespace LibRendererDll
{
	struct RenderData
	{
		struct RenderState
		{
			// AlphaState
			unsigned int		mAlphaBlendEnable;
			unsigned int		mAlphaSrcBlend;
			unsigned int		mAlphaDstBlend;
			unsigned int		mAlphaTestEnable;
			unsigned int		mAlphaFunc;
			unsigned int		mAlphaRef;
			unsigned int		mBlendFactor;

			// CullState
			unsigned int		mCullMode;

			// DepthState
			unsigned int		mZEnable;
			unsigned int		mZFunc;
			unsigned int		mZWriteEnable;

			// OffsetState
			unsigned int		mSlopeScaleDepthBias;
			unsigned int		mDepthBias;

			// StencilState
			unsigned int		mStencilEnable;
			unsigned int		mStencilFunc;
			unsigned int		mStencilRef;
			unsigned int		mStencilMask;
			unsigned int		mStencilWriteMask;
			unsigned int		mStencilFail;
			unsigned int		mStencilZFail;
			unsigned int		mStencilPass;

			// WireState
			unsigned int		mFillMode;
		} renderState;

		Vec2i		backBufferSize;

		struct DestRect
		{
			Vec2i topLeft; Vec2i bottomRight;
		} dstRect;

		struct Viewport
		{
			Vec2i topLeft; Vec2i sizeWH; Vec2f minMaxZ;
		} viewport;
	};
}

#endif //RENDERDATA_H