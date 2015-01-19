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

#include "ShaderProgram.h"
using namespace LibRendererDll;

ShaderProgram::ShaderProgram(ShaderProgramType programType)
	: m_eProgramType(programType)
{}

ShaderProgram::~ShaderProgram()
{}

void ShaderProgram::SetValue(const RegisterType registerType, const unsigned int registerIndex, const void* const data, const unsigned int registerCount)
{
	switch (registerType)
	{
	case RT_BOOL:
		SetBool(registerIndex, (const bool* const)data, registerCount);
		break;
	case RT_INT4:
		SetInt(registerIndex, (const int* const)data, registerCount);
		break;
	case RT_FLOAT4:
		SetFloat(registerIndex, (const float* const)data, registerCount);
	}
}