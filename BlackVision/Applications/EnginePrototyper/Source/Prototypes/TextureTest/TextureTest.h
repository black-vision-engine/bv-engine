#pragma once

#include <map>

#include "Interfaces/IBasicLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Audio/AudioRenderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "BaseTextureTest.h"

namespace bv {

class TextureTest : public IBasicLogic
{
private:
	Camera *				m_camera;
	glm::mat4				m_projMat;

	Renderer *				m_renderer;

	BaseTextureTest *		m_test;
	UInt32					m_currTestIdx;

	UInt32					m_width;
	UInt32					m_height;

public:

						TextureTest         ( Renderer * renderer, audio::AudioRenderer * audioRenderer );
						~TextureTest        ();

    virtual     void    Initialize          ()                      override;
    virtual     void    Update              ( TimeType t )          override;
    virtual     void    Render              ()                      override;
    virtual     void    Key                 ( unsigned char c )     override;
    virtual     void    Resize              ( UInt32 w, UInt32 h )  override;

    static IBasicLogicUnqPtr	Create( Renderer * renderer, audio::AudioRenderer * audioRenderer );
};

} //bv
