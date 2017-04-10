#pragma once

#include <vector>


namespace bv { 

class RenderTarget;

namespace nrl {

typedef std::vector< const RenderTarget * > RenderTargetVec;

class RenderedData
{
private:

    RenderTargetVec m_renderTargets;

    unsigned int    m_lastRenderTargetIdx;

public:

                            RenderedData           ( unsigned int numRenderTargets );
                            ~RenderedData          ();

    void                    SetEntry                ( unsigned int i, const RenderTarget * rt );
    const RenderTarget *    GetEntry                ( unsigned int i ) const;

    unsigned int            GetNumEntries           () const;

    const RenderTargetVec & GetAllRenderTargets     () const;

    void                    SetLastRenderTargetIdx  ( unsigned int idx );
    unsigned int            GetLastRenderTargetIdx  () const;

};

} //nrl
} //bv
