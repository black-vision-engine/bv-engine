#pragma once


namespace bv { namespace nrl {

enum class CustomOutputType : unsigned int
{
    COT_PREVIEW = 0,
    COT_VIDEO,
    COT_STREAM,

    COT_TOTAL
};

class RenderResult;
class NRenderContext;

// FIXME: nrl - rename this class somehow (if necessary)
class OutputInstance
{
private:

public:

    virtual         ~OutputInstance     ();

    virtual void    ProcessFrameData    ( NRenderContext * ctx, const RenderResult * data ) = 0;

};

} //nrl
} //bv
