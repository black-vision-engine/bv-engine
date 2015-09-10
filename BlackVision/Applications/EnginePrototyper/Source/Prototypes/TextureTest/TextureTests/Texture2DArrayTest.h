#pragma once

#include "Interfaces/IAppLogicPrototype.h"
#include "../BaseTextureTest.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture2DArray.h"
#include "Engine\Graphics\Renderers\OGLRenderer\PdrSampler.h"

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"

namespace bv {

class Renderer;

class Texture2DArrayTest : public BaseTextureTest
{
private:
	glm::uvec2				m_texSize;

	Texture2DArray *		m_texture;
	PdrTexture2DArray *		m_pdrTexture;

	TextureSampler *		m_sampler;
	PdrSampler *			m_pdrSampler;

    VBORect					m_rct;

	Renderer *				m_renderer;
	std::vector< glm::mat4 >m_mvp;

	unsigned int			m_numLayer;

public:

					Texture2DArrayTest	();
	virtual			~Texture2DArrayTest	();

    virtual     void    Initialize	( Renderer *, glm::mat4 mvp )	override;
    virtual     void    Render      ()								override;
    virtual     void    Update      ( TimeType t )					override;

private:
	void				InitTexture	();

};

} //bv