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
	class Renderer
	{
	protected:
		virtual		~Renderer();

	public:
		enum API
		{
			API_DX9,
		};

		/* Create an instance of the Renderer object based on API of choice */
		static	LIBRENDERER_DLL			void					CreateInstance(API eApi);
		/* Destroys the instance of the Renderer object */
		static	LIBRENDERER_DLL			void					DestroyInstance();
		/* Retrieves the instance of the Renderer object */
		static	LIBRENDERER_DLL			Renderer*				GetInstance();

		/* After you create an application window, you are ready to initialize the graphics
		object that you will use to render the scene. This process includes creating the
		object, setting the presentation parameters, and finally creating the device. */
		virtual	LIBRENDERER_DLL			void					Initialize(void* hWnd, int backBufferWidth = 0, int backBufferHeight = 0) = 0;

		/* Get presentation parameters, global states, etc. */
		virtual	LIBRENDERER_DLL	const	RenderData&				GetRenderData() const;
		/* Set presentation parameters, global states, etc. */
		virtual	LIBRENDERER_DLL			void					SetRenderData(const RenderData& renderData) = 0;
		
		virtual	LIBRENDERER_DLL			void					RenderScene() = 0;

	protected:
		static	Renderer*			m_pInstance;
				RenderData			m_RenderData;
	};
}

#endif	//RENDERER_H
