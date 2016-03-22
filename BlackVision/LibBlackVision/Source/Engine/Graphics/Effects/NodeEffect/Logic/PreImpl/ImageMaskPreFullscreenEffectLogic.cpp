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
        if( !fitObject )
        {
            mathematics::Box bb;
            PFLogicUtils::CalcCommonBoxForNode( node, &bb );

            if( !bb.m_empty )
            {
                // from uv to [-1, 1]x[-1, 1]
                auto txToR3 = glm::translate( glm::mat4( 1.f ), glm::vec3( -1.f, -1.f, 0.f ) ) * glm::scale( glm::mat4( 1.f ), glm::vec3( 2.f, 2.f, 1.f ) );

                // scale to object size
                auto objectSize = glm::vec3( bb.xmax - bb.xmin, bb.ymax - bb.ymin, bb.zmax - bb.zmin );
                auto scale = glm::scale( glm::mat4( 1.f ), glm::vec3( objectSize.x / 2.f, objectSize.y / 2.f, 1.f ) );

                // translate to object center.
                auto objectCenter = glm::vec3( ( bb.xmin + bb.xmax ) / 2.f, ( bb.ymin + bb.ymax ) / 2.f, 0.f/*( bb.zmin + bb.zmax ) / 2.f*/ );
                auto translate = glm::translate( glm::mat4( 1.f ), objectCenter );

                // node world transform matrix
                auto transformMatrix = node->GetTransformable()->WorldTransform().Matrix();

                // projection matrix
                auto projMat = ctx->GetRenderer()->GetCamera()->GetProjectionMatrix();

                // from [-1, 1]x[-1, 1] back to uv
                auto txToUV = glm::scale( glm::mat4( 1.f ), glm::vec3( 0.5f, 0.5f, 1.f ) ) * glm::translate( glm::mat4( 1.f ), glm::vec3( 1.f, 1.f, 0.f ) );

                ret = txToUV * transformMatrix * translate * scale * txToR3;

                ret = glm::inverse( ret );
            }
            else
            {

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