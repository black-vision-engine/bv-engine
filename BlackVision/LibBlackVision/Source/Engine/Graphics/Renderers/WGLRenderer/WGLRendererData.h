#pragma once

#include <Windows.h>

#include "Engine/Graphics/Renderers/OGLRenderer/GLRendererData.h"


namespace bv
{

class WGLRendererData : public RendererData
{
public:

    HWND	m_WindowHandle;
    HDC		m_WindowDC;
    HGLRC	m_WindowRC;
};

} // bv
