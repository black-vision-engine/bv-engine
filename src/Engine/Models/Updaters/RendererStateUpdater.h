#pragma once

#include "IUpdater.h"

namespace bv {

//TODO: forward declarations from bv namespace go here

namespace model
{
    //TODO: forward declarations from model namespace go here
}

class RenderStateUpdater : public IUpdater
{
private:


public:

                    RenderStateUpdater     ( /*Engine side of things , Model side of things */ ); 
                    ~RenderStateUpdater    ();

    virtual void    Update                  ( float t );

};

}
