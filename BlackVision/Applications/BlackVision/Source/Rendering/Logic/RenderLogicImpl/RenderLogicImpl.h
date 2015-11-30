#pragma once

namespace bv {

class RenderLogicImpl
{
private:

    bool    m_useVideoCard;

public:

                        RenderLogicImpl     ( bool videoCardEnabled );
    virtual             ~RenderLogicImpl    ();


    virtual     void    RenderFrame         () = 0;

};

} // bv
