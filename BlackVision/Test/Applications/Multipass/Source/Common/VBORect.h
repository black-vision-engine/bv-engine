#pragma once

#include "Common/PrototyperCore.h"

#include "IRenderable.h"


namespace bv {

class VBORect : public IRenderable
{
private:

    GLuint  m_vaoHandle;

public:

    VBORect                 ( float w, float h );
    ~VBORect                ();

    virtual void    Render  () override;

};

} //bv
