#include "MaskChannelsEffect.h"


namespace bv {

// ****************************
//
MaskChannelEffect::MaskChannelEffect ()
    : m_maskParam( nullptr )
{
}

// ****************************
//
MaskChannelEffect::~MaskChannelEffect()
{
}

// ****************************
//
PixelShader *       MaskChannelEffect::CreatePS        ()
{
    // TODO: implement
    return nullptr;
}

// ****************************
//
VertexShader *      MaskChannelEffect::CreateVS        ()
{
    // TODO: implement
    return nullptr;
}

} // bv

