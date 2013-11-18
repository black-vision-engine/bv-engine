#pragma once

#include "Engine/Interfaces/IUpdater.h"


namespace bv {

class StateInstance;

namespace model
{
    class RendererContext;
}

class RenderStateUpdater : public IUpdater
{
private:

    StateInstance *                 m_out;
    const model::RendererContext *  m_in;

public:

                    RenderStateUpdater     ( StateInstance * out , const model::RendererContext * in ); 
                    ~RenderStateUpdater    ();

    virtual void    Update                  ( TimeType t );

};

}
