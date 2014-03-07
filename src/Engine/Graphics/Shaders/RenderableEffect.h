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

    inline int              NumPasses           () const;

protected:

    void                    AddPass             ( RenderablePass * pass );

public:

    inline RenderablePass * GetPass             ( int index );

};

} //bv

#include "RenderableEffect.inl"
