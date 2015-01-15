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
#ifndef SHADERTEMPLATE_H
#define SHADERTEMPLATE_H

#include <string>
#include <vector>

namespace LibRendererDll
{
	class ShaderProgram;
	class ShaderInput;

	class ShaderTemplate
	{
	public:
		ShaderTemplate(ShaderProgram* shaderProgram);
		virtual ~ShaderTemplate();

		void Enable(ShaderInput* shaderInput);
		void Enable(ShaderInput& shaderInput) { Enable(&shaderInput); }
		void Disable();

		enum InputType
		{
			IT_NONE,

			IT_VOID,
			IT_BOOL,
			IT_INT,
			IT_FLOAT,
			IT_TEXTURE,
			IT_TEXTURE1D,
			IT_TEXTURE2D,
			IT_TEXTURE3D,
			IT_TEXTURECUBE,
			IT_SAMPLER,
			IT_SAMPLER1D,
			IT_SAMPLER2D,
			IT_SAMPLER3D,
			IT_SAMPLERCUBE,

			IT_MAX
		};

		enum RegisterType
		{
			RT_NONE,

			RT_BOOL,
			RT_INT4,
			RT_FLOAT4,
			RT_SAMPLER,

			RT_MAX
		};

		struct InputDesc
		{
			std::string szName;
			InputType eInputType;
			RegisterType eRegisterType;
			unsigned int nRegisterIndex;
			unsigned int nRegisterCount;
			unsigned int nRows;
			unsigned int nColumns;
			unsigned int nArrayElements;
			unsigned int nBytes;
			unsigned int nOffsetInBytes;
		};

	protected:
		void DescribeShaderInputs();
		const unsigned int GetTotalNumberOfUsedRegisters() const;
		const unsigned int GetTotalSizeOfInputConstants() const;

		ShaderProgram* m_pProgram;
		std::vector<InputDesc> m_arrInputDesc;
		ShaderInput* m_pShaderInput;

		friend class ShaderInput;
	};
}

#endif // SHADERTEMPLATE_H