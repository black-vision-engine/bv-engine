#pragma once

#include "Interfaces/IAppLogicPrototype.h"
#include "../BaseTextureTest.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture3D.h"
#include "Engine\Graphics\Renderers\OGLRenderer\PdrSampler.h"

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"

namespace bv {

class Renderer;

class Texture3DTest : public BaseTextureTest
{
private:
	glm::uvec2				m_texSize;

	Texture3D *				m_texture;
	PdrTexture3D *			m_pdrTexture;

	TextureSampler *		m_sampler;
	PdrSampler *			m_pdrSampler;

    VBORect					m_rct;

	Renderer *				m_renderer;
	std::vector< glm::mat4 >m_mvp;

	unsigned int			m_depth;

public:

					Texture3DTest	();
	virtual			~Texture3DTest	();

    virtual     void    Initialize	( Renderer *, glm::mat4 projMat )	override;
    virtual     void    Render      ()									override;
    virtual     void    Update      ( TimeType t )						override;

private:
	void				InitTexture	();

};

} //bv
