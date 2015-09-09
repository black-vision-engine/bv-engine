#pragma once

#include "Interfaces/IAppLogicPrototype.h"
#include "../BaseTextureTest.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture1D.h"
#include "Engine\Graphics\Renderers\OGLRenderer\PdrSampler.h"

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"

namespace bv {

class Renderer;

class Texture1DTest : public BaseTextureTest
{
private:
	glm::uvec2				m_texSize;

	Texture1D *				m_texture;
	PdrTexture1D *			m_pdrTexture;

	TextureSampler *		m_sampler;
	PdrSampler *			m_pdrSampler;

	GLSLProgram				m_program;
    VBORect					m_rct;

	Renderer *				m_renderer;
	glm::mat4				m_mvp;

public:

					Texture1DTest	();
	virtual			~Texture1DTest	();

    virtual     void    Initialize	( Renderer *, glm::mat4 projMat )	override;
    virtual     void    Render      ()								override;

private:
	void				InitTexture	();

};

} //bv
