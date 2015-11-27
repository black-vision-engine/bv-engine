//#include "OffscreenRenderLogic.h"
//
//#include "Engine/Graphics/Resources/RenderTarget.h"
//#include "Engine/Graphics/Renderers/Renderer.h"
//
//#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"
//#include "Engine/Graphics/SceneGraph/Camera.h"
//#include "Engine/Graphics/SceneGraph/TriangleStrip.h"
//
//#include "Engine/Graphics/Effects/Texture2DEffect.h"
//
//
//namespace bv {
//
//namespace {
//
//const unsigned int GNumRenderTargets = 2;
//
//} //anonymous
//
//
//// ************************************************* RENDER TARGET DATA ***********************************************************
//
//// **************************
////
//RenderTargetData::RenderTargetData      ()
//    : renderTarget( nullptr )
//    , quad( nullptr )
//{
//}
//
//// **************************
////
//RenderTargetData::~RenderTargetData     ()
//{
//}
//
//
//// **************************************************** TEXTURE DATA **************************************************************
//
//// **************************
////
//TextureData::TextureData( unsigned int width, unsigned int height, TextureFormat fmt )
//{
//    m_width     = width;
//    m_height    = height;
//    m_fmt       = fmt;
//
//}
//
//
//// *********************************************** OFFSCREEN RENDER DATA **********************************************************
//
//// **************************
////
//OffscreenRenderData::OffscreenRenderData()
//{
//    auxQuad                 = MainDisplayTarget::CreateAuxRect( nullptr );
//    effectTexture2D         = std::static_pointer_cast<Texture2DEffect>( auxQuad->GetRenderableEffect() );
//    effectTexture2DWithMask = std::make_shared<Texture2DEffectWithMask>( nullptr, nullptr, true );
//
//    std::vector< bv::Transform > vec;
//    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );
//
//    auxQuad->SetWorldTransforms( vec );
//}
//
//// **************************
////
//void    OffscreenRenderData::UseTexture2DEffect ( const IValue * val, Texture2DPtr tex )
//{
//    effectTexture2D->SetAlphaValModel( val );
//    effectTexture2D->SetTexture( tex );
//
//    auxQuad->SetRenderableEffect( effectTexture2D );
//}
//
//// **************************
////
//void    OffscreenRenderData::UseTexture2DEffect ( const IValue * val, Texture2DPtr tex, Texture2DPtr mask )
//{
//    effectTexture2DWithMask->SetAlphaValModel( val );
//    effectTexture2DWithMask->SetTexture( tex );
//    effectTexture2DWithMask->SetMask( mask );
//
//    auxQuad->SetRenderableEffect( effectTexture2DWithMask );
//}
//
//
//// *********************************************** OFFSCREEN RENDER LOGIC *********************************************************
//
//// **************************
////
//OffscreenRenderLogic::OffscreenRenderLogic   ( unsigned int width, unsigned int height, unsigned int numReadBuffers, Camera * camera, TextureFormat fmt )
//    : m_textureData( width, height, fmt )
//    , m_usedStackedRenderTargets( 0 )
//    , m_topRenderTargetEnabled( false )
//    , m_readbackTextures( numReadBuffers * GNumRenderTargets ) //two display targets that can be potentially used
//    , m_displayCamera( nullptr )
//    , m_rendererCamera( camera )
//    , m_buffersPerTarget( numReadBuffers )
//    , m_rtStack( width, height, fmt )
//{
//    m_displayCamera         = MainDisplayTarget::CreateDisplayCamera();
//
//    m_displayRenderTargetData[ 0 ] = CreateDisplayRenderTargetData();
//    m_displayRenderTargetData[ 1 ] = CreateDisplayRenderTargetData();
//
//    m_videoOutputRenderTarget = CreateVideoOutputRenderTargetData();
//
//    for( unsigned int i = 0; i < m_readbackTextures.size(); ++i )
//    {
//        m_readbackTextures[ i ] = nullptr;
//    }
//}
//
//// **************************
////
//OffscreenRenderLogic::~OffscreenRenderLogic  ()
//{
//    for( auto rtd : m_auxRenderTargets )
//    {
//        delete rtd;
//    }
//
//    for( unsigned int i = 0; i < 2; ++i )
//    {
//        delete m_displayRenderTargetData[ i ].renderTarget;
//        delete m_displayRenderTargetData[ i ].quad;
//    }
//
//    delete m_displayCamera;
//}
//
//// **************************
////
//void                OffscreenRenderLogic::SetRendererCamera         ( Camera * camera )
//{
//    m_rendererCamera = camera;
//}
//
//// **************************
////
//void                OffscreenRenderLogic::AllocateNewRenderTarget     ( Renderer * renderer )
//{
//    if( m_rtStack.TotalActiveRenderTargets() == 0 )
//    {
//        m_rtStack.AllocateNewRenderTarget( renderer, RenderTarget::RTSemantic::S_DRAW_READ );
//    }
//    else
//    {
//        m_rtStack.AllocateNewRenderTarget( renderer, RenderTarget::RTSemantic::S_DRAW_ONLY );
//    }
//}
//
//// **************************
////
//void                OffscreenRenderLogic::EnableTopRenderTarget       ( Renderer * renderer )
//{
//    m_rtStack.EnableTopRenderTarget( renderer );
//}
//
//// **************************
////
//void                OffscreenRenderLogic::DiscardCurrentRenderTarget  ( Renderer * renderer )
//{
//    m_rtStack.DiscardCurrentRenderTarget( renderer );
//}
//
//// **************************
////
//void                OffscreenRenderLogic::DisableTopRenderTarget    ( Renderer * renderer )
//{
//    m_rtStack.DisableTopRenderTarget( renderer );
//}
//
//// **************************
////
//void                OffscreenRenderLogic::DrawTopAuxRenderTarget    ( Renderer * renderer, const IValue * alphaVal )
//{
//    DisableTopRenderTarget( renderer );
//
//    auto topRTD = m_rtStack.GetRenderTargetAt( -1 );
//    auto prvRTD = m_rtStack.GetRenderTargetAt( -2 );
//
//    m_renderData.UseTexture2DEffect ( alphaVal, topRTD->ColorTexture( 0 ) );
//
//    renderer->Enable( prvRTD );
//    renderer->SetCamera( m_displayCamera );
//    renderer->Draw( m_renderData.auxQuad );
//    renderer->SetCamera( m_rendererCamera );
//    renderer->Disable( prvRTD );
//}
//
//// **************************
////
//void                OffscreenRenderLogic::DrawAMTopTwoRenderTargets ( Renderer * renderer, const IValue * alphaVal )
//{
//    DisableTopRenderTarget( renderer );
//
//    auto maskRT     = m_rtStack.GetRenderTargetAt( -1 );
//    auto textureRT  = m_rtStack.GetRenderTargetAt( -2 );
//    auto mainRT     = m_rtStack.GetRenderTargetAt( -3 );
//
//    m_renderData.UseTexture2DEffect( alphaVal, textureRT->ColorTexture( 0 ), maskRT->ColorTexture( 0 ) );
//
//    renderer->Enable( mainRT );
//    renderer->SetCamera( m_displayCamera );
//    renderer->Draw( m_renderData.auxQuad );
//    renderer->SetCamera( m_rendererCamera );
//    renderer->Disable( mainRT );
//}
//
//// **************************
////
//void                OffscreenRenderLogic::DrawWithAllVideoEffects   ( Renderer * renderer )
//{
//    { renderer; }
//}
//
//// **************************
////
//void                OffscreenRenderLogic::DrawDisplayRenderTarget   ( Renderer * renderer )
//{
//    // FIXME: make sure that render logic does not discard this target before it is used (right now it is not a problem as discarding RT does not free any data and simply changes and index)
//    assert( m_rtStack.TotalAllocatedRenderTargets() >= 1 );
//    assert( m_rtStack.TotalActiveRenderTargets() == 0 );
//
//    //FIXME: hack - make sure that top render target (display) is active
//    m_rtStack.AllocateNewRenderTarget( renderer, RenderTarget::RTSemantic::S_DRAW_READ );
//    auto rt = m_rtStack.GetRenderTargetAt( 0 ); // DISPLAY
//
//    // FIXME: create fake, shitty effect
//    // FIXME: this shit requires an immediate fix
//    if( m_tmpOutput.quad == nullptr )
//    {
//        std::vector< bv::Transform > vec;
//        vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );
//
//        m_tmpOutput.renderTarget = nullptr;
//
//        m_tmpOutput.quad = MainDisplayTarget::CreateDisplayRect( rt->ColorTexture( 0 ) );
//        m_tmpOutput.quad->SetWorldTransforms( vec );
//    }
//
//    auto & rtd = CurDisplayRenderTargetData();
//
//    //Display render target
//    renderer->SetCamera( m_displayCamera );
//    renderer->Draw( rtd.quad );
//    renderer->SetCamera( m_rendererCamera );
//            
//    m_rtStack.DiscardCurrentRenderTarget( renderer );
//}
//
//// **************************
////
//void                OffscreenRenderLogic::SwapDisplayRenderTargets  ()
//{
//    // FIXME: what here
//    // m_curDisplayTarget = ( m_curDisplayTarget + 1 ) % GNumRenderTargets;
//}
//
//// **************************
////
//unsigned int    OffscreenRenderLogic::TotalNumReadBuffers           () const
//{
//    return (unsigned int) m_readbackTextures.size();
//}
//
//// **************************
////
//unsigned int    OffscreenRenderLogic::NumReadBuffersPerRT           () const
//{
//    return TotalNumReadBuffers() / GNumRenderTargets;
//}
//
//// **************************
////
//Texture2DConstPtr   OffscreenRenderLogic::ReadDisplayTarget         ( Renderer * renderer, unsigned int bufNum )
//{
//    unsigned int bufferIdx = GNumRenderTargets * bufNum + CurDisplayRenderTargetNum();
//
//    assert( bufferIdx < m_readbackTextures.size() );
//
//    renderer->ReadColorTexture( 0, CurDisplayRenderTargetData().renderTarget, m_readbackTextures[ bufferIdx ] );
//
//    return m_readbackTextures[ bufferIdx ];
//}
//
//// **************************
//// Python-like logic, where negative numbers are used to index the array backwards
//RenderTarget *      OffscreenRenderLogic::GetRenderTargetAt         ( int i )
//{
//    int numUsedRT = (int) m_usedStackedRenderTargets;
//
//    if( i < 0 )
//    {
//        i = numUsedRT + i;
//    }
//
//    if( i < 0 || i >= numUsedRT )
//    {
//        assert( false );
//
//        return nullptr;
//    }
//
//    if( i == 0 )
//    {
//        return CurDisplayRenderTargetData().renderTarget;
//    }
//    else
//    {
//        return m_auxRenderTargets[ i - 1 ]; 
//    }
//}
//
//// **************************
////
//RenderTargetData    OffscreenRenderLogic::CreateDisplayRenderTargetData     () const
//{
//    RenderTargetData ret;
//
//    std::vector< bv::Transform > vec;
//    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );
//
//    auto rt   = MainDisplayTarget::CreateDisplayRenderTarget( m_textureData.m_width, m_textureData.m_height, m_textureData.m_fmt );
//    auto quad = MainDisplayTarget::CreateDisplayRect( rt->ColorTexture( 0 ) );
//    
//    quad->SetWorldTransforms( vec );
//
//    ret.renderTarget            = rt;
//    ret.quad                    = quad;
//
//    return ret;
//}
//
//// **************************
////
//RenderTargetData    OffscreenRenderLogic::CreateVideoOutputRenderTargetData () const
//{
//    //FIXME: implement additional logic for VideoOutput render target
//    RenderTargetData ret;
//
//    std::vector< bv::Transform > vec;
//    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );
//
//    auto rt   = MainDisplayTarget::CreateDisplayRenderTarget( m_textureData.m_width, m_textureData.m_height, m_textureData.m_fmt );
//    auto quad = MainDisplayTarget::CreateDisplayRect( rt->ColorTexture( 0 ) );
//    
//    quad->SetWorldTransforms( vec );
//
//    ret.renderTarget            = rt;
//    ret.quad                    = quad;
//
//    return ret;
//}
//
//// **************************
////
//unsigned int      OffscreenRenderLogic::CurDisplayRenderTargetNum           () const
//{
//    //FIXME: what here
//    // return m_curDisplayTarget;
//
//    return 0;
//}
//
//// **************************
////
//RenderTargetData &  OffscreenRenderLogic::CurDisplayRenderTargetData        ()
//{
//    assert( m_tmpOutput.quad != nullptr );
//
//    return m_tmpOutput;
//}
//
//} //bv
