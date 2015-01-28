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
#ifndef MODELLOADER_H
#define MODELLOADER_H

namespace LibRendererDll
{
	class VertexFormat;
	class VertexBuffer;
	class IndexBuffer;

	class ModelLoader
	{
	private:
		ModelLoader() {}
		~ModelLoader() {}

		static const unsigned int LoadModelToVertexBuffer(const char* path, VertexFormat*& vfOut, VertexBuffer*& vbOut, IndexBuffer*& ibOut);

		friend class ResourceManager;
	};
}

#endif // MODELLOADER_H