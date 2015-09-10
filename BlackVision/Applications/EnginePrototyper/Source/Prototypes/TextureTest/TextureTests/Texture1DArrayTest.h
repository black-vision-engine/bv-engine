#pragma once

#include "Interfaces/IAppLogicPrototype.h"
#include "../BaseTextureTest.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture1DArray.h"
#include "Engine\Graphics\Renderers\OGLRenderer\PdrSampler.h"

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"

namespace bv {

class Renderer;

class Texture1DArrayTest : public BaseTextureTest
{
private:
	glm::uvec2				m_texSize;

	Texture1DArray *		m_texture;
	PdrTexture1DArray *		m_pdrTexture;

	TextureSampler *		m_sampler;
	PdrSampler *			m_pdrSampler;

    VBORect					m_rct;

	Renderer *				m_renderer;
	std::vector< glm::mat4 >m_mvp;

	unsigned int			m_numLayer;

public:

					Texture1DArrayTest	();
	virtual			~Texture1DArrayTest	();

    virtual     void    Initialize	( Renderer *, glm::mat4 mvp )	override;
    virtual     void    Render      ()								override;
    virtual     void    Update      ( TimeType t )					override;

private:
	void				InitTexture	();

};

} //bv