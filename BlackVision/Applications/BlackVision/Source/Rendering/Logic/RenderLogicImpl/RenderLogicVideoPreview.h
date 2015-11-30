#pragma once

#include "Rendering/Logic/RenderLogicImpl/RenderLogicImpl.h"


namespace bv {

class RenderLogicVideoPreview : public RenderLogicImpl
{
public:

                    RenderLogicVideoPreview ( bool videoCardEnabled );

    virtual void    RenderFrame             () override;

};

} // bv
