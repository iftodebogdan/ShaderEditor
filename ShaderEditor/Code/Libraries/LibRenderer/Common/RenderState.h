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
#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <gmtl/gmtl.h>
using namespace gmtl;

#include "RenderData.h"

namespace LibRendererDll
{
	class Renderer;

	class RenderState
	{
	public:
		virtual const bool SetAlphaBlendEnable(const bool enabled) { m_bAlphaBlendEnable = enabled; return true; }
		virtual const bool SetAlphaSrcBlend(const Blend alphaSrc) { m_eAlphaSrcBlend = alphaSrc; return true; }
		virtual const bool SetAlphaDstBlend(const Blend alphaDst) { m_eAlphaDstBlend = alphaDst; return true; }
		virtual const bool SetAlphaTestEnable(const bool enabled) { m_bAlphaTestEnable = enabled; return true; }
		virtual const bool SetAlphaFunc(const Cmp alphaFunc) { m_eAlphaFunc = alphaFunc; return true; }
		// normAlphaRef is normalized (i.e. between [0, 1])
		virtual const bool SetAlphaRef(const float normAlphaRef) { m_fAlphaRef = normAlphaRef; return true; }
		// Color components are normalized (i.e. between [0, 1])
		virtual const bool SetBlendFactor(const Vec4f rgba) { m_vBlendFactor = rgba; return true; }

		virtual const bool SetCullMode(const Cull cullMode) { m_eCullMode = cullMode; return true; }

		virtual const bool SetZEnable(const ZBuffer enabled) { m_eZEnable = enabled; return true; }
		virtual const bool SetZFunc(const Cmp zFunc) { m_eZFunc = zFunc; return true; }
		virtual const bool SetZWriteEnabled(const bool enabled) { m_bZWriteEnable = enabled; return true; }

		virtual const bool SetSlopeScaleDepthBias(const float scale) { m_fSlopeScaleDepthBias = scale; return true; }
		virtual const bool SetDepthBias(const float bias) { m_fDepthBias = bias; return true; }

		virtual const bool SetStencilEnable(const bool enabled) { m_bStencilEnable = enabled; return true; }
		virtual const bool SetStencilFunc(const Cmp stencilFunc) { m_eStencilFunc = stencilFunc; return true; }
		virtual const bool SetStencilRef(const unsigned long stencilRef) { m_lStencilRef = stencilRef; return true; }
		virtual const bool SetStencilMask(const unsigned long stencilMask) { m_lStencilMask = stencilMask; return true; }
		virtual const bool SetStencilWriteMask(const unsigned long stencilWriteMask) { m_lStencilWriteMask = stencilWriteMask; return true; }
		virtual const bool SetStencilFail(const StencilOp stencilFail) { m_eStencilFail = stencilFail; return true; }
		virtual const bool SetStencilZFail(const StencilOp stencilZFail) { m_eStencilZFail = stencilZFail; return true; }
		virtual const bool SetStencilPass(const StencilOp stencilPass) { m_eStencilPass = stencilPass; return true; }

		virtual const bool SetFillMode(const Fill fillMode) { m_eFillMode = fillMode; return true; }


		const bool GetAlphaBlendEnable() const { return m_bAlphaBlendEnable; }
		const Blend GetAlphaSrcBlend() const { return m_eAlphaSrcBlend; }
		const Blend GetAlphaDstBlend() const { return m_eAlphaDstBlend; }
		const bool GetAlphaTestEnable() const { return m_bAlphaTestEnable; }
		const Cmp GetAlphaFunc() const { return m_eAlphaFunc; }
		const float GetAlphaRef() const { return m_fAlphaRef; }
		const Vec4f GetBlendFactor() const { return m_vBlendFactor; }

		const Cull GetCullMode() const { return m_eCullMode; }

		const ZBuffer GetZEnable() const { return m_eZEnable; }
		const Cmp GetZFunc() const { return m_eZFunc; }
		const bool GetZWriteEnabled() const { return m_bZWriteEnable; }

		const float GetSlopeScaleDepthBias() const { return m_fSlopeScaleDepthBias; }
		const float GetDepthBias() const { return m_fDepthBias; }

		const bool GetStencilEnable() const { return m_bStencilEnable; }
		const Cmp GetStencilFunc() const { return m_eStencilFunc; }
		const unsigned long GetStencilRef() const { return m_lStencilRef; }
		const unsigned long GetStencilMask() const { return m_lStencilMask; }
		const unsigned long GetStencilWriteMask() const { return m_lStencilWriteMask; }
		const StencilOp GetStencilFail() const { return m_eStencilFail; }
		const StencilOp GetStencilZFail() const { return m_eStencilZFail; }
		const StencilOp GetStencilPass() const { return m_eStencilPass; }

		const Fill GetFillMode() const { return m_eFillMode; }


	protected:
		RenderState();
		virtual ~RenderState();

		// Alpha states
		bool			m_bAlphaBlendEnable;
		Blend			m_eAlphaSrcBlend;
		Blend			m_eAlphaDstBlend;
		bool			m_bAlphaTestEnable;
		Cmp				m_eAlphaFunc;
		float			m_fAlphaRef;
		Vec4f			m_vBlendFactor;

		// Culling states
		Cull			m_eCullMode;

		// Depth states
		ZBuffer			m_eZEnable;
		Cmp				m_eZFunc;
		bool			m_bZWriteEnable;

		// Depth bias states
		float			m_fSlopeScaleDepthBias;
		float			m_fDepthBias;

		// Stencil states
		bool			m_bStencilEnable;
		Cmp				m_eStencilFunc;
		unsigned long	m_lStencilRef;
		unsigned long	m_lStencilMask;
		unsigned long	m_lStencilWriteMask;
		StencilOp		m_eStencilFail;
		StencilOp		m_eStencilZFail;
		StencilOp		m_eStencilPass;

		// Fill mode
		Fill			m_eFillMode;

		friend class Renderer;
	};
}

#endif //RENDERSTATE_H