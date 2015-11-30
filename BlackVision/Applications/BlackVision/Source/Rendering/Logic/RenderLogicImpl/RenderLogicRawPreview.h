#pragma once

#include "Rendering/Logic/RenderLogicImpl/RenderLogicImpl.h"


namespace bv {

class RenderLogicRawPreview : public RenderLogicImpl
{
public:

                    RenderLogicRawPreview   ( bool videoCardEnabled );

    virtual void    RenderFrame             () override;

};

} // bv
