#include "PluginsFactory.h"

#include "Engine/Models/Plugins/SimpleTransformPlugin.h"
#include "Engine/Models/Plugins/GeometryPluginRing.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"
#include "Engine/Models/Plugins/GeometryMultiRectPlugin.h"


namespace bv
{
namespace model
{

// *******************************
//
FloatInterpolator                   PluginsFactory::CreateConstValueFloat               ( float val )
{
	FloatInterpolator inter; inter.setWrapPostMethod( bv::WrapMethod::pingPong );
	inter.addKey( 0.f, val );

	return inter;
}

// *******************************
//
Vec4Interpolator                   PluginsFactory::CreateConstValueVec4                 ( const glm::vec4& val )
{
	Vec4Interpolator inter; inter.setWrapPostMethod( bv::WrapMethod::pingPong );
	inter.addKey( 0.f, val );

	return inter;
}

// *******************************
//
model::GeometryRectPlugin*          PluginsFactory::CreateGeometryRectPlugin            ( float w, float h )
{
	FloatInterpolator wi; wi.setWrapPostMethod( bv::WrapMethod::pingPong );
	FloatInterpolator hi; hi.setWrapPostMethod( bv::WrapMethod::pingPong );
    
	wi.addKey(0.f, w);
	hi.addKey(0.f, h);

    auto rectPlugin = new model::GeometryRectPlugin(wi, hi);

	return rectPlugin;
}

// *******************************
//
model::GeometryMultiRectPlugin*       PluginsFactory::CreateGeometryMultiRectPlugin            ()
{
    return new model::GeometryMultiRectPlugin();
}

// *******************************
//
model::GeometryRingPlugin*          PluginsFactory::CreateGeometryRingPlugin            ( float startAngle, float endAngle, float innerRadius, float outerRadius, int segmentsNum )
{
    model::GeometryRingPlugin*  ringPlugin = new model::GeometryRingPlugin( startAngle, endAngle, innerRadius, outerRadius, segmentsNum );

    return ringPlugin;
}

// *******************************
//
model::GeometryChannel*             PluginsFactory::CreateGeometryChannel               (model::IConnectedComponent* connComp)
{
	model::GeometryChannelDescriptor desc;

	for( auto compDesc : connComp->GetVertexAttributeChannels() )
	{
		desc.AddVertexAttrChannelDesc( static_cast< const model::VertexAttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
	}

	model::GeometryChannel* ret = new model::GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc );

	ret->AddConnectedComponent(connComp);

	return ret;
}

// *******************************
//
model::TransformChannel*           PluginsFactory::CreateTransformChannel              (TransformF* transformation)
{
	model::SimpleTransformChannel*      trasformChannel  = new model::SimpleTransformChannel();
	trasformChannel->AddTransform( transformation );

	return trasformChannel;
}

// *******************************
//
model::SolidColorPlugin*            PluginsFactory::CreateSolidColorPlugin              (model::IPlugin* prevPlugin, const glm::vec4& color)
{
	auto solidPlugin = new model::SolidColorPlugin( prevPlugin, CreateConstValueVec4( color ) );

	// Set Pixel Shader Channel
	

	return solidPlugin;
}

// *******************************
//
model::SimpleTexturePlugin*         PluginsFactory::CreateTexturePlugin                 ( model::IPlugin* prevPlugin, const std::vector< std::string >& texturesPaths, TextureAttachmentMode mode )
{
	auto texturePlugin = new model::SimpleTexturePlugin( prevPlugin, texturesPaths, mode );

	return texturePlugin;
}

// *******************************
//
model::SimpleAnimationPlugin *      PluginsFactory::CreateAnimationPlugin               ( model::IPlugin * prevPlugin, const std::vector< std::string > & texturesPaths, unsigned int animationFPS )
{
	auto animationPlugin = new model::SimpleAnimationPlugin( prevPlugin, texturesPaths, animationFPS );

    return animationPlugin;
}

// *******************************
//
model::SimpleTextPlugin *            PluginsFactory::CreateTextPlugin                    ( const std::wstring & text, const std::string & fontFile, int size, bool bolded, bool italic )
{
    auto texPlugin = model::SimpleTextPlugin::Create( text, fontFile, size, bolded, italic );

	return texPlugin;
}

// *******************************
//
model::SimpleTransformPlugin *      PluginsFactory::CreateTransformPlugin               ( const model::IPlugin * prev, TransformF * trans )
{
    return model::SimpleTransformPlugin::Create( prev, trans );
}

// *******************************
//
model::IGeometryShaderChannel *     PluginsFactory::CreateGeometryShaderExtrude         ( float scale )
{
	FloatInterpolator extrudeScale = CreateConstValueFloat( scale );
	return new model::ExtrudeGeometryShaderChannel("../dep/media/shaders/extrude.geom", extrudeScale);
}

// *******************************
//
model::SimpleColorPlugin*           PluginsFactory::CreateSimpleColorPlugin             ( model::IPlugin* prevPlugin, const Vec4Interpolator& color)
{
    return new model::SimpleColorPlugin( prevPlugin, color );
}

// *******************************
//
model::SimplePixelShaderPlugin*     PluginsFactory::CreateSimplePixelShaderPlugin       ( model::IPlugin* prevPlugin, const std::string& shaderPath, model::RendererContext * ctx )
{
    return new model::SimplePixelShaderPlugin( prevPlugin, shaderPath, ctx );
}

// *******************************
//
model::SimpleVertexShaderPlugin*    PluginsFactory::CreateSimpleVertexShaderPlugin      ( model::IPlugin* prevPlugin, const std::string& shaderPath)
{
    return new model::SimpleVertexShaderPlugin( prevPlugin, shaderPath );
}

// *******************************
//
model::SimpleGeometryShaderPlugin*  PluginsFactory::CreateSimpleGeometryShaderPlugin    ( model::IPlugin* prevPlugin, const std::string& shaderPath)
{
    return new model::SimpleGeometryShaderPlugin( prevPlugin, shaderPath );
}

// *******************************
//
model::RendererContext*             PluginsFactory::CreateDefaultRenderableContext()
{
    auto ctx = new model::RendererContext();
    ctx->alphaCtx = new model::AlphaContext();
    ctx->cullCtx = new model::CullContext();
    ctx->depthCtx = new model::DepthContext();
    ctx->fillCtx = new model::FillContext();

    return ctx;
}

} // model
} //bv
