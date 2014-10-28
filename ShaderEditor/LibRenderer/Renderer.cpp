// LibRenderer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "Renderer.h"
#include "DX9/RendererDX9.h"

using namespace LibRendererDll;

Renderer* Renderer::m_pInstance = nullptr;

const bool Renderer::CreateInstance(API eApi)
{
	if(m_pInstance)
		return false;

	switch (eApi)
	{
		case API_DX9:
			m_pInstance = new RendererDX9;
	}

	return m_pInstance != nullptr;
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

const Renderer::tRenderParameters& Renderer::GetRenderParameters() const
{
	return m_renderParams;
}