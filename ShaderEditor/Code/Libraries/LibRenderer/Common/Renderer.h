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
#ifndef RENDERER_H
#define RENDERER_H

#ifdef LIBRENDERER_EXPORTS
#define LIBRENDERER_DLL __declspec(dllexport) 
#else
#define LIBRENDERER_DLL __declspec(dllimport) 
#endif

#include "RenderData.h"

namespace LibRendererDll
{
	class RenderState;
	class SamplerState;

	// This is the platform independent renderer interface
	class Renderer
	{
	public:
		/* Create an instance of the Renderer object based on API of choice */
		static	LIBRENDERER_DLL			void					CreateInstance(API eApi);
		/* Destroys the instance of the Renderer object */
		static	LIBRENDERER_DLL			void					DestroyInstance();
		/* Retrieves the instance of the Renderer object */
		static	LIBRENDERER_DLL			Renderer*				GetInstance() { return m_pInstance; }
		static	LIBRENDERER_DLL			API						GetAPI() { return m_eAPI; }

		/* After you create an application window, you are ready to initialize the graphics
		object that you will use to render the scene. This process includes creating the
		object, setting the presentation parameters, and finally creating the device. */
		virtual	LIBRENDERER_DLL			void					Initialize(void* hWnd) = 0;

		/* Set viewport size and offset */
		virtual	LIBRENDERER_DLL			void					SetViewport(const Vec2i size, const Vec2i offset = Vec2i(0, 0)) { m_vViewportSize = size; m_vViewportOffset = offset; }
		
		/* Render the scene */
		virtual	LIBRENDERER_DLL			void					RenderScene() = 0;

				LIBRENDERER_DLL		RenderState*				GetRenderStateManager() { return m_pRenderState; }
				LIBRENDERER_DLL		DeviceCaps					GetDeviceCaps() { return m_tDeviceCaps; }

	protected:
							Renderer();
		virtual				~Renderer();

				Vec2i		m_vViewportSize;
				Vec2i		m_vViewportOffset;
			RenderState*	m_pRenderState;
			SamplerState*	m_pSamplerState;
			DeviceCaps		m_tDeviceCaps;

		static	Renderer*	m_pInstance;
		static	API			m_eAPI;
	};
}

#endif	//RENDERER_H
