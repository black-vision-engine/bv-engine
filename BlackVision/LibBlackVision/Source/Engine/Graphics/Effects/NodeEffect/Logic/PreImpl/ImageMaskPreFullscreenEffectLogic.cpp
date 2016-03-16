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

    auto screenWidth  = outputs[ 0 ]->Width();
    auto screenHeight = outputs[ 0 ]->Height();

    auto txt = node->GetNodeEffect()->GetTexture( 0 );

    auto maskW = txt->GetWidth();
    auto maskH = txt->GetHeight();

    auto fit = m_fitVal->GetValue();

    auto maskAspect = m_maskAspectVal->GetValue();

    auto tx = CalculateMaskTransformation( maskW, maskH, screenWidth, screenHeight, maskAspect == 1, fit == 1, node,  ctx );

    m_maskTxVal->SetValue( tx );
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

// *********************************
//
glm::mat4                   ImageMaskPreFullscreenEffectLogic::CalculateMaskTransformation( SizeType maskW, SizeType maskH, SizeType screenW, SizeType screenH , bool aspectMask, bool fitMask, SceneNode * node, RenderLogicContext * ctx ) const
{
    glm::mat4 ret( 1.f );

    if( aspectMask )
    {
        ret = glm::scale( ret, glm::vec3( float( screenW ) / float( maskW ), float( screenH ) / float( maskH ), 1.f ) );

        if( fitMask )
        {
            auto bb = node->GetBoundingBox();

            if( bb != nullptr )
            {
                auto projMat = ctx->GetRenderer()->GetCamera()->GetProjectionMatrix();

                auto tbb = mathematics::TransformationUtils::Transform( bb, projMat * node->GetTransformable()->WorldTransform().Matrix() );

                auto omin = PFLogicUtils::ScreenPosToFullScreenTexPos( glm::vec3( tbb.xmin, tbb.ymin, tbb.zmin ), screenW, screenH );
                auto omax = PFLogicUtils::ScreenPosToFullScreenTexPos( glm::vec3( tbb.xmax, tbb.ymax, tbb.zmax ), screenW, screenH );

                auto cxo = ( omin.x + omax.x ) / 2.f;
                auto cyo = ( omin.y + omax.y ) / 2.f;

                auto cxm = ( ( maskW / 2.f ) / screenW );
                auto cym = ( ( maskH / 2.f ) / screenH );

                auto sx = cxo - cxm;
                auto sy = cyo - cym;

                ret = glm::translate( ret, glm::vec3( -sx, -sy, 0.0 ) );
            }
        }
        else // fit to the screen
        {
            auto sx = ( screenW - maskW ) / ( 2.f * screenW );
            auto sy = ( screenH - maskH ) / ( 2.f * screenH );

            ret = glm::translate( ret, glm::vec3( -sx, -sy, 0.0 ) );
        }
    }
    else
    {
        if( fitMask )
        {
        }
        else
        {
            auto cxm = ( ( maskW / 2.f ) / screenW );
            auto cym = ( ( maskH / 2.f ) / screenH );

            auto sx = 0.5f - cxm;
            auto sy = 0.5f - cym;

            ret = glm::translate( ret, glm::vec3( -sx, -sy, 0.0 ) );
        }
    }

    return ret;
}


} // bv