//////////////////////////////////////////////////////////////////////////
// This file is part of the "LibRenderer" 3D graphics library           //
//                                                                      //
// Copyright (C) 2014 - Iftode Bogdan-Marius <iftode.bogdan@gmail.com>  //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation: either version 3 of the License: or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful:      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not: see <http://www.gnu.org/licenses/>. //
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "ShaderProgramDX9.h"
#include "RendererDX9.h"
#include "TextureDX9.h"
using namespace LibRendererDll;

#define CONST_MAX_ARRAY_SIZE 16;

ShaderProgramDX9::ShaderProgramDX9(const ShaderProgram::ShaderProgramType programType,
	const char* srcData, const char* entryPoint, const char* profile)
	: ShaderProgram(programType)
	, m_pVertexShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_pConstantTable(nullptr)
{
	if (strlen(srcData))
		Compile(srcData, nullptr, entryPoint, profile);
}

ShaderProgramDX9::~ShaderProgramDX9()
{
	if (m_pVertexShader)
		m_pVertexShader->Release();
	if (m_pPixelShader)
		m_pPixelShader->Release();
	if (m_pConstantTable)
		m_pConstantTable->Release();
}

void ShaderProgramDX9::Enable()
{
	assert(m_pVertexShader || m_pPixelShader);
	
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();
	HRESULT hr;
	switch (m_eProgramType)
	{
	case SPT_VERTEX:
		hr = device->SetVertexShader(m_pVertexShader);
		break;
	case SPT_PIXEL:
		hr = device->SetPixelShader(m_pPixelShader);
	}
	assert(SUCCEEDED(hr));
}

void ShaderProgramDX9::Disable()
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();
	HRESULT hr;

#ifdef _DEBUG
	IDirect3DVertexShader9* activeVShader = nullptr;
	IDirect3DPixelShader9* activePShader = nullptr;
	switch (m_eProgramType)
	{
	case SPT_VERTEX:
		hr = device->GetVertexShader(&activeVShader);
		assert(SUCCEEDED(hr));
		assert(activeVShader == m_pVertexShader);
		activeVShader->Release();
		break;
	case SPT_PIXEL:
		hr = device->GetPixelShader(&activePShader);
		assert(SUCCEEDED(hr));
		assert(activePShader == m_pPixelShader);
		activePShader->Release();
	}
#endif

	switch (m_eProgramType)
	{
	case SPT_VERTEX:
		hr = device->SetVertexShader(0);
		break;
	case SPT_PIXEL:
		hr = device->SetPixelShader(0);
	}
	assert(SUCCEEDED(hr));
}

const bool ShaderProgramDX9::Compile(const char* srcData, char* const errors, const char* entryPoint, const char* profile)
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();

	switch (m_eProgramType)
	{
	case ShaderProgram::SPT_VERTEX:
		if (strlen(profile) == 0)
			profile = "vs_3_0";
		if (strlen(entryPoint) == 0)
			entryPoint = "vsmain";
		break;
	case ShaderProgram::SPT_PIXEL:
		if (strlen(profile) == 0)
			profile = "ps_3_0";
		if (strlen(entryPoint) == 0)
			entryPoint = "psmain";
	}
	LPD3DXBUFFER compiledData = nullptr;
	LPD3DXBUFFER errorMsg = nullptr;
	HRESULT hr = D3DXCompileShader(srcData, (UINT)strlen(srcData), NULL, NULL, entryPoint, profile,
		NULL, &compiledData, &errorMsg, &m_pConstantTable);

#ifdef _DEBUG
	if (errorMsg)
	{
		DWORD size = errorMsg->GetBufferSize();
		const char* data = (const char*)errorMsg->GetBufferPointer();
		cout << endl << data << endl;
		assert(compiledData);
	}
#endif

	if (errors)
		strcpy_s(errors, (int)errorMsg->GetBufferSize(), (const char*)errorMsg->GetBufferPointer());

	if (FAILED(hr))
		return false;

	switch (m_eProgramType)
	{
	case ShaderProgram::SPT_VERTEX:
		if (m_pVertexShader)
			m_pVertexShader->Release();
		hr = device->CreateVertexShader((DWORD*)(compiledData->GetBufferPointer()), &m_pVertexShader);
		break;
	case ShaderProgram::SPT_PIXEL:
		if (m_pPixelShader)
			m_pPixelShader->Release();
		hr = device->CreatePixelShader((DWORD*)(compiledData->GetBufferPointer()), &m_pPixelShader);
	}
	assert(SUCCEEDED(hr));

	if (compiledData)
		compiledData->Release();
	if (errorMsg)
		errorMsg->Release();

	return true;
}

const unsigned int ShaderProgramDX9::GetConstantCount() const
{
	D3DXCONSTANTTABLE_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetDesc(&constDesc);
	assert(SUCCEEDED(hr));
	
	return constDesc.Constants;
}

const char* ShaderProgramDX9::GetConstantName(const unsigned int handle) const
{
	unsigned int count = 1u;
	D3DXCONSTANT_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetConstantDesc(m_pConstantTable->GetConstant(NULL, handle), &constDesc, &count);
	assert(SUCCEEDED(hr));

	return constDesc.Name;
}

const ShaderTemplate::InputType ShaderProgramDX9::GetConstantType(const unsigned int handle) const
{
	unsigned int count = 1u;
	D3DXCONSTANT_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetConstantDesc(m_pConstantTable->GetConstant(NULL, handle), &constDesc, &count);
	assert(SUCCEEDED(hr));
	assert(count == 1);

	switch (constDesc.Type)
	{
	case D3DXPT_VOID:
		assert(false); // no structs or whatever
		return ShaderTemplate::IT_NONE;
	case D3DXPT_BOOL:
		return ShaderTemplate::IT_BOOL;
	case D3DXPT_INT:
		return ShaderTemplate::IT_INT;
	case D3DXPT_FLOAT:
		return ShaderTemplate::IT_FLOAT;
	case D3DXPT_STRING:
		assert(false); // no strings
		return ShaderTemplate::IT_NONE;
	case D3DXPT_TEXTURE:
		assert(false); // DX9 documentation isn't very clear, but apparently this is only for the Effects framework
		return ShaderTemplate::IT_TEXTURE;
	case D3DXPT_TEXTURE1D:
		assert(false); // DX9 documentation isn't very clear, but apparently this is only for the Effects framework
		return ShaderTemplate::IT_TEXTURE1D;
	case D3DXPT_TEXTURE2D:
		assert(false); // DX9 documentation isn't very clear, but apparently this is only for the Effects framework
		return ShaderTemplate::IT_TEXTURE2D;
	case D3DXPT_TEXTURE3D:
		assert(false); // DX9 documentation isn't very clear, but apparently this is only for the Effects framework
		return ShaderTemplate::IT_TEXTURE3D;
	case D3DXPT_TEXTURECUBE:
		assert(false); // DX9 documentation isn't very clear, but apparently this is only for the Effects framework
		return ShaderTemplate::IT_TEXTURECUBE;
	case D3DXPT_SAMPLER:
		return ShaderTemplate::IT_SAMPLER;
	case D3DXPT_SAMPLER1D:
		return ShaderTemplate::IT_SAMPLER1D;
	case D3DXPT_SAMPLER2D:
		return ShaderTemplate::IT_SAMPLER2D;
	case D3DXPT_SAMPLER3D:
		return ShaderTemplate::IT_SAMPLER3D;
	case D3DXPT_SAMPLERCUBE:
		return ShaderTemplate::IT_SAMPLERCUBE;
	default:
		assert(false);
	}

	return ShaderTemplate::IT_NONE;
}

const ShaderTemplate::RegisterType ShaderProgramDX9::GetConstantRegisterType(const unsigned int handle) const
{
	unsigned int count = 1u;
	D3DXCONSTANT_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetConstantDesc(m_pConstantTable->GetConstant(NULL, handle), &constDesc, &count);
	assert(SUCCEEDED(hr));
	assert(count == 1);

	switch (constDesc.RegisterSet)
	{
	case D3DXRS_BOOL:
		return ShaderTemplate::RT_BOOL;
	case D3DXRS_INT4:
		return ShaderTemplate::RT_INT4;
	case D3DXRS_FLOAT4:
		return ShaderTemplate::RT_FLOAT4;
	case D3DXRS_SAMPLER:
		return ShaderTemplate::RT_SAMPLER;
	default:
		return ShaderTemplate::RT_NONE;
	}
}

const unsigned int ShaderProgramDX9::GetConstantRegisterIndex(const unsigned int handle) const
{
	unsigned int count = 1u;
	D3DXCONSTANT_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetConstantDesc(m_pConstantTable->GetConstant(NULL, handle), &constDesc, &count);
	assert(SUCCEEDED(hr));

	return constDesc.RegisterIndex;
}

const unsigned int ShaderProgramDX9::GetConstantRegisterCount(const unsigned int handle) const
{
	unsigned int count = 1u;
	D3DXCONSTANT_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetConstantDesc(m_pConstantTable->GetConstant(NULL, handle), &constDesc, &count);
	assert(SUCCEEDED(hr));

	return constDesc.RegisterCount;
}

const unsigned int ShaderProgramDX9::GetConstantRowCount(const unsigned int handle) const
{
	unsigned int count = 1u;
	D3DXCONSTANT_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetConstantDesc(m_pConstantTable->GetConstant(NULL, handle), &constDesc, &count);
	assert(SUCCEEDED(hr));

	return constDesc.Rows;
}
const unsigned int ShaderProgramDX9::GetConstantColumnCount(const unsigned int handle) const
{
	unsigned int count = 1u;
	D3DXCONSTANT_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetConstantDesc(m_pConstantTable->GetConstant(NULL, handle), &constDesc, &count);
	assert(SUCCEEDED(hr));

	return constDesc.Columns;
}

const unsigned int ShaderProgramDX9::GetConstantArrayElementCount(const unsigned int handle) const
{
	unsigned int count = 1u;
	D3DXCONSTANT_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetConstantDesc(m_pConstantTable->GetConstant(NULL, handle), &constDesc, &count);
	assert(SUCCEEDED(hr));

	return constDesc.Elements;
}

const unsigned int ShaderProgramDX9::GetConstantStructMemberCount(const unsigned int handle) const
{
	unsigned int count = 1u;
	D3DXCONSTANT_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetConstantDesc(m_pConstantTable->GetConstant(NULL, handle), &constDesc, &count);
	assert(SUCCEEDED(hr));

	return constDesc.StructMembers;
}

const unsigned int ShaderProgramDX9::GetConstantByteCount(const unsigned int handle) const
{
	unsigned int count = 1u;
	D3DXCONSTANT_DESC constDesc;
	HRESULT hr = m_pConstantTable->GetConstantDesc(m_pConstantTable->GetConstant(NULL, handle), &constDesc, &count);
	assert(SUCCEEDED(hr));

	return constDesc.Bytes;
}

void ShaderProgramDX9::SetFloat(const unsigned int registerIndex, const float* const data, const unsigned int registerCount)
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();
	HRESULT hr;
	switch (m_eProgramType)
	{
	case SPT_VERTEX:
		hr = device->SetVertexShaderConstantF(registerIndex, data, registerCount);
		break;
	case SPT_PIXEL:
		hr = device->SetPixelShaderConstantF(registerIndex, data, registerCount);
	}
	assert(SUCCEEDED(hr));
}


void ShaderProgramDX9::SetInt(const unsigned int registerIndex, const int* const data, const unsigned int registerCount)
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();
	HRESULT hr;
	switch (m_eProgramType)
	{
	case SPT_VERTEX:
		hr = device->SetVertexShaderConstantI(registerIndex, data, registerCount);
		break;
	case SPT_PIXEL:
		hr = device->SetPixelShaderConstantI(registerIndex, data, registerCount);
	}
	assert(SUCCEEDED(hr));
}

void ShaderProgramDX9::SetBool(const unsigned int registerIndex, const bool* const data, const unsigned int registerCount)
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();
	HRESULT hr;
	
	// Convert the bool (2 bytes) array to a Microsoft BOOL (4 bytes) array
	BOOL* tempBuffer = new BOOL[registerCount];
	for (unsigned int i = 0; i < registerCount; i++)
		tempBuffer[i] = data[i];

	switch (m_eProgramType)
	{
	case SPT_VERTEX:
		hr = device->SetVertexShaderConstantB(registerIndex, tempBuffer, registerCount);
		break;
	case SPT_PIXEL:
		hr = device->SetPixelShaderConstantB(registerIndex, tempBuffer, registerCount);
	}
	delete[] tempBuffer;
	assert(SUCCEEDED(hr));
}

void ShaderProgramDX9::SetTexture(const unsigned int registerIndex, const Texture* const tex)
{
	assert(tex);
	tex->Enable(registerIndex);
}