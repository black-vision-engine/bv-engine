#include "PluginsFactory.h"

#include "Engine/Models/Plugins/SimpleTransformPlugin.h"
#include "Engine/Models/Plugins/GeometryPluginRing.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"
#include "Engine/Models/Plugins/GeometryMultiRectPlugin.h"

#include "Engine/Models/Timeline/Timeline.h"


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
ParamVec4                          PluginsFactory::CreateParameter                     ( const std::string & name, Vec4Interpolator & interpolator, const Timeline * timeline )
{
    return ParamVec4( name, interpolator, timeline );
}

// *******************************
//
ParamFloat                          PluginsFactory::CreateParameter                     ( const std::string & name, FloatInterpolator & interpolator, const Timeline * timeline )
{
    return ParamFloat( name, interpolator, timeline );
}

// *******************************
//
ParamTransform                       PluginsFactory::CreateParameter                     ( const std::string & name, const TransformF & interpolator, const Timeline * timeline )
{
    return ParamTransform( name, interpolator, timeline );
}

// *******************************
//
model::GeometryRectPlugin *         PluginsFactory::CreateGeometryRectPlugin            ( float w, float h )
{
    auto rectPlugin = new model::GeometryRectPlugin(CreateParameter("width", CreateConstValueFloat( w )), CreateParameter("height", CreateConstValueFloat( h )));

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
model::VertexAttributesChannel*             PluginsFactory::CreateGeometryChannel               (model::IConnectedComponent* connComp)
{
	model::VertexAttributesChannelDescriptor desc;

	for( auto compDesc : connComp->GetAttributeChannels() )
	{
		desc.AddAttrChannelDesc( static_cast< const model::AttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
	}

	model::VertexAttributesChannel * ret = new model::VertexAttributesChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc );

	ret->AddConnectedComponent(connComp);

	return ret;
}

// *******************************
//
model::TransformChannel*           PluginsFactory::CreateTransformChannel              ( const ParamTransform& transformation )
{
	model::SimpleTransformChannel*      trasformChannel  = new model::SimpleTransformChannel();
	trasformChannel->AddTransform( transformation );

	return trasformChannel;
}

// *******************************
//
model::SolidColorPlugin*            PluginsFactory::CreateSolidColorPlugin              (model::IPlugin* prevPlugin, const glm::vec4& color)
{
	auto solidPlugin = new model::SolidColorPlugin( prevPlugin, CreateParameter( "color", CreateConstValueVec4( color ) ) );

	// Set Pixel Shader Channel
	

	return solidPlugin;
}

// *******************************
//
model::SimpleTexturePlugin*         PluginsFactory::CreateTexturePlugin                 ( model::IPlugin* prevPlugin, const std::vector< std::string >& texturesPaths, const std::vector< TransformF > txTransforms,bv::model::RendererContext * ctx, TextureAttachmentMode mode )
{
	auto texturePlugin = new model::SimpleTexturePlugin( prevPlugin, texturesPaths, txTransforms,ctx, mode );

	return texturePlugin;
}

// *******************************
//
model::SimpleTexturePlugin*         PluginsFactory::CreateTexturePlugin                 ( model::IPlugin* prevPlugin, const std::vector< std::string >& texturesPaths, const std::vector< TransformF > txTransforms, TextureAttachmentMode mode )
{
	auto texturePlugin = new model::SimpleTexturePlugin( prevPlugin, texturesPaths, txTransforms, mode );

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
model::SimpleTransformPlugin *      PluginsFactory::CreateTransformPlugin               ( const model::IPlugin * prev, const ParamTransform& transformation )
{
    return model::SimpleTransformPlugin::Create( prev, transformation );
}

// *******************************
//
model::IGeometryShaderChannel *     PluginsFactory::CreateGeometryShaderExtrude         ( float scale )
{
    auto extrudeScale = CreateParameter( "scale", CreateConstValueFloat( scale ) );
	return new model::ExtrudeGeometryShaderChannel("../dep/media/shaders/extrude.geom", extrudeScale);
}

// *******************************
//
model::SimpleColorPlugin *          PluginsFactory::CreateSimpleColorPlugin             ( model::IPlugin* prevPlugin, const ParamVec4 & color)
{
    return new model::SimpleColorPlugin( prevPlugin, color );
}

// *******************************
//
model::SimplePixelShaderPlugin *    PluginsFactory::CreateSimplePixelShaderPlugin       ( model::IPlugin* prevPlugin, const std::string& shaderPath, model::RendererContext * ctx )
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
