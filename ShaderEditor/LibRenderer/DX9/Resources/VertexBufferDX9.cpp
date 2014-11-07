#include "stdafx.h"

#include "../RendererDX9.h"
#include "PlatformSpecificMappingsDX9.h"

#include "VertexFormatDX9.h"
#include "VertexBufferDX9.h"
using namespace LibRendererDll;

VertexBufferDX9::VertexBufferDX9(VertexFormatDX9* vertexFormat, unsigned int vertexCount, BufferUsage usage)
	: VertexBuffer(vertexFormat, vertexCount, usage)
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();
	HRESULT hr = device->CreateVertexBuffer((UINT)m_nSize, BufferUsageDX9[usage], 0, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0);
	assert(hr == D3D_OK);
}

VertexBufferDX9::~VertexBufferDX9()
{
	ULONG refCount = 0;
	refCount = m_pVertexBuffer->Release();
	assert(refCount == 0);
}

void VertexBufferDX9::Enable(unsigned int offset)
{
	//Enable the proper vertex format for our vertex buffer
	assert(m_pVertexFormat != nullptr);
	m_pVertexFormat->Enable();

	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();
	HRESULT hr = device->SetStreamSource(0, m_pVertexBuffer, offset, m_pVertexFormat->GetStride());
	assert(hr == D3D_OK);
}

void VertexBufferDX9::Disable()
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();

#ifdef _DEBUG
	//Check to see if this vertex buffer is the one currently enabled
	IDirect3DVertexBuffer9* dbgBuffer = 0;
	unsigned int dbgOffset = 0;
	unsigned int dbgStride = 0;
	HRESULT dbghr = device->GetStreamSource(0, &dbgBuffer, &dbgOffset, &dbgStride);
	assert(dbghr == D3D_OK);
	assert(dbgBuffer == m_pVertexBuffer);
	ULONG refCount = 0;
	refCount = dbgBuffer->Release();
	assert(refCount == 1);
#endif

	HRESULT hr = device->SetStreamSource(0, 0, 0, 0);
	assert(hr == D3D_OK);

	//Disable our vertex format
	assert(m_pVertexFormat != nullptr);
	m_pVertexFormat->Disable();
}

void VertexBufferDX9::Lock(BufferLocking lockMode)
{
	//The pointer to the locked data is saved for future use
	assert(m_pTempBuffer == nullptr);
	HRESULT hr = m_pVertexBuffer->Lock(0, 0, &m_pTempBuffer, BufferLockingDX9[lockMode]);
	assert(hr == D3D_OK);
}

void VertexBufferDX9::Unlock()
{
	//Unlock the vertex data
	assert(m_pTempBuffer != nullptr);
	HRESULT hr = m_pVertexBuffer->Unlock();
	assert(hr == D3D_OK);
	m_pTempBuffer = nullptr;
}

void VertexBufferDX9::Update()
{
	//Copy the local changes to our vertex buffer to where the locked data is
	assert(m_pTempBuffer != nullptr);
	memcpy(m_pTempBuffer, GetData(), GetSize());
}