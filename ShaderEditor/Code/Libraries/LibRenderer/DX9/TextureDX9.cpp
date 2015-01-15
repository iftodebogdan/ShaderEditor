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

#include "RendererDX9.h"
#include "MappingsDX9.h"
#include "TextureDX9.h"
using namespace LibRendererDll;

TextureDX9::TextureDX9(
	const TexFormat texFormat, const TexType texType,
	const unsigned int sizeX, const unsigned int sizeY, const unsigned int sizeZ,
	const unsigned int mipmapLevelCount, const BufferUsage usage)
	: Texture(texFormat, texType, sizeX, sizeY, sizeZ, mipmapLevelCount, usage)
	, m_pTexture(nullptr)
	, m_pTempBuffer(nullptr)
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();

	D3DPOOL pool;
	if (GetUsage() == BU_TEXTURE)
		pool = D3DPOOL_MANAGED;
	else
		pool = D3DPOOL_DEFAULT;

	HRESULT hr;
	switch (GetTextureType())
	{
	case TT_1D:
		hr = device->CreateTexture(
				GetWidth(), 1u, GetMipmapLevelCount(),
				BufferUsageDX9[m_eBufferUsage], TextureFormatDX9[m_eTexFormat],
				pool, (IDirect3DTexture9**)&m_pTexture, 0);
		break;

	case TT_2D:
		hr = device->CreateTexture(
				GetWidth(), GetHeight(), GetMipmapLevelCount(),
				BufferUsageDX9[m_eBufferUsage], TextureFormatDX9[m_eTexFormat],
				pool, (IDirect3DTexture9**)&m_pTexture, 0);
		break;

	case TT_3D:
		hr = device->CreateVolumeTexture(
				GetWidth(), GetHeight(), GetDepth(), GetMipmapLevelCount(),
				BufferUsageDX9[m_eBufferUsage], TextureFormatDX9[m_eTexFormat],
				pool, (IDirect3DVolumeTexture9**)&m_pTexture, 0);
		break;

	case TT_CUBE:
		hr = device->CreateCubeTexture(
				GetWidth(), GetMipmapLevelCount(),
				BufferUsageDX9[m_eBufferUsage], TextureFormatDX9[m_eTexFormat],
				pool, (IDirect3DCubeTexture9**)&m_pTexture, 0);
	}
	assert(SUCCEEDED(hr));
}

TextureDX9::~TextureDX9()
{
	m_pTexture->Release();
}

void TextureDX9::Enable(const unsigned int texUnit) const
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();

	HRESULT hr = device->SetTexture(texUnit, m_pTexture);
	assert(SUCCEEDED(hr));
}

void TextureDX9::Disable(const unsigned int texUnit) const
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();
	HRESULT hr;

#ifdef _DEBUG
	IDirect3DBaseTexture9* activeTex = 0;
	hr = device->GetTexture(texUnit, &activeTex);
	assert(SUCCEEDED(hr));
	assert(activeTex == m_pTexture);
	activeTex->Release();
#endif

	hr = device->SetTexture(texUnit, 0);
	assert(SUCCEEDED(hr));
}

void TextureDX9::Lock(const unsigned int mipmapLevel, const BufferLocking lockMode)
{
	assert(m_pTempBuffer == nullptr);
	D3DLOCKED_RECT rect;
	D3DLOCKED_BOX box;
	HRESULT hr;
	switch (GetTextureType())
	{
	case TT_1D:
	case TT_2D:
		hr = ((IDirect3DTexture9*)m_pTexture)->LockRect(mipmapLevel, &rect, 0, BufferLockingDX9[lockMode]);
		assert(SUCCEEDED(hr));
		m_pTempBuffer = rect.pBits;
		break;

	case TT_3D:
		hr = ((IDirect3DVolumeTexture9*)m_pTexture)->LockBox(mipmapLevel, &box, 0, BufferLockingDX9[lockMode]);
		assert(SUCCEEDED(hr));
		m_pTempBuffer = box.pBits;
	}
}

void TextureDX9::Lock(const unsigned int cubeFace, const unsigned int mipmapLevel, const BufferLocking lockMode)
{
	assert(m_pTempBuffer == nullptr);
	D3DLOCKED_RECT rect;
	HRESULT hr = ((IDirect3DCubeTexture9*)m_pTexture)->LockRect((D3DCUBEMAP_FACES)cubeFace, mipmapLevel, &rect, 0, BufferLockingDX9[lockMode]);
	assert(SUCCEEDED(hr));
	m_pTempBuffer = rect.pBits;
}

void TextureDX9::Unlock(const unsigned int mipmapLevel)
{
	assert(m_pTempBuffer != nullptr);
	HRESULT hr = ((IDirect3DTexture9*)m_pTexture)->UnlockRect((UINT)mipmapLevel);
	assert(SUCCEEDED(hr));
	m_pTempBuffer = nullptr;
}

void TextureDX9::Unlock(const unsigned int cubeFace, const unsigned int mipmapLevel)
{
	assert(m_pTempBuffer != nullptr);
	HRESULT hr = ((IDirect3DCubeTexture9*)m_pTexture)->UnlockRect((D3DCUBEMAP_FACES)cubeFace, mipmapLevel);
	assert(SUCCEEDED(hr));
	m_pTempBuffer = nullptr;
}

void TextureDX9::Update(const unsigned int mipmapLevel)
{
	assert(m_pTempBuffer != nullptr);
	memcpy(m_pTempBuffer, GetMipmapLevelData(mipmapLevel), GetMipmapLevelByteCount(mipmapLevel));
}

void TextureDX9::Update(const unsigned int cubeFace, const unsigned int mipmapLevel)
{
	assert(m_pTempBuffer != nullptr);
	memcpy(m_pTempBuffer, GetMipmapLevelData(cubeFace, mipmapLevel), GetMipmapLevelByteCount(mipmapLevel));
}