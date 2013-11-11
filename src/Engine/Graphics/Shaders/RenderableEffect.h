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

public:

                        RenderableEffect    ();
                        ~RenderableEffect   ();

    int                 NumPasses           () const;

    void                AddPass             ( RenderablePass * pass );
    RenderablePass *    GetPass             ( int index );

};

}