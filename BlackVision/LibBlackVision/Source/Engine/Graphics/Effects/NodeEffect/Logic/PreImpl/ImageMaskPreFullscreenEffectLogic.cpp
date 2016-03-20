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

    m_debugPoint0 = ValuesFactory::CreateValueVec2( "debugPoint0" );
    m_debugPoint1 = ValuesFactory::CreateValueVec2( "debugPoint1" );
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
    std::vector< IValuePtr > ret( 5 );

    ret[ 0 ] = m_fitVal;
    ret[ 1 ] = m_maskAspectVal;
    ret[ 2 ] = m_maskTxVal;
    ret[ 3 ] = m_debugPoint0;
    ret[ 4 ] = m_debugPoint1;

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
glm::mat4                   ImageMaskPreFullscreenEffectLogic::CalculateMaskTransformation( SizeType maskW, SizeType maskH, SizeType screenW, SizeType screenH , bool aspectMask, bool fitObject, SceneNode * node, RenderLogicContext * ctx ) const
{
    glm::mat4 ret( 1.f );

    if( aspectMask )
    {
        //ret = glm::scale( ret, glm::vec3( float( maskW ) / float( screenW ), float( maskH ) / float( screenH ), 1.f ) );

        if( fitObject )
        {
            mathematics::Box bb;
            PFLogicUtils::CalcCommonBoxForNode( node, &bb );

            if( !bb.m_empty )
            {
                auto projMat = ctx->GetRenderer()->GetCamera()->GetProjectionMatrix();

                auto transformMatrix = node->GetTransformable()->WorldTransform().Matrix();

                auto tbb = mathematics::TransformationUtils::Transform( &bb, transformMatrix );

                tbb = mathematics::TransformationUtils::Project( &tbb, projMat );

                auto omin = mathematics::TransformationUtils::ToScreenNormCoords( glm::vec2( tbb.xmin, tbb.ymin ) );
                auto omax = mathematics::TransformationUtils::ToScreenNormCoords( glm::vec2( tbb.xmax, tbb.ymax ) );

                //{ // DEBUGGING
                //    auto dp = PFLogicUtils::ScreenPosToFullScreenTexPos( glm::vec3( 1.0f, 0.f, 3.f ), screenW, screenH );
                    m_debugPoint0->SetValue( omin );
                    m_debugPoint1->SetValue( omax );
                //}

                auto maskCenter = glm::vec2( 0.5f, 0.5f );

                auto maskAspect = float( maskW ) / float( maskH );

                auto txKeepAspect = glm::scale( glm::mat4( 1.f ), glm::vec3( maskAspect, 1.f, 1.f ) );

                maskCenter = glm::vec2( txKeepAspect * glm::vec4( maskCenter, 0.f, 1.f ) );

                auto objectCenter = glm::vec2( ( omin.x + omax.x ) / 2.f, ( omin.y + omax.y ) / 2.f );

                auto txTranslateToObjCenter = glm::translate( glm::mat4( 1.f ), glm::vec3( objectCenter - maskCenter, 0.f ) );
                maskCenter = glm::vec2( txTranslateToObjCenter * glm::vec4( maskCenter, 0.f, 1.f ) );

                auto maskSize = glm::vec2( txKeepAspect * glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
                auto objectSize = glm::vec2( omax.x - omin.x, omax.y - omin.y );

                auto scaleToFit = std::min( objectSize.x / maskSize.x, objectSize.y / maskSize.y );

                auto scaleToFitMat = glm::scale( glm::mat4( 1.f ), glm::vec3( scaleToFit, scaleToFit, 1.f ) );

                auto translateToCenter = glm::translate( glm::mat4( 1.f ), glm::vec3( maskCenter, 0.f) );
                auto translateToCenterInv = glm::translate( glm::mat4( 1.f ), glm::vec3( -maskCenter, 0.f) );

                ret = translateToCenter * scaleToFitMat * translateToCenterInv * txTranslateToObjCenter * txKeepAspect;

                ret = glm::inverse( ret );

                auto debug = ret * glm::vec4( 0.5f, 0.5f, 0.f, 1.f );
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
        if( fitObject )
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