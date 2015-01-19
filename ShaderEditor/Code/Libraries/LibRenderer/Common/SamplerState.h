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
#ifndef SAMPLERSTATE_H
#define SAMPLERSTATE_H

#include <gmtl/gmtl.h>
using namespace gmtl;

#include "RenderData.h"

namespace LibRendererDll
{
	class Renderer;

	class SamplerState
	{
	protected:
		SamplerState();
		virtual ~SamplerState();

		SamplerStateDesc m_tCurrentState[MAX_NUM_PSAMPLERS];

	public:
		virtual const bool SetAnisotropy(const unsigned int slot, const float anisotropy) { assert(slot < MAX_NUM_PSAMPLERS); m_tCurrentState[slot].fAnisotropy = anisotropy; return true; }
		virtual const bool SetLodBias(const unsigned int slot, const float lodBias) { assert(slot < MAX_NUM_PSAMPLERS); m_tCurrentState[slot].fLodBias = lodBias; return true; }
		virtual const bool SetFilter(const unsigned int slot, const SamplerFilter filter) { assert(slot < MAX_NUM_PSAMPLERS); m_tCurrentState[slot].eFilter = filter; return true; }
		virtual const bool SetBorderColor(const unsigned int slot, const Vec4f rgba) { assert(slot < MAX_NUM_PSAMPLERS); m_tCurrentState[slot].vBorderColor = rgba; return true; }
		virtual const bool SetAddressingModeU(const unsigned int slot, const SamplerAddressingMode samU) { assert(slot < MAX_NUM_PSAMPLERS); m_tCurrentState[slot].eAddressingMode[0] = samU; return true; }
		virtual const bool SetAddressingModeV(const unsigned int slot, const SamplerAddressingMode samV) { assert(slot < MAX_NUM_PSAMPLERS); m_tCurrentState[slot].eAddressingMode[1] = samV; return true; }
		virtual const bool SetAddressingModeW(const unsigned int slot, const SamplerAddressingMode samW) { assert(slot < MAX_NUM_PSAMPLERS); m_tCurrentState[slot].eAddressingMode[2] = samW; return true; }
		virtual const bool SetAddressingMode(const unsigned int slot, const SamplerAddressingMode samUVW) { return SetAddressingModeU(slot, samUVW) && SetAddressingModeV(slot, samUVW) && SetAddressingModeW(slot, samUVW); }

		const float GetAnisotropy(const unsigned int slot) { assert(slot < MAX_NUM_PSAMPLERS); return m_tCurrentState[slot].fAnisotropy; }
		const float GetLodBias(const unsigned int slot) { assert(slot < MAX_NUM_PSAMPLERS); return m_tCurrentState[slot].fLodBias; }
		const SamplerFilter GetFilter(const unsigned int slot) { assert(slot < MAX_NUM_PSAMPLERS); return m_tCurrentState[slot].eFilter; }
		const Vec4f GetBorderColor(const unsigned int slot) { assert(slot < MAX_NUM_PSAMPLERS); return m_tCurrentState[slot].vBorderColor; }
		const SamplerAddressingMode GetAddressingModeU(const unsigned int slot) { assert(slot < MAX_NUM_PSAMPLERS); return m_tCurrentState[slot].eAddressingMode[0]; }
		const SamplerAddressingMode GetAddressingModeV(const unsigned int slot) { assert(slot < MAX_NUM_PSAMPLERS); return m_tCurrentState[slot].eAddressingMode[1]; }
		const SamplerAddressingMode GetAddressingModeW(const unsigned int slot) { assert(slot < MAX_NUM_PSAMPLERS); return m_tCurrentState[slot].eAddressingMode[2]; }
		const SamplerAddressingMode GetAddressingMode(const unsigned int slot) { return (GetAddressingModeU(slot) == GetAddressingModeV(slot) && GetAddressingModeV(slot) == GetAddressingModeW(slot)) ? GetAddressingModeU(slot) : SAM_NONE; }

		friend class Renderer;
	};
}

#endif // SAMPLERSTATE_H
