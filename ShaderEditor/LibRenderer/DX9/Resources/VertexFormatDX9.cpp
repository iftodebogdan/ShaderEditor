#include "stdafx.h"

#include "../RendererDX9.h"
#include "PlatformSpecificMappingsDX9.h"
#include "VertexFormatDX9.h"
using namespace LibRendererDll;

VertexFormatDX9::VertexFormatDX9(unsigned int attributeCount)
	: VertexFormat(attributeCount)
	, m_pVertexDeclaration(nullptr)
{}

VertexFormatDX9::~VertexFormatDX9()
{
	ULONG refCount = 0;
	refCount = m_pVertexDeclaration->Release();
	assert(refCount == 0);
}

void VertexFormatDX9::Enable()
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();
	
	//Sync our vertex declaration before enabling it
	if (!m_pVertexDeclaration)
		Update();

	HRESULT hr = device->SetVertexDeclaration(m_pVertexDeclaration);
	assert(hr == D3D_OK);
}

void VertexFormatDX9::Disable()
{
	//Apparently this is not needed, resulting in a warning when using the debug DX9 libraries
	/*
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();
	HRESULT hr;

#ifdef _DEBUG
	//Check to see if this vertex declaration is the one currently enabled
	IDirect3DVertexDeclaration9* activeDecl = nullptr;
	hr = device->GetVertexDeclaration(&activeDecl);
	assert(hr == D3D_OK);
	assert(activeDecl == m_pVertexDeclaration);
	ULONG refCount = 0;
	refCount = activeDecl->Release();
	assert(refCount == 1);
#endif //_DEBUG

	hr = device->SetVertexDeclaration(0);
	assert(hr == D3D_OK);
	*/
}

void VertexFormatDX9::Update()
{
	IDirect3DDevice9* device = RendererDX9::GetInstance()->GetDevice();

	for (unsigned int i = 0; i < GetAttributeCount(); i++)
	{
		m_pVertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
		m_pVertexElements[i].Offset = (WORD)GetOffset(i);
		m_pVertexElements[i].Stream = (WORD)0;
		m_pVertexElements[i].Type = VertexAttributeTypeDX9[GetAttributeType(i)];
		m_pVertexElements[i].Usage = VertexAttributeUsageDX9[GetAttributeUsage(i)];
		m_pVertexElements[i].UsageIndex = (BYTE)GetUsageIndex(i);
	}

	m_pVertexElements[GetAttributeCount()].Method = 0;
	m_pVertexElements[GetAttributeCount()].Offset = 0;
	m_pVertexElements[GetAttributeCount()].Stream = 0xFF;
	m_pVertexElements[GetAttributeCount()].Type = D3DDECLTYPE_UNUSED;
	m_pVertexElements[GetAttributeCount()].Usage = 0;
	m_pVertexElements[GetAttributeCount()].UsageIndex = 0;

	HRESULT hr = device->CreateVertexDeclaration(m_pVertexElements, &m_pVertexDeclaration);
	assert(hr == D3D_OK);
}