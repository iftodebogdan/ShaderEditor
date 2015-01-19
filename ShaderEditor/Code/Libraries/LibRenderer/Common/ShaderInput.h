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
#ifndef SHADERINPUT_H
#define SHADERINPUT_H

#include <gmtl\gmtl.h>
using namespace gmtl;

#include "Buffer.h"
#include "ShaderTemplate.h"
#include "RenderData.h"

namespace LibRendererDll
{
	class Texture;

	class ShaderInput : public Buffer
	{
	public:
		ShaderInput(ShaderTemplate* const shaderTemplate);
		virtual ~ShaderInput();

		void SetTexture(const unsigned int handle, const Texture* const tex);

		// BOOLS
		void SetBoolArray(const unsigned int handle, const bool* const data);
		void SetBool(const unsigned int handle, const bool data) { SetBoolArray(handle, &data); }
		void SetBool2(const unsigned int handle, const Vec<bool, 2> data) { SetBoolArray(handle, data.getData()); }
		void SetBool3(const unsigned int handle, const Vec<bool, 3> data) { SetBoolArray(handle, data.getData()); }
		void SetBool4(const unsigned int handle, const Vec<bool, 4> data) { SetBoolArray(handle, data.getData()); }
		///////////////////////////////////////////////////

		// FLOATS
		void SetFloatArray(const unsigned int handle, const float* const data);
		void SetFloat(const unsigned int handle, const float data) { SetFloatArray(handle, &data); }
		void SetFloat2(const unsigned int handle, const Vec2f data) { SetFloatArray(handle, data.getData()); }
		void SetFloat3(const unsigned int handle, const Vec3f data) { SetFloatArray(handle, data.getData()); }
		void SetFloat4(const unsigned int handle, const Vec4f data) { SetFloatArray(handle, data.getData()); }
		////////////////////////////////////////////////////

		// INTS
		void SetIntArray(const unsigned int handle, const int* const data);
		void SetInt(const unsigned int handle, const int data) { SetIntArray(handle, &data); }
		void SetInt2(const unsigned int handle, const Vec2i data) { SetIntArray(handle, data.getData()); }
		void SetInt3(const unsigned int handle, const Vec3i data) { SetIntArray(handle, data.getData()); }
		void SetInt4(const unsigned int handle, const Vec4i data) { SetIntArray(handle, data.getData()); }
		/////////////////////////////////////////////////////

		// MATRICES
		template < typename T, const unsigned int ROWS, const unsigned int COLS >
		inline void SetMatrix(const unsigned int handle, const Matrix<T, ROWS, COLS> data);
		template < typename T, const unsigned int ROWS, const unsigned int COLS >
		inline void SetMatrixArray(const unsigned int handle, const Matrix<T, ROWS, COLS>* const data);
		//////////////////////////////////////////////////////

		// FLOAT MATRICES
		template < const unsigned int ROWS, const unsigned int COLS >
		inline void SetMatrix(const unsigned int handle, const Matrix<float, ROWS, COLS> data);
		template < const unsigned int ROWS, const unsigned int COLS >
		inline void SetMatrixArray(const unsigned int handle, const Matrix<float, ROWS, COLS>* const data);
		
		void SetMatrix3x3(const unsigned int handle, const Matrix33f data);
		void SetMatrix4x4(const unsigned int handle, const Matrix44f data);
		//////////////////////////////////////////////////////////////////
		
		Texture* const GetTexture(const unsigned int handle) const;

		const bool GetBool(const unsigned int handle, const unsigned int idx = 0) const;
		const Vec<bool, 2> GetBool2(const unsigned int handle, const unsigned int idx = 0) const;
		const Vec<bool, 3> GetBool3(const unsigned int handle, const unsigned int idx = 0) const;
		const Vec<bool, 4> GetBool4(const unsigned int handle, const unsigned int idx = 0) const;

		const float GetFloat(const unsigned int handle, const unsigned int idx = 0) const;
		const Vec2f GetFloat2(const unsigned int handle, const unsigned int idx = 0) const;
		const Vec3f GetFloat3(const unsigned int handle, const unsigned int idx = 0) const;
		const Vec4f GetFloat4(const unsigned int handle, const unsigned int idx = 0) const;

		const int GetInt(const unsigned int handle, const unsigned int idx = 0) const;
		const Vec2i GetInt2(const unsigned int handle, const unsigned int idx = 0) const;
		const Vec3i GetInt3(const unsigned int handle, const unsigned int idx = 0) const;
		const Vec4i GetInt4(const unsigned int handle, const unsigned int idx = 0) const;

		template < typename T, const unsigned int ROWS, const unsigned int COLS >
		inline const Matrix<T, ROWS, COLS> GetMatrix(const unsigned int handle, const unsigned int idx = 0) const;
		template < const unsigned int ROWS, const unsigned int COLS >
		inline const Matrix<float, ROWS, COLS> GetMatrix(const unsigned int handle, const unsigned int idx = 0) const;
		const Matrix33f GetMatrix3x3(const unsigned int handle, const unsigned int idx = 0) const;
		const Matrix44f GetMatrix4x4(const unsigned int handle, const unsigned int idx = 0) const;

		const unsigned int GetInputCount() const { return (unsigned int)m_pShaderTemplate->m_arrInputDesc.size(); }
		const bool GetInputHandleByName(const char* const inputName, unsigned int& inputHandle) const;
		const ShaderInputDesc GetInputDesc(const unsigned int handle) const { return m_pShaderTemplate->m_arrInputDesc[handle]; }

	protected:
		ShaderTemplate* m_pShaderTemplate;
	};

#include "ShaderInput.inl"
}

#endif // SHADERINPUT_H