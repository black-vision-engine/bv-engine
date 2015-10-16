#include "NodeMaskNodeEffect.h"


namespace bv {

// *********************************
//
NodeMaskNodeEffect::NodeMaskNodeEffect     ( unsigned int backgroundNodeIdx, unsigned int foregroundNodeIdx )
    : NodeEffect( NodeEffect::Type::T_NODE_MASK )
    , m_bgNodeIdx( backgroundNodeIdx  )
    , m_fgNodeIdx( foregroundNodeIdx )
{
}

// *********************************
//
NodeMaskNodeEffect::~NodeMaskNodeEffect    ()
{
}

// *********************************
//
unsigned int    NodeMaskNodeEffect::GetForegroundIdx    () const
{
    return m_fgNodeIdx;
}

// *********************************
//
unsigned int    NodeMaskNodeEffect::GetBackgroundIdx    () const
{
    return m_bgNodeIdx;
}

} // bv
