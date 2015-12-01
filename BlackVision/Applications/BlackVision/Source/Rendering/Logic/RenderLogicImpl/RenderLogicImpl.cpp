#include "RenderLogicImpl.h"


namespace bv {

// ***************************
//
RenderLogicImpl::RenderLogicImpl     ( bool videoCardEnabled, RenderTargetStack * rtStack )
    : m_useVideoCard( videoCardEnabled )
    , m_rtStack( rtStack )
{

}

// ***************************
//
RenderLogicImpl::~RenderLogicImpl    ()
{
}

} //bv
