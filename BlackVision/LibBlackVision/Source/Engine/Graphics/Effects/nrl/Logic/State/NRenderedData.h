#pragma once

#include <vector>


namespace bv { 

class RenderTarget;

namespace nrl {

typedef std::vector< const RenderTarget * > RenderTargetVec;

class NRenderedData
{
private:

    RenderTargetVec m_renderTargets;

    unsigned int    m_lastRenderTargetIdx;

public:

                            NRenderedData           ( unsigned int numRenderTargets );
                            ~NRenderedData          ();

    void                    SetEntry                ( unsigned int i, const RenderTarget * rt );
    const RenderTarget *    GetEntry                ( unsigned int i ) const;

    unsigned int            GetNumEntries           () const;

    const RenderTargetVec & GetAllRenderTargets     () const;

    void                    SetLastRenderTargetIdx  ( unsigned int idx );
    unsigned int            GetLastRenderTargetIdx  () const;

};

} //nrl
} //bv
