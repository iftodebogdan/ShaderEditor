// LibRenderer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "Renderer.h"
#include "DX9/RendererDX9.h"

using namespace LibRendererDll;

Renderer* Renderer::m_pInstance = nullptr;

Renderer::~Renderer()
{
}

void Renderer::CreateInstance(API eApi)
{
	switch (eApi)
	{
		case API_DX9:
			m_pInstance = new RendererDX9;
	}
}

void Renderer::DestroyInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

Renderer* Renderer::GetInstance()
{
	return m_pInstance;
}

const RenderData& Renderer::GetRenderData() const
{
	return m_RenderData;
}