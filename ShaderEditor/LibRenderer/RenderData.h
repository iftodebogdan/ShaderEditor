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