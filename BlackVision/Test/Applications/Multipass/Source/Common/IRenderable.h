#pragma once

namespace bv {

class IRenderable
{
public:

    virtual void    Render      ()  = 0;

    virtual         ~IRenderable()  = 0 {}
};

} // bv
