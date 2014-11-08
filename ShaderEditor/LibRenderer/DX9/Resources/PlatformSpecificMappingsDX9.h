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
#ifndef FLAGMAPPINGDX9_H
#define FLAGMAPPINGDX9_H

#include <d3d9.h>

#include "../../Resources/Buffer.h"
#include "../../Resources/VertexFormat.h"

namespace LibRendererDll
{
	//These are used to translate platform independent flags to D3D9 format
	extern DWORD BufferUsageDX9[Buffer::BU_MAX];
	extern DWORD BufferLockingDX9[Buffer::BL_MAX];
	extern BYTE VertexAttributeTypeDX9[VertexFormat::VAT_MAX];
	extern BYTE VertexAttributeUsageDX9[VertexFormat::VAU_MAX];
}

#endif //FLAGMAPPINGDX9_H
