#include "PluginsFactory.h"

#include "Engine/Models/Plugins/GeometryPluginRing.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"
#include "Engine/Models/Plugins/GeometryMultiRectPlugin.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"


namespace bv
{
namespace model
{

// *******************************
//
GeometryRectPlugin *         PluginsFactory::CreateGeometryRectPlugin            ( float w, float h, ITimeEvaluatorPtr timeEvaluator )
{
    auto pw = ParametersFactory::CreateParameter( "width", InterpolatorsHelper::CreateConstValue( w ), timeEvaluator );
    auto ph = ParametersFactory::CreateParameter( "height", InterpolatorsHelper::CreateConstValue( h ), timeEvaluator );

    auto rectPlugin = new GeometryRectPlugin( pw, ph );

	return rectPlugin;
}

//// *******************************
////
//model::GeometryMultiRectPlugin *       PluginsFactory::CreateGeometryMultiRectPlugin            ()
//{
//    return new model::GeometryMultiRectPlugin();
//}
//
//// *******************************
////
//model::GeometryRingPlugin *          PluginsFactory::CreateGeometryRingPlugin            ( float startAngle, float endAngle, float innerRadius, float outerRadius, int segmentsNum )
//{
//    model::GeometryRingPlugin*  ringPlugin = new model::GeometryRingPlugin( startAngle, endAngle, innerRadius, outerRadius, segmentsNum );
//
//    return ringPlugin;
//}
//
//// *******************************
////
//model::IPlugin *                     PluginsFactory::CreateSolidColorPlugin              ( BasicNode* root, model::IPlugin * prevPlugin, const glm::vec4 & color )
//{
//    auto colorPlugin = PluginsFactory::CreateSimpleColorPlugin( prevPlugin, ParametersFactory::CreateParameter( "color", InterpolatorsHelper::CreateConstValue( color ) ) );
//    root->AddPlugin( colorPlugin );
//
//    auto colorShaderPlugin = PluginsFactory::CreateSimplePixelShaderPlugin( colorPlugin, "../dep/media/shaders/solid.frag" );
//    root->AddPlugin( colorShaderPlugin );
//
//    return colorShaderPlugin;
//}
//
//// *******************************
////
//model::SimpleTexturePlugin*         PluginsFactory::CreateSimpleTexturePlugin                 ( model::IPlugin* prevPlugin, const std::vector< const TextureDescriptor > & textureDescs, bv::model::RendererContext * ctx, TextureAttachmentMode mode )
//{
//	auto texturePlugin = new model::SimpleTexturePlugin( prevPlugin, textureDescs, ctx, mode );
//
//	return texturePlugin;
//}
//
//// *******************************
////
//model::SimpleTexturePlugin*         PluginsFactory::CreateSimpleTexturePlugin                 ( model::IPlugin* prevPlugin, const std::vector< const TextureDescriptor > & textureDescs, TextureAttachmentMode mode )
//{
//	auto texturePlugin = new model::SimpleTexturePlugin( prevPlugin, textureDescs, mode );
//
//	return texturePlugin;
//}
//
//// *******************************
////
//model::SimpleAnimationPlugin *      PluginsFactory::CreateSimpleAnimationPlugin               ( model::IPlugin * prevPlugin, const std::vector< std::string > & texturesPaths, const ParamFloat & frameCounter, model::RendererContext * ctx )
//{
//	auto animationPlugin = new model::SimpleAnimationPlugin( prevPlugin, texturesPaths, frameCounter, ctx );
//
//    return animationPlugin;
//}
//
//// *******************************
////
//model::SimpleTextPlugin *            PluginsFactory::CreateSimpleTextPlugin                    ( const std::wstring & text, const std::string & fontFile, int size, bool bolded, bool italic )
//{
//    auto texPlugin = model::SimpleTextPlugin::Create( text, fontFile, size, bolded, italic );
//
//	return texPlugin;
//}
//
//// *******************************
////
//model::TimerPlugin *                 PluginsFactory::CreateTimerPlugin                          ( const ParamFloat & time, unsigned int size )
//{
//    auto timerPlugin = model::TimerPlugin::Create( time, size );
//
//	return timerPlugin;
//}
//
//// *******************************
////
//model::SimpleTransformPlugin *      PluginsFactory::CreateSimpleTransformPlugin               ( const model::IPlugin * prev, const ParamTransformVec & transformVec )
//{
//    return model::SimpleTransformPlugin::Create( prev, transformVec );
//}
//
//// *******************************
////
//model::SimpleColorPlugin *          PluginsFactory::CreateSimpleColorPlugin             ( model::IPlugin* prevPlugin, const ParamVec4 & color )
//{
//    return new model::SimpleColorPlugin( prevPlugin, color );
//}
//
//// *******************************
////
//model::SimplePixelShaderPlugin *    PluginsFactory::CreateSimplePixelShaderPlugin       ( model::IPlugin* prevPlugin, const std::string & shaderPath, model::RendererContext * ctx )
//{
//    return new model::SimplePixelShaderPlugin( prevPlugin, shaderPath, ctx );
//}
//
//// *******************************
////
//model::SimpleVertexShaderPlugin*    PluginsFactory::CreateSimpleVertexShaderPlugin      ( model::IPlugin* prevPlugin, const std::string& shaderPath )
//{
//    return new model::SimpleVertexShaderPlugin( prevPlugin, shaderPath );
//}
//
//// *******************************
////
//model::SimpleGeometryShaderPlugin*  PluginsFactory::CreateSimpleGeometryShaderPlugin    ( model::IPlugin* prevPlugin, const std::string& shaderPath )
//{
//    return new model::SimpleGeometryShaderPlugin( prevPlugin, shaderPath );
//}

ParamTransformVecPtr						GetCurrentParamTransform( const IPlugin * pl )
{
	if( pl )
	{	
		auto trParam = pl->GetParamTransform();

		if( !trParam )
			return GetCurrentParamTransform( pl->GetPrevPlugin().get() );

		return trParam;
	}
	
	return nullptr;

}

} // model
} //bv
