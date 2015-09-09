#pragma once

#include "Interfaces/IAppLogicPrototype.h"
#include "../BaseTextureTest.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture2D.h"
#include "Engine\Graphics\Renderers\OGLRenderer\PdrSampler.h"

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"

namespace bv {

class Renderer;

class Texture2DTest : public BaseTextureTest
{
private:
	glm::uvec2				m_texSize;

	Texture2D *				m_texture;
	PdrTexture2D *			m_pdrTexture;

	TextureSampler *		m_sampler;
	PdrSampler *			m_pdrSampler;

	GLSLProgram				m_program;
    VBORect					m_rct;

	Renderer *				m_renderer;
	glm::mat4				m_mvp;

public:

					Texture2DTest	();
	virtual			~Texture2DTest	();

    virtual     void    Initialize	( Renderer *, glm::mat4 projMat )	override;
    virtual     void    Render      ()								override;

private:
	void				InitTexture	();

};

} //bv
