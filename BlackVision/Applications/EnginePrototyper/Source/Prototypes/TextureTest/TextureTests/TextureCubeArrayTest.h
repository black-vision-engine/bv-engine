#pragma once

#include "Interfaces/IAppLogicPrototype.h"
#include "../BaseTextureTest.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTextureCubeArray.h"
#include "Engine\Graphics\Renderers\OGLRenderer\PdrSampler.h"

#include "Common/GLSLProgram.h"
#include "../VBOCube.h"

namespace bv {

class Renderer;

class TextureCubeArrayTest : public BaseTextureTest
{
private:
	glm::uvec2				m_texSize;

	TextureCubeArray *		m_texture;
	PdrTextureCubeArray *	m_pdrTexture;

	TextureSampler *		m_sampler;
	PdrSampler *			m_pdrSampler;

    VBOCube					m_cube;

	Renderer *				m_renderer;
	std::vector< glm::mat4 >m_mvp;
	
	unsigned int			m_numLayer;

	float					m_angleX;
	float					m_angleY;

public:

					TextureCubeArrayTest	();
	virtual			~TextureCubeArrayTest	();

    virtual     void    Initialize	( Renderer *, glm::mat4 projMat )	override;
    virtual     void    Render      ()									override;
    virtual     void    Update      ( TimeType t )						override;

private:
	void				InitTexture	();

};

} //bv
