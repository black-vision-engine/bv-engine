#include "RenderTargetEntryDesc.h"


namespace bv {

// *********************************
//
RenderTargetDataEntryDesc::RenderTargetDataEntryDesc( unsigned int width_, unsigned int height_, TextureFormat fmt_ )
    : width( width_ )
    , height( height_ )
    , fmt( fmt_ )
{
}

} // bv
