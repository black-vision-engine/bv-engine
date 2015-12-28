#pragma once

#include "Engine/Graphics/Shaders/TextureSampler.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

namespace bv
{

class Renderer;

class PdrSampler
{
private:

    GLuint          m_samplerName;
    GLuint          m_prevSamplerID;

public:

			        PdrSampler		();
			        PdrSampler		( const TextureSampler * sampler );
                    ~PdrSampler		();

	void			SetSamplerParameters	( const TextureSampler * sampler );

    GLuint			Bind            ( unsigned int textureUnit );
    void            Unbind          ( unsigned int textureUnit );


    GLuint          GetSamplerName  () const;

};

} // bv
