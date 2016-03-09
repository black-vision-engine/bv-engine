#include "stdafx.h"

#include "ImageMaskPreFullscreenEffectLogic.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "PreFullscreenEffectLogicUtils.h"

namespace bv {

// *********************************
//
ImageMaskPreFullscreenEffectLogic::ImageMaskPreFullscreenEffectLogic              ()
{
    m_blurSize      = ValuesFactory::CreateValueFloat( "blurSize", 0.f );
    m_textureSize   = ValuesFactory::CreateValueVec2( "textureSize" );
}

// *********************************
//
ImageMaskPreFullscreenEffectLogic::~ImageMaskPreFullscreenEffectLogic             ()
{
}

// *********************************
//
void                        ImageMaskPreFullscreenEffectLogic::RenderImpl    ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs )
{
    assert( outputs.size() == 1 );
    
    PFLogicUtils::RenderSceneNodeToRenderTarget( node, ctx, outputs[ 0 ] );

    m_textureSize->SetValue( glm::vec2( outputs[ 0 ]->Width(), outputs[ 0 ]->Height() ) );
}

// *********************************
//
std::vector< IValuePtr >    ImageMaskPreFullscreenEffectLogic::GetValues     () const
{
    std::vector< IValuePtr > res( 2 );

    res[ 0 ] = m_blurSize;
    res[ 1 ] = m_textureSize;

    return res;
}

// *********************************
//
unsigned int                ImageMaskPreFullscreenEffectLogic::GetPreferredNumOutputs() const
{
    return 1;
}

// *********************************
//
bool                        ImageMaskPreFullscreenEffectLogic::IsFSERequired () const
{
    auto bs = m_blurSize->GetValue();

    return bs > 0;
}

} // bv