#include "stdafx.h"

#include "ImageMaskPreFullscreenEffectLogic.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "PreFullscreenEffectLogicUtils.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

namespace bv {

// *********************************
//
ImageMaskPreFullscreenEffectLogic::ImageMaskPreFullscreenEffectLogic              ()
    : m_txDesc( nullptr )
{
    m_fitVal = ValuesFactory::CreateValueInt( "fit" );
    m_maskAspectVal = ValuesFactory::CreateValueInt( "maskAspect" );

    m_maskTxVal = ValuesFactory::CreateValueMat4( "maskTx" );
    m_maskTxVal->SetValue( glm::mat4( 1.f ) );
}

// *********************************
//
ImageMaskPreFullscreenEffectLogic::~ImageMaskPreFullscreenEffectLogic             ()
{}

// *********************************
//
void                        ImageMaskPreFullscreenEffectLogic::RenderImpl    ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs )
{
    assert( outputs.size() == 1 );
    
    PFLogicUtils::RenderSceneNodeToRenderTarget( node, ctx, outputs[ 0 ] );

    auto screenWidth  = float( outputs[ 0 ]->Width() );
    auto screenHeight = float( outputs[ 0 ]->Height() );

    auto fit = m_fitVal->GetValue();

    auto maskAspect = m_maskAspectVal->GetValue();

    if( maskAspect == 1 )
    {
        auto txt = node->GetNodeEffect()->GetTexture( 0 );
        auto maskW = float( txt->GetWidth() );
        auto maskH = float( txt->GetHeight() );

        glm::mat4 tx( 1.f );

        tx = glm::scale( tx, glm::vec3( screenWidth / maskW, screenHeight / maskH, 1.f ) );

        if( fit == 1 ) // fit to the object
        {

        }
        else // fit to the screen
        {
            auto sx = ( screenWidth - maskW ) / ( 2.f * screenWidth );
            auto sy = ( screenHeight - maskH ) / ( 2.f * screenHeight );

            tx = glm::translate( tx, glm::vec3( -sx, -sy, 0.0 ) );
        }

        m_maskTxVal->SetValue( tx );
    }
}

// *********************************
//
std::vector< IValuePtr >    ImageMaskPreFullscreenEffectLogic::GetValues        () const
{
    std::vector< IValuePtr > ret( 3 );

    ret[ 0 ] = m_fitVal;
    ret[ 1 ] = m_maskAspectVal;
    ret[ 2 ] = m_maskTxVal;

    return ret;
}

// *********************************
//
unsigned int                ImageMaskPreFullscreenEffectLogic::GetPreferredNumOutputs() const
{
    return 1;
}

// *********************************
//
void                        ImageMaskPreFullscreenEffectLogic::AddTexture       ( const ITextureDescriptorConstPtr & txDesc )
{
    m_txDesc = txDesc;
}

// *********************************
//
bool                        ImageMaskPreFullscreenEffectLogic::IsFSERequired    () const
{
    return m_txDesc != nullptr;
}

} // bv