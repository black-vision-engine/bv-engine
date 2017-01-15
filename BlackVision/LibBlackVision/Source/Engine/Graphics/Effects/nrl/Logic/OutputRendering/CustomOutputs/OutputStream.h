#pragma once


namespace bv { namespace nrl {

class RenderResult;
class NRenderContext;

class VideoOutput
{
private:

public:

    void    HandleFrame( NRenderContext * ctx, const RenderResult * result );

};

} //nrl
} //bv
