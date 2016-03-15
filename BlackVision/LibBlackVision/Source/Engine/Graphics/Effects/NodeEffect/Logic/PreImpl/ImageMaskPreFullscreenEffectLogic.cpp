#include "stdafx.h"

#include "ImageMaskPreFullscreenEffectLogic.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "PreFullscreenEffectLogicUtils.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Mathematics/Util/Transformation.h"

#include "Engine/Graphics/SceneGraph/Camera.h"

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
            auto bb = node->GetBoundingBox();

            if( bb != nullptr )
            {
                auto projMat = ctx->GetRenderer()->GetCamera()->GetProjectionMatrix();

                auto tbb = mathematics::TransformationUtils::Transform( bb, projMat * node->GetTransformable()->WorldTransform().Matrix() );

                auto cxo = ( tbb.xmax / tbb.zmax + tbb.xmin / tbb.zmin ) / 2.f;
                auto cyo = ( tbb.ymax / tbb.zmax + tbb.ymin / tbb.zmin ) / 2.f;

                auto screenAspectRatio = screenWidth / screenHeight;

                auto xd = 1.f;
                auto yd = 1.f;

                if( screenAspectRatio >= 1.f )
                {
                    xd = screenAspectRatio;
                }
                else
                {
                    yd = screenAspectRatio;
                }

                cxo = ( cxo + xd ) / ( 2 * xd );
                cyo = ( cyo + yd ) / ( 2 * yd );

                auto cxm = ( ( maskW / 2.f ) / screenWidth );
                auto cym = ( ( maskH / 2.f ) / screenHeight );

                auto sx = cxo - cxm;
                auto sy = cyo - cym;

                tx = glm::translate( tx, glm::vec3( -sx, -sy, 0.0 ) );
            }

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