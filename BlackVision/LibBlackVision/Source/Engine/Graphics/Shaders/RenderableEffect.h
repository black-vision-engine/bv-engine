#pragma once

#include <vector>

#include "Engine/Graphics/Shaders/RenderablePass.h"

namespace bv {

class RenderableEffect
{
private:

    typedef std::vector< RenderablePass * > TRenderablePassVector;

private:

    TRenderablePassVector   m_passes;

protected:

                            RenderableEffect    ();
public:

                            ~RenderableEffect   ();

    inline unsigned int     NumPasses           () const;

protected:

    void                    AddPass             ( RenderablePass * pass );

public:
    
    explicit                RenderableEffect    ( RenderablePass * pass );

    inline RenderablePass * GetPass             ( unsigned int index );

};

DEFINE_PTR_TYPE(RenderableEffect)
DEFINE_CONST_PTR_TYPE(RenderableEffect)

} //bv

#include "RenderableEffect.inl"
