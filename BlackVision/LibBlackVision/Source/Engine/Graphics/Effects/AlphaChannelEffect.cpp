//#include "AlphaChannelEffect.h"
//
//#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
//#include "Engine/Models/Builder/RendererStatesBuilder.h"
//
//#include "Engine/Models/Plugins/EngineConstantsMapper.h"
//
//#include "Engine/Graphics/Effects/AlphaChannelShaders.h"
//
//
//namespace bv {
//
//// ****************************
////
//AlphaChannelEffect::AlphaChannelEffect ()
//{
//    auto ps = CreatePS();
//    auto vs = CreateVS();
//
//    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );
//    auto sinst = pass->GetStateInstance();
//
//    RendererStatesBuilder::Create( sinst );
//
//    auto as = RenderStateAccessor::AccessAlphaState( sinst );
//    auto ds = RenderStateAccessor::AccessDepthState( sinst );
//    auto cs = RenderStateAccessor::AccessCullState( sinst );
//
//    as->blendEnabled = false;
//    ds->enabled = false;
//    cs->enabled = false;
//
//    AddPass( pass );
//}
//
//// ****************************
////
//AlphaChannelEffect::~AlphaChannelEffect()
//{
//    auto pass = GetPass( 0 );
//
//    delete pass->GetPixelShader();
//    delete pass->GetVertexShader();
//}
//
//
//// ****************************
////
//void    AlphaChannelEffect::SetTexture         ( Texture2DPtr texture )
//{
//    auto ps = GetPass( 0 )->GetPixelShader();
//    auto params = ps->GetParameters();
//
//	if( params->NumTextures() == 0 )
//	{
//		params->AddTexture( texture );
//	}
//	else
//	{
//		params->SetTexture( 0, texture );
//	}
//}
//
//// ****************************
////
//PixelShader *       AlphaChannelEffect::CreatePS  ()
//{
//    auto params = new ShaderParameters();
//
//    auto shader = new PixelShader( GetAlphaChannelPixelShaderCode(), params );
//    auto sampler = new TextureSampler( 0, "Tex0", SamplerSamplingMode::SSM_MODE_2D );
//    shader->AddTextureSampler( sampler );
//
//    return shader;
//}
//
//// ****************************
////
//VertexShader *      AlphaChannelEffect::CreateVS        ()
//{
//    auto params     = new ShaderParameters();
//    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();
//
//    params->AddParameter( mvpParam );
//
//    auto shader = new VertexShader( GetAlphaChannelVertexShaderCode(), params );
//
//    return shader;
//}
//
//} // bv
//
