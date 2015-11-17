#pragma once

#include "../BaseTextureTest.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTextureCube.h"
#include "Engine\Graphics\Renderers\OGLRenderer\PdrSampler.h"

#include "Common/GLSLProgram.h"
#include "../VBOCube.h"

namespace bv {

class Renderer;

class TextureCubeTest : public BaseTextureTest
{
private:
	glm::uvec2				m_texSize;

	TextureCube *			m_texture;
	PdrTextureCube *		m_pdrTexture;

	TextureSampler *		m_sampler;
	PdrSampler *			m_pdrSampler;

    VBOCube					m_cube;

	Renderer *				m_renderer;
	glm::mat4				m_mvp;

	float					m_angleX;
	float					m_angleY;

public:

					TextureCubeTest	();
	virtual			~TextureCubeTest	();

    virtual     void    Initialize	( Renderer *, glm::mat4 projMat )	override;
    virtual     void    Render      ()									override;
    virtual     void    Update      ( TimeType t )						override;

private:
	void				InitTexture	();

};

} //bv
